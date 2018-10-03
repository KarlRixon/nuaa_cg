#include<gl/glut.h>

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_FILL);//设置正面填充模式
	glPolygonMode(GL_BACK, GL_LINE);//设置反面线形模式
	glFrontFace(GL_CCW);//设置逆时针方向为正面
	glBegin(GL_POLYGON);//在左下方按逆时针绘制一个正方形
	glVertex2f(-0.5, -0.5);
	glVertex2f(0.0, -0.5);
	glVertex2f(0.0, 0.0);
	glVertex2f(-0.5, 0.0);
	glEnd();
	glBegin(GL_POLYGON);//在右上方按顺时针绘制一个正方形
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0.0);
	glEnd();
	glFlush();
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("test");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}