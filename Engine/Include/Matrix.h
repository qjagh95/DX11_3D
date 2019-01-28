#pragma once
//경고를 무시한다
#pragma warning(disable:4251)
#include "Vector4.h"

//바이트정렬 바이트를 16배수로 맞춰준다 (SIMD 레지스터를 사용한다)
union MATH_DLL __declspec(align(16)) Matrix
{
	XMMATRIX matrix;
	struct
	{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
	};

	struct
	{
		Vector4 vec[4];
	};

	Matrix();
	Matrix(const Matrix& mat);
	Matrix(const XMMATRIX& mat);

	void operator = (const Matrix& mat);
	void operator = (const XMMATRIX& mat);

	Matrix operator * (const Matrix& mat) const;
	Matrix operator * (const XMMATRIX& mat)	const;
	Matrix operator * (float val) const;
	Matrix operator * (int val)	const;
	// *=
	Matrix& operator *= (const Matrix& mat);
	Matrix& operator *= (const XMMATRIX& mat);
	Matrix& operator *= (float val);
	Matrix& operator *= (int val);

	Vector4& operator[] (unsigned int Index);

	void* operator new (size_t Size);
	void operator delete (void* Data);

	//항등행렬
	XMMATRIX Identity();
	//전치행렬
	XMMATRIX Transpose();
	//역행렬
	XMMATRIX Inverse();

	//S R T 연산 곱하기.
	XMMATRIX Scaling(float x, float y, float z);
	XMMATRIX Scaling(const union Vector3& Scale);
	XMMATRIX Rotation(float x, float y, float z);
	XMMATRIX Rotation(const union Vector3& Rot);
	XMMATRIX RotationX(float x);
	XMMATRIX RotationY(float y);
	XMMATRIX RotationZ(float z);

	//축회전
	XMMATRIX RotationAxis(float Angle, union Vector3& Axis);

	//행렬이동연산.
	XMMATRIX Translation(float x, float y, float z);
	XMMATRIX Translation(const union Vector3& vPos);
};