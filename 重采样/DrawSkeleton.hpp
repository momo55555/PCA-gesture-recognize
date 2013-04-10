#ifndef _DRAWSKELETON_H
#define  _DRAWSKELETON_H

#include <GLTools.h>
#include <GLShaderManager.h>
#include <openglut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glew.h>
#include "DataProc.hpp"
//#include "K-Means.hpp"
//#include "Curve.hpp"

/*
    Time: 12-07-2012
    @Author: Rudy Snow
    Description: ��OpenGL������̬����ͼ
*/

using namespace std;

// ����glut���ڴ�С��λ�õ�һЩ����
const GLint winX = 1000, winY = 100;
const GLint windowWidth = 800, windowHeight = 800;
const char* windowTitle = "ReSampling";

// �ı䴰�ڴ�С���ô˺�������������Ϊ���ɸı��С
void reshape(int w, int h)
{
	glViewport(0, 0,(GLsizei)windowWidth, (GLsizei)windowHeight);
	glutReshapeWindow(windowWidth, windowHeight);
}

// ��Ⱦ�ص�����
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_MULTISAMPLE);

	for (int i = 0; i < 4; ++i)
	{
		// ��鵱ǰUsers�Ƿ����
		if (currentUsers[i] == true)
		{
			glColor3f(0.0, 1.0, 0.0f);
			glLineWidth(2.5f);

			if(triggered[i] == 1) glColor3f(0.0, 0.0, 1.0f);

			//for(int j = 0; j < 24; ++j)
			//{
			//	skelPointsIn[i][j].Y -= 200.0f;
			//}

			// ������
			glBegin(GL_LINE_STRIP);
			glVertex2f(skelPointsIn[i][0].X/1000.0f, skelPointsIn[i][0].Y/1000.0f);
			glVertex2f(skelPointsIn[i][1].X/1000.0f, skelPointsIn[i][1].Y/1000.0f);
			glVertex2f(skelPointsIn[i][2].X/1000.0f, skelPointsIn[i][2].Y/1000.0f);
			glVertex2f(skelPointsIn[i][3].X/1000.0f, skelPointsIn[i][3].Y/1000.0f);
			glEnd();

			// ������
			glBegin(GL_LINE_STRIP);
			glVertex2f(skelPointsIn[i][9].X/1000.0f, skelPointsIn[i][9].Y/1000.0f);
			glVertex2f(skelPointsIn[i][8].X/1000.0f, skelPointsIn[i][8].Y/1000.0f);
			glVertex2f(skelPointsIn[i][7].X/1000.0f, skelPointsIn[i][7].Y/1000.0f);
			glVertex2f(skelPointsIn[i][6].X/1000.0f, skelPointsIn[i][6].Y/1000.0f);
			glVertex2f(skelPointsIn[i][5].X/1000.0f, skelPointsIn[i][5].Y/1000.0f);
			glVertex2f(skelPointsIn[i][1].X/1000.0f, skelPointsIn[i][1].Y/1000.0f);
			glEnd();

			// ������
			glBegin(GL_LINE_STRIP);
			glVertex2f(skelPointsIn[i][15].X/1000.0f, skelPointsIn[i][15].Y/1000.0f);
			glVertex2f(skelPointsIn[i][14].X/1000.0f, skelPointsIn[i][14].Y/1000.0f);
			glVertex2f(skelPointsIn[i][13].X/1000.0f, skelPointsIn[i][13].Y/1000.0f);
			glVertex2f(skelPointsIn[i][12].X/1000.0f, skelPointsIn[i][12].Y/1000.0f);
			glVertex2f(skelPointsIn[i][11].X/1000.0f, skelPointsIn[i][11].Y/1000.0f);
			glVertex2f(skelPointsIn[i][1].X/1000.0f, skelPointsIn[i][1].Y/1000.0f);
			glEnd();

			// ������
			glBegin(GL_LINE_STRIP);
			glVertex2f(skelPointsIn[i][19].X/1000.0f, skelPointsIn[i][19].Y/1000.0f);
			glVertex2f(skelPointsIn[i][18].X/1000.0f, skelPointsIn[i][18].Y/1000.0f);
			glVertex2f(skelPointsIn[i][17].X/1000.0f, skelPointsIn[i][17].Y/1000.0f);
			glVertex2f(skelPointsIn[i][16].X/1000.0f, skelPointsIn[i][16].Y/1000.0f);
			glVertex2f(skelPointsIn[i][4].X/1000.0f, skelPointsIn[i][4].Y/1000.0f);
			glEnd();

			// ������
			glFlush();
			glBegin(GL_LINE_STRIP);
			glVertex2f(skelPointsIn[i][23].X/1000.0f, skelPointsIn[i][23].Y/1000.0f);
			glVertex2f(skelPointsIn[i][22].X/1000.0f, skelPointsIn[i][22].Y/1000.0f);
			glVertex2f(skelPointsIn[i][21].X/1000.0f, skelPointsIn[i][21].Y/1000.0f);
			glVertex2f(skelPointsIn[i][20].X/1000.0f, skelPointsIn[i][20].Y/1000.0f);
			glVertex2f(skelPointsIn[i][4].X/1000.0f, skelPointsIn[i][4].Y/1000.0f);
			glEnd();

			// ��������
			for(int iter = 0; iter < 24; ++iter)
			{
				glColor3f(1.0, 0.0, 0.0f);
				glPointSize(7.0f);
				glBegin(GL_POINTS);
				glVertex2f(skelPointsIn[i][iter].X/1000.0f, skelPointsIn[i][iter].Y/1000.0f);
				glEnd();
			}
			glFlush();

			//for(int j = 0; j < 24; ++j)
			//{
			//	skelPointsIn[i][j].Y += 200.0f;
			//}
		}
	}

	glutSwapBuffers();
}

// ע����лص�����
void idle()
{
	collectData();
}

// ע��һ����̰����ص�����
void keyboard(unsigned char c, int x, int y)
{
	if(c == 'n' || c == 'N')
	{
		closeFlag = !closeFlag;
		if(!closeFlag) cout << "��ʼ��������" << endl;
		else cout << "ֹͣ��������" << endl;
	}
}

// ��ʼ��glut����
void glInit(int* argc, char* argv[])
{
	// ��ʼ��Ϊδ����
	memset(triggered, 0, sizeof(triggered));

	// ��ʼ��glut����
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(winX, winY);
	glutInitWindowSize(windowWidth, windowHeight);
	int gMainHandle = glutCreateWindow(windowTitle);
	glutSetWindow(gMainHandle);
	glClearColor (0.0, 0.0, 0.0, 0.0);

	// ע��ص�����
	glutDisplayFunc(&renderScene);
	glutIdleFunc(&idle);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);

	// ʹ��OpenGlut��glutMainLoopEvent������ʹ�ùر�glut����ʱ���ݽ����̰߳�ȫ����
	while(1)
	{
		glutMainLoopEvent();
		collectData();
	}
}

#endif