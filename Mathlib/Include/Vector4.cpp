#include "MathHeader.h"
#include "Vector4.h"

Vector4	Vector4::Axis[4] = { Vector4(1.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 0.0f) , Vector4(0.0f, 0.0f, 0.0f, 1.0f) };
Vector4	Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
Vector4	Vector4::One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

Vector4 Vector4::AliceBlue	= { 0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f };
Vector4 Vector4::AntiqueWhite = { 0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f };
Vector4 Vector4::Aqua = { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
Vector4 Vector4::Aquamarine = { 0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f };
Vector4 Vector4::Azure = { 0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f };
Vector4 Vector4::Beige = { 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f };
Vector4 Vector4::Bisque = { 1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f };
Vector4 Vector4::Black = { 0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
Vector4 Vector4::BlanchedAlmond = { 1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f };
Vector4 Vector4::Blue = { 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
Vector4 Vector4::BlueViolet = { 0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f };
Vector4 Vector4::Brown = { 0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f };
Vector4 Vector4::BurlyWood = { 0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f };
Vector4 Vector4::CadetBlue = { 0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f };
Vector4 Vector4::Chartreuse = { 0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f };
Vector4 Vector4::Chocolate = { 0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f };
Vector4 Vector4::Coral = { 1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f };
Vector4 Vector4::CornflowerBlue = { 0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f };
Vector4 Vector4::Cornsilk = { 1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f };
Vector4 Vector4::Crimson = { 0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f };
Vector4 Vector4::Cyan = { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
Vector4 Vector4::DarkBlue = { 0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f };
Vector4 Vector4::DarkCyan = { 0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f };
Vector4 Vector4::DarkGoldenrod = { 0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f };
Vector4 Vector4::DarkGray = { 0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f };
Vector4 Vector4::DarkGreen = { 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f };
Vector4 Vector4::DarkKhaki = { 0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f };
Vector4 Vector4::DarkMagenta = { 0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f };
Vector4 Vector4::DarkOliveGreen = { 0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f };
Vector4 Vector4::DarkOrange = { 1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f };
Vector4 Vector4::DarkOrchid = { 0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f };
Vector4 Vector4::DarkRed = { 0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f };
Vector4 Vector4::DarkSalmon = { 0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f };
Vector4 Vector4::DarkSeaGreen = { 0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f };
Vector4 Vector4::DarkSlateBlue = { 0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f };
Vector4 Vector4::DarkSlateGray = { 0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f };
Vector4 Vector4::DarkTurquoise = { 0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f };
Vector4 Vector4::DarkViolet = { 0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f };
Vector4 Vector4::DeepPink = { 1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f };
Vector4 Vector4::DeepSkyBlue = { 0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f };
Vector4 Vector4::DimGray = { 0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f };
Vector4 Vector4::DodgerBlue = { 0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f };
Vector4 Vector4::Firebrick = { 0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f };
Vector4 Vector4::FloralWhite = { 1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f };
Vector4 Vector4::ForestGreen = { 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f };
Vector4 Vector4::Fuchsia = { 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
Vector4 Vector4::Gainsboro = { 0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f };
Vector4 Vector4::GhostWhite = { 0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f };
Vector4 Vector4::Gold = { 1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f };
Vector4 Vector4::Goldenrod = { 0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f };
Vector4 Vector4::Gray = { 0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f };
Vector4 Vector4::Green = { 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f };
Vector4 Vector4::GreenYellow = { 0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f };
Vector4 Vector4::Honeydew = { 0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f };
Vector4 Vector4::HotPink = { 1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f };
Vector4 Vector4::IndianRed = { 0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f };
Vector4 Vector4::Indigo = { 0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f };
Vector4 Vector4::Ivory = { 1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f };
Vector4 Vector4::Khaki = { 0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f };
Vector4 Vector4::Lavender = { 0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f };
Vector4 Vector4::LavenderBlush = { 1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f };
Vector4 Vector4::LawnGreen = { 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f };
Vector4 Vector4::LemonChiffon = { 1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f };
Vector4 Vector4::LightBlue = { 0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f };
Vector4 Vector4::LightCoral = { 0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f };
Vector4 Vector4::LightCyan = { 0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f };
Vector4 Vector4::LightGoldenrodYellow = { 0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f };
Vector4 Vector4::LightGreen = { 0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f };
Vector4 Vector4::LightGray = { 0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f };
Vector4 Vector4::LightPink = { 1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f };
Vector4 Vector4::LightSalmon = { 1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f };
Vector4 Vector4::LightSeaGreen = { 0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f };
Vector4 Vector4::LightSkyBlue = { 0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f };
Vector4 Vector4::LightSlateGray = { 0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f };
Vector4 Vector4::LightSteelBlue = { 0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f };
Vector4 Vector4::LightYellow = { 1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f };
Vector4 Vector4::Lime = { 0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
Vector4 Vector4::LimeGreen = { 0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f };
Vector4 Vector4::Linen = { 0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f };
Vector4 Vector4::Magenta = { 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
Vector4 Vector4::Maroon = { 0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f };
Vector4 Vector4::MediumAquamarine = { 0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f };
Vector4 Vector4::MediumBlue = { 0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f };
Vector4 Vector4::MediumOrchid = { 0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f };
Vector4 Vector4::MediumPurple = { 0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f };
Vector4 Vector4::MediumSeaGreen = { 0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f };
Vector4 Vector4::MediumSlateBlue = { 0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f };
Vector4 Vector4::MediumSpringGreen = { 0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f };
Vector4 Vector4::MediumTurquoise = { 0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f };
Vector4 Vector4::MediumVioletRed = { 0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f };
Vector4 Vector4::MidnightBlue = { 0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f };
Vector4 Vector4::MintCream = { 0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f };
Vector4 Vector4::MistyRose = { 1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f };
Vector4 Vector4::Moccasin = { 1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f };
Vector4 Vector4::NavajoWhite = { 1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f };
Vector4 Vector4::Navy = { 0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f };
Vector4 Vector4::OldLace = { 0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f };
Vector4 Vector4::Olive = { 0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f };
Vector4 Vector4::OliveDrab = { 0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f };
Vector4 Vector4::Orange = { 1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f };
Vector4 Vector4::OrangeRed = { 1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f };
Vector4 Vector4::Orchid = { 0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f };
Vector4 Vector4::PaleGoldenrod = { 0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f };
Vector4 Vector4::PaleGreen = { 0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f };
Vector4 Vector4::PaleTurquoise = { 0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f };
Vector4 Vector4::PaleVioletRed = { 0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f };
Vector4 Vector4::PapayaWhip = { 1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f };
Vector4 Vector4::PeachPuff = { 1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f };
Vector4 Vector4::Peru = { 0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f };
Vector4 Vector4::Pink = { 1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f };
Vector4 Vector4::Plum = { 0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f };
Vector4 Vector4::PowderBlue = { 0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f };
Vector4 Vector4::Purple = { 0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f };
Vector4 Vector4::Red = { 1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
Vector4 Vector4::RosyBrown = { 0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f };
Vector4 Vector4::RoyalBlue = { 0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f };
Vector4 Vector4::SaddleBrown = { 0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f };
Vector4 Vector4::Salmon = { 0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f };
Vector4 Vector4::SandyBrown = { 0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f };
Vector4 Vector4::SeaGreen = { 0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f };
Vector4 Vector4::SeaShell = { 1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f };
Vector4 Vector4::Sienna = { 0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f };
Vector4 Vector4::Silver = { 0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f };
Vector4 Vector4::SkyBlue = { 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f };
Vector4 Vector4::SlateBlue = { 0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f };
Vector4 Vector4::SlateGray = { 0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f };
Vector4 Vector4::Snow = { 1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f };
Vector4 Vector4::SpringGreen = { 0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f };
Vector4 Vector4::SteelBlue = { 0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f };
Vector4 Vector4::Tan = { 0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f };
Vector4 Vector4::Teal = { 0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f };
Vector4 Vector4::Thistle = { 0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f };
Vector4 Vector4::Tomato = { 1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f };
Vector4 Vector4::Transparent = { 0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f };
Vector4 Vector4::Turquoise = { 0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f };
Vector4 Vector4::Violet = { 0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f };
Vector4 Vector4::Wheat = { 0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f };
Vector4 Vector4::White = { 1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
Vector4 Vector4::WhiteSmoke = { 0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f };
Vector4 Vector4::Yellow = { 1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
Vector4 Vector4::YellowGreen = { 0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f };

float & Vector4::operator[](unsigned int Index)
{

	switch (Index)
	{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		case 3:
			return w;
			break;
	}

	TrueAssert(true);
	return x;
}

void Vector4::operator=(const Vector4 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

void Vector4::operator=(const XMVECTOR & Xmvec)
{
	XMStoreFloat4((XMFLOAT4*)this, Xmvec);
}

void Vector4::operator=(const XMFLOAT4 & XmFloat)
{
	x = XmFloat.x;
	y = XmFloat.y;
	z = XmFloat.z;
	w = XmFloat.w;
}

void Vector4::operator=(float Val)
{
	x = Val;
	y = Val;
	z = Val;
	w = Val;
}

void Vector4::operator=(int Val)
{
	x = (float)Val;
	y = (float)Val;
	z = (float)Val;
	w = (float)Val;
}

void Vector4::operator=(float ValArr[4])
{
	x = (float)ValArr[0];
	y = (float)ValArr[1];
	z = (float)ValArr[2];
	w = (float)ValArr[4];
}

void Vector4::operator=(int ValArr[4])
{
	x = (float)ValArr[0];
	y = (float)ValArr[1];
	z = (float)ValArr[2];
	w = (float)ValArr[4];
}

Vector4 Vector4::operator+(const Vector4 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	return Vector4(Src + Dest);
}

Vector4 Vector4::operator+(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat4((XMFLOAT4*)this);

	return Vector4(Src + Xmvec);
}

Vector4 Vector4::operator+(const XMFLOAT4 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	return Vector4(Src + Dest);
}

Vector4 Vector4::operator+(float Val) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src + Dest);
}

Vector4 Vector4::operator+(int Val) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4((float)Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src + Dest);
}

Vector4 Vector4::operator+(float ValArr[4]) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src + Dest);
}

Vector4 Vector4::operator+(int ValArr[4]) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src + Dest);
}
/////////////////////////////////////////////////////////////////////
Vector4 Vector4::operator-(const Vector4 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	return Vector4(Src - Dest);
}

Vector4 Vector4::operator-(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat4((XMFLOAT4*)this);

	return Vector4(Src - Xmvec);
}

Vector4 Vector4::operator-(const XMFLOAT4 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	return Vector4(Src - Dest);
}

Vector4 Vector4::operator-(float Val) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src - Dest);
}

Vector4 Vector4::operator-(int Val) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4((float)Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src - Dest);
}

Vector4 Vector4::operator-(float ValArr[4]) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src - Dest);
}

Vector4 Vector4::operator-(int ValArr[4]) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src - Dest);
}
/////////////////////////////////////////////////////////////////////////
Vector4 Vector4::operator*(const Vector4 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	return Vector4(Src * Dest);
}

Vector4 Vector4::operator*(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat4((XMFLOAT4*)this);

	return Vector4(Src * Xmvec);
}

Vector4 Vector4::operator*(const XMFLOAT4 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	return Vector4(Src * Dest);
}

Vector4 Vector4::operator*(float Val) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src * Dest);
}

Vector4 Vector4::operator*(int Val) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4((float)Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src * Dest);
}

Vector4 Vector4::operator*(float ValArr[4]) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src * Dest);
}

Vector4 Vector4::operator*(int ValArr[4]) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src * Dest);
}
/////////////////////////////////////////////////////////////////////////
Vector4 Vector4::operator/(const Vector4 & vec) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	return Vector4(Src / Dest);
}

Vector4 Vector4::operator/(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src;
	Src = XMLoadFloat4((XMFLOAT4*)this);

	return Vector4(Src / Xmvec);
}

Vector4 Vector4::operator/(const XMFLOAT4 & XmFloat) const
{
	XMVECTOR Src, Dest;
	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	return Vector4(Src / Dest);
}

Vector4 Vector4::operator/(float Val) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src / Dest);
}

Vector4 Vector4::operator/(int Val) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4((float)Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src / Dest);
}

Vector4 Vector4::operator/(float ValArr[4]) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src / Dest);
}

Vector4 Vector4::operator/(int ValArr[4]) const
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return Vector4(Src / Dest);
}
/////////////////////////////////////////////////////////////////////////
void Vector4::operator+=(const Vector4 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	*this = (Src += Dest);
}

void Vector4::operator+=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Xmvec);

	*this = (Src += Dest);
}

void Vector4::operator+=(const XMFLOAT4 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	*this = (Src += Dest);
}
void Vector4::operator+=(float Val)
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src += Dest);
}

void Vector4::operator+=(int Val)
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4((float)Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src += Dest);
}
void Vector4::operator+=(float ValArr[4])
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src += Dest);
}
void Vector4::operator+=(int ValArr[4])
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src += Dest);
}
///////////////////////////////////////////////////////////////////////////////////
void Vector4::operator-=(const Vector4 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	*this = (Src -= Dest);
}

void Vector4::operator-=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Xmvec);

	*this = (Src -= Dest);
}

void Vector4::operator-=(const XMFLOAT4 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	*this = (Src -= Dest);
}
void Vector4::operator-=(float Val)
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src -= Dest);
}

void Vector4::operator-=(int Val)
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4((float)Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src -= Dest);
}
void Vector4::operator-=(float ValArr[4])
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src -= Dest);
}
void Vector4::operator-=(int ValArr[4])
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src -= Dest);
}
////////////////////////////////////////////////////////////////////////////////
void Vector4::operator*=(const Vector4 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	*this = (Src *= Dest);
}

void Vector4::operator*=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Xmvec);

	*this = (Src *= Dest);
}

void Vector4::operator*=(const XMFLOAT4 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	*this = (Src *= Dest);
}
void Vector4::operator*=(float Val)
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src *= Dest);
}

void Vector4::operator*=(int Val)
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4((float)Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src *= Dest);
}
void Vector4::operator*=(float ValArr[4])
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src *= Dest);
}
void Vector4::operator*=(int ValArr[4])
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src *= Dest);
}
////////////////////////////////////////////////////////////////////////////////
void Vector4::operator/=(const Vector4 & vec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	*this = (Src /= Dest);
}

void Vector4::operator/=(const XMVECTOR & Xmvec)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Xmvec);

	*this = (Src /= Dest);
}

void Vector4::operator/=(const XMFLOAT4 & XmFloat)
{
	XMVECTOR Src, Dest;

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	*this = (Src /= Dest);
}
void Vector4::operator/=(float Val)
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src /= Dest);
}

void Vector4::operator/=(int Val)
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4((float)Val);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src /= Dest);
}
void Vector4::operator/=(float ValArr[4])
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src /= Dest);
}
void Vector4::operator/=(int ValArr[4])
{
	XMVECTOR Src, Dest;
	Vector4 Temp = Vector4(ValArr);

	Src = XMLoadFloat4((XMFLOAT4*)this);
	Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	*this = (Src /= Dest);
}
////////////////////////////////////////////////////////////////////////////////
bool Vector4::operator==(const Vector4 & vec) const
{
	return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
}
bool Vector4::operator==(const XMVECTOR & Xmvec) const
{
	Vector4 Temp = Vector4(Xmvec);

	return Temp.x == x && Temp.y == y && Temp.z == z && w == Temp.w;
}
bool Vector4::operator==(const XMFLOAT4 & XmFloat) const
{
	Vector4 Temp = Vector4(XmFloat);

	return Temp.x == x && Temp.y == y && Temp.z == z && w == Temp.w;
}
bool Vector4::operator==(float Val) const
{
	Vector4 Temp = Vector4(Val);

	return Temp.x == x && Temp.y == y && Temp.z == z && w == Temp.w;
}
bool Vector4::operator==(int Val) const
{
	Vector4 Temp = Vector4(Val);

	return Temp.x == x && Temp.y == y && Temp.z == z && w == Temp.w;
}
bool Vector4::operator==(float ValArr[4]) const
{
	Vector4 Temp = Vector4(ValArr);

	return Temp.x == x && Temp.y == y && Temp.z == z && w == Temp.w;
}
bool Vector4::operator==(int ValArr[4]) const
{
	Vector4 Temp = Vector4(ValArr);

	return Temp.x == x && Temp.y == y && Temp.z == z && w == Temp.w;
}
/////////////////////////////////////////////////////////////////////////
bool Vector4::operator!=(const Vector4 & vec) const
{
	return (!(x == vec.x && y == vec.y && z == vec.z));
}
bool Vector4::operator!=(const XMVECTOR & Xmvec) const
{
	Vector4 Temp = Vector4(Xmvec);
	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}

bool Vector4::operator!=(const XMFLOAT4 & XmFloat) const
{
	Vector4 Temp = Vector4(XmFloat);
	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}
bool Vector4::operator!=(float Val) const
{
	Vector4 Temp = Vector4(Val);
	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}
bool Vector4::operator!=(int Val) const
{
	Vector4 Temp = Vector4(Val);

	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}
bool Vector4::operator!=(float ValArr[4]) const
{
	Vector4 Temp = Vector4(ValArr);
	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}
bool Vector4::operator!=(int ValArr[4]) const
{
	Vector4 Temp = Vector4(ValArr);
	return (!(x == Temp.x && y == Temp.y && z == Temp.z));
}
float Vector4::Lenth() const
{
	XMVECTOR Src = XMLoadFloat4((XMFLOAT4*)this);

	return XMVectorGetX(XMVector3Length(Src));
}
void Vector4::Nomallize()
{
	XMVECTOR Src = XMLoadFloat4((XMFLOAT4*)this);

	*this = XMVector3Normalize(Src);
}
/////////////////////////////////////////////////////////////////////////
float Vector4::Dot(const Vector4 & vec) const
{
	XMVECTOR Src = XMLoadFloat4((XMFLOAT4*)this);
	XMVECTOR Dest = XMLoadFloat4((XMFLOAT4*)&vec);

	return XMVectorGetX(XMVector4Dot(Src, Dest));
}
float Vector4::Dot(const XMVECTOR & Xmvec) const
{
	XMVECTOR Src = XMLoadFloat4((XMFLOAT4*)this);

	return XMVectorGetX(XMVector4Dot(Src, Xmvec));
}
float Vector4::Dot(const XMFLOAT4 & XmFloat) const
{
	XMVECTOR Src = XMLoadFloat4((XMFLOAT4*)this);
	XMVECTOR Dest = XMLoadFloat4((XMFLOAT4*)&XmFloat);

	return XMVectorGetX(XMVector4Dot(Src, Dest));
}
float Vector4::Dot(float Val[4]) const
{
	Vector4 Temp = Vector4(Val);

	XMVECTOR Src = XMLoadFloat4((XMFLOAT4*)this);
	XMVECTOR Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return XMVectorGetX(XMVector4Dot(Src, Dest));

}
float Vector4::Dot(int Val[4]) const
{
	Vector4 Temp = Vector4(Val);

	XMVECTOR Src = XMLoadFloat4((XMFLOAT4*)this);
	XMVECTOR Dest = XMLoadFloat4((XMFLOAT4*)&Temp);

	return XMVectorGetX(XMVector4Dot(Src, Dest));
}
Vector4 Vector4::Nomallize(const Vector4 & vec)
{
	XMVECTOR Src = XMLoadFloat4((XMFLOAT4*)&vec);

	return Vector4(XMVector3Normalize(Src));
}
/////////////////////////////////////////////////////////////////////////