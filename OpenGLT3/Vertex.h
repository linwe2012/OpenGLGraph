#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#define INIT_VERTEX_AMOUNT
const int ARC_RSOLUTION=360;
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct mLine
{
	Vertex start;
	Vertex end;
};

struct mEllipse
{
	GLfloat postion[ARC_RSOLUTION];
	GLfloat color;
};


std::vector<mLine> VBO_Line;
std::vector<mEllipse> VBO_Ellipse;
//std::vector<Ellipse> VBO;