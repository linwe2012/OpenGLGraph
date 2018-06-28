#include "OpenGLGraph.h"
#define PI 3.1415926535
#define RAD(x) ((x) / 180 * PI)
#define DES(x, y) (-(x) + (y))

void drawPolarLine(double r, double theta);
void koch(double x1, double y1, double x2, double y2, double angle, double len, int level);

void Main()
{
	double len = 4;
	int amount = 5;
	double sx, sy;
	double leftx, lefty, middlex, middley, rightx, righty;
	int i;
	InitGraphics();
	SetPenSize(1);
	sx = GetWindowWidth();
	sy = GetWindowHeight();
	printf("Input the length of the first triangle: ");
	scanf_s("%lf", &len);
	printf("Input the amount: ");
	scanf_s("%d", &amount);

	leftx = (sx - len) / 2;
	rightx = sx - leftx;
	lefty = righty = (sy - len * sin(60 * PI / 180)) / 2;
	middley = sy - lefty;
	middlex = (leftx + rightx) / 2;

	koch(leftx, lefty, middlex, middley, 60, len, amount);
	koch(middlex, middley, rightx, righty, -60, len, amount);
	koch(rightx, righty, leftx, lefty, -180, len, amount);
}

void drawPolarLine(double r, double theta) {
	double radians;
	radians = theta / 180 * 3.1415926535;
	DrawLine(r * cos(radians), r * sin(radians));
}

void koch(double x1, double y1, double x2, double y2, double angle, double len, int level)
{
	double x3 = (x2 - x1) / 3 + x1;
	double y3 = (y2 - y1) / 3 + y1;
	double x4 = (x2 - x1) / 3 * 2 + x1;
	double y4 = (y2 - y1) / 3 * 2 + y1;
	double x5 = x3 + len * cos(RAD(angle + 60)) / 3;
	double y5 = y3 + len * sin(RAD(angle + 60)) / 3;

	MovePen(x1, y1);
	drawPolarLine(len, angle);
	if (level == 0) {
		return;
	}
	SetPenColor(0x334C4C);
	MovePen(x3, y3);
	drawPolarLine(len / 3, angle);
	SetPenSize(1);
	SetPenColor(0x3121f0, 1.0f, 0x01ff1f);
	MovePen(x3, y3);
	drawPolarLine(len / 3, 60 + angle);
	MovePen(x4, y4);
	drawPolarLine(len / 3, 120 + angle);

	koch(x3, y3, x5, y5, angle + 60, len / 3, level - 1);
	koch(x5, y5, x4, y4, angle - 60, len / 3, level - 1);
	koch(x1, y1, x3, y3, angle, len / 3, level - 1);
	koch(x4, y4, x2, y2, angle, len / 3, level - 1);
}
