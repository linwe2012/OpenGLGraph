#include"OpenGLGraph.h"

/*
* Constants
* ---------
* The following constants control the sizes of the
* various elements in the display.
*/

#define HouseHeight         2.0  	  *4.0
#define HouseWidth          3.0		  *4.0
#define AtticHeight         0.7		  *4.0
									  
#define DoorWidth           0.4		  *4.0
#define DoorHeight          0.7		  *4.0
#define DoorknobRadius      0.03	  *4.0
#define DoorknobInset       0.07	  *4.0
									  
#define PaneHeight          0.25	  *4.0
#define PaneWidth           0.2		  *4.0
									  
#define FirstFloorWindows   0.3		  *4.0
#define SecondFloorWindows  1.25	  *4.0
									  
#define MenWidth 0.2				  *4.0
#define MenHeight 0.6				  *4.0
/* Function prototypes */

void DrawHouse(double x, double y);
void DrawOutline(double x, double y);
void DrawWindows(double x, double y);
void DrawDoor(double x, double y);
void DrawBox(double x, double y, double width, double height);
void DrawTriangle(double x, double y, double base, double height);
void DrawCenteredCircle(double x, double y, double r);
void DrawGrid(double x, double y, double width, double height,
	int columns, int rows);
void DrawMen(double x, double y, GLfloat z);
/* Main program */

void Main()
{
	double cx, cy;

	//InitGraphics();
	cx = GetWindowWidth() / 2;
	cy = GetWindowHeight() / 2;
	DrawHouse(cx - HouseWidth / 2,
		cy - (HouseHeight + AtticHeight) / 2);
	DrawMen(cx,
		cy - (HouseHeight) / 2, -1.0f);
}

/*
* Function: DrawHouse
* Usage: DrawHouse(x, y);
* -----------------------
* This function draws a house diagram with the lower left corner
* at (x, y).  This level of the function merely divides up
* the work.
*/

void DrawHouse(double x, double y)
{
	DrawOutline(x, y);
	DrawDoor(x + (HouseWidth - DoorWidth) / 2, y);
	DrawWindows(x, y);
}

/*
* Function: DrawOutline
* Usage: DrawOutline(x, y);
* -------------------------
* This function draws the outline for the house, using (x, y)
* as the origin.  The outline consists of a box with a triangle
* on top.
*/

void DrawOutline(double x, double y)
{
	DrawBox(x, y, HouseWidth, HouseHeight);
	DrawTriangle(x, y + HouseHeight, HouseWidth, AtticHeight);
}

/*
* Function: DrawDoor
* Usage: DrawDoor(x, y);
* ----------------------
* This function draws a door, with its doorknob.  As usual,
* (x, y) specifies the lower left corner of the door.
*/

void DrawDoor(double x, double y)
{
	DrawBox(x, y, DoorWidth, DoorHeight);
	DrawCenteredCircle(x + DoorWidth - DoorknobInset,
		y + DoorHeight / 2, DoorknobRadius);
}

/*
* Function: DrawWindows
* Usage: DrawWindows(x, y);
* -------------------------
* This function draws all the windows for the house,
* taking advantage of the fact that the windows are all
* arranged in two-dimensional grids of equal-sized panes.
* By calling the function DrawGrid, this implementation
* can create all of the window structures using a single
* tool.
*/

void DrawWindows(double x, double y)
{
	double xleft, xright;

	xleft = x + HouseWidth * 0.25;
	xright = x + HouseWidth * 0.75;
	DrawGrid(xleft - PaneWidth * 1.5, y + FirstFloorWindows,
		PaneWidth, PaneHeight, 3, 2);
	DrawGrid(xright - PaneWidth * 1.5, y + FirstFloorWindows,
		PaneWidth, PaneHeight, 3, 2);
	DrawGrid(xleft - PaneWidth, y + SecondFloorWindows,
		PaneWidth, PaneHeight, 2, 2);
	DrawGrid(xright - PaneWidth, y + SecondFloorWindows,
		PaneWidth, PaneHeight, 2, 2);
}

/*
* Function: DrawBox
* Usage: DrawBox(x, y, width, height)
* -----------------------------------
* This function draws a rectangle of the given width and
* height with its lower left corner at (x, y).
*/

void DrawBox(double x, double y, double width, double height)
{
	MovePen(x, y);
	DrawLine(0, height);
	DrawLine(width, 0);
	DrawLine(0, -height);
	DrawLine(-width, 0);
}

/*
* Function: DrawTriangle
* Usage: DrawTriangle(x, y, base, height)
* ---------------------------------------
* This function draws an isosceles triangle (i.e., one with
* two equal sides) with a horizontal base.  The coordinate of
* the left endpoint of the base is (x, y), and the triangle
* has the indicated base length and height.  If height is
* positive, the triangle points upward.  If height is negative,
* the triangle points downward.
*/

void DrawTriangle(double x, double y, double base, double height)
{
	MovePen(x, y);
	DrawLine(base, 0);
	DrawLine(-base / 2, height);
	DrawLine(-base / 2, -height);
}

/*
* Function: DrawCenteredCircle
* Usage: DrawCenteredCircle(x, y, r);
* -----------------------------------
* This function draws a circle of radius r with its
* center at (x, y).
*/

void DrawCenteredCircle(double x, double y, double r)
{
	MovePen(x + r, y);
	DrawArc(r, 0, 360);
}

/*
* Function: DrawGrid
* Usage: DrawGrid(x, y, width, height, columns, rows);
* ----------------------------------------------------
* DrawGrid draws rectangles arranged in a two-dimensional
* grid.  As always, (x, y) specifies the lower left corner
* of the figure.
*/

void DrawGrid(double x, double y, double width, double height,
	int columns, int rows)
{
	int i, j;

	for (i = 0; i < columns; i++) {
		for (j = 0; j < rows; j++) {
			DrawBox(x + i * width, y + j * height,
				width, height);
		}
	}
}

//featured 
void DrawMen(double x, double y, GLfloat z) {
	double bodyHeight = (MenHeight - MenWidth) * 2 / 3.0;
	MovePen(x, y, z);
	SetPenColor(0xff0000, 1.0f, 0x0000ff);
	//head
	DrawArc(MenWidth / 2.0, 0, 360);
	MovePen(x - MenWidth / 2.0, y - MenWidth / 2.0, z);
	//body
	DrawLine(0, -bodyHeight);
	//legs
	DrawLine(-MenWidth / 2.0, -bodyHeight / 2.0);
	MovePen(x - MenWidth / 2.0, y - MenWidth / 2.0 - bodyHeight, z);
	DrawLine(MenWidth / 2.0, -bodyHeight / 2.0);
	//hands
	MovePen(x - MenWidth / 2.0, y - MenWidth / 2.0, z);
	DrawLine(MenWidth / 2.0, -bodyHeight / 2.0);
	MovePen(x - MenWidth / 2.0, y - MenWidth / 2.0, z);
	DrawLine(-MenWidth / 2.0, -bodyHeight / 2.0);
}