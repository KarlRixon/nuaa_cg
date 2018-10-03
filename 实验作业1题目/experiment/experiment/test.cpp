#include<gl/glut.h>
#include<stdlib.h>
#include<stdio.h>

const GLdouble FRUSTDIM = 100.0f;

void display() {
	static GLfloat wall_mat[] = { 1.f, 1.f, 1.f, 1.f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat);

	// sphere
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(-50.0, -30.0, -350.0);
	glutSolidSphere(20.0, 40.0, 50.0);
	glPopMatrix();

	// cone
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(0.0, -50.0, -350.0);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(15.0, 40.0, 40.0, 50.0);
	glPopMatrix();

	// teapot
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslated(50.0, -30.0, -350.0);
	glutSolidTeapot(20);
	glPopMatrix();

	// cylinder
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(-50.0, 40.0, -350.0);
	glRotated(50.0, 2.0, 3.0, 0.0);
	glutSolidCube(30);
	glPopMatrix();

	// torus
	glColor3f(1.0, 0.0, 1.0);
	glPushMatrix();
	glTranslated(0.0, 40.0, -350.0);
	glRotated(40, 2.0, 3.0, 0.0);
	glutSolidTorus(8.0, 15.0, 100, 100);
	glPopMatrix();

	// dodecahedron
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslated(50.0, 40.0, -350.0);
	glScaled(10.0, 10.0, 10.0);
	glutSolidDodecahedron();
	glPopMatrix();


	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	// floor
	glBegin(GL_QUADS);
	glNormal3f(0.f, 1.f, 0.f);
	glTexCoord2i(0, 0);
	glVertex3f(-100.f, -100.f, -320.f);
	glTexCoord2i(1, 0);
	glVertex3f(100.f, -100.f, -320.f);
	glTexCoord2i(1, 1);
	glVertex3f(100.f, -100.f, -520.f);
	glTexCoord2i(0, 1);
	glVertex3f(-100.f, -100.f, -520.f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// walls
	glBegin(GL_QUADS);
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

	/* ceiling */
	glNormal3f(0.f, -1.f, 0.f);
	glVertex3f(-100.f, 100.f, -320.f);
	glVertex3f(-100.f, 100.f, -520.f);
	glVertex3f(100.f, 100.f, -520.f);
	glVertex3f(100.f, 100.f, -320.f);

	/* back wall */
	glNormal3f(0.f, 0.f, 1.f);
	glVertex3f(-100.f, -100.f, -520.f);
	glVertex3f(100.f, -100.f, -520.f);
	glVertex3f(100.f, 100.f, -520.f);
	glVertex3f(-100.f, 100.f, -520.f);
	glEnd();

	glutSwapBuffers();
	glFlush();

	//glutPostRedisplay();
}

void init() {
	static GLfloat lightpos[] = { 50.f, 50.f, -320.f, 1.f };
	static GLfloat sphere_mat[] = { 1.f, .5f, 0.f, 1.f };
	static GLfloat cone_mat[] = { 0.f, .5f, 1.f, 1.f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glFrontFace(GL_CCW);
	glEnable(GL_COLOR_MATERIAL);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-FRUSTDIM, FRUSTDIM, -FRUSTDIM, FRUSTDIM, 320.f, 640.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {

}

void idle() {

}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(200, 200);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_ACCUM | GLUT_DOUBLE);
	(void)glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}