#include "ShapeGenerator.h"
#define NUM_ARRARY_ELEMENTS(a) sizeof(a) / sizeof(*a)

bool ellipseCompare(const ellipseInfo &base, const ellipseInfo &tobe)
{
	return (base.center == tobe.center && base.rx == tobe.rx && base.ry == tobe.ry && base.start == tobe.start && base.sweep == tobe.sweep);
}

std::vector<mLine> VBO_Line;
std::map<std::string, int>color_reference;
//[0] start color  [1] end color
GLfloat glgh_current_color[2][3] = { 0 };
GLfloat current_opacity[2] = { 1.0f };
TextObject FixedText;
DynamicObject<ellipseInfo>LinedEllipse(ellipseCompare);
DynamicObject<ellipseInfo>FilledEllipse(ellipseCompare);
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

void ShapeGenerator::makeLine(std::vector<mLine> &VBO_Line, glm::vec3 start, glm::vec3 end, int ifdelete)
{
	mLine m =
	{
		start,
		glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]),

		end,
		glm::vec3(glgh_current_color[1][0], glgh_current_color[1][1], glgh_current_color[1][2])
	};
	if (ifdelete == FALSE) {
		VBO_Line.push_back(m);
	}
	else {
		for (int i = 0, len = VBO_Line.size(); i < len; i++) {
			if (m.start.position == VBO_Line[i].start.position
				&& m.end.position == VBO_Line[i].end.position) {
				VBO_Line.erase(VBO_Line.begin() + i);
				return;
			}
		}
	}
	
}
void ShapeGenerator::makeEllipse(GLfloat _rx, GLfloat _ry, GLfloat _start, GLfloat _sweep, glm::vec3 _center, GLfloat _rotate, glm::vec3 _axis, int ifdelete)
{
	ellipseInfo info = { _rx, _ry, _start, _sweep, _center, _rotate , _axis};
	Vertex temp;
	if (ifdelete == FALSE) {
		LinedEllipse.begin(info, GLGH_COLOR_SMOOTH_LOOP);
		for (GLfloat i = _start, end = _sweep + _start; i < end; i += ARC_RSOLUTION) {
			temp.position = glm::vec3(_rx*cosf(i), _ry*sinf(i), 0.0f) + _center;
			temp.color = glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]);
			LinedEllipse.add(temp);
		}
		LinedEllipse.end();
	}
	else {
		LinedEllipse.remove(info);
	}
}

void ShapeGenerator::makeFilledEllipse(GLfloat _rx, GLfloat _ry, GLfloat _start, GLfloat _sweep, glm::vec3 _center, GLfloat _rotate, glm::vec3 _axis, int ifdelete)
{
	ellipseInfo info = { _rx, _ry, _start, _sweep, _center, _rotate , _axis };
	Vertex temp;
	if (ifdelete == FALSE) {
		FilledEllipse.begin(info, GLGH_COLOR_SMOOTH_LOOP);
		for (GLfloat i = _start, end = _sweep + _start; i < end; i += ARC_RSOLUTION) {
			temp.position = glm::vec3(_rx*cosf(i), _ry*sinf(i), 0.0f) + _center;
			temp.color = glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]);
			FilledEllipse.add(temp);
		}
		FilledEllipse.end();
	}
	else {
		FilledEllipse.remove(info);
	}
}

void ShapeGenerator::makeFilledPolygon(glm::vec3 vtx, int ifends, int ifdelete) {
	static ellipseInfo info = { 0.0f, 0.0f,  0.0f,  0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f , glm::vec3(0.0f, 0.0f, 0.0f) };
	Vertex temp = { vtx,  glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]) };
	static int firsttime = 1;
	if (ifends == TRUE) {
		firsttime = 1;
		if (ifdelete == TRUE) {
			FilledEllipse.remove(info);
		}
		else if (ifdelete == FALSE) {
			FilledEllipse.EBO_info.back() = info;
			FilledEllipse.end();
		}
	}
	else if (ifends == FALSE) {
		if (firsttime == 1) {
			info.center = vtx;
			if (ifdelete == FALSE) {
				FilledEllipse.begin(info, GLGH_COLOR_SMOOTH_LOOP);
			}
			firsttime ++;
		}
		else if (firsttime == 2) {
			info.axis = vtx;
			firsttime++;
		}
		else if (firsttime == 3) {
			info.rx = vtx.x;
			info.ry = vtx.y;
			info.rotate = vtx.z;
			firsttime++;
		}
		else if (firsttime == 4) {
			info.start = vtx.x;
			info.sweep = vtx.y;
			firsttime++;
		}
		if (ifdelete == FALSE) {
			LinedEllipse.add(temp);
		}
	}

}

void ShapeGenerator::makeFixedText(const char *s, glm::vec3 pos, GLfloat scale, int ifdelete)
{
	TextInfo t;
	t.color = glm::vec3(glgh_current_color[0][0], glgh_current_color[0][1], glgh_current_color[0][2]);
	t.content = s;
	t.position = pos;
	t.scale = scale;
	if (ifdelete == FALSE) {
		FixedText.addText(t);
	}
	else {
		FixedText.deleteText(t);
	}
}
