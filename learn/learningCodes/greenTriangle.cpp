#include<gl/glut.h>
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-0.7, -0.7);
	glVertex2f(0.7, -0.7);
	glVertex2f(-0.0, 0.7);
	glEnd();
	glFlush();
}
void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_2D);
}
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Green Triangle");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}