#pragma once
#include <stdlib.h>
#include <time.h>
#include "camera.h"
#define GLGH_BACK 0
#define GLGH_FORWARDS 1
#define GLGH_CENETR 2
#define GLGH_LEFT 3

///////////////////////////////////////////////////////
////           compatibale functions               ////
///////////////////////////////////////////////////////

//1. Basics
void InitGraphics(int _allowCamera = TRUE, int _anti_aliasing = TRUE);
void InitConsole(void);
//void FreeConsole(void);
void MovePen(double x, double y, GLfloat z=0.0f);
void DrawLine(double dx, double dy, GLfloat dz = 0.0f);
void DrawEllipticalArc(double rx, double ry, double start, double sweep, int _startPosition = GLGH_LEFT, float _rotate = 0.0f, glm::vec3 _axis = glm::vec3(1.0f, 0.0f, 0.0f));
void DrawArc(double r, double start, double sweep);
void DrawTextString(const char *text, int fixed = TRUE);
double GetCurrentX();
double GetCurrentY();

double GetWindowWidth(void);
double GetWindowHeight(void);
void SetEraseMode(int mode);
int GetEraseMode(void);

//mouse position
double GetMouseX(void);
double GetMouseY(void);
double ScaleXInches(int x);
double ScaleYInches(int y);
void WaitForMouseDown(void);
void WaitForMouseUp(void);

//deprecated
void SetPenColor(std::string color);
void DefineColor(std::string name, double red, double green, double blue);
//recommended
void SetPenColor(int RGB_Start, float Start_opacity = 1.0f, int RGB_End = -1, float End_opacity=1.0f);

void SetPenSize(int size);
void SetPointSize(int size);
void SetPointSize(float size);
int GetPointSize(float *size=NULL);
void StartFilledRegion(double density);
void EndFilledRegion(void);

//window functions
void SetWindowTitle(const char* title);
const char *GetWindowTitle(void);
void ExitGraphics(void);

void UpdateDisplay(void);
void Pause(double seconds);
double GetFullScreenWidth(void);
double GetFullScreenHeight(void);
void SetWindowSize(double width, double height);

typedef void(*KeyboardEventCallback) (int key, int event);
typedef void(*CharEventCallback) (char c);
typedef void(*MouseEventCallback) (int x, int y, int button, int event);
typedef void(*TimerEventCallback) (int timerID);

void registerKeyboardEvent(KeyboardEventCallback callback);
void registerCharEvent(CharEventCallback callback);
void registerMouseEvent(MouseEventCallback callback);
void registerTimerEvent(TimerEventCallback callback);

void cancelKeyboardEvent();
void cancelCharEvent();
void cancelMouseEvent();
void cancelTimerEvent();

void startTimer(int _id, int _timeinterval);
void cancelTimer(int _id);
void Main();


///////////////////////////////////////////////////////
////            featured functions                 ////
///////////////////////////////////////////////////////

//camera will take control some mouse controls
//cursor will be hidden and constrained to the window
//you can look around in a 3D space using mouse & use WASD to move around & use scroll to zoom in/out
//escape key will cancel camera
void EnableCamera();
//if you are trying to set you own method to cancel camera
////// do not use escape

void CancelCamera(int Mode= GLGH_BACK);

//x,y are coordinations in OpenGL position
//Expamples
//----------
//MoveCamera(1.0);
//----------------or
//double x = GetCurrentX(), y = GetCurrentY();  
//ScaleInchesToGLFloat(&x, &y);
//MoveCamera(1.0, GLGH_LINEAR, float(x), float(y));
void MoveCamera(
	float time,  //time cost to move (seconds)
	int function = GLGH_SMOOTH_COS, //the way it moves ,smooth(cos), linear£¨GLGH_LINEAR), or instant (GLGH_INSTANT_COS).
	float x = 0.0f, //destination x, please use function ScaleInchesToGLFloat() if you're using inches, then use float to reinforce a trasition from double to GLfloat
	float y = 0.0f, //destination y
	float z = 3.0f, //destination z, note that initial z pos is 3.0f instead of 0.0f
	float yaw = -90.0f, //Euler Angle--yaw
	float pitch = -0.0f, //Euler Angle--pitch
	float zoom = 45.0f, //zoom in or out
	bool constrainPitch = true //constrain pitch form -89.0f to 89.0f
);

void ScaleInchesToGLFloat(double *x, double *y);


///////////////////////////////////////////////////////
////             Defined Constants                 ////
///////////////////////////////////////////////////////
typedef enum
{
	NO_BUTTON = 0,
	LEFT_BUTTON,
	MIDDLE_BUTTON,
	RIGHT_BUTTON
} ACL_Mouse_Button;

typedef enum
{
	BUTTON_DOWN,
	BUTTON_DOUBLECLICK,
	BUTTON_UP,
	ROLL_UP,
	ROLL_DOWN,
	MOUSEMOVE
} ACL_Mouse_Event;

typedef enum
{
	KEY_DOWN,
	KEY_UP
} ACL_Keyboard_Event;