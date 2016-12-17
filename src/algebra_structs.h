#pragma once
#include <cmath>

struct facet
{
	int triangle[3];
	int normal[3];
	facet(int v1, int v2, int v3, int n1, int n2, int n3)
	{
		triangle[0] = v1;
		triangle[1] = v2;
		triangle[2] = v3;

		normal[0] = n1;
		normal[1] = n2;
		normal[2] = n3;
	}
	facet(){}
};

class float3
{
public:
	float x, y, z;
	float3(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z) {}
	float3() : x(0), y(0), z(0) {}
	void normalize()
	{
		float norm = std::sqrt(x*x + y*y + z*z);
		x /= norm;
		y /= norm;
		z /= norm;
	}

	float3 operator +(float3 right) {
		return float3(x + right.x,
			y + right.y,
			z + right.z);
	}
};




class Matrix3
{
public:
	float a11_, a12_, a13_,
		  a21_, a22_, a23_,
		  a31_, a32_, a33_;
	Matrix3(float a11, float a12, float a13,
			float a21, float a22, float a23,
			float a31, float a32, float a33)
		: 
		a11_(a11), a12_(a12), a13_(a13),
		a21_(a21), a22_(a22), a23_(a23),
		a31_(a31), a32_(a32), a33_(a33)

	{}


	float3 operator*(float3 vec) {
		return float3(a11_ * vec.x + a12_ * vec.y + a13_ * vec.z,
					  a21_ * vec.x + a22_ * vec.y + a23_ * vec.z,
					  a31_ * vec.x + a32_ * vec.y + a33_ * vec.z);
	}


};

class MatrixRotateX : public Matrix3
{
public:

	MatrixRotateX(float angle): 
		Matrix3(1,      0,         0,
				0, cos(angle), -sin(angle),
				0, sin(angle), cos(angle))
	{}
};

class MatrixRotateY : public Matrix3
{
public:

	MatrixRotateY(float angle) :
		Matrix3(cos(angle),  0, sin(angle),
				     0,      1,     0,
				-sin(angle), 0, cos(angle))
	{}
};

class MatrixRotateZ : public Matrix3
{
public:
	MatrixRotateZ(float angle) :
		Matrix3(cos(angle), -sin(angle), 0,
				sin(angle), cos(angle),  0,
			     	0,          0,       1)
	{}
};

class MatrixScale : public Matrix3
{
public:

	MatrixScale(float x, float y, float z) :
		Matrix3(x, 0, 0,
				0, y, 0,
				0, 0, z)
	{}
};


