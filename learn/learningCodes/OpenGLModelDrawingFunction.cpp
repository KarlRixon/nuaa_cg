#include<gl/glut.h>
static GLsizei iMode = 1;
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
GLUquadric *obj;
void Initial() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);
}
void ChangeSize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.5f, 1.5f, -1.5f, 1.5f);
}
void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	switch (iMode) {
	case 1:
		glutWireTetrahedron();break;
	case 2:
		glutSolidTetrahedron();break;
	case 3:
		glutWireOctahedron();break;
	case 4:
		glutSolidOctahedron();break;
	case 5:
		glutWireSphere(1.0f, 15, 15);break;
	case 6:
		glutSolidSphere(1.0f, 15, 15);break;
	case 7:
		glutWireTeapot(1.0f);break;
	case 8:
		glutSolidTeapot(1.0f);break;
	case 9:
		gluSphere(obj, 1.0f, 15, 15);break;
	case 10:
		gluCylinder(obj, 1.0f, 0.0f, 1.0f, 15, 15);break;
	case 11:
		gluPartialDisk(obj, 0.3f, 0.8f, 15, 15, 30.0f, 260.0f);break;
	default:break;
	}
	glFlush();
}
void ProcessMenu(int value) {
	iMode = value;
	glutPostRedisplay();
}
void SpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	if (xRot > 356.0f)
		xRot = 0.0f;
	if (xRot < -1.0f)
		xRot = 355.0f;
	if (yRot > 356.0f)
		yRot = 0.0f;
	if (yRot < -1.0)
		yRot = 355.0;
	glutPostRedisplay();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("OpenGL模型绘制函数示例");
	int nGlutPolyMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("线框正四面体", 1);
	glutAddMenuEntry("实体正四面体", 2);
	glutAddMenuEntry("线框正八面体", 3);
	glutAddMenuEntry("实体正八面体", 4);
	int nGlutCurveMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("线框球面", 5);
	glutAddMenuEntry("实体球面", 6);
	glutAddMenuEntry("线框茶壶", 7);
	glutAddMenuEntry("实体茶壶", 8);
	int nGluCurveMenu = glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("线框球面", 9);
	glutAddMenuEntry("线框圆锥面", 10);
	glutAddMenuEntry("线框圆环面", 11);
	int nMainMenu = glutCreateMenu(ProcessMenu);
	glutAddSubMenu("GLUT多面体", nGlutPolyMenu);
	glutAddSubMenu("GLUT曲面", nGlutCurveMenu);
	glutAddSubMenu("GLU曲面", nGluCurveMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(Display);
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	Initial();
	glutMainLoop();
	return 0;
}