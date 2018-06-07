#include "ShapeGenerator.h"
#define NUM_ARRARY_ELEMENTS(a) sizeof(a) / sizeof(*a)

bool ellipseCompare(const ellipseInfo &base, const ellipseInfo &tobe)
{
	return (base.center == tobe.center && base.rx == tobe.rx && base.ry == tobe.ry && base.start == tobe.start && base.sweep == tobe.sweep);
}

std::vector<mLine> VBO_Line;
std::map<std::string, int>color_reference;
GLfloat glgh_current_color[2][3] = { 0 };
GLfloat current_opacity[2] = { 1.0f };

DynamicObject<ellipseInfo>LinedEllipse(ellipseCompare);
/*
ShapeData ShapeGenerator::makeTriangle()
{
	ShapeData ret;
	Vertex myTri[] =
	{
		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};
	ret.numVertices = NUM_ARRARY_ELEMENTS(myTri);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, myTri, sizeof(myTri));

	GLushort indices[] = { 0, 1, 2 };
	ret.numIndices = NUM_ARRARY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}*/

ShapeGenerator::ShapeGenerator()
{
	;
}

void ShapeGenerator::makeLine(std::vector<mLine> &VBO_Line, glm::vec3 start, glm::vec3 end)
{
	mLine m =
	{
		start,
		glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]),

		end,
		glm::vec3(glgh_current_color[1][0], glgh_current_color[1][1], glgh_current_color[1][2])
	};
	VBO_Line.push_back(m);
}
void ShapeGenerator::makeEllipse(GLfloat _rx, GLfloat _ry, GLfloat _start, GLfloat _sweep, glm::vec3 _center, GLfloat _rotate, glm::vec3 _axis)
{
	ellipseInfo info = { _rx, _ry, _start, _sweep, _center, _rotate , _axis};
	Vertex temp;
	LinedEllipse.begin(info, GLGH_COLOR_SMOOTH_LOOP);
	for (GLfloat i = _start, end = _sweep + _start; i < end; i += ARC_RSOLUTION) {
		temp.position = glm::vec3(_rx*cosf(i), _ry*sinf(i), 0.0f)+_center;
		temp.color    = glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]);
		LinedEllipse.add(temp);
	}
	LinedEllipse.end();
}
