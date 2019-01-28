#pragma once

//XMVector를 멤버로 들고있지않음, 바이트정렬필요X
union MATH_DLL Vector3
{
	struct 
	{
		float x, y, z;
	};
	struct 
	{
		float r, g, b;
	};

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(const Vector3& vec3) : x(vec3.x), y(vec3.y) , z(vec3.z) {}
	Vector3(const Vector2& vec2) : x(vec2.x), y(vec2.y), z(0.0f) {}
	Vector3(const XMVECTOR& Xmvec) { XMStoreFloat3((XMFLOAT3*)this, Xmvec); } ///XMVector -> XMFLOAT로 변환 (어차피 같은 메모리크기 여서 XMFLOAT3로 형변환해주면 된다)
	Vector3(const XMFLOAT3& Xmfloat) { x = Xmfloat.x, y = Xmfloat.y, z = Xmfloat.z; }
	Vector3(float x, float y, float z) : x(x), y(y) , z(z){}
	Vector3(float ValArr[3]) :x(ValArr[0]), y(ValArr[1]) , z(ValArr[2]) {}
	Vector3(int ValArr[3]) :x((float)ValArr[0]), y((float)ValArr[1]), z((float)ValArr[2]) {}
	Vector3(float Val) : x(Val), y(Val) ,z(Val) {}
	Vector3(int Val) : x((float)Val), y((float)Val), z((float)Val) {}

	//operator
	float& operator[] (unsigned int Index);

	void operator = (const Vector3& vec);
	void operator = (const XMVECTOR& Xmvec);
	void operator = (const XMFLOAT3& XmFloat);
	void operator = (float Val);
	void operator = (int Val);
	void operator = (float ValArr[3]);
	void operator = (int ValArr[3]);

	Vector3 operator + (const Vector3& vec)	const;
	Vector3 operator + (const XMVECTOR& Xmvec) const;
	Vector3 operator + (const XMFLOAT3& XmFloat) const;
	Vector3 operator + (float Val) const;
	Vector3 operator + (int Val) const;
	Vector3 operator + (float ValArr[3]) const;
	Vector3 operator + (int ValArr[3]) const;

	Vector3 operator - (const Vector3& vec)	const;
	Vector3 operator - (const XMVECTOR& Xmvec) const;
	Vector3 operator - (const XMFLOAT3& XmFloat) const;
	Vector3 operator - (float Val) const;
	Vector3 operator - (int Val) const;
	Vector3 operator - (float ValArr[3]) const;
	Vector3 operator - (int ValArr[3]) const;

	Vector3 operator * (const Vector3& vec)	const;
	Vector3 operator * (const XMVECTOR& Xmvec) const;
	Vector3 operator * (const XMFLOAT3& XmFloat) const;
	Vector3 operator * (float Val) const;
	Vector3 operator * (int Val) const;
	Vector3 operator * (float ValArr[3]) const;
	Vector3 operator * (int ValArr[3]) const;

	Vector3 operator / (const Vector3& vec)	const;
	Vector3 operator / (const XMVECTOR& Xmvec) const;
	Vector3 operator / (const XMFLOAT3& XmFloat) const;
	Vector3 operator / (float Val) const;
	Vector3 operator / (int Val) const;
	Vector3 operator / (float ValArr[3]) const;
	Vector3 operator / (int ValArr[3]) const;

	void operator += (const Vector3& vec);
	void operator += (const XMVECTOR& Xmvec);
	void operator += (const XMFLOAT3& XmFloat);
	void operator += (float Val);
	void operator += (int Val);
	void operator += (float ValArr[3]);
	void operator += (int ValArr[3]);

	void operator -= (const Vector3& vec);
	void operator -= (const XMVECTOR& Xmvec);
	void operator -= (const XMFLOAT3& XmFloat);
	void operator -= (float Val);
	void operator -= (int Val);
	void operator -= (float ValArr[3]);
	void operator -= (int ValArr[3]);

	void operator *= (const Vector3& vec);
	void operator *= (const XMVECTOR& Xmvec);
	void operator *= (const XMFLOAT3& XmFloat);
	void operator *= (float Val);
	void operator *= (int Val);
	void operator *= (float ValArr[3]);
	void operator *= (int ValArr[3]);

	void operator /= (const Vector3& vec);
	void operator /= (const XMVECTOR& Xmvec);
	void operator /= (const XMFLOAT3& XmFloat);
	void operator /= (float Val);
	void operator /= (int Val);
	void operator /= (float ValArr[3]);
	void operator /= (int ValArr[3]);

	bool operator == (const Vector3& vec)	const;
	bool operator == (const XMVECTOR& Xmvec) const;
	bool operator == (const XMFLOAT3& XmFloat) const;
	bool operator == (float Val) const;
	bool operator == (int Val) const;
	bool operator == (float ValArr[3]) const;
	bool operator == (int ValArr[3]) const;

	bool operator != (const Vector3& vec)	const;
	bool operator != (const XMVECTOR& Xmvec) const;
	bool operator != (const XMFLOAT3& XmFloat) const;
	bool operator != (float Val) const;
	bool operator != (int Val) const;
	bool operator != (float ValArr[3]) const;
	bool operator != (int ValArr[3]) const;

	//벡터의 크기를 구해주는 함수
	float Lenth() const;
	//노말라이즈
	void Normalize();

	//Vector3의 값을 강제로 Vector4로 바꾸고 연산한다.
	//그러면 w값이 추가되는데 w값이 0 -> Normal, 1 -> Coord
	Vector3 TransformNormal(const union Matrix& mat);
	Vector3 TransformNormal(const XMMATRIX& mat);
	Vector3 TransformCoord(const union Matrix& mat);
	Vector3 TransformCoord(const XMMATRIX& mat);

	//거리
	float GetDistance(const Vector3& Vec);

	//내적
	float Dot(const Vector3& vec) const;
	float Dot(const XMVECTOR& Xmvec) const;
	float Dot(const XMFLOAT3& XmFloat) const;
	float Dot(float Val[3]) const;
	float Dot(int Val[3]) const;

	//각도
	float GetAngle(const Vector3& vec) const;
	float GetAngle(const XMVECTOR& Xmvec) const;
	float GetAngle(const XMFLOAT3& XmFloat) const;
	float GetAngle(float Val[3]) const;
	float GetAngle(int Val[3]) const;

	//외적
	Vector3 Cross(const Vector3& vec) const;
	Vector3 Cross(const XMVECTOR& Xmvec) const;
	Vector3 Cross(const XMFLOAT3& XmFloat) const;
	Vector3 Cross(float Val[3])	const;
	Vector3 Cross(int Val[3]) const;

	XMVECTOR Convert();
	void Convert(const XMVECTOR & Xmvec);

	static Vector3 Normalize(const Vector3& vec);

	static Vector3 Axis[3];
	static Vector3 Zero;
	static Vector3 One;
};
