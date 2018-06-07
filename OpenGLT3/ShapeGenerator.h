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
#include "DynamicObject.h"
#define INIT_VERTEX_AMOUNT
const GLfloat ARC_RSOLUTION = GLGH_PI / 90.0f;


struct ellipseInfo {
	GLfloat rx;
	GLfloat ry;
	GLfloat start;
	GLfloat sweep;
	glm::vec3 center;
	GLfloat rotate;
	glm::vec3 axis;
};

struct mLine
{
	Vertex start;
	Vertex end;
};



extern std::vector<mLine> VBO_Line;
//------test--------------------------
extern std::vector<Vertex> VBO_Ellipse;
extern std::vector<GLuint> EBO_Ellipse;
extern std::vector<ellipseInfo> EBO_Ellipse_Indice;
//------------------------------------

extern GLfloat glgh_current_color[2][3];
extern GLfloat current_opacity[2];
extern std::map<std::string, int>color_reference;
extern DynamicObject<ellipseInfo>LinedEllipse;
class ShapeGenerator
{
public:
	ShapeGenerator();
	//static ShapeData makeTriangle();
	static void makeLine(std::vector<mLine> &VBO_Line, glm::vec3 start, glm::vec3 end);
	static void makeEllipse(GLfloat _rx, GLfloat _ry, GLfloat _start, GLfloat _sweep, glm::vec3 _center = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat _rotate = 0.0f, glm::vec3 _axis = glm::vec3(1.0f, 0.0f, 0.0f));
};
