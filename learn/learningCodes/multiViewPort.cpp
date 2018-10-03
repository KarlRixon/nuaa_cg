#include<gl/glut.h>
void initial() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
}
void triangle(GLsizei mode) {
	if (mode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0, 5.0);
	glVertex2f(5.0, -5.0);
	glVertex2f(-5.0, -5.0);
	glEnd();
}
void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glViewport(0, 0, 200, 200);
	triangle(1);
	glColor3f(0.0, 0.0, 1.0);
	glViewport(200, 0, 200, 200);
	triangle(2);
	glFlush();

}
void main(int argc, char** argv) {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 200);
	glutCreateWindow("¶àÊÓÇø");
	initial();
	glutDisplayFunc(Display);
	glutMainLoop();
}