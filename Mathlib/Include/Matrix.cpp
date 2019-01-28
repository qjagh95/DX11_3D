#include "MathHeader.h"
#include "Matrix.h"
#include "Vector3.h"

Matrix::Matrix()
{
	/*
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
	*/
	// XMMatrixIdentity() : 항등행렬을 리턴해주는 함수이다.
	matrix = XMMatrixIdentity();
}

Matrix::Matrix(const Matrix & mat)
{
	matrix = mat.matrix;
}

Matrix::Matrix(const XMMATRIX & mat)
{
	matrix = mat;
}

Matrix Matrix::operator*(const union Matrix & mat) const
{
	return matrix * mat.matrix;
}

Matrix Matrix::operator*(const XMMATRIX & mat) const
{
	return matrix * mat;
}

Matrix Matrix::operator*(float val) const
{
	return matrix * val;
}

Matrix Matrix::operator*(int val) const
{
	return matrix * (float)val;
}

Matrix & Matrix::operator*=(const union Matrix & mat)
{
	matrix *= mat.matrix;

	return *this;
}

void Matrix::operator=(const Matrix & mat)
{
	matrix = mat.matrix;
}

void Matrix::operator=(const XMMATRIX & mat)
{
	matrix = mat;
}

Matrix & Matrix::operator*=(const XMMATRIX & mat)
{
	matrix *= mat;

	return *this;
}

Matrix & Matrix::operator*=(float val)
{
	matrix *= val;

	return *this;
}

Matrix & Matrix::operator*=(int val)
{
	matrix *= (float)val;

	return *this;
}

//(항등행렬로 만든다)
XMMATRIX Matrix::Identity()
{
	matrix = XMMatrixIdentity();
	return matrix;
}
//전치행렬(행과열을 바꾼다)
XMMATRIX Matrix::Transpose()
{
	matrix = XMMatrixTranspose(matrix);

	return matrix;
}

//역행렬
XMMATRIX Matrix::Inverse()
{
	matrix = XMMatrixInverse(&XMMatrixDeterminant(matrix), matrix);
	return matrix;
}

//크기조절
XMMATRIX Matrix::Scaling(float x, float y, float z)
{
	matrix = XMMatrixScaling(x, y, z);

	return matrix;
}

XMMATRIX Matrix::Scaling(const Vector3 & Scale)
{
	matrix = XMMatrixScaling(Scale.x, Scale.y, Scale.z);

	return matrix;
}

XMMATRIX Matrix::Rotation(float x, float y, float z)
{
	XMMATRIX matRotX, matRotY, matRotZ;
	matRotX = XMMatrixRotationX(DegreeToRadian(x));
	matRotY = XMMatrixRotationY(DegreeToRadian(y));
	matRotZ = XMMatrixRotationZ(DegreeToRadian(z));

	matrix = matRotX * matRotY * matRotZ;
	return matrix;
}

XMMATRIX Matrix::Rotation(const Vector3 & Rot)
{
	XMMATRIX matRotX, matRotY, matRotZ;
	matRotX = XMMatrixRotationX(DegreeToRadian(Rot.x));
	matRotY = XMMatrixRotationY(DegreeToRadian(Rot.y));
	matRotZ = XMMatrixRotationZ(DegreeToRadian(Rot.z));

	matrix = matRotX * matRotY * matRotZ;
	return matrix;
}

XMMATRIX Matrix::RotationX(float x)
{
	matrix = XMMatrixRotationX(DegreeToRadian(x));
	return matrix;
}

XMMATRIX Matrix::RotationY(float y)
{
	matrix = XMMatrixRotationY(DegreeToRadian(y));
	return matrix;
}

XMMATRIX Matrix::RotationZ(float z)
{
	matrix = XMMatrixRotationZ(DegreeToRadian(z));
	return matrix;
}

XMMATRIX Matrix::Translation(float x, float y, float z)
{
	matrix = XMMatrixTranslation(x, y, z);
	return matrix;
}

XMMATRIX Matrix::Translation(const Vector3 & vPos)
{
	matrix = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	return matrix;
}

XMMATRIX Matrix::RotationAxis(float Angle, Vector3 & Axis)
{
	matrix = XMMatrixRotationAxis(Axis.Convert(), DegreeToRadian(Angle));
	return matrix;  
}

Vector4 & Matrix::operator[](unsigned int Index)
{
	return vec[Index];
}

void Matrix::operator delete(void * Data)
{
	_aligned_free(Data);
}

void* Matrix::operator new(size_t Size)
{
	return _aligned_malloc(Size, 16);
}
