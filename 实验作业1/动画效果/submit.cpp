/*****************************************************************************
FILE : submit.c (Assignment 1)
NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
Student Information
Student ID: 161620222
Student Name: 刘鸿搏
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <GL/glut.h>
const GLdouble FRUSTDIM = 100.0f;
GLfloat spherex = -50.0;
GLfloat conex = -90.0;
GLfloat teapotx = 20.0;
GLfloat cylinderx = -350.0;
GLfloat torusx = 10;
GLfloat dodecahedronx = 0.1;

void init(void) // All Setup For OpenGL Goes Here
{
	glEnable(GL_NORMALIZE);		// 不加此项会出现放大后物体变暗问题
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glFrontFace(GL_CCW);
	glEnable(GL_COLOR_MATERIAL);
	
}

void display(void) // Here's Where We Do All The Drawing
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	//必须放在display中否则会有bug
	// TODO:
	// Place light source here
	GLfloat lightpos[] = { 50.f, 50.f, -320.f, 1.f };
	GLfloat light0_mat1[] = { 0.1, 0.1, 0.1, 0.3f };
	GLfloat light0_diff[] = { 1.0, 1.0, 1.0, 0.3 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);

	GLfloat no_mat[] = { 0.0,0.0,0.0,1.0 };
	GLfloat mat_diffuse[] = { 0.9,0.9,0.9,1.0 };
	GLfloat mat_specular[] = { 0.3,0.3,0.3,1.0 };
	GLfloat high_shininess[] = { 20.0 };
	GLfloat high_mat[] = { dodecahedronx, dodecahedronx, dodecahedronx, 0.5 };

	//glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, high_mat);
	
	// TODO:
	// Draw walls and objects here
	static GLfloat wall_mat[] = { 1.f, 1.f, 1.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat);

	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_QUADS);
	// floor
	glNormal3f(0.f, 1.f, 0.f);
	glVertex3f(-100.f, -100.f, -320.f);
	glVertex3f(100.f, -100.f, -320.f);
	glVertex3f(100.f, -100.f, -520.f);
	glVertex3f(-100.f, -100.f, -520.f);
	// left wall
	glNormal3f(1.f, 0.f, 0.f);
	glVertex3f(-100.f, -100.f, -320.f);
	glVertex3f(-100.f, -100.f, -520.f);
	glVertex3f(-100.f, 100.f, -520.f);
	glVertex3f(-100.f, 100.f, -320.f);
	// right wall
	glNormal3f(-1.f, 0.f, 0.f);
	glVertex3f(100.f, -100.f, -320.f);
	glVertex3f(100.f, 100.f, -320.f);
	glVertex3f(100.f, 100.f, -520.f);
	glVertex3f(100.f, -100.f, -520.f);
	// ceiling
	glNormal3f(0.f, -1.f, 0.f);
	glVertex3f(-100.f, 100.f, -320.f);
	glVertex3f(-100.f, 100.f, -520.f);
	glVertex3f(100.f, 100.f, -520.f);
	glVertex3f(100.f, 100.f, -320.f);
	// back wall
	glNormal3f(0.f, 0.f, 1.f);
	glVertex3f(-100.f, -100.f, -520.f);
	glVertex3f(100.f, -100.f, -520.f);
	glVertex3f(100.f, 100.f, -520.f);
	glVertex3f(-100.f, 100.f, -520.f);

	glEnd();

	// TODO:
	// Add animation here

	// sphere
	
	glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(spherex, -30.0, -350.0);
		glRotatef(0.0, 1.0, 0.0, 0.0);
		glutSolidSphere(20.0, 50.0, 50.0);
	glPopMatrix();

	// cone
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(0.0, -50.0, -370.0);
	glRotated(conex, 1.0, 0.0, 0.0);
	glutSolidCone(15.0, 40.0, 40.0, 50.0);
	glPopMatrix();

	// teapot
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslated(50.0, -30.0, -350.0);
	glutSolidTeapot(teapotx);
	glPopMatrix();

	// cylinder
	//GLfloat shininess[] = { 100.0 };
	//glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(-50.0, 40.0, cylinderx);
	glRotated(50.0, 2.0, 3.0, 0.0);
	glutSolidCube(30);
	glPopMatrix();

	// torus
	glColor3f(1.0, 0.0, 1.0);
	glPushMatrix();
	glTranslated(0.0, 40.0, -350.0);
	glRotated(40, 2.0, 3.0, 0.0);
	glutSolidTorus(8.0, 15.0, torusx, torusx);
	glPopMatrix();

	// dodecahedron

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslated(50.0, 40.0, -350.0);
	glScaled(13.0, 13.0, 13.0);
	glRotated(20, 3, 5, 0);
	glutSolidDodecahedron();
	glPopMatrix();

	
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-FRUSTDIM, FRUSTDIM, -FRUSTDIM, FRUSTDIM, 320., 640.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void help() {
	printf("[1]\tsphere translating\n");
	printf("[2]\tcone rotaating\n");
	printf("[3]\tteapot scaling");
	printf("[h]\tprint this information\n");
	printf("[p]\tpause\n");
	printf("[4]\tcylinder translating\n");
	printf("[5]\ttorus transforming\n");
	printf("[6]\tlight shining\n");
}

int option = 0;

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{
	switch (key)
	{
	case'\033'://press 'esc' to quit
		exit(0);
		break;

		// TODO:
		// Add keyboard control here
	case'1':
		printf("sphere translating\n");
		//glutIdleFunc(sphereDisplay);
		option = 1;
		break;
	case'2':
		printf("cone rotating\n");
		//glutIdleFunc(coneDisplay);
		option = 2;
		break;
	case'3':
		printf("teapot scaling\n");
		//glutIdleFunc(teapotDisplay);
		option = 3;
		break;
	case'h':
		help();
		break;
	case'p':
		printf("paused\n");
		option = 4;
		break;
	case'4':
		printf("cylinder translating\n");
		option = 5;
		break;
	case'5':
		printf("torus transforming");
		option = 6;
		break;
	case'6':
		printf("light shining\n");
		option = 7;
		break;
	default:
		printf("error\ttry 'h' for help\n");
		break;
	}
}

double step = 1.0;

void idle()		//部分物体添加了同步化操作
{
	if (option == 1) {
		if (spherex <= -70.0) {
			spherex = -70.0;
		}
		if (spherex >= -30.0) {
			spherex = -30.0;
		}
		if (spherex == -70.0 || spherex == -30.0) {
			step *= -1.0;
		}
		spherex += step;
	}
	if (option == 2) {
		conex = (double)((int)--conex%360);
		//Sleep(100);
	}
	if (option == 3) {
		if (teapotx == 30 || teapotx == 10) {
			step *= -1;
		}
		Sleep(10);
		teapotx += step;
	}
	if (option == 4) {
	}
	if (option == 5) {
		if (cylinderx >= -350.0) {
			cylinderx = -350.0;
		}
		if (cylinderx <= -500.0) {
			cylinderx = -500.0;
		}
		if (cylinderx == -350 || cylinderx == -500) {
			step *= -1;
		}
		cylinderx += step;
	}
	if (option == 6) {
		if (torusx == 3 || torusx == 50) {
			step *= -1;
		}
		Sleep(100-torusx*2);
		torusx += step;
	}
	if (option == 7) {
		if (dodecahedronx <= 0.0 || dodecahedronx >= 0.5) {
			step *= -1.0;
		}
		if (dodecahedronx <= 0.0) {
			dodecahedronx = 0.0;
		}
		if (dodecahedronx >= 0.7) {
			dodecahedronx = 0.7;
		}
		Sleep((1 - dodecahedronx )*(1 - dodecahedronx )*100);
		dodecahedronx += step*0.1;
	}
}

void main(int argc, char** argv)
{
	printf("hello\n");
	/*Initialization of GLUT Library */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	//双缓冲
	//glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);		//单缓冲

	/*Create a window with title specified */
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 1");
	help();

	init();	/*not GLUT call, initialize several parameters */

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

}