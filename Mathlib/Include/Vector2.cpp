#include "MathHeader.h"
#include "Vector2.h"

Vector2	Vector2::Axis[2] = { Vector2(1.f, 0.f), Vector2(0.f, 1.f) };
Vector2	Vector2::Zero = Vector2(0.f, 0.f);
Vector2	Vector2::One = Vector2(1.f, 1.f);

float & Vector2::operator[](unsigned int Index)
{
	switch (Index)
	{
		case 1:
			return x;
			break;
		case 2:
			return y;
			break;
	}

	TrueAssert(true);
	return x;
}

void Vector2::operator=(const Vector2 & vec)
{
	x = vec.x;
	y = vec.y;
}

void Vector2::operator=(const XMVECTOR & Xmvec)
{
	XMStoreFloat2((XMFLOAT2*)this, Xmvec);
}

void Vector2::operator=(const XMFLOAT2 & XmFloat)
{
	x = XmFloat.x;
	y = XmFloat.y;
}

void Vector2::operator=(float Val)
{
	x = Val;
	y = Val;
}

void Vector2::operator=(int Val)
{
	x = (float)Val;
	y = (float)Val;

}

void Vector2::operator=(float ValArr[2])
{
	x = (float)ValArr[0];
	y = (float)ValArr[1];
}

void Vector2::operator=(int ValArr[2])
{
	x = (float)ValArr[0];
	y = (float)ValArr[1];
}

Vector2 Vector2::operator+(const Vector2 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	return Vector2(Src + Dest);
}

Vector2 Vector2::operator+(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat2((XMFLOAT2*)this);

	return Vector2(Src + Xmvec);
}

Vector2 Vector2::operator+(const XMFLOAT2 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	return Vector2(Src + Dest);
}

Vector2 Vector2::operator+(float Val) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src + Dest);
}

Vector2 Vector2::operator+(int Val) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2((float)Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src + Dest);
}

Vector2 Vector2::operator+(float ValArr[2]) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src + Dest);
}

Vector2 Vector2::operator+(int ValArr[2]) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src + Dest);
}
/////////////////////////////////////////////////////////////////////
Vector2 Vector2::operator-(const Vector2 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	return Vector2(Src - Dest);
}

Vector2 Vector2::operator-(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat2((XMFLOAT2*)this);

	return Vector2(Src - Xmvec);
}

Vector2 Vector2::operator-(const XMFLOAT2 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	return Vector2(Src - Dest);
}

Vector2 Vector2::operator-(float Val) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src - Dest);
}

Vector2 Vector2::operator-(int Val) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2((float)Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src - Dest);
}

Vector2 Vector2::operator-(float ValArr[2]) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src - Dest);
}

Vector2 Vector2::operator-(int ValArr[2]) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src - Dest);
}
/////////////////////////////////////////////////////////////////////////
Vector2 Vector2::operator*(const Vector2 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	return Vector2(Src * Dest);
}

Vector2 Vector2::operator*(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat2((XMFLOAT2*)this);

	return Vector2(Src * Xmvec);
}

Vector2 Vector2::operator*(const XMFLOAT2 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	return Vector2(Src * Dest);
}

Vector2 Vector2::operator*(float Val) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src * Dest);
}

Vector2 Vector2::operator*(int Val) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2((float)Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src * Dest);
}

Vector2 Vector2::operator*(float ValArr[2]) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src * Dest);
}

Vector2 Vector2::operator*(int ValArr[2]) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src * Dest);
}
/////////////////////////////////////////////////////////////////////////
Vector2 Vector2::operator/(const Vector2 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	return Vector2(Src / Dest);
}

Vector2 Vector2::operator/(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat2((XMFLOAT2*)this);

	return Vector2(Src / Xmvec);
}

Vector2 Vector2::operator/(const XMFLOAT2 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	return Vector2(Src / Dest);
}

Vector2 Vector2::operator/(float Val) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src / Dest);
}

Vector2 Vector2::operator/(int Val) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2((float)Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src / Dest);
}

Vector2 Vector2::operator/(float ValArr[2]) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src / Dest);
}

Vector2 Vector2::operator/(int ValArr[2]) const
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return Vector2(Src / Dest);
}
/////////////////////////////////////////////////////////////////////////
void Vector2::operator+=(const Vector2 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	*this = (Src += Dest);
}

void Vector2::operator+=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Xmvec);

	*this = (Src += Dest);
}

void Vector2::operator+=(const XMFLOAT2 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	*this = (Src += Dest);
}
void Vector2::operator+=(float Val)
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src += Dest);
}

void Vector2::operator+=(int Val)
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2((float)Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src += Dest);
}
void Vector2::operator+=(float ValArr[2])
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src += Dest);
}
void Vector2::operator+=(int ValArr[2])
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src += Dest);
}
///////////////////////////////////////////////////////////////////////////////////
void Vector2::operator-=(const Vector2 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	*this = (Src -= Dest);
}

void Vector2::operator-=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Xmvec);

	*this = (Src -= Dest);
}

void Vector2::operator-=(const XMFLOAT2 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	*this = (Src -= Dest);
}
void Vector2::operator-=(float Val)
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src -= Dest);
}

void Vector2::operator-=(int Val)
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2((float)Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src -= Dest);
}
void Vector2::operator-=(float ValArr[2])
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src -= Dest);
}
void Vector2::operator-=(int ValArr[2])
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src -= Dest);
}
////////////////////////////////////////////////////////////////////////////////
void Vector2::operator*=(const Vector2 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	*this = (Src *= Dest);
}

void Vector2::operator*=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Xmvec);

	*this = (Src *= Dest);
}

void Vector2::operator*=(const XMFLOAT2 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	*this = (Src *= Dest);
}
void Vector2::operator*=(float Val)
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src *= Dest);
}

void Vector2::operator*=(int Val)
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2((float)Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src *= Dest);
}
void Vector2::operator*=(float ValArr[2])
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src *= Dest);
}
void Vector2::operator*=(int ValArr[2])
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src *= Dest);
}
////////////////////////////////////////////////////////////////////////////////
void Vector2::operator/=(const Vector2 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	*this = (Src /= Dest);
}

void Vector2::operator/=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Xmvec);

	*this = (Src /= Dest);
}

void Vector2::operator/=(const XMFLOAT2 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	*this = (Src /= Dest);
}
void Vector2::operator/=(float Val)
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src /= Dest);
}

void Vector2::operator/=(int Val)
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2((float)Val);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src /= Dest);
}
void Vector2::operator/=(float ValArr[2])
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src /= Dest);
}
void Vector2::operator/=(int ValArr[2])
{
	XMVECTOR Src, Dest;
	Vector2 Temp = Vector2(ValArr);

	Src = XMLoadFloat2((XMFLOAT2*)this);
	Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	*this = (Src /= Dest);
}
////////////////////////////////////////////////////////////////////////////////
bool Vector2::operator==(const Vector2 & vec) const
{
	return x == vec.x && y == vec.y;
}
bool Vector2::operator==(const XMVECTOR & Xmvec) const
{
	Vector2 Temp = Vector2(Xmvec);

	return Temp.x == x && Temp.y == y;
}
bool Vector2::operator==(const XMFLOAT2 & XmFloat) const
{
	Vector2 Temp = Vector2(XmFloat);

	return Temp.x == x && Temp.y == y;
}
bool Vector2::operator==(float Val) const
{
	Vector2 Temp = Vector2(Val);

	return Temp.x == x && Temp.y == y;
}
bool Vector2::operator==(int Val) const
{
	Vector2 Temp = Vector2(Val);

	return Temp.x == x && Temp.y == y;
}
bool Vector2::operator==(float ValArr[2]) const
{
	Vector2 Temp = Vector2(ValArr);

	return Temp.x == x && Temp.y == y;
}
bool Vector2::operator==(int ValArr[2]) const
{
	Vector2 Temp = Vector2(ValArr);

	return Temp.x == x && Temp.y == y;
}
/////////////////////////////////////////////////////////////////////////
bool Vector2::operator!=(const Vector2 & vec) const
{
	return (!(x == vec.x && y == vec.y));
}
bool Vector2::operator!=(const XMVECTOR & Xmvec) const
{
	Vector2 Temp = Vector2(Xmvec);
	return (!(x == Temp.x && y == Temp.y));
}

bool Vector2::operator!=(const XMFLOAT2 & XmFloat) const
{
	Vector2 Temp = Vector2(XmFloat);
	return (!(x == Temp.x && y == Temp.y));
}
bool Vector2::operator!=(float Val) const
{
	Vector2 Temp = Vector2(Val);
	return (!(x == Temp.x && y == Temp.y));
}
bool Vector2::operator!=(int Val) const
{
	Vector2 Temp = Vector2(Val);

	return (!(x == Temp.x && y == Temp.y));
}
bool Vector2::operator!=(float ValArr[2]) const
{
	Vector2 Temp = Vector2(ValArr);
	return (!(x == Temp.x && y == Temp.y));
}
bool Vector2::operator!=(int ValArr[2]) const
{
	Vector2 Temp = Vector2(ValArr);
	return (!(x == Temp.x && y == Temp.y));
}
float Vector2::Lenth() const
{
	XMVECTOR Src = XMLoadFloat2((XMFLOAT2*)this);

	return XMVectorGetX(XMVector3Length(Src));
}
void Vector2::Nomallize()
{
	XMVECTOR Src = XMLoadFloat2((XMFLOAT2*)this);

	*this = XMVector3Normalize(Src);
}
/////////////////////////////////////////////////////////////////////////
float Vector2::Dot(const Vector2 & vec) const
{
	XMVECTOR Src = XMLoadFloat2((XMFLOAT2*)this);
	XMVECTOR Dest = XMLoadFloat2((XMFLOAT2*)&vec);

	return XMVectorGetX(XMVector3Dot(Src, Dest));
}
float Vector2::Dot(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src = XMLoadFloat2((XMFLOAT2*)this);

	return XMVectorGetX(XMVector3Dot(Src, Xmvec));
}
float Vector2::Dot(const XMFLOAT2 & XmFloat) const
{
	XMVECTOR Src = XMLoadFloat2((XMFLOAT2*)this);
	XMVECTOR Dest = XMLoadFloat2((XMFLOAT2*)&XmFloat);

	return XMVectorGetX(XMVector3Dot(Src, Dest));
}
float Vector2::Dot(float Val[2]) const
{
	Vector2 Temp = Vector2(Val);

	XMVECTOR Src = XMLoadFloat2((XMFLOAT2*)this);
	XMVECTOR Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return XMVectorGetX(XMVector3Dot(Src, Dest));

}
float Vector2::Dot(int Val[2]) const
{
	Vector2 Temp = Vector2(Val);

	XMVECTOR Src = XMLoadFloat2((XMFLOAT2*)this);
	XMVECTOR Dest = XMLoadFloat2((XMFLOAT2*)&Temp);

	return XMVectorGetX(XMVector3Dot(Src, Dest));
}
/////////////////////////////////////////////////////////////////////////

Vector2 Vector2::Nomallize(const Vector2 & vec)
{
	XMVECTOR Src = XMLoadFloat2((XMFLOAT2*)&vec);

	return Vector2(XMVector3Normalize(Src));
}
/////////////////////////////////////////////////////////////////////////