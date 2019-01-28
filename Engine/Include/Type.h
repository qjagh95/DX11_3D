#pragma once

JEONG_BEGIN

struct JEONG_DLL WinSize
{
	unsigned int Width;
	unsigned int Height;

	WinSize() : Width(0), Height(0) {}
	WinSize(const WinSize& size) {*this = size;}
	WinSize(unsigned int Width, unsigned Height) : Width(Width), Height(Height) {}
};

struct VertexColor
{
	Vector3 m_Pos;
	Vector4 m_Color;

	VertexColor() {}
	VertexColor(const VertexColor& Value) { *this = Value; }
	VertexColor(const Vector3& vec3, const Vector4& vec4) { m_Pos = vec3, m_Color = vec4; }
};

//UV좌표란 이미지크기에 상관없이 무조건 0에서 1까지의 범위를 갖는 숫자이다. (좌상단 좌표를 사용한다)
//Left = 0 , Right = 1
//Top = 0 , Bottom = 1
struct VertexUV
{
	Vector3 m_Pos;
	Vector2 m_UV;

	VertexUV() {}
	VertexUV(const VertexUV& Value) { *this = Value; }
	VertexUV(const Vector3& vec3, const Vector2& vec2) { m_Pos = vec3, m_UV = vec2; }
};

enum JEONG_DLL SHADER_TYPE
{
	ST_VERTEX,
	ST_PIXEL,
	ST_MAX,
};

//CBuffer은 상수버퍼 (C++코드에서 쉐이더로 보내기위함)
struct JEONG_DLL CBuffer
{
	ID3D11Buffer* cBuffer;
	int BufferSize;
	int ShaderType;
	int RegisterNumber;
};

struct JEONG_DLL MaterialCbuffer
{
	//재질정보 (색상) 나머지필요한 변수들은 나중에 추가 예정.
	Vector4 Diffuse;

	MaterialCbuffer() : Diffuse(Vector4::White) {}
};

struct JEONG_DLL BoxInfo
{
	Vector3 Min;
	Vector3 Max;
	Vector3 Lenth;
};

struct JEONG_DLL CircleInfo
{
	float Radius;
	Vector3 CenterPos;
};

struct JEONG_DLL OBB2DInfo
{
	Vector3 CenterPos;
	Vector3 Axis[2];
	float Lenth[2];
};

struct JEONG_DLL Pixel24
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct JEONG_DLL Pixel32
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct JEONG_DLL PixelInfo
{
	BoxInfo ImageRect;
	Pixel24 *Color;
	Pixel24 OutColor;
	int Width;
	int Height;
};

struct JEONG_DLL Clip2DFrame
{
	Vector2	LeftTop;
	Vector2	RightBottom;
};

struct JEONG_DLL AnimationClip2D
{
	ANIMATION2D_TYPE AnimationType;		///이미지종류(아틀라스, 프레임)
	ANIMATION_OPTION AnimationOption;	///루프여부
	string AnimationName;				///현재 내 애니메이션 이름
	class Texture* CurTexture;			///나를 돌리고있는 텍스쳐
	float TextureWidth;					///이미지크기
	float TextureHeight;				
	vector<Clip2DFrame>	vecFrame;		///이미지위치(프레임위치)
	int Frame;							///현재프레임
	float PlayTime;						///애니메이션 진행 시간
	float PlayLimitTime;				///모든 애니메이션 완료 시간.
};

//쉐이더에서 이미지를 쪼개서 뿌려줄것이기때문에 쉐이더로 넘겨줄 상수버퍼 선언
struct JEONG_DLL Animation2DCBuffer
{
	Vector2 LeftTopUV;
	Vector2 RightBottomUV; 
	int Frame;
	Vector3 Empty;
};

//이걸 또 선언해주는 이유는 위에선언된 2DCBuffer를 void*로 넘겨주기위함이다.
//어차피 상수버퍼도 똑같은 바이트크기여서 위 내용 그대로 포인터로받아서 넘겨주는형식이다.
//왜나하면 map, unmap에서 memcpy후 쉐이더에 셋팅해주기때문.
struct JEONG_DLL RendererCBuffer
{
	void* pBuffer;
	string Name;
	int BufferSize;
};

//16
struct JEONG_DLL TransformCBuffer
{
	Matrix World;
	Matrix View;
	Matrix Projection;
	Matrix WV;
	Matrix WVP;
	Vector3 Pivot; //12
	float Empty1;	//4바이트
	Vector3 Lenth; //12
	float Empty2;   //4바이트 (16바이트패딩으로 바이트 맞춰주기위함)
};

//나중에 추가할 컴포넌트가 상수버퍼를 사용하는지 안하는지를 판단하는 상수버퍼.
struct JEONG_DLL ComponentCBuffer
{
	int TextureAnimation2D;
	Vector3 Empty;
};

struct JEONG_DLL ButtonCBuffer
{
	Vector4 DiffuseColor;
};

struct JEONG_DLL CheckBoxCBuffer
{
	int isCheck;
	Vector3 Empty;
	Vector4 CheckBoxColor;
};

struct JEONG_DLL BarCBuffer
{
	Vector4 Light;
};

JEONG_END
