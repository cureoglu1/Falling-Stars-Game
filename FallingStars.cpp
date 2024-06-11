/*********
ADDITIONAL FEATURES:
1. Start/stop button
2. Randomly moving UFO
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define HALFWIDTH 500 // to make easy calculations half of the width and height of the screen is also defined
#define HALFHEIGHT 300
/* Global Variables for Template File */
//defining variables
typedef vec_t point_t;
typedef struct {
	point_t pos;
	bool hit;
}fstar_t;

typedef struct {
	point_t pos;
}ufo_t;

bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
float ufoSpeed = 2.0;
//defining objects
fstar_t fstar = { {0,200} };
ufo_t ufo = { {250,250} };
bool animate = true;
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void vertex(point_t P, point_t Tr, double angle) {
	double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
	double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
	glVertex2d(xp, yp);
}

void background() {
	//space
	glBegin(GL_POLYGON);
	glColor3ub(51, 51, 102);
	glVertex2f(-500, 300);
	glVertex2f(500, 300);
	glColor3ub(45, 56, 58);
	glVertex2f(500, -300);
	glVertex2f(-500, -300);
	glEnd();

	//world
	glColor3f(0, 0, 1);
	circle(0, -1200, 1000);

	glColor3f(0, 1, 0);
	circle(330, -300, 30);
	circle(300, -300, 40);
	circle(270, -300, 30);

	circle(130, -270, 30);
	circle(100, -270, 40);
	circle(70, -270, 30);

	circle(-130, -285, 30);
	circle(-100, -285, 40);
	circle(-70, -285, 30);

	circle(-350, -320, 30);
	circle(-320, -320, 40);
	circle(-290, -320, 30);
}
void stars() {

	glColor3ub(252, 252, 75);
	// when you divide the screen to four your first area;
	circle(67 - HALFWIDTH, HALFHEIGHT - 91, 3);
	circle(206 - HALFWIDTH, HALFHEIGHT - 151, 3);
	circle(378 - HALFWIDTH, HALFHEIGHT - 94, 3);
	circle(471 - HALFWIDTH, HALFHEIGHT - 273, 3);
	circle(331 - HALFWIDTH, HALFHEIGHT - 242, 3);
	// second area
	circle(643 - HALFWIDTH, HALFHEIGHT - 100, 3);
	circle(659 - HALFWIDTH, HALFHEIGHT - 234, 3);
	circle(815 - HALFWIDTH, HALFHEIGHT - 149, 3);
	circle(812 - HALFWIDTH, HALFHEIGHT - 252, 3);
	circle(945 - HALFWIDTH, HALFHEIGHT - 90, 3);
	// third area
	circle(587 - HALFWIDTH, HALFHEIGHT - 436, 3);
	circle(685 - HALFWIDTH, HALFHEIGHT - 324, 3);
	circle(750 - HALFWIDTH, HALFHEIGHT - 456, 3);
	circle(888 - HALFWIDTH, HALFHEIGHT - 348, 3);
	circle(930 - HALFWIDTH, HALFHEIGHT - 500, 3);
	// fourth area
	circle(450 - HALFWIDTH, HALFHEIGHT - 450, 3);
	circle(300 - HALFWIDTH, HALFHEIGHT - 410, 3);
	circle(150 - HALFWIDTH, HALFHEIGHT - 470, 3);
	circle(86 - HALFWIDTH, HALFHEIGHT - 318, 3);
}
void drawUfo() {

	glColor3ub(180, 180, 180);
	circle(ufo.pos.x + 0, ufo.pos.y + 10, 23);
	glColor3ub(11, 208, 31);
	circle(ufo.pos.x + 0, ufo.pos.y + 10, 22);
	// Body
	glColor3ub(209, 209, 209);
	glBegin(GL_POLYGON);
	glVertex2f(ufo.pos.x - 50, ufo.pos.y - 10);
	glVertex2f(ufo.pos.x - 20, ufo.pos.y + 10);//2
	glVertex2f(ufo.pos.x + 20, ufo.pos.y + 10);//3
	glVertex2f(ufo.pos.x + 50, ufo.pos.y - 10);
	glVertex2f(ufo.pos.x + 50, ufo.pos.y - 20);//5
	glVertex2f(ufo.pos.x - 50, ufo.pos.y - 20);//6
	glEnd();

	// Windows
	glColor3ub(0, 0, 0);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex2f(ufo.pos.x - 30, ufo.pos.y - 5);
	glVertex2f(ufo.pos.x - 10, ufo.pos.y - 5);
	glVertex2f(ufo.pos.x + 10, ufo.pos.y - 5);
	glVertex2f(ufo.pos.x + 30, ufo.pos.y - 5);
	glEnd();
}
void fallingStar() {
	//star
	glColor3ub(252, 252, 75);
	circle(fstar.pos.x - 2, fstar.pos.y, 4);

	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3ub(252, 252, 75);
	glVertex2f(fstar.pos.x, fstar.pos.y);
	//shining part
	glColor3f(1, 1, 1);
	glVertex2f(fstar.pos.x - 60, fstar.pos.y);
	glEnd();

}
void board() {
	//board for describe more 
	glLineWidth(1);
	glColor3f(1, 1, 1);
	vprint2(-450, 260, .1, "Left Click to place fallingstar and hit the UFO - MUSTAFA CUREOGLU");
	vprint2(-450, 260, .1, "_________________________________________________");
	vprint2(-450, 240, .1, "Spacebar to start/stop - ESC to exit - Ufo will do nothing until see another star");

}
//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	background();
	stars();
	drawUfo();
	fallingStar();
	board();

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{




	// exit when ESC is pressed.
	if (key == 27)
		exit(0);



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// ESC to exit
	if (key == 27)
		exit(0);
	if (key == ' ') { // spacebar to stop
		animate = !animate;
	}


	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP:
		up = true;
		break;
	case GLUT_KEY_DOWN:
		down = true;
		break;
	case GLUT_KEY_LEFT:
		left = true;
		break;
	case GLUT_KEY_RIGHT:
		right = true;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP:
		up = false;
		break;
	case GLUT_KEY_DOWN:
		down = false;
		break;
	case GLUT_KEY_LEFT:
		left = false;
		break;
	case GLUT_KEY_RIGHT:
		right = false;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
	{
		fstar.pos.x = x - winWidth / 2;
		fstar.pos.y = winHeight / 2 - y;
		animate = true;
	}



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	//random function call
	srand(time(NULL));
	if (animate) {
		//for randomly moving
		float dx = (float)(rand() % 3 - 1);
		float dy = (float)(rand() % 3 - 1);

		ufo.pos.x += dx * ufoSpeed;
		ufo.pos.y += dy * ufoSpeed;

		if (ufo.pos.x > HALFWIDTH || ufo.pos.x < -HALFWIDTH)
			ufo.pos.x -= dx * 150;
		if (ufo.pos.y > HALFHEIGHT || ufo.pos.y < -HALFHEIGHT)
			ufo.pos.y -= dy * 10;

		//when falling star hits the ufo animation stops 
		if (sqrt(fstar.pos.x - ufo.pos.x) < 5 && sqrt(fstar.pos.y - ufo.pos.y) < 5 || fstar.pos.x == 0) {

			animate = false;

			fstar.pos.x = 9999;
			fstar.pos.y = 0;

		}


		vec_t vel = mulV(6, unitV(subV(ufo.pos, fstar.pos)));


		fstar.pos = addV(fstar.pos, vel);

	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Falling stars hits the UFO - Mustafa Cureoglu");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}