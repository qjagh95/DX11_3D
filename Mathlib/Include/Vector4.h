#pragma once

//XMVector를 멤버로 들고있지않음, 바이트정렬필요X
union MATH_DLL Vector4
{
	struct
	{
		float x, y, z, w;
	};
	struct
	{
		float r, g, b, a;
	};

	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(const Vector4& vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(vec3.w) {}
	Vector4(const Vector2& vec2) : x(vec2.x), y(vec2.y), z(0.0f), w(0.0f) {}
	Vector4(const Vector2& 첫번째, const Vector2& 두번째) : x(첫번째.x), y(첫번째.y), z(두번째.x), w(두번째.y) {}
	Vector4(const XMVECTOR& Xmvec) { XMStoreFloat4((XMFLOAT4*)this, Xmvec); } ///XMVector -> XMFLOAT로 변환 (어차피 같은 메모리크기 여서 XMFLOAT4로 형변환해주면 된다)
	Vector4(const XMFLOAT4& Xmfloat) { x = Xmfloat.x, y = Xmfloat.y, z = Xmfloat.z, w = Xmfloat.w; }
	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	Vector4(float ValArr[4]) :x(ValArr[0]), y(ValArr[1]), z(ValArr[2]), w(ValArr[3]) {}
	Vector4(int ValArr[4]) :x((float)ValArr[0]), y((float)ValArr[1]), z((float)ValArr[2]), w((float)ValArr[3]) {}
	Vector4(float Val) : x(Val), y(Val), z(Val), w(Val) {}
	Vector4(int Val) : x((float)Val), y((float)Val), z((float)Val), w((float)Val) {}

	//operator
	float& operator[] (unsigned int Index);

	void operator = (const Vector4& vec);
	void operator = (const XMVECTOR& Xmvec);
	void operator = (const XMFLOAT4& XmFloat);
	void operator = (float Val);
	void operator = (int Val);
	void operator = (float ValArr[4]);
	void operator = (int ValArr[4]);

	Vector4 operator + (const Vector4& vec)	const;
	Vector4 operator + (const XMVECTOR& Xmvec) const;
	Vector4 operator + (const XMFLOAT4& XmFloat) const;
	Vector4 operator + (float Val) const;
	Vector4 operator + (int Val) const;
	Vector4 operator + (float ValArr[4]) const;
	Vector4 operator + (int ValArr[4]) const;

	Vector4 operator - (const Vector4& vec)	const;
	Vector4 operator - (const XMVECTOR& Xmvec) const;
	Vector4 operator - (const XMFLOAT4& XmFloat) const;
	Vector4 operator - (float Val) const;
	Vector4 operator - (int Val) const;
	Vector4 operator - (float ValArr[4]) const;
	Vector4 operator - (int ValArr[4]) const;

	Vector4 operator * (const Vector4& vec)	const;
	Vector4 operator * (const XMVECTOR& Xmvec) const;
	Vector4 operator * (const XMFLOAT4& XmFloat) const;
	Vector4 operator * (float Val) const;
	Vector4 operator * (int Val) const;
	Vector4 operator * (float ValArr[4]) const;
	Vector4 operator * (int ValArr[4]) const;

	Vector4 operator / (const Vector4& vec)	const;
	Vector4 operator / (const XMVECTOR& Xmvec) const;
	Vector4 operator / (const XMFLOAT4& XmFloat) const;
	Vector4 operator / (float Val) const;
	Vector4 operator / (int Val) const;
	Vector4 operator / (float ValArr[4]) const;
	Vector4 operator / (int ValArr[4]) const;

	void operator += (const Vector4& vec);
	void operator += (const XMVECTOR& Xmvec);
	void operator += (const XMFLOAT4& XmFloat);
	void operator += (float Val);
	void operator += (int Val);
	void operator += (float ValArr[4]);
	void operator += (int ValArr[4]);

	void operator -= (const Vector4& vec);
	void operator -= (const XMVECTOR& Xmvec);
	void operator -= (const XMFLOAT4& XmFloat);
	void operator -= (float Val);
	void operator -= (int Val);
	void operator -= (float ValArr[4]);
	void operator -= (int ValArr[4]);

	void operator *= (const Vector4& vec);
	void operator *= (const XMVECTOR& Xmvec);
	void operator *= (const XMFLOAT4& XmFloat);
	void operator *= (float Val);
	void operator *= (int Val);
	void operator *= (float ValArr[4]);
	void operator *= (int ValArr[4]);

	void operator /= (const Vector4& vec);
	void operator /= (const XMVECTOR& Xmvec);
	void operator /= (const XMFLOAT4& XmFloat);
	void operator /= (float Val);
	void operator /= (int Val);
	void operator /= (float ValArr[4]);
	void operator /= (int ValArr[4]);

	bool operator == (const Vector4& vec)	const;
	bool operator == (const XMVECTOR& Xmvec) const;
	bool operator == (const XMFLOAT4& XmFloat) const;
	bool operator == (float Val) const;
	bool operator == (int Val) const;
	bool operator == (float ValArr[4]) const;
	bool operator == (int ValArr[4]) const;

	bool operator != (const Vector4& vec)	const;
	bool operator != (const XMVECTOR& Xmvec) const;
	bool operator != (const XMFLOAT4& XmFloat) const;
	bool operator != (float Val) const;
	bool operator != (int Val) const;
	bool operator != (float ValArr[4]) const;
	bool operator != (int ValArr[4]) const;

	//벡터의 크기를 구해주는 함수
	float Lenth() const;
	//노말라이즈
	void Nomallize();

	//내적
	float Dot(const Vector4& vec) const;
	float Dot(const XMVECTOR& Xmvec) const;
	float Dot(const XMFLOAT4& XmFloat) const;
	float Dot(float Val[4]) const;
	float Dot(int Val[4]) const;

	static Vector4 Nomallize(const Vector4& vec);
	static Vector4 Axis[4];
	static Vector4 Zero;
	static Vector4 One;

	static Vector4 AliceBlue;
	static Vector4 AntiqueWhite;
	static Vector4 Aqua;
	static Vector4 Aquamarine;
	static Vector4 Azure;
	static Vector4 Beige;
	static Vector4 Bisque;
	static Vector4 Black;
	static Vector4 BlanchedAlmond;
	static Vector4 Blue;
	static Vector4 BlueViolet;
	static Vector4 Brown;
	static Vector4 BurlyWood;
	static Vector4 CadetBlue;
	static Vector4 Chartreuse;
	static Vector4 Chocolate;
	static Vector4 Coral;
	static Vector4 CornflowerBlue;
	static Vector4 Cornsilk;
	static Vector4 Crimson;
	static Vector4 Cyan;
	static Vector4 DarkBlue;
	static Vector4 DarkCyan;
	static Vector4 DarkGoldenrod;
	static Vector4 DarkGray;
	static Vector4 DarkGreen;
	static Vector4 DarkKhaki;
	static Vector4 DarkMagenta;
	static Vector4 DarkOliveGreen;
	static Vector4 DarkOrange;
	static Vector4 DarkOrchid;
	static Vector4 DarkRed;
	static Vector4 DarkSalmon;
	static Vector4 DarkSeaGreen;
	static Vector4 DarkSlateBlue;
	static Vector4 DarkSlateGray;
	static Vector4 DarkTurquoise;
	static Vector4 DarkViolet;
	static Vector4 DeepPink;
	static Vector4 DeepSkyBlue;
	static Vector4 DimGray;
	static Vector4 DodgerBlue;
	static Vector4 Firebrick;
	static Vector4 FloralWhite;
	static Vector4 ForestGreen;
	static Vector4 Fuchsia;
	static Vector4 Gainsboro;
	static Vector4 GhostWhite;
	static Vector4 Gold;
	static Vector4 Goldenrod;
	static Vector4 Gray;
	static Vector4 Green;
	static Vector4 GreenYellow;
	static Vector4 Honeydew;
	static Vector4 HotPink;
	static Vector4 IndianRed;
	static Vector4 Indigo;
	static Vector4 Ivory;
	static Vector4 Khaki;
	static Vector4 Lavender;
	static Vector4 LavenderBlush;
	static Vector4 LawnGreen;
	static Vector4 LemonChiffon;
	static Vector4 LightBlue;
	static Vector4 LightCoral;
	static Vector4 LightCyan;
	static Vector4 LightGoldenrodYellow;
	static Vector4 LightGreen;
	static Vector4 LightGray;
	static Vector4 LightPink;
	static Vector4 LightSalmon;
	static Vector4 LightSeaGreen;
	static Vector4 LightSkyBlue;
	static Vector4 LightSlateGray;
	static Vector4 LightSteelBlue;
	static Vector4 LightYellow;
	static Vector4 Lime;
	static Vector4 LimeGreen;
	static Vector4 Linen;
	static Vector4 Magenta;
	static Vector4 Maroon;
	static Vector4 MediumAquamarine;
	static Vector4 MediumBlue;
	static Vector4 MediumOrchid;
	static Vector4 MediumPurple;
	static Vector4 MediumSeaGreen;
	static Vector4 MediumSlateBlue;
	static Vector4 MediumSpringGreen;
	static Vector4 MediumTurquoise;
	static Vector4 MediumVioletRed;
	static Vector4 MidnightBlue;
	static Vector4 MintCream;
	static Vector4 MistyRose;
	static Vector4 Moccasin;
	static Vector4 NavajoWhite;
	static Vector4 Navy;
	static Vector4 OldLace;
	static Vector4 Olive;
	static Vector4 OliveDrab;
	static Vector4 Orange;
	static Vector4 OrangeRed;
	static Vector4 Orchid;
	static Vector4 PaleGoldenrod;
	static Vector4 PaleGreen;
	static Vector4 PaleTurquoise;
	static Vector4 PaleVioletRed;
	static Vector4 PapayaWhip;
	static Vector4 PeachPuff;
	static Vector4 Peru;
	static Vector4 Pink;
	static Vector4 Plum;
	static Vector4 PowderBlue;
	static Vector4 Purple;
	static Vector4 Red;
	static Vector4 RosyBrown;
	static Vector4 RoyalBlue;
	static Vector4 SaddleBrown;
	static Vector4 Salmon;
	static Vector4 SandyBrown;
	static Vector4 SeaGreen;
	static Vector4 SeaShell;
	static Vector4 Sienna;
	static Vector4 Silver;
	static Vector4 SkyBlue;
	static Vector4 SlateBlue;
	static Vector4 SlateGray;
	static Vector4 Snow;
	static Vector4 SpringGreen;
	static Vector4 SteelBlue;
	static Vector4 Tan;
	static Vector4 Teal;
	static Vector4 Thistle;
	static Vector4 Tomato;
	static Vector4 Transparent;
	static Vector4 Turquoise;
	static Vector4 Violet;
	static Vector4 Wheat;
	static Vector4 White;
	static Vector4 WhiteSmoke;
	static Vector4 Yellow;
	static Vector4 YellowGreen;
};
