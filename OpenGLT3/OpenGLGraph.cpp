#include "shader.h"
//#include"ShapeData.h"
//#include "shader.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include "ShapeGenerator.h"
#include "OpenGLGraph.h"
#include "camera.h"
#define ANTI_ALIASING
//warning: conversion from 'double'/'int' to 'float', possible loss of data
//warning: conversion from 'time_t' to 'unsigned int', possible loss of data
#pragma warning(disable:4244) 
//LINK : warning LNK4098: defaultlib 'MSVCRTD' conflicts with use of other libs; use /NODEFAULTLIB:library

struct glgl_Timer {
	double maxTime;
	double time;
};
std::map<int, glgl_Timer> glgh_timer;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
void camera_mouse_callback(GLFWwindow* window, double xpos, double ypos);
void camera_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static float deltaTime = 0.0f;	// time between current frame and last frame
static float lastFrame = 0.0f;
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;
static int allowCameraScroll_scale = true;
static std::vector<glgh_CameraMovement> camMove;

GLFWwindow *window;
static int anti_aliasing = TRUE;

GLFWwindow *initWindow(const char *s);
void window_size_callback(GLFWwindow* mwindow, int width, int height);
void processInput(GLFWwindow *mwindow);

static GLfloat pen_current_x = 0;
static GLfloat pen_current_y = 0;
static GLfloat pen_current_z = 0;

static double cursor_current_x = 0;
static double cursor_current_y = 0;
static double cursor_current_z = 0;

static int scr_width = SCR_WIDTH;
static int scr_height = SCR_HEIGHT;
static double inches_to_pixels = 40;

static GLuint vbo_line, vao_line;
static GLuint vbo_ellipse, vao_ellipse;
static GLuint VAOE, VBOE, EBOE;
static int lineChanged = 0, LinedPolygonChanged = 0;
static Shader* ourShader;

static float penSize = 1;

static BOOL ifInitialzed = false;
static BOOL allowCamera = true;
static KeyboardEventCallback key_call = NULL;
static CharEventCallback char_call = NULL;
static MouseEventCallback mouse_call = NULL;
static TimerEventCallback timer_call = NULL;

//void character_callback(GLFWwindow* mwindow, unsigned int codepoint);
void glgh_key_callback(GLFWwindow* mwindow, int key, int scancode, int action, int mods);
void glgh_mouse_callback(GLFWwindow* window, double xpos, double ypos);
void glgh_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Calculate Macros
#define PIXEL_TO_GLFLOAT_X(x) GLfloat(GLfloat(x)*2/scr_width-1)
#define PIXEL_TO_GLFLOAT_Y(y) GLfloat(GLfloat(y)*2/scr_height-1)
#define INCHES_TO_PIXELS(x) int(x*inches_to_pixels)
#define PIXELS_TO_INCHES(x) (x/inches_to_pixels)
#define SCREEN_INCHES_TO_GLFLOAT_X(x) GLfloat(x*inches_to_pixels*2/scr_width-1)
#define SCREEN_INCHES_TO_GLFLOAT_Y(y) GLfloat(y*inches_to_pixels*2/scr_height - 1)
#define INCHES_TO_GLFLOAT_X(x) GLfloat(x*inches_to_pixels*2 / scr_width)
#define INCHES_TO_GLFLOAT_Y(y) GLfloat(y*inches_to_pixels*2 / scr_height)

inline void rgbToArray(GLfloat *arr, int rgb);
void updatePolygon();
//GL Draw
void updateLine(std::vector<mLine>&VBO_Line);
//--------TESTING---------------------------------------
GLfloat vertices_test[] = {
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f ,
	0.1f, 0.2f, 0.3f, 0.1f, 0.2f, 0.3f,
	0.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f,
	0.2f, 0.4f, 0.8f, 0.1f, 0.9f, 0.7f };
GLfloat vertices_anotehr[] = {
	0.7f, -0.1f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	-0.3f, -0.9f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f ,
	0.9f, 0.2f, 0.0f, 1.0f, 1.0f, 1.0f,
	-1.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,
	0.8f, 0.1f, 0.0f, 0.1f, 0.9f, 0.7f };

GLuint indices[] =
{
	0, 1, 2,
	1, 2, 3,
	0, 2, 3,
};
//-------------------------------------------------------------
int main()
{

	InitGraphics();
	ourShader=new Shader("./shaders/3.3.shader.vs.c", "./shaders/3.3.shader.fs.c");
	
//#ifdef ANTI_ALIASING
//	glEnable(GL_MULTISAMPLE);
//#endif // ANTI_ALIASING
	glGenVertexArrays(1, &vao_line);
	glGenBuffers(1, &vbo_line);

	glBindVertexArray(vao_line);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_line);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	//color 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//ellipse VAO & VBO
	//------------------
	//glGenVertexArrays(1, &vao_ellipse);
	//glGenBuffers(1, &vbo_ellipse);
	//
	//glBindVertexArray(vao_ellipse);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_ellipse);
	//
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	//glEnableVertexAttribArray(0);
	//
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	int test_i=0;
	ourShader->use();
	//ShapeGenerator::makeLine(VBO_Line, glm::vec3(0.7f, -0.1f, 0.0f), glm::vec3(-0.3f, -0.9f, 0.0f));
	glBindBuffer(GL_ARRAY_BUFFER, vbo_line);
	glBufferData(GL_ARRAY_BUFFER, VBO_Line.size()* sizeof(mLine), &VBO_Line[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_ellipse);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_test), vertices_test, GL_STATIC_DRAW);
	//----------testing---------------------------------------------------------------------------------
	GLuint VAOT, VBOT, EBOT;
	glGenVertexArrays(1, &VAOT);
	glGenBuffers(1, &VBOT);
	glGenBuffers(1, &EBOT);
	glBindVertexArray(VAOT);
	glBindBuffer(GL_ARRAY_BUFFER, VBOT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_anotehr), vertices_anotehr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOT);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glEnable(GL_PRIMITIVE_RESTART); 
	glPrimitiveRestartIndex(~0);
	
	glGenVertexArrays(1, &VAOE);
	glGenBuffers(1, &VBOE);
	glGenBuffers(1, &EBOE);
	glBindVertexArray(VAOE);
	glBindBuffer(GL_ARRAY_BUFFER, VBOE);
	glBufferData(GL_ARRAY_BUFFER, LinedEllipse.VBO.size() * sizeof(Vertex), &(LinedEllipse.VBO[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOE);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, LinedEllipse.EBO.size() * sizeof(GLuint), &(LinedEllipse.EBO[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	SetPenColor(0xffffff, 1.0f, 0x01771f);
	//MovePen(9, 9);
	//DrawArc(5, 0, 360);
	SetPenColor(0x3121f0, 1.0f, 0x01ff1f);
	//MovePen(10, 6, -2.0f);
	glm::mat4 ViewMatrix = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	ViewMatrix = projection * ViewMatrix;
	//ViewMatrix = projection;
	glm::vec4 start = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 end = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//ViewMatrix = glm::inverse(ViewMatrix);
	start = start * ViewMatrix;
	end = end * ViewMatrix;
	printf("%f  %f  %f\n", start.x, start.y, start.z);
	//printf("pen xyz %f, %f, %f, xyz %f, %f, %f\n", pen_current_x, pen_current_y, pen_current_z, x, y, z);
	//ShapeGenerator::makeLine(VBO_Line, glm::vec3(pen_current_x, pen_current_y, pen_current_z), glm::vec3(x, y, z));
	ShapeGenerator::makeLine(VBO_Line, glm::vec3(start.x, start.y, start.z), glm::vec3(end.x, end.y, end.z));
	updateLine(VBO_Line);
	//DrawEllipticalArc(6, 3, 0, 360);
	//--------------------------------------------------------------------------------------------------------------------
	Main();
	EnableCamera();
	//allowCamera = false;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		if (LinedPolygonChanged == 1) {
			updatePolygon();
			LinedPolygonChanged = 0;
		}
		if (lineChanged == 1) {
			updateLine(VBO_Line);
			lineChanged = 0;
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ourShader->use();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//pass delta time to determine if it should move back
		camera.PassDeltaTime(deltaTime);
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ourShader->setMat4("view", view);

		glBindVertexArray(vao_line);
		glDrawArrays(GL_LINES, 0, 2*VBO_Line.size());
		//glBindVertexArray(vao_ellipse);
		//glDrawArrays(GL_LINE_LOOP, 0, 3);

		glBindVertexArray(VAOT);
		glDrawElements(GL_LINE_LOOP, 9, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAOE);
		glDrawElements(GL_LINE_STRIP, LinedEllipse.EBO.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	return 0;
}

//init part
//---------------------------------------------------------------------------------------------------
void InitGraphics(int _allowCamera, int _anti_aliasing)
{
	anti_aliasing = _anti_aliasing;
	window = initWindow("OpenGL Graph");
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Running against GLFW %i.%i.%i\n", major, minor, revision);
	srand(time(NULL));
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, glgh_key_callback);
	glfwSetCursorPosCallback(window, glgh_mouse_callback);
	glfwSetScrollCallback(window, glgh_scroll_callback);
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	if (_anti_aliasing == TRUE) {
		glEnable(GL_MULTISAMPLE);
	}
}
//callbacks
void window_size_callback(GLFWwindow* mwindow, int width, int height) {
	scr_width = width;
	scr_height = height;
	glViewport(0, 0, width, height);
}


void glgh_key_callback(GLFWwindow* mwindow, int key, int scancode, int action, int mods)
{
	int state;
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		MovePen(rand() % 15 + rand() % 10000 / 10000.0, rand() % 15 + rand() % 10000 / 10000.0);
		DrawLine(rand() % 15 + rand() % 10000 / 10000.0, rand() % 15 + rand() % 10000 / 10000.0);
	}
	if (key_call == NULL && key == GLFW_KEY_M && allowCamera == FALSE) {
		EnableCamera();
	}
	else if (key_call != NULL) {
		if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
			if ((mods & GLFW_MOD_SHIFT) !=0) {
				switch (key)
				{
				case GLFW_KEY_0: char_call(')'); break;
				case GLFW_KEY_1: char_call('!'); break;
				case GLFW_KEY_2: char_call('@'); break;
				case GLFW_KEY_3: char_call('#'); break;
				case GLFW_KEY_4: char_call('$'); break;
				case GLFW_KEY_5: char_call('%'); break;
				case GLFW_KEY_6: char_call('^'); break;
				case GLFW_KEY_7: char_call('&'); break;
				case GLFW_KEY_8: char_call('*'); break;
				case GLFW_KEY_9: char_call('('); break;
				default:
					break;
				}
			}
			else{
				char_call(char(key));
			}
		}
		else if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
			if (mods & GLFW_MOD_CONTROL && key == GLFW_KEY_M) {
				EnableCamera();
			}
			else if (mods & GLFW_MOD_SHIFT || mods & GLFW_MOD_CAPS_LOCK) {
				char_call(char(key));
			}
			else {
				char_call(char(key+0x20));//lower case
			}
		}
		else if (key >= GLFW_KEY_SPACE && key <= GLFW_KEY_SLASH || key == GLFW_KEY_SEMICOLON) {
			char_call(char(key));
		}
		else if(key == GLFW_KEY_EQUAL){
			char_call('=');
		}
		else if (key >= GLFW_KEY_LEFT_BRACKET && key <= GLFW_KEY_GRAVE_ACCENT) {
			char_call(char(key));
		}
		else if(key >= GLFW_KEY_KP_DIVIDE && key <= GLFW_KEY_KP_ADD)
		{
			switch (key)
			{
			case GLFW_KEY_KP_DIVIDE: char_call('/'); break;
			case GLFW_KEY_KP_MULTIPLY: char_call('*'); break;
			case GLFW_KEY_KP_SUBTRACT: char_call('-'); break;
			case GLFW_KEY_KP_ADD: char_call('+'); break;
			default:
				break;
			}
		}
	}
}

void glgh_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static int left = FALSE;
	static int right = FALSE;
	int right_button;
	int left_button;
	int button, event;
	cursor_current_x = xpos;
	cursor_current_y = ypos;
	if (allowCamera == TRUE) {
		camera_mouse_callback(window, xpos, ypos);
	}
	if (mouse_call != NULL) {
		right_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		left_button = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		button = NO_BUTTON;
		event = MOUSEMOVE;
		if (left_button == GLFW_PRESS && left == FALSE) {
			left = TRUE;
			button = LEFT_BUTTON;
			event = BUTTON_DOWN;
		}
		else if (left_button == GLFW_RELEASE && left == TRUE) {
			left = FALSE;
			button = LEFT_BUTTON;
			event = BUTTON_UP;
		}
		else if (right_button == GLFW_PRESS && right == FALSE) {
			right = TRUE;
			button = RIGHT_BUTTON;
			event = BUTTON_DOWN;
		}
		else if(right_button == GLFW_RELEASE && right == TRUE){
			right = FALSE;
			button = RIGHT_BUTTON;
			event = BUTTON_UP;
		}
		mouse_call(int(xpos), scr_height - int(ypos), button, event);
	}
}
void glgh_scroll_callback(GLFWwindow* mwindow, double xoffset, double yoffset) {
	if (allowCameraScroll_scale == TRUE) {
		camera_scroll_callback(mwindow, xoffset, yoffset);
	}
}
void processInput(GLFWwindow *mwindow)
{
	static double LastTime;
	static bool allowClose = true;
	static double cam_gap = 0.0;
	static double cam_max = 0.0;
	static std::map<int, glgl_Timer>::iterator timer_itr;
	if (glfwGetKey(mwindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (allowCamera == false && allowClose) {
			glfwSetWindowShouldClose(mwindow, true);
		}
		else if(allowClose == false)
		{
			if (glfwGetTime() - LastTime > 1)
				allowClose = true;
		}
		else {
			allowClose = false;
			LastTime = glfwGetTime();
			CancelCamera();
		}
	}
	if (allowCamera == TRUE) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (timer_call != NULL) {
		double timer_gap = lastFrame - glfwGetTime();
		timer_itr = glgh_timer.begin();
		while (timer_itr != glgh_timer.end()) {
			glgh_timer[timer_itr->first].time += timer_gap;
			if (glgh_timer[timer_itr->first].time >= glgh_timer[timer_itr->first].maxTime) {
				timer_call(timer_itr->first);
			}
			timer_itr++;
		}
	}
	if (!camMove.empty()) {
		cam_gap += lastFrame - glfwGetTime();
		if (cam_max < cam_gap) {
			glgh_CameraMovement mc = camMove.back();
			camMove.pop_back();
			cam_gap = 0.0f;
			cam_max = mc.time;
			camera.BudgeCamera(0.0f, mc.time, mc.function, mc.x, mc.y, mc.z, mc.yaw, mc.pitch, mc.zoom, mc.constrainPitch);
		}
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
GLFWwindow *initWindow(const char *windowTitle)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (anti_aliasing == TRUE) {
		glfwWindowHint(GLFW_SAMPLES, 4);
	}
#ifdef  __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //something for mac
#endif //  __APPLE__

	GLFWwindow* mwindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle, NULL, NULL);
	if (mwindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(mwindow);
	glfwSetFramebufferSizeCallback(mwindow, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	return mwindow;
}



//functions 
//1. Basics
void MovePen(double x, double y, GLfloat z)
{
	pen_current_x = SCREEN_INCHES_TO_GLFLOAT_X(x);
	pen_current_y = SCREEN_INCHES_TO_GLFLOAT_Y(y);
	pen_current_z = z;
}

void DrawLine(double dx, double dy, GLfloat dz)
{
	GLfloat x = INCHES_TO_GLFLOAT_X(dx) + pen_current_x;
	GLfloat y = INCHES_TO_GLFLOAT_Y(dy) + pen_current_y;
	GLfloat z = dz                      + pen_current_z;
	glm::mat4 ViewMatrix = camera.GetViewMatrix();
	glm::vec4 start = glm::vec4(x, y, z, 1.0f);
	glm::vec4 end = glm::vec4(pen_current_x, pen_current_y, pen_current_z, 1.0f);
	start = start / ViewMatrix;
	end = end / ViewMatrix;
	//printf("pen xyz %f, %f, %f, xyz %f, %f, %f\n", pen_current_x, pen_current_y, pen_current_z, x, y, z);
	//ShapeGenerator::makeLine(VBO_Line, glm::vec3(pen_current_x, pen_current_y, pen_current_z), glm::vec3(x, y, z));
	ShapeGenerator::makeLine(VBO_Line, glm::vec3(start.x, start.y, start.z), glm::vec3(end.x, end.y, end.z));
	lineChanged = 1;
	pen_current_x = x;
	pen_current_y = y;
	pen_current_z = z;
}

void DrawArc(double r, double start, double sweep)
{
	DrawEllipticalArc(r, r, start, sweep);
}

void DrawEllipticalArc(double rx, double ry, double start, double sweep, int _startPosition, float _rotate, glm::vec3 _axis)
{
	GLfloat cx, cy;
	if (_startPosition == GLGH_LEFT) {
		cx = pen_current_x - INCHES_TO_GLFLOAT_X(rx);
	}
	else {
		cx = pen_current_x;
	}
	cy = pen_current_y;
	ShapeGenerator::makeEllipse(INCHES_TO_GLFLOAT_X(rx), INCHES_TO_GLFLOAT_Y(ry), GLfloat(start/180.0*GLGH_PI), GLfloat(sweep/180.0 * GLGH_PI), glm::vec3(cx, cy, pen_current_z));
	LinedPolygonChanged = 1;
}

double GetWindowWidth(void)
{
	return PIXELS_TO_INCHES(scr_width);
}

double GetWindowHeight(void)
{
	return PIXELS_TO_INCHES(scr_height);
}

void registerKeyboardEvent(KeyboardEventCallback callback) {
	key_call = callback;
}
void cancelKeyboardEvent() {
	key_call = NULL;
}
void registerCharEvent(CharEventCallback callback) {
	char_call = callback;
}
void cancelCharEvent() {
	char_call = NULL;
}
void registerMouseEvent(MouseEventCallback callback)
{
	mouse_call = callback;
}
void cancelMouseEvent()
{
	mouse_call = NULL;
}
void registerTimerEvent(TimerEventCallback callback)
{
	timer_call = callback;
}
void cancelTimerEvent() {
	timer_call = NULL;
}

void SetPenColor(int RGB_Start, float Start_opacity, int RGB_End, float End_opacity) 
{
	rgbToArray(glgh_current_color[0], RGB_Start);
	current_opacity[0] = Start_opacity;
	current_opacity[1] = End_opacity;
	if (RGB_End < 0) 
		rgbToArray(glgh_current_color[1], RGB_Start);
	else
		rgbToArray(glgh_current_color[1], RGB_End);
}
void SetPenColor(std::string color)
{
	SetPenColor(color_reference[color]);
}
void DefineColor(std::string name, double red, double green, double blue) 
{
	int c = 0;
	c += (unsigned char)blue*255;
	c = c << 8;
	c += (unsigned char)green*255;
	c = c << 8;
	c += (unsigned char)red*255;
	color_reference[name] = c;
}
//Miscellaneous functions
void SetWindowTitle(const char* title)
{
	glfwSetWindowTitle(window, title);
}
void ExitGraphics()
{
	delete ourShader;
	glfwSetWindowShouldClose(window, GLFW_TRUE);
	glfwTerminate();
}

void SetPenSize(int size) {
	penSize = size;
	glLineWidth(penSize);
}
void SetPointSize(int size) {
	penSize = size;
	glLineWidth(penSize);
}

void SetPointSize(float size) {
	penSize = size;
	glLineWidth(penSize);
}

int GetPointSize(float *size)
{
	if (size != NULL) {
		*size = penSize;
	}
	return int(penSize);
}
double GetCurrentX()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return PIXELS_TO_INCHES(xpos);
}

double GetCurrentY()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return PIXELS_TO_INCHES(ypos);
}
double ScaleXInches(int x) {
	return PIXELS_TO_INCHES(x);
}
double ScaleYInches(int y) {
	return PIXELS_TO_INCHES(y);
}

void startTimer(int _id, int _timeinterval)
{
	glgl_Timer _timer = {double(_timeinterval) / 1000, 0.0 };
	glgh_timer[_id] = _timer;
}
void cancelTimer(int _id)
{
	glgh_timer.erase(_id);
}

//Auxiliary functions
void ScaleInchesToGLFloat(double *x, double *y)
{
	*x = (*x) * inches_to_pixels * 2 / scr_width - 1;
	*y = 1 - (*y)*inches_to_pixels * 2 / scr_height;
}
void rgbToArray(GLfloat *arr, int rgb)
{
	unsigned char *c = (unsigned char *) &rgb;
	arr[0] = c[2] / 255.0f;
	arr[1] = c[1] / 255.0f;
	arr[2] = c[0] / 255.0f;
}

//GL Draw
void updateLine(std::vector<mLine>&VBO_Line) {
	glBindVertexArray(vao_line);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_line);
	glBufferData(GL_ARRAY_BUFFER, VBO_Line.size() * sizeof(mLine), &VBO_Line[0], GL_STATIC_DRAW);
}
void updatePolygon() {
	glBindVertexArray(VAOE);
	glBindBuffer(GL_ARRAY_BUFFER, VBOE);
	glBufferData(GL_ARRAY_BUFFER, LinedEllipse.VBO.size() * sizeof(Vertex), &(LinedEllipse.VBO[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOE);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, LinedEllipse.EBO.size() * sizeof(GLuint), &(LinedEllipse.EBO[0]), GL_STATIC_DRAW);
}

void camera_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void camera_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void EnableCamera()
{
	allowCamera = true;
	firstMouse = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void CancelCamera(int Mode)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	allowCamera = false;
	if (Mode == GLGH_BACK) {
		camera.BudgeCamera(0.0f, 0.7f);
	}
}

void MoveCamera(float time, int function, float x, float y, float z, float yaw, float pitch, float zoom, bool constrainPitch) 
{
	//camera.BudgeCamera(0, time, function, x, y, z, yaw, pitch, zoom, constrainPitch);
	glgh_CameraMovement cm = { time, function, x, y, z, yaw, pitch, zoom, constrainPitch };
	camMove.push_back(cm);
}

