// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <stdlib.h>

#include <math.h>

#include <stdio.h>


static int year = 0, spin = 0, day = 0; static GLint fogMode; const int n = 100; const GLfloat R = 1.0f; const GLfloat Pi = 3.1415926536f;

void DrawCircle() {



	int  i;

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINE_LOOP);



	for (i = 0; i < n; ++i)

	{

		glColor3f(1.0, 1.0, 0.0);

		glVertex2f(R*cos(2 * Pi / n*i), R*sin(2 * Pi / n*i));

	}



	glEnd();

	glFlush();

}

void init(void) {

	GLfloat position[] = { 0.5, 0.5, 3.0, 0.0 };

	glEnable(GL_DEPTH_TEST);                          //防止遮挡    glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);



	{

		GLfloat mat[3] = { 0.1745, 0.01175, 0.01175 };

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat);

		mat[0] = 0.61424; mat[1] = 0.04136; mat[2] = 0.04136;

		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);

		mat[0] = 0.727811; mat[1] = 0.626959; mat[2] = 0.626959;

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat);

		glMaterialf(GL_FRONT, GL_SHININESS, 0.6*128.0);

	}



	glEnable(GL_FOG);



	{

		GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };

		fogMode = GL_EXP;

		glFogi(GL_FOG_MODE, fogMode);

		glFogfv(GL_FOG_COLOR, fogColor);

		glFogf(GL_FOG_DENSITY, 0.35);

		glHint(GL_FOG_HINT, GL_DONT_CARE);

		glFogf(GL_FOG_START, 1.0);

		glFogf(GL_FOG_END, 5.0);

	}



	glClearColor(0.5, 0.9, 0.9, 1.0);  /* fog color */



}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0, 1.0, 1.0);

	glPushMatrix(); //记住自己的位置

	glutSolidSphere(1.0, 20, 16);   /* 画太阳半径、 20经度、16纬度*/

	glRotatef(spin, 0.0, 1.0, 0.0);  //自转，绕着一个向量以给定角度旋转（正的为逆时针）

	glTranslatef(2.0, 1.0, 0.0);

	glRotatef(spin, 1.0, 0.0, 0.0); //公转

	glRectf(0.1, 0.1, 0.5, 0.5);

	glColor3f(0.0, 0.0, 1.0);

	glutWireSphere(0.2, 8, 8);    /* 画第一颗小行星 */

	glColor3f(1.0, 0.0, 0.0);

	glTranslatef(2.0, 1.0, 0.0);

	glRotatef(2 * spin, 0.0, 1.0, 0.0);

	glutSolidSphere(0.5, 16, 8);

	glPopMatrix();//回到原来的位置    glutSwapBuffers();

}

void spinDisplay(void) {

	spin = spin + 2;

	if (spin > 360)

		spin = spin - 360;

	glutPostRedisplay();

}

void mouse(int button, int state, int x, int y) {

	switch (button)

	{

	case GLUT_LEFT_BUTTON:

		if (state == GLUT_DOWN)

			glutIdleFunc(spinDisplay);

		break;



	case GLUT_MIDDLE_BUTTON:

		if (state == GLUT_DOWN)

			glutIdleFunc(NULL);

		break;



	default:

		break;

	}



}

void reshape(int w, int h) {

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.5, 20.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(0.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

void keyboard(unsigned char key, int x, int y) {

	switch (key) {

	case 'd':

		day = (day + 10) % 360;

		glutPostRedisplay();

		break;

	case 'D':

		day = (day - 10) % 360;

		glutPostRedisplay();

		break;

	case 'y':

		year = (year + 5) % 360;

		glutPostRedisplay();

		break;

	case 'Y':

		year = (year - 5) % 360;

		glutPostRedisplay();

		break;

	case 27:

		exit(0);

		break;

	default:

		break;

	}

}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(500, 400);

	glutInitWindowPosition(100, 100);

	glutCreateWindow("OpengGL 程序设计测试");

	init();

	//glutDisplayFunc(DrawCircle); 

	//glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);    glutMouseFunc(mouse);

	glutMainLoop();



	return 0;

}


//
//
//void init(void)
//{
//	glClearColor(1.0, 1.0, 1.0, 0.0);  //窗口颜色(红:[0.0~1.0]R, 绿:[0.0~1.0]G，蓝:[0.0~1.0]B, 透明度:[0.0~1.0]A)
//									   /*使用正投影将世界坐标系二位矩形区域的内容映射到屏幕上，区域的x坐标值从0.0到200.0，y坐标值从0.0到150.0*/
//	glMatrixMode(GL_PROGRAM);
//	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
//}
//
//void lineSegment(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);//GL_COLOR_BUFFER_BIT是一个OpenGL符号常量，用来指定它的颜色缓存（刷新缓存）中的位值，该缓存将使用                                      glClearColor函数中指定的值来设定。
//	glColor3f(1.0f, 0.0f, 0.0f);//设置线段颜色为红色
//
//								/*定义了一个从整数笛卡尔端点坐标（180，15）到(10, 145)的二维直线段*/
//	glBegin(GL_LINES);
//	glVertex2i(180, 15);
//	glVertex2i(10, 145);
//	glEnd();
//
//	glFlush(); //执行所有的OpenGL程序
//}
//int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);//初始化GULT
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置显示模式
//	glutInitWindowPosition(50, 100);//设置左上角窗口显示位置
//	glutInitWindowSize(400, 300);//设置窗口显示的宽与高
//	glutCreateWindow("An Example OpenGL Program！");//创建一个窗口
//	init();//执行初始化程序
//	glutDisplayFunc(lineSegment);//把图形显示在窗口
//	glutMainLoop();//显示所有并进入等待状态
//}
