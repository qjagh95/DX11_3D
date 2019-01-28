#pragma once
//XMVector를 멤버로 들고있지않음, 바이트정렬필요X
union MATH_DLL Vector2
{
	struct
	{
		float x, y;
	};
	struct
	{
		float r, g;
	};

	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(const Vector2& vec3) : x(vec3.x), y(vec3.y) {}
	Vector2(const XMVECTOR& Xmvec) { XMStoreFloat2((XMFLOAT2*)this, Xmvec); } ///XMVector -> XMFLOAT로 변환 (어차피 같은 메모리크기 여서 XMFLOAT3로 형변환해주면 된다)
	Vector2(const XMFLOAT2& Xmfloat) { x = Xmfloat.x, y = Xmfloat.y; }
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(float ValArr[2]) :x(ValArr[0]), y(ValArr[1]) {}
	Vector2(int ValArr[2]) :x((float)ValArr[0]), y((float)ValArr[1] ){}
	Vector2(float Val) : x(Val), y(Val) {}
	Vector2(int Val) : x((float)Val), y((float)Val) {}

	//operator
	float& operator[] (unsigned int Index);

	void operator = (const Vector2& vec);
	void operator = (const XMVECTOR& Xmvec);
	void operator = (const XMFLOAT2& XmFloat);
	void operator = (float Val);
	void operator = (int Val);
	void operator = (float ValArr[2]);
	void operator = (int ValArr[2]);

	Vector2 operator + (const Vector2& vec)	const;
	Vector2 operator + (const XMVECTOR& Xmvec) const;
	Vector2 operator + (const XMFLOAT2& XmFloat) const;
	Vector2 operator + (float Val) const;
	Vector2 operator + (int Val) const;
	Vector2 operator + (float ValArr[2]) const;
	Vector2 operator + (int ValArr[2]) const;

	Vector2 operator - (const Vector2& vec)	const;
	Vector2 operator - (const XMVECTOR& Xmvec) const;
	Vector2 operator - (const XMFLOAT2& XmFloat) const;
	Vector2 operator - (float Val) const;
	Vector2 operator - (int Val) const;
	Vector2 operator - (float ValArr[2]) const;
	Vector2 operator - (int ValArr[2]) const;

	Vector2 operator * (const Vector2& vec)	const;
	Vector2 operator * (const XMVECTOR& Xmvec) const;
	Vector2 operator * (const XMFLOAT2& XmFloat) const;
	Vector2 operator * (float Val) const;
	Vector2 operator * (int Val) const;
	Vector2 operator * (float ValArr[2]) const;
	Vector2 operator * (int ValArr[2]) const;

	Vector2 operator / (const Vector2& vec)	const;
	Vector2 operator / (const XMVECTOR& Xmvec) const;
	Vector2 operator / (const XMFLOAT2& XmFloat) const;
	Vector2 operator / (float Val) const;
	Vector2 operator / (int Val) const;
	Vector2 operator / (float ValArr[2]) const;
	Vector2 operator / (int ValArr[2]) const;

	void operator += (const Vector2& vec);
	void operator += (const XMVECTOR& Xmvec);
	void operator += (const XMFLOAT2& XmFloat);
	void operator += (float Val);
	void operator += (int Val);
	void operator += (float ValArr[2]);
	void operator += (int ValArr[2]);

	void operator -= (const Vector2& vec);
	void operator -= (const XMVECTOR& Xmvec);
	void operator -= (const XMFLOAT2& XmFloat);
	void operator -= (float Val);
	void operator -= (int Val);
	void operator -= (float ValArr[2]);
	void operator -= (int ValArr[2]);

	void operator *= (const Vector2& vec);
	void operator *= (const XMVECTOR& Xmvec);
	void operator *= (const XMFLOAT2& XmFloat);
	void operator *= (float Val);
	void operator *= (int Val);
	void operator *= (float ValArr[2]);
	void operator *= (int ValArr[2]);

	void operator /= (const Vector2& vec);
	void operator /= (const XMVECTOR& Xmvec);
	void operator /= (const XMFLOAT2& XmFloat);
	void operator /= (float Val);
	void operator /= (int Val);
	void operator /= (float ValArr[2]);
	void operator /= (int ValArr[2]);

	bool operator == (const Vector2& vec)	const;
	bool operator == (const XMVECTOR& Xmvec) const;
	bool operator == (const XMFLOAT2& XmFloat) const;
	bool operator == (float Val) const;
	bool operator == (int Val) const;
	bool operator == (float ValArr[2]) const;
	bool operator == (int ValArr[2]) const;

	bool operator != (const Vector2& vec)	const;
	bool operator != (const XMVECTOR& Xmvec) const;
	bool operator != (const XMFLOAT2& XmFloat) const;
	bool operator != (float Val) const;
	bool operator != (int Val) const;
	bool operator != (float ValArr[2]) const;
	bool operator != (int ValArr[2]) const;

	//벡터의 크기를 구해주는 함수
	float Lenth() const;
	//노말라이즈
	void Nomallize();

	//내적
	float Dot(const Vector2& vec) const;
	float Dot(const XMVECTOR& Xmvec) const;
	float Dot(const XMFLOAT2& XmFloat) const;
	float Dot(float Val[2]) const;
	float Dot(int Val[2]) const;

	static Vector2 Nomallize(const Vector2& vec);
	static Vector2 Axis[2];
	static Vector2 Zero;
	static Vector2 One;
};