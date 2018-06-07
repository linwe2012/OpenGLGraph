/*
#include "DynamicObject.h"

GLfloat glgh_current_color[2][3] = { 0 };
GLfloat current_opacity[2] = { 1.0f };

template<class T>
DynamicObject<T>::DynamicObject(bool(*compare)(const T &base, const T &toBeTested))
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
	
	if (_ifSmoothColor == true) {
		beginColor = glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]);
		endColor   = glm::vec3(glgh_current_color[1][0], glgh_current_color[1][1], glgh_current_color[1][2]);
		ifSmoothColor = true;
	}
	else
	{
		ifSmoothColor = false;
	}
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
		VBO_max_id++;
		VBO.push_back(vtx);
	}
	Strides++;
}

template<class T>
void DynamicObject<T>::end()
{
	GLuint offset = 0, end;
	int i;
	if (ifSmoothColor == true) {
		glm::vec3 colorIncrement = endColor - beginColor;
		colorIncrement = colorIncrement / Strides;
		for (i = 0, len = EBO_indices.size(); i < len; i++) {
			offset += EBO_indices[i];
		}
		for (i = 0, end = offset + Strides; offset < end; offset++, i++) {
			VBO[offset].color = beginColor + colorIncrement * i;
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
			for(end = offset + EBO_indices[i]; offset < end; offset++)
			{
				EBO_recycle.push_back(offset);
			}
			EBO_indices.erase(EBO_indices.begin() + i);
			break;
		}
		offset += EBO_indices[i];
	}
}

*/

