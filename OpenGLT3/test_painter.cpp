#include "OpenGLGraph.h"

int pensz = 5;
void drawDot(double x, double y, double r);
void MouseEventProcess(int x, int y, int button, int event);

void Main()
{
	//InitGraphics();
	SetPenColor(0xeefffe);
	registerMouseEvent(MouseEventProcess);
	SetPenSize(pensz);
}

void drawDot(double x, double y, double r)
{

	MovePen(x + r, y);
	//StartFilledRegion(1);
	DrawArc(r, 0.0, 360.0);
	//EndFilledRegion();
}

void MouseEventProcess(int x, int y, int button, int event)
{
	double mx, my;
	static double omx, omy;
	int i = 1;
	static bool isMove = FALSE;
	mx = ScaleXInches(x);
	my = ScaleYInches(y);
	switch (event)
	{
	case BUTTON_DOWN: {
		if (button == LEFT_BUTTON) {
			omx = mx;
			omy = my;
			MovePen(mx, my);
			isMove = TRUE;
		}
		break;
	}
	case BUTTON_DOUBLECLICK:
		break;
	case BUTTON_UP:
		isMove = FALSE;
		break;
	case MOUSEMOVE:
		if (isMove == TRUE) {
			//MovePen(mx, my);
			//DrawLine(omx - mx, omy - my);
			DrawLine(mx - omx, my - omy);
			omx = mx;
			omy = my;
		}
		break;
	}
}
