#include<gl/glut.h>
#include<iostream>

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(0.0, 0.7);

	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(-0.7, -0.7);

	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(0.7, -0.7);

	glEnd();

	glFlush();
}

void init() {
	glClearColor(0.35, 0.35, 0.35, 0.5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("helloworld");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}