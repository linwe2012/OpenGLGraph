#pragma once
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <string>
#define GLGH_PI 3.14159265f
#define GLGH_COLOR_SMOOTH_FASLE 0xA0
#define GLGH_COLOR_SMOOTH 0xA1
#define GLGH_COLOR_SMOOTH_LOOP 0xA2

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

extern GLfloat glgh_current_color[2][3];
extern GLfloat current_opacity[2];

template <class T> 
class DynamicObject
{
public:
	DynamicObject(bool(*_compare)(const T &base, const T &toBeTested));
	~DynamicObject();
	void begin(T _EBO_info, int _ifSmoothColor);
	void end();
	void add(Vertex vtx);
	void remove(const T &_EBO_info);
	void clear();

	std::vector<T>EBO_info;
	std::vector<GLuint>EBO_indices;
	std::vector<GLuint>EBO_recycle;
	std::vector<GLuint>EBO;

	std::vector<Vertex>VBO;
	GLuint VBO_max_id;

	glm::vec3 beginColor;
	glm::vec3 endColor;

	GLuint Strides;
	bool(*compare)(const T &base,const T &toBeTested);

	int ifSmoothColor;
};

template<class T>
DynamicObject<T>::DynamicObject(bool(*_compare)(const T &base, const T &toBeTested))
{
	VBO_max_id = 0;
	compare = _compare;
}

template<class T>
DynamicObject<T>::~DynamicObject()
{

}

template<class T>
void DynamicObject<T>::clear()
{
	EBO_info.clear();
	EBO_indices.clear();
	EBO_recycle.clear();
	EBO.clear();

	VBO.clear();
	VBO_max_id = 0;

	ifSmoothColor = false;
}

template<class T>
void DynamicObject<T>::begin(T _EBO_info, int _ifSmoothColor)
{
	EBO_info.push_back(_EBO_info);
	Strides = 0;

	if (_ifSmoothColor != GLGH_COLOR_SMOOTH_FASLE) {
		beginColor = glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]);
		endColor = glm::vec3(glgh_current_color[1][0], glgh_current_color[1][1], glgh_current_color[1][2]);
	}
	ifSmoothColor = _ifSmoothColor;
}

template<class T>
void DynamicObject<T>::add(Vertex vtx)
{
	GLuint id;
	if (!EBO_recycle.empty()) {
		id = EBO_recycle.back();
		EBO_recycle.pop_back();
		EBO.push_back(id);
		id *= 6;
		VBO[id] = vtx;
	}
	else
	{
		VBO.push_back(vtx);
		EBO.push_back(VBO_max_id);
		VBO_max_id++;
	}
	Strides++;
}

template<class T>
void DynamicObject<T>::end()
{
	GLuint offset = 0, end;
	int i, len;
	if (ifSmoothColor != GLGH_COLOR_SMOOTH_FASLE) {
		glm::vec3 colorIncrement = endColor - beginColor;
		colorIncrement = colorIncrement / GLfloat(Strides);
		for (i = 0, len = EBO_indices.size(); i < len; i++) {
			offset += EBO_indices[i];
		}
		if (ifSmoothColor == GLGH_COLOR_SMOOTH) {
			for (i = 0, end = offset + Strides; offset < end; offset++, i++) {
				VBO[offset].color = beginColor + colorIncrement * GLfloat(i);
			}
		}
		else if (ifSmoothColor == GLGH_COLOR_SMOOTH_LOOP) {
			colorIncrement *= 2.0f;
			for (i = 0, end = offset + Strides - 1; offset <= end; offset++, i++, end--) {
				VBO[offset].color = beginColor + colorIncrement * GLfloat(i);
				VBO[end].color = beginColor + colorIncrement * GLfloat(i);
			}
		}
	}
	EBO.push_back(~0);
	EBO_indices.push_back(Strides + 1);
}

template<class T>
void DynamicObject<T>::remove(const T &_EBO_info)
{
	GLuint offset = 0, end;
	for (int i = 0, len = EBO_info.size(); i < len; i++) {
		if (compare(_EBO_info, EBO_info[i]) == true) {
			for (end = offset + EBO_indices[i]; offset < end; offset++)
			{
				EBO_recycle.push_back(offset);
			}
			EBO_indices.erase(EBO_indices.begin() + i);
			break;
		}
		offset += EBO_indices[i];
	}
}



