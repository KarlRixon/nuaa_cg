#include<gl/glut.h>
void Initial() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}
void ChangeSize(int w, int h) {
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat fAspect;
	fAspect = (float)w / (float)h;
	gluPerspective(45.0, fAspect, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();;
}
void Display() {
	static float fElect = 0.0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -250.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(12.0, 15, 15);
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(fElect, 0.0, 1.0, 0.0);
	glTranslatef(90.0, 0.0, 0.0);
	glutSolidSphere(6.0, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glRotatef(45.0, 0.0, 0.0, 1.0);
	glRotatef(fElect, 0.0, 1.0, 0.0);
	glTranslatef(-70.0, 0.0, 0.0);
	glutSolidSphere(6.0, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-45.0, 0.0, 0.0, 1.0);
	glRotatef(fElect, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, 60.0);
	glutSolidSphere(6.0, 15, 15);
	glPopMatrix();

	fElect += 10.0;
	if (fElect > 360.0)fElect = 10.0;
	glutSwapBuffers();
}
void TimerFunc(int value) {
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunc, 1);
}
void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("分子动画示例");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(Display);
	glutTimerFunc(500, TimerFunc, 1);
	Initial();
	glutMainLoop();
}