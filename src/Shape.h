#pragma once
#include <vector>
#include "gl/GLU.h"
#include "algebra_structs.h"
using namespace std;


class Shape
{
protected:
	static GLuint getID() {
		static GLuint curID = 0;
		curID++;
		return curID;
	}
public:
	float3 center;

	vector<float3> verteces;
	//vector<float3> colours;
	vector<float3> normals;
	vector<facet> facets;
	GLuint ID;
	float3 colour;
	bool marked;

	Shape()
		: center(0, 0, 0),
		colour(0.3, 0.3, 0.3),
		marked(false)
	{
		ID = getID();

	}
	virtual ~Shape() {};
	Shape(float3 _center, float3 _colour, GLuint _ID,
		vector<float3> &_verteces,
		vector<float3> &_normals,
		vector<facet> &_facets) 
		:
		center(_center), colour(_colour), ID(_ID), 
		verteces(_verteces), normals(_normals), facets(_facets)
	{}
	virtual char* type() = 0;
	virtual void draw()
	{
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);
		
		float3 f, s, t, n1, n2, n3; //first, second, third verteces and there normal
		glBegin(GL_TRIANGLES);
		for (auto &i : facets)
		{
			f = verteces.at(i.triangle[0]);
			s = verteces.at(i.triangle[1]);
			t = verteces.at(i.triangle[2]);
			n1 = normals.at(i.normal[0]);
			n2 = normals.at(i.normal[1]);
			n3 = normals.at(i.normal[2]);

			glNormal3f(n1.x, n1.y, n1.z); glVertex3f(f.x, f.y, f.z);
			glNormal3f(n2.x, n2.y, n2.z); glVertex3f(s.x, s.y, s.z);
			glNormal3f(n3.x, n3.y, n3.z); glVertex3f(t.x, t.y, t.z);
		}
		glEnd();
		if (marked)
		{
			glPushMatrix();
			glScalef(1.1, 1.1, 1.1);
			glColor3f(1, 1, 0);
			float3 f, s, t, n1, n2, n3; //first, second, third verteces and there normal
			glDisable(GL_LIGHTING);
			glBegin(GL_LINE_STRIP);
			for (auto &i : facets)
			{
				f = verteces.at(i.triangle[0]);
				s = verteces.at(i.triangle[1]);
				t = verteces.at(i.triangle[2]);
				n1 = normals.at(i.normal[0]);
				n2 = normals.at(i.normal[1]);
				n3 = normals.at(i.normal[2]);

				glNormal3f(n1.x, n1.y, n1.z); glVertex3f(f.x, f.y, f.z);
				glNormal3f(n2.x, n2.y, n2.z); glVertex3f(s.x, s.y, s.z);
				glNormal3f(n3.x, n3.y, n3.z); glVertex3f(t.x, t.y, t.z);
			}
			glEnd();
			glPopMatrix();
			glEnable(GL_LIGHTING);
		}

		glPopMatrix();
	}

	void scale(float x, float y, float z)
	{
		if (x == 0 || y == 0 || z == 0)
			throw runtime_error("dividing by zero");
		MatrixScale scale(x, y, z);
		for (auto &i : verteces)
			i = scale * i;
	}

	void move(float x, float y, float z) 
	{
		center.x += x;
		center.y += y;
		center.z += z;
	}

	void rotateX(float angle)
	{
		MatrixRotateX rotate(angle);
		for (auto &i: verteces) {
			i = rotate * i;
		}
		for (auto &i : normals) {
			i = rotate * i;
		}
	}

	void rotateY(float angle)
	{
		MatrixRotateY rotate(angle);
		for (auto &i : verteces) {
			i = rotate * i;
		}
		for (auto &i : normals) {
			i = rotate * i;
		}
	}

	void rotateZ(float angle)
	{
		MatrixRotateZ rotate(angle);
		for (auto &i : verteces) {
			i = rotate * i;
		}
		for (auto &i : normals) {
			i = rotate * i;
		}
	}

	void change_color(float r, float g, float b)
	{
		colour = float3(r, g, b);
	}
};

class Cuboid: public Shape
{
public:
	Cuboid(float a, float b, float c)
	{
		verteces.resize(8);
		verteces[0] = float3(-a / 2, -b / 2, -c / 2);
		verteces[1] = float3(-a / 2,  b / 2, -c / 2);
		verteces[2] = float3( a / 2,  b / 2, -c / 2);
		verteces[3] = float3( a / 2, -b / 2, -c / 2);

		verteces[4] = float3(-a / 2, -b / 2, c / 2);
		verteces[5] = float3(-a / 2,  b / 2, c / 2);
		verteces[6] = float3( a / 2,  b / 2, c / 2);
		verteces[7] = float3( a / 2, -b / 2, c / 2);

		normals.resize(8);

		normals[0] = float3(0, 0, -1);
		normals[1] = float3(0, 0,  1);
		normals[2] = float3(-1, 0, 0);
		normals[3] = float3( 1, 0, 0);
		normals[4] = float3(0,  1, 0);
		normals[5] = float3(0, -1, 0);

		//dummy
		normals[6] = float3(0, 0, 0);
		normals[7] = float3(0, 0, 0);

		facets.resize(12);
		for (int i = 0; i < 12; i++)
		{
			facets[i].normal[0] = i / 2;
			facets[i].normal[1] = i / 2;
			facets[i].normal[2] = i / 2;
		}
#pragma region facets_initialization
		facets[0].triangle[0] = 0;
		facets[0].triangle[1] = 1;
		facets[0].triangle[2] = 2;

		facets[1].triangle[0] = 2;
		facets[1].triangle[1] = 3;
		facets[1].triangle[2] = 0;

		facets[2].triangle[0] = 4;
		facets[2].triangle[1] = 5;
		facets[2].triangle[2] = 6;

		facets[3].triangle[0] = 6;
		facets[3].triangle[1] = 7;
		facets[3].triangle[2] = 4;

		facets[4].triangle[0] = 0;
		facets[4].triangle[1] = 1;
		facets[4].triangle[2] = 5;

		facets[5].triangle[0] = 5;
		facets[5].triangle[1] = 4;
		facets[5].triangle[2] = 0;
		
		facets[6].triangle[0] = 2;
		facets[6].triangle[1] = 3;
		facets[6].triangle[2] = 7;

		facets[7].triangle[0] = 7;
		facets[7].triangle[1] = 6;
		facets[7].triangle[2] = 2;
		
		facets[8].triangle[0] = 1;
		facets[8].triangle[1] = 2;
		facets[8].triangle[2] = 6;

		facets[9].triangle[0] = 6;
		facets[9].triangle[1] = 5;
		facets[9].triangle[2] = 1;
		
		facets[10].triangle[0] = 3;
		facets[10].triangle[1] = 0;
		facets[10].triangle[2] = 4;

		facets[11].triangle[0] = 4;
		facets[11].triangle[1] = 7;
		facets[11].triangle[2] = 3;
#pragma endregion
	}

	Cuboid(float3 center, float3 colour, GLuint ID,
		vector<float3> &verteces,
		vector<float3> &normals,
		vector<facet> &facets) : 
		Shape(center, colour, ID, verteces, normals, facets){}
	char* type() { return "Cuboid"; }

};

class Grid: public Shape
{
public:
	char* type() override { return "Grid"; }
	void draw() override
	{
		static const int gridRes = 5;
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_LIGHTING);

		glPushMatrix();
		glTranslatef(center.x, center.y, center.z);

		glBegin(GL_LINES);
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, -gridRes);
		glVertex3f(0, 0, gridRes);
		glColor3f(0.2f, 0.2f, 0.2f);

		for (int i = -gridRes; i <= gridRes; i++)
		{
			if (i == 0) glColor3f(1, 0, 0);
			glVertex2f(-gridRes, i);
			glVertex2f(gridRes, i);

			if (i == 0) glColor3f(0, 1, 0);
			glVertex2f(i, -gridRes);
			glVertex2f(i, gridRes);
			glColor3f(0.2f, 0.2f, 0.2f);
		}
		glEnd();
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}
};

class Ellipsoid : public Shape
{
	const float SPHERE_RADIUS = 1.0f;
	const int SPHERE_LAT_SLICES = 12;//96
	const int SPHERE_LONG_SLICES = 24;//192
	const float PI = 3.141592654f;

	static float3 rotate_x(float3 v, float sin_ang, float cos_ang) {
		return float3(v.x,
			          v.y * cos_ang + v.z * sin_ang,
			          v.z * cos_ang - v.y * sin_ang);
	}

	static float3 rotate_y(float3 v, float sin_ang, float cos_ang) {
		return float3(v.x * cos_ang + v.z * sin_ang, 
				      v.y,
			          v.z * cos_ang - v.x * sin_ang);
	}
	float _a, _b, _c;
public:
	Ellipsoid(float a, float b, float c) : _a(a), _b(b), _c(c)
	{
		const float lat_angle = PI / (float)SPHERE_LAT_SLICES;
		const float long_angle = 2.0f * PI / (float)SPHERE_LONG_SLICES;

		const float sin_lat = sin(lat_angle);
		const float cos_lat = cos(lat_angle);
		const float sin_long = sin(long_angle);
		const float cos_long = cos(long_angle);
		//scale(_a, _b, _c);

		float3 lat_0(0.0f, SPHERE_RADIUS, 0.0);

		for (int y = 0; y < SPHERE_LAT_SLICES; ++y)
		{
			float3 lat_1 = rotate_x(lat_0, sin_lat, cos_lat);

			float3 long_0_0 = lat_0;
			float3 long_1_0 = lat_1;

			for (int x = 0; x < SPHERE_LONG_SLICES; ++x)
			{
				float3 long_0_1 = rotate_y(long_0_0, sin_long, cos_long);
				float3 long_1_1 = rotate_y(long_1_0, sin_long, cos_long);

				verteces.push_back(long_0_0);
				verteces.push_back(long_1_0);
				verteces.push_back(long_1_1);
				verteces.push_back(long_0_1);

				normals.push_back(long_0_0);
				normals.push_back(long_1_0);
				normals.push_back(long_1_1);
				normals.push_back(long_0_1);
				int base = 4 * (y * SPHERE_LONG_SLICES + x);
				facets.push_back(facet(base, base + 1, base + 2, base, base + 1, base + 2));
				facets.push_back(facet(base, base + 3, base + 2, base, base + 3, base + 2));

				long_0_0 = long_0_1;
				long_1_0 = long_1_1;
			}

			lat_0 = lat_1;
		}
	}

	char* type() override { return "Elipsoid"; }
};

class Tetraedr: public Shape
{
public:
	char* type() override { return "Tetraedr"; }
	Tetraedr(float a)
	{
		verteces.resize(4);
		verteces[0] = float3(a, a, a);
		verteces[1] = float3(-a, -a, a);
		verteces[2] = float3(-a, a, -a);
		verteces[3] = float3(a, -a, -a);


		normals.resize(4);

		normals[0] = float3(1, -1, -1); //012
		normals[1] = float3(1, -1, 1); //013
		normals[2] = float3(1, 1, -1); //023
		normals[3] = float3(1, 1, 1); //123
	

		facets.resize(4);
		for (int i = 0; i < 4; i++)
		{
			facets[i].normal[0] = i;
			facets[i].normal[1] = i;
			facets[i].normal[2] = i;
		}
		facets[0].triangle[0] = 0;
		facets[0].triangle[1] = 1;
		facets[0].triangle[2] = 2;

		facets[1].triangle[0] = 0;
		facets[1].triangle[1] = 1;
		facets[1].triangle[2] = 3;

		facets[2].triangle[0] = 0;
		facets[2].triangle[1] = 2;
		facets[2].triangle[2] = 3;

		facets[3].triangle[0] = 1;
		facets[3].triangle[1] = 2;
		facets[3].triangle[2] = 3;
	}
	Tetraedr(float3 center, float3 colour, GLuint ID,
		vector<float3> &verteces,
		vector<float3> &normals,
		vector<facet> &facets) :
		Shape(center, colour, ID, verteces, normals, facets) {}
};