// rose.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL\glut.h>
#include "glcamera.h"
#include <windows.h>

//#define DEBUG
using namespace std;
//���ڱ���ģ����Ϣ�����ݽṹ
vector<string> texture;
vector<vector<GLfloat> > ambient;
vector<vector<GLfloat> > diffuse;
vector<vector<GLfloat> > specular;
vector<vector<GLfloat> > emission;
vector<GLfloat> shininess;
vector<GLint> texture_index;
vector<vector<GLfloat> > vertex;
vector<vector<GLfloat> > texture_pos;
vector<vector<GLfloat> > normals;
vector<GLfloat> zoom_factor(3);

GLint texture_count;
GLint material_count;
GLint vertex_count;
GLint t_count;
GLint n_count;
GLint submodel_count;

//////////////////////////////////////////////////////////////////////////
//camera
GLCamera *camera;
//////////////////////////////////////////////////////////////////////////

class submodel
{
public:
	GLint triangle_count;
	GLint material_index;
	vector<vector<unsigned int> > v;
	vector<vector<unsigned int> > n;
	vector<vector<unsigned int> > t;
};
vector<submodel> smodel;

//��������


void inportModel(const string filename)
{
	
	

	ifstream model (filename);
	if (!model)
	{
		cerr << "ģ���ļ���ʧ��" << endl;
		exit(-1);
	}
	//��ȡ�����ļ�
	model >> texture_count;
	for (int i = 0; i < texture_count;i++)
	{//��ȡ�����ļ��ļ�����texture��
		string temp;
		model >> temp;
		texture.push_back(temp);
		//cout << texture[i];
	}
	
	//��ȡ������Ϣ

	model >> material_count;
	for (int i = 0; i < material_count; i++)
	{
		vector<GLfloat> temp(4);
		int t;
		model >> temp[0] >>temp[1] >> temp[2] >> temp[3]; 
		ambient.push_back(temp);

		model >> temp[0] >> temp[1] >> temp[2] >> temp[3];
		diffuse.push_back(temp);

		model >> temp[0] >> temp[1] >> temp[2] >> temp[3];
		specular.push_back(temp);

		model >> temp[0] >> temp[1] >> temp[2] >> temp[3];
		emission.push_back(temp);

		
		model >> temp[0];
		shininess.push_back(temp[0]);

		model >> t;
		texture_index.push_back(t);
	}

	//��ȡ������Ϣ
	model >> vertex_count;
	for (int i = 0; i < vertex_count; i++)
	{
		vector<GLfloat> temp(3);
		model >>temp[0] >> temp[1] >> temp[2] ;
		vertex.push_back(temp);
		
	}

	//��ȡ��ͼ����
	model >> t_count;
	for (int i = 0; i < t_count;i++)
	{
		vector<GLfloat> temp(2);
		model >> temp[0] >> temp[1];
		texture_pos.push_back(temp);
	}
	//��ȡ����
	model >> n_count;
	for (int i = 0; i < n_count; i++)
	{
		vector<GLfloat> temp(3);
		model >> temp[0] >> temp[1] >>temp[2];
		normals.push_back(temp);
	}
	//��ȡģ�ͷ������
	model >> submodel_count;
	model >> zoom_factor[0] >> zoom_factor[1] >> zoom_factor[2];
	for (int i = 0; i < submodel_count;i++)
	{
		smodel.resize(submodel_count);

		model >> smodel[i].triangle_count>> smodel[i].material_index;
		//��չ������Ƭ��Ϣ�Ŀռ�
 		smodel[i].v.resize(smodel[i].triangle_count);
 		smodel[i].t.resize(smodel[i].triangle_count);
 		smodel[i].n.resize(smodel[i].triangle_count);

		for (int j = 0; j < smodel[i].triangle_count; j++)
		{
			smodel[i].v[j].resize(3);
			smodel[i].t[j].resize(3);
			smodel[i].n[j].resize(3);
			for (int k = 0; k < 3;k++)
			{
				model >> smodel[i].v[j][k];
				model >> smodel[i].t[j][k];
				model >> smodel[i].n[j][k];
				
			}
			
		}

	}


#ifdef DEBUG
	ofstream importedModel("debug.txt");
	if (!importedModel)
	{
		cerr << "Create debug.txt failed" << endl;
		exit(-1);
	}

	importedModel << texture_count << endl;
	for (int i = 0; i < texture_count;i++)
	{
		importedModel << texture[i] << endl;
	}
	importedModel << material_count << endl;
	for (int i = 0; i < material_count; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			importedModel << ambient[i][j] << ' ';
		}
		importedModel << endl;

		for (int j = 0; j < 4; j++)
		{
			importedModel << diffuse[i][j] << ' ';
		}
		importedModel << endl;

		for (int j = 0; j < 4; j++)
		{
			importedModel << specular[i][j] << ' ';
		}
		importedModel << endl;

		for (int j = 0; j < 4; j++)
		{
			importedModel << emission[i][j] << ' ';
		}
		importedModel << endl;

		importedModel << shininess[i] << endl;
		importedModel << texture_index[i] << endl;
	}

	importedModel << vertex_count << endl;
	for (int i = 0; i < vertex_count;i++)
	{
		importedModel << vertex[i][0] <<' '<< vertex[i][1] << ' ' << vertex[i][2]<<endl;
	}
	importedModel << t_count << endl;
	for (int i = 0; i < t_count; i++)
	{
		importedModel << texture_pos[i][0] << ' ' << texture_pos[i][1] << ' ' << endl;

	}
	importedModel << n_count << endl;
	for (int i = 0; i < n_count;i++)
	{
		importedModel << normals[i][0] << ' ' << normals[i][1] << ' ' << normals[i][2] << endl;
	}

	importedModel << submodel_count << endl;
	importedModel << zoom_factor[0] << ' ' << zoom_factor[1] << ' ' << zoom_factor[2] << endl;

	for (int i = 0; i < submodel_count; i++)
	{
		importedModel << smodel[i].triangle_count << endl;
		importedModel << smodel[i].material_index << endl;
		for (int j = 0; j < smodel[i].triangle_count; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				importedModel << smodel[i].v[j][k]<<' ';
				importedModel << smodel[i].t[j][k]<<' ';
				importedModel << smodel[i].n[j][k]<<' ';
			}
			importedModel << endl;
		}
	}
#endif // DEBUG	
}

//////////////////////////////////////////////////////////////////////////
//��һ��ʹ��opengl�Ŀ��
//�ο�nehe�Ľ̳̣�Ӧ�ð���init����������resize�ĺ�����drawģ�͵ĺ���
//////////////////////////////////////////////////////////////////////////
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	camera->setShape(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
														// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	Vector3d pos(0.0, 0.0, 1.0);
	Vector3d target(0.0, 0.0, 0.0);
	Vector3d up(0.0, 1.0, 0.0);
	camera = new GLCamera(pos, target, up);

	
	return TRUE;										// Initialization Went OK
}

void DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	camera->setModelViewMatrix();//
	//����ģ��
	glColor3f(0.0, 1.0, 0.0);
	glutWireTeapot(1);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < submodel_count; i++)
	{
		for (int j = 0; j < smodel[i].triangle_count; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				glVertex3f(vertex[smodel[i].v[j][k] - 1][0], vertex[smodel[i].v[j][k] - 1][1], vertex[smodel[i].v[j][k] - 1][2]);//V[j][k]��ʾ��j�������εĵ�k������
			}

		}

	}

	glEnd();
	glFlush();
	return ;										// Everything Went OK
}


//////////////////////////////////////////////////////////////////////////
//�����Ǵ�nehe�Ľ̳���ѧϰ������
//////////////////////////////////////////////////////////////////////////
void RotateX(float angle)
{
	float d = camera->getDist();
	int cnt = 100;
	float theta = angle / cnt;
	float slide_d = -2 * d*sin(theta*3.14159265 / 360);
	camera->yaw(theta / 2);
	for (; cnt != 0; --cnt)
	{
		camera->slide(slide_d, 0, 0);
		camera->yaw(theta);
	}
	camera->yaw(-theta / 2);
}

void RotateY(float angle)
{
	float d = camera->getDist();
	int cnt = 100;
	float theta = angle / cnt;
	float slide_d = 2 * d*sin(theta*3.14159265 / 360);
	camera->pitch(theta / 2);
	for (; cnt != 0; --cnt)
	{
		camera->slide(0, slide_d, 0);
		camera->pitch(theta);
	}
	camera->pitch(-theta / 2);
}


GLint oldmx, oldmy;
GLint buttonstate;
void mouse(GLint button, GLint state, GLint x, GLint y)
{
	if (state==GLUT_DOWN)
	{
		oldmx = x;
		oldmy = y;
	}
	buttonstate = button;
}

void onMouseMove(GLint x, GLint y)
{

	GLint dx = x - oldmx;
	GLint dy = y-oldmy;
	if (buttonstate == GLUT_RIGHT_BUTTON)
	{
		RotateX((GLfloat)dx/10);
		RotateY((GLfloat)dy/10);
	}
	else if (buttonstate==GLUT_LEFT_BUTTON)
	{
		camera->roll((GLfloat)dx/10);

	}
	else if (buttonstate == GLUT_MIDDLE_BUTTON)
	{
		camera->slide(-(GLfloat)dx/300, (GLfloat)dy/300, 0);
	}

	else if (buttonstate == 4)
	{
		cout << "!!" << endl;
	}
	oldmx = x;
	oldmy = y;


	glutPostRedisplay();

}


int main(int argc,char** argv)
{
	string filename;
	cout << "������ģ���ļ���" << endl;
	cin >> filename;
	inportModel(filename);

	glutInit(&argc, argv);

	InitGL();

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(800, 600);
	glutCreateWindow("test!");
	glutDisplayFunc(DrawGLScene);
	glutReshapeFunc(ReSizeGLScene);
	
	glutMouseFunc(mouse);
	glutMotionFunc(onMouseMove);
	glutMainLoop();

	system("pause");
    return 0;
}

