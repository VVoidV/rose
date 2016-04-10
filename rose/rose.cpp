// rose.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL\glut.h>
#define DEBUG
using namespace std;
//用于保存模型信息的数据结构
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

void inportModel(const string filename)
{
	
	

	ifstream model (filename);
	if (!model)
	{
		cerr << "模型文件打开失败" << endl;
		exit(-1);
	}
	//读取纹理文件
	model >> texture_count;
	for (int i = 0; i < texture_count;i++)
	{//读取纹理文件文件名到texture中
		string temp;
		model >> temp;
		texture.push_back(temp);
		//cout << texture[i];
	}
	
	//读取材质信息

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

	//读取顶点信息
	model >> vertex_count;
	for (int i = 0; i < vertex_count; i++)
	{
		vector<GLfloat> temp(3);
		model >>temp[0] >> temp[1] >> temp[2] ;
		vertex.push_back(temp);
		
	}

	//读取贴图坐标
	model >> t_count;
	for (int i = 0; i < t_count;i++)
	{
		vector<GLfloat> temp(2);
		model >> temp[0] >> temp[1];
		texture_pos.push_back(temp);
	}
	//读取法线
	model >> n_count;
	for (int i = 0; i < n_count; i++)
	{
		vector<GLfloat> temp(3);
		model >> temp[0] >> temp[1] >>temp[2];
		normals.push_back(temp);
	}
	//读取模型分组情况
	model >> submodel_count;
	model >> zoom_factor[0] >> zoom_factor[1] >> zoom_factor[2];
	for (int i = 0; i < submodel_count;i++)
	{
		smodel.resize(submodel_count);

		model >> smodel[i].triangle_count>> smodel[i].material_index;
		//扩展保存面片信息的空间
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


void drawmodel(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	
	for (int i = 0; i < submodel_count; i++)
	{
		for (int j = 0; j < smodel[i].triangle_count; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				glVertex3f(vertex[smodel[i].v[j][k]-1][0], vertex[smodel[i].v[j][k]-1][1], vertex[smodel[i].v[k][k]-1][2]);//V[j][k]表示第j个三角形的第k个顶点
			}
			
		}
		
	}
	
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(-1,1, -1, 1, -1, 1);
	gluLookAt(1, 1, 1, 0, 0, 0, 0, 1, 0);
	glColor3f(1.0, 0.0, 0.0);
	drawmodel();
	glFlush();
}

int main(int argc,char** argv)
{
	string filename;
	cout << "请输入模型文件名" << endl;
	cin >> filename;
	inportModel(filename);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(800, 600);
	glutCreateWindow("test!");
	glutDisplayFunc(display);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	system("pause");
    return 0;
}

