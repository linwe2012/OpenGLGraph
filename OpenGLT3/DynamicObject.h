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
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader.h"
#include "camera.h"
#define GLGH_PI 3.14159265f
#define GLGH_COLOR_SMOOTH_FASLE 0xA0
#define GLGH_COLOR_SMOOTH 0xA1
#define GLGH_COLOR_SMOOTH_LOOP 0xA2

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct TextInfo
{
	glm::vec3 position;
	glm::vec3 color;
	GLfloat scale;
	const char *content;
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
			EBO.erase(EBO.begin() + offset, EBO.begin() + EBO_indices[i]);
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


class TextObject
{
public:
	void init(int src_w, int scr_h)
	{
		shader = new Shader("shaders/3.3.text.vs.c", "shaders/3.3.text.fs.c");
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(src_w), 0.0f, static_cast<GLfloat>(scr_h));
		shader->use();
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);
		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		// Load first 128 characters of ASCII set
		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<GLuint>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	void render(const char *text, glm::vec3 pos, GLfloat scale, glm::vec3 color, Camera * cam)
	{
		// Activate corresponding render state
		shader->use();
		if (cam != NULL) {
			;
		}
		glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		for (const char *s = text; *s != '\0'; s++) {
			Character ch = Characters[*s];
			GLfloat xpos = pos.x + ch.Bearing.x * scale;
			GLfloat ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// Update VBO for each character
			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
			};
			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			pos.x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void renderAll(Camera * cam)
	{
		int itr = 0;
		for (int len = texts.size(); itr < len; itr++) {
			render(texts[itr].content, texts[itr].position, texts[itr].scale, texts[itr].color, cam);
		}
	}
	void addText(TextInfo t) {
		texts.push_back(t);
	}
	//succeed 0, fails 1
	int deleteText(TextInfo t) {
		int itr = 0;
		for (int len = texts.size(); itr < len; itr++) {
			if (texts[itr].content == t.content) {
				texts.erase(texts.begin() + itr);
				return 0;
			}
		}
		return 1;
	}

	Shader *shader;
	// FreeType
	FT_Library ft;
	FT_Face face;
	GLuint VAO, VBO;
	std::map<GLchar, Character> Characters;
	std::vector<TextInfo>texts;
};


