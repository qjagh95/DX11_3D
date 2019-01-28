#include "MathHeader.h"
#include "Vector3.h"
#include "Matrix.h"

Vector3	Vector3::Axis[3] = { Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) };
Vector3	Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
Vector3	Vector3::One = Vector3(1.0f, 1.0f, 1.0f);

Vector3	Vector3::CameraZoom = Vector3(1.0f, 1.0f, 1.0f);

float & Vector3::operator[](unsigned int Index)
{
	switch (Index)
	{
		case 1:
			return x;
			break;
		case 2:
			return y;
			break;
		case 3:
			return z;
			break;
	}

	TrueAssert(true);
	return x;
}

void Vector3::operator=(const Vector3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void Vector3::operator=(const XMVECTOR & Xmvec)
{
	XMStoreFloat3((XMFLOAT3*)this, Xmvec);
}

void Vector3::operator=(const XMFLOAT3 & XmFloat)
{
	x = XmFloat.x;
	y = XmFloat.y;
	z = XmFloat.z;
}

void Vector3::operator=(float Val)
{
	x = Val;
	y = Val;
	z = Val;
}

void Vector3::operator=(int Val)
{
	x = (float)Val;
	y = (float)Val;
	z = (float)Val;
}

void Vector3::operator=(float ValArr[3])
{
	x = (float)ValArr[0];
	y = (float)ValArr[1];
	z = (float)ValArr[2];
}

void Vector3::operator=(int ValArr[3])
{
	x = (float)ValArr[0];
	y = (float)ValArr[1];
	z = (float)ValArr[2];
}

Vector3 Vector3::operator+(const Vector3 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	return Vector3(Src + Dest);
}

Vector3 Vector3::operator+(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat3((XMFLOAT3*)this);

	return Vector3(Src + Xmvec);
}

Vector3 Vector3::operator+(const XMFLOAT3 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	return Vector3(Src + Dest);
}

Vector3 Vector3::operator+(float Val) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src + Dest);
}

Vector3 Vector3::operator+(int Val) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3((float)Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src + Dest);
}

Vector3 Vector3::operator+(float ValArr[3]) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src + Dest);
}

Vector3 Vector3::operator+(int ValArr[3]) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src + Dest);
}
/////////////////////////////////////////////////////////////////////
Vector3 Vector3::operator-(const Vector3 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	return Vector3(Src - Dest);
}

Vector3 Vector3::operator-(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat3((XMFLOAT3*)this);

	return Vector3(Src - Xmvec);
}

Vector3 Vector3::operator-(const XMFLOAT3 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	return Vector3(Src - Dest);
}

Vector3 Vector3::operator-(float Val) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src - Dest);
}

Vector3 Vector3::operator-(int Val) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3((float)Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src - Dest);
}

Vector3 Vector3::operator-(float ValArr[3]) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src - Dest);
}

Vector3 Vector3::operator-(int ValArr[3]) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src - Dest);
}
/////////////////////////////////////////////////////////////////////////
Vector3 Vector3::operator*(const Vector3 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	return Vector3(Src * Dest);
}

Vector3 Vector3::operator*(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat3((XMFLOAT3*)this);

	return Vector3(Src * Xmvec);
}

Vector3 Vector3::operator*(const XMFLOAT3 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	return Vector3(Src * Dest);
}

Vector3 Vector3::operator*(float Val) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src * Dest);
}

Vector3 Vector3::operator*(int Val) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3((float)Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src * Dest);
}

Vector3 Vector3::operator*(float ValArr[3]) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src * Dest);
}

Vector3 Vector3::operator*(int ValArr[3]) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src * Dest);
}

/////////////////////////////////////////////////////////////////////////
Vector3 Vector3::operator/(const Vector3 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	return Vector3(Src / Dest);
}

Vector3 Vector3::operator/(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat3((XMFLOAT3*)this);

	return Vector3(Src / Xmvec);
}

Vector3 Vector3::operator/(const XMFLOAT3 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	return Vector3(Src / Dest);
}

Vector3 Vector3::operator/(float Val) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src / Dest);
}

Vector3 Vector3::operator/(int Val) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3((float)Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src / Dest);
}

Vector3 Vector3::operator/(float ValArr[3]) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src / Dest);
}

Vector3 Vector3::operator/(int ValArr[3]) const
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return Vector3(Src / Dest);
}

/////////////////////////////////////////////////////////////////////////
void Vector3::operator+=(const Vector3 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	Src += Dest;
	Convert(Src);
}

void Vector3::operator+=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Xmvec);

	Src += Dest;
	Convert(Src);
}

void Vector3::operator+=(const XMFLOAT3 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	Src += Dest;
	Convert(Src);
}

void Vector3::operator+=(float Val)
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src += Dest;
	Convert(Src);
}

void Vector3::operator+=(int Val)
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3((float)Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src += Dest;
	Convert(Src);
}

void Vector3::operator+=(float ValArr[3])
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src += Dest;
	Convert(Src);
}
void Vector3::operator+=(int ValArr[3])
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src += Dest;
	Convert(Src);
}
///////////////////////////////////////////////////////////////////////////////////
void Vector3::operator-=(const Vector3 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	Src -= Dest;
	Convert(Src);
}

void Vector3::operator-=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Xmvec);

	Src -= Dest;
	Convert(Src);
}

void Vector3::operator-=(const XMFLOAT3 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	Src -= Dest;
	Convert(Src);
}
void Vector3::operator-=(float Val)
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src -= Dest;
	Convert(Src);
}

void Vector3::operator-=(int Val)
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3((float)Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src -= Dest;
	Convert(Src);
}
void Vector3::operator-=(float ValArr[3])
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src -= Dest;
	Convert(Src);
}
void Vector3::operator-=(int ValArr[3])
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src -= Dest;
	Convert(Src);
}
////////////////////////////////////////////////////////////////////////////////
void Vector3::operator*=(const Vector3 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	Src *= Dest;
	Convert(Src);
}

void Vector3::operator*=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Xmvec);

	Src *= Dest;
	Convert(Src);
}

void Vector3::operator*=(const XMFLOAT3 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	Src *= Dest;
	Convert(Src);
}
void Vector3::operator*=(float Val)
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src *= Dest;
	Convert(Src);
}

void Vector3::operator*=(int Val)
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3((float)Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src *= Dest;
	Convert(Src);
}

void Vector3::operator*=(float ValArr[3])
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src *= Dest;
	Convert(Src);
}

void Vector3::operator*=(int ValArr[3])
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src *= Dest;
	Convert(Src);
}

////////////////////////////////////////////////////////////////////////////////
void Vector3::operator/=(const Vector3 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	Src /= Dest;
	Convert(Src);
}

void Vector3::operator/=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Xmvec);

	Src /= Dest;
	Convert(Src);
}

void Vector3::operator/=(const XMFLOAT3 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	Src /= Dest;
	Convert(Src);
}

void Vector3::operator/=(float Val)
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src /= Dest;
	Convert(Src);
}

void Vector3::operator/=(int Val)
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3((float)Val);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src /= Dest;
	Convert(Src);
}

void Vector3::operator/=(float ValArr[3])
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src /= Dest;
	Convert(Src);
}

void Vector3::operator/=(int ValArr[3])
{
	XMVECTOR Src, Dest;
	Vector3 Temp = Vector3(ValArr);

	Src = XMLoadFloat3((XMFLOAT3*)this);
	Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Src /= Dest;
	Convert(Src);
}

////////////////////////////////////////////////////////////////////////////////
bool Vector3::operator==(const Vector3 & vec) const
{
	return x == vec.x && y == vec.y && z == vec.z;
}
bool Vector3::operator==(const XMVECTOR & Xmvec) const
{
	Vector3 Temp = Vector3(Xmvec);

	return Temp.x == x && Temp.y == y && Temp.z == z;
}
bool Vector3::operator==(const XMFLOAT3 & XmFloat) const
{
	Vector3 Temp = Vector3(XmFloat);

	return Temp.x == x && Temp.y == y && Temp.z == z;
}
bool Vector3::operator==(float Val) const
{
	Vector3 Temp = Vector3(Val);

	return Temp.x == x && Temp.y == y && Temp.z == z;
}
bool Vector3::operator==(int Val) const
{
	Vector3 Temp = Vector3(Val);

	return Temp.x == x && Temp.y == y && Temp.z == z;
}
bool Vector3::operator==(float ValArr[3]) const
{
	Vector3 Temp = Vector3(ValArr);

	return Temp.x == x && Temp.y == y && Temp.z == z;
}
bool Vector3::operator==(int ValArr[3]) const
{
	Vector3 Temp = Vector3(ValArr);

	return Temp.x == x && Temp.y == y && Temp.z == z;
}
/////////////////////////////////////////////////////////////////////////
bool Vector3::operator!=(const Vector3 & vec) const
{
	return (!(x == vec.x && y == vec.y && z == vec.z));
}
bool Vector3::operator!=(const XMVECTOR & Xmvec) const
{
	Vector3 Temp = Vector3(Xmvec);

	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}

bool Vector3::operator!=(const XMFLOAT3 & XmFloat) const
{
	Vector3 Temp = Vector3(XmFloat);

	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}

bool Vector3::operator!=(float Val) const
{
	Vector3 Temp = Vector3(Val);
	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}

bool Vector3::operator!=(int Val) const
{
	Vector3 Temp = Vector3(Val);

	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}

bool Vector3::operator!=(float ValArr[3]) const
{
	Vector3 Temp = Vector3(ValArr);

	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}

bool Vector3::operator!=(int ValArr[3]) const
{
	Vector3 Temp = Vector3(ValArr);

	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}

float Vector3::Lenth() const
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);

	return XMVectorGetX(XMVector3Length(Src));
}

void Vector3::Nomallize()
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);

	*this = XMVector3Normalize(Src);
}

//벡터 행렬 곱
Vector3 Vector3::TransformNormal(const Matrix & mat)
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	return Vector3(XMVector3TransformNormal(Src, mat.matrix));
}

//벡터 행렬곱
Vector3 Vector3::TransformNormal(const XMMATRIX & mat)
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	return Vector3(XMVector3TransformNormal(Src, mat));
}

Vector3 Vector3::TransformCoord(const Matrix & mat)
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	return Vector3(XMVector3TransformCoord(Src, mat.matrix));
}

Vector3 Vector3::TransformCoord(const XMMATRIX & mat)
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	return Vector3(XMVector3TransformCoord(Src, mat));
}

float Vector3::GetDistance(const Vector3 & Vec)
{
	return Vector3(*this - Vec).Lenth();
}

/////////////////////////////////////////////////////////////////////////
float Vector3::Dot(const Vector3 & vec) const
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&vec);

	return XMVectorGetX(XMVector3Dot(Src, Dest));
}

float Vector3::Dot(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);

	return XMVectorGetX(XMVector3Dot(Src, Xmvec));
}

float Vector3::Dot(const XMFLOAT3 & XmFloat) const
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	return XMVectorGetX(XMVector3Dot(Src, Dest));
}

float Vector3::Dot(float Val[3]) const
{
	Vector3 Temp = Vector3(Val);

	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return XMVectorGetX(XMVector3Dot(Src, Dest));
}

float Vector3::Dot(int Val[3]) const
{
	Vector3 Temp = Vector3(Val);

	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	return XMVectorGetX(XMVector3Dot(Src, Dest));
}

/////////////////////////////////////////////////////////////////////////
Vector3 Vector3::Cross(const Vector3 & vec) const
{
	Vector3 Temp = Vector3(vec);

	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Vector3 Result = Vector3(XMVector3Cross(Src, Dest));

	if (isnan(Result.x) || isnan(Result.z) || isnan(Result.z))
		return Vector3();

	return Result;
}

Vector3 Vector3::Cross(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&Xmvec);

	Vector3 Result = Vector3(XMVector3Cross(Src, Dest));

	if (isnan(Result.x) || isnan(Result.z) || isnan(Result.z))
		return Vector3();

	return Result;
}

Vector3 Vector3::Cross(const XMFLOAT3 & XmFloat) const
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&XmFloat);

	Vector3 Result = Vector3(XMVector3Cross(Src, Dest));

	if (isnan(Result.x) || isnan(Result.z) || isnan(Result.z))
		return Vector3();

	return Result;
}

Vector3 Vector3::Cross(float Val[3]) const
{
	Vector3 Temp = Vector3(Val);

	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Vector3 Result = Vector3(XMVector3Cross(Src, Dest));

	if (isnan(Result.x) || isnan(Result.z) || isnan(Result.z))
		return Vector3();

	return Result;
}

Vector3 Vector3::Cross(int Val[3]) const
{
	Vector3 Temp = Vector3(Val);

	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)this);
	XMVECTOR Dest = XMLoadFloat3((XMFLOAT3*)&Temp);

	Vector3 Result = Vector3(XMVector3Cross(Src, Dest));

	if (isnan(Result.x) || isnan(Result.z) || isnan(Result.z))
		return Vector3();

	return Result;
}

Vector3 Vector3::Nomallize(const Vector3 & vec)
{
	XMVECTOR Src = XMLoadFloat3((XMFLOAT3*)&vec);

	return Vector3(XMVector3Normalize(Src));
}
/////////////////////////////////////////////////////////////////////////

float Vector3::GetAngle(const Vector3 & vec) const
{
	//단위벡터로만든다
	Vector3 Src = Nomallize(*this);
	Vector3 Dest = Nomallize(vec);

	//단위벡터를 내적한다(cos각도가 나온다)
	float Angle = Src.Dot(Dest);

	//나온 cos각도를 역함수로 진짜각도로 변환한다
	Angle = RadianToDegree(acosf(Angle));

	return Angle;
}
float Vector3::GetAngle(const XMVECTOR & Xmvec) const
{
	//단위벡터로만든다
	Vector3 Src = Nomallize(*this);
	Vector3 Dest = Nomallize(Xmvec);

	//단위벡터를 내적한다(cos각도가 나온다)
	float Angle = Src.Dot(Dest);

	//나온 cos각도를 역함수로 진짜각도로 변환한다
	Angle = acosf(Angle);

	//Degree로 변환한다.
	return RadianToDegree(Angle);
}

float Vector3::GetAngle(const XMFLOAT3 & XmFloat) const
{
	//단위벡터로만든다
	Vector3 Src = Nomallize(*this);
	Vector3 Dest = Nomallize(XmFloat);

	//단위벡터를 내적한다(cos각도가 나온다)
	float Angle = Src.Dot(Dest);

	//나온 cos각도를 역함수로 진짜각도로 변환한다
	Angle = acosf(Angle);

	//Degree로 변환한다.
	return RadianToDegree(Angle);
}

float Vector3::GetAngle(float Val[3]) const
{
	//단위벡터로만든다
	Vector3 Src = Nomallize(*this);
	Vector3 Dest = Nomallize(Val);

	//단위벡터를 내적한다(cos각도가 나온다)
	float Angle = Src.Dot(Dest);

	//나온 cos각도를 역함수로 진짜각도로 변환한다
	Angle = acosf(Angle);

	//Degree로 변환한다.
	return RadianToDegree(Angle);
}

float Vector3::GetAngle(int Val[3]) const
{
	//단위벡터로만든다
	Vector3 Src = Nomallize(*this);
	Vector3 Dest = Nomallize(Val);

	//단위벡터를 내적한다(cos각도가 나온다)
	float Angle = Src.Dot(Dest);

	//나온 cos각도를 역함수로 진짜각도로 변환한다
	Angle = acosf(Angle);

	//Degree로 변환한다.
	return RadianToDegree(Angle);
}

XMVECTOR Vector3::Convert()
{
	return XMLoadFloat3((XMFLOAT3*)this);;
}

void Vector3::Convert(const XMVECTOR & Xmvec)
{
	XMStoreFloat3((XMFLOAT3*)this, Xmvec);
}

void Vector3::AddCameraZoom(const Vector3& Amount)
{
	CameraZoom += Amount;
}

void Vector3::AddCameraZoom(const Vector2& Amount)
{
	CameraZoom.x += Amount.x;
	CameraZoom.y += Amount.y;
}

