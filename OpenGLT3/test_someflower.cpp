#include "OpenGLGraph.h"
#include <math.h>
#define PI 3.14159265
#define RAD(x) ((x) / 180 * PI)
#define RGB_t(x) ((x) * 1.0 / 0xFF)

double globe_angle = 0;
double globe_x;
double globe_y;
const int edges = 6;/*it is considered as a Hexagonal*/
double interiorAngle;

void forward(double distance); /*沿当前方向画线，长度为
							   distance，当distance为负数时后退*/
void turn(double angle);  /*顺时针旋转角度angle，角度单位为
						  DEG，当angle为负数时逆时针旋转*/
void move(double distance); /*沿当前方向移动笔（不画线），长度
							为distance，当distance为负数时后退*/

void drawPoly(double len, double iniAngle);
void initData();
void Main()
{
	double amount = 18;
	double rotate = 360 / amount;
	double angle = rotate / 2;
	InitGraphics();
	initData();
	while (angle <360) {
		drawPoly(3, angle);
		angle += rotate;
	}
}

void move(double distance)
{
	double x = GetCurrentX();
	double y = GetCurrentY();
	MovePen(x + distance * cos(globe_angle / 180 * PI), y + distance * sin(globe_angle / 180 * PI));
}

void turn(double angle)
{
	globe_angle -= angle;
	if (globe_angle >= 360) {
		globe_angle -= 360;
	}
	else if (globe_angle < 0) {
		globe_angle += 360;
	}
}

void forward(double distance)
{
	DrawLine(distance*cos(globe_angle / 180 * PI), distance*sin(globe_angle / 180 * PI));
}

void drawPoly(double len, double iniAngle)
{
	double rds = (len / 2) / cos(RAD(interiorAngle / 2));
	double rotate = 360 / edges;
	double x, y;
	int i = 0;
	//draw the interior branch-shaped stuff
	globe_angle = iniAngle;
	forward(rds / 2);
	turn(interiorAngle / 2);
	forward(len / 2);
	move(-len / 2);
	turn(-interiorAngle);
	forward(rds / 2);
	move(-len / 2);
	turn(interiorAngle / 2);
	move(-rds / 2);
	turn(rotate);
	i = 0;
	while (globe_angle != iniAngle && i< edges) {
		forward(rds / 2);
		turn(interiorAngle / 2);
		forward(len / 2);
		move(-len / 2);
		turn(-interiorAngle);
		forward(rds / 2);
		move(-len / 2);
		turn(interiorAngle / 2);
		move(-rds / 2);
		turn(rotate);
		i++;
	}
	//draw the outline
	i = 0;
	globe_angle = iniAngle;
	move(rds);
	turn(180 - interiorAngle / 2);
	forward(len);
	while (globe_angle != iniAngle && i< edges) {
		turn(180 - interiorAngle);
		forward(len);
		i++;
	}
	turn(180 - interiorAngle);
	forward(len);
	turn(-interiorAngle / 2);
	move(-rds);
}

void initData()
{
	interiorAngle = 180 - 360 / edges;
	globe_x = GetWindowWidth() / 2;
	globe_y = GetWindowHeight() / 2;
	//DefineColor("DrkGreen", RGB_t(0x00), RGB_t(0x82), RGB_t(0x07));
	SetPenColor(0x3121f0, 1.0f, 0x01ff1f);
	MovePen(globe_x, globe_y);
	//SetPenColor("DrkGreen");
	SetPenSize(3);
}
