#pragma once
//클라이언트 프로젝트냐 엔진프로젝트냐에 다라서 자동으로 DLL의 import export가 정해진다.
//프로젝트속성-> 전처리기 -> 클라이언트(JEONG_IMPORT;) , 엔진(JEONG_EXPORT)
#ifdef JEONG_EXPORT
#define JEONG_DLL __declspec(dllexport)
#else
#define JEONG_DLL __declspec(dllimport)
#endif

#define SAFE_DELETE(p) if(p) {delete p; p = NULLPTR;}
#define SAFE_RELEASE(p) if(p) {p->Release(); p = NULLPTR;}
#define SAFE_DELETE_ARRARY(p) if(p){delete[] p; p = NULLPTR;}

#define NULLPTR nullptr

#define ROOT_PATH "RootPath"
#define TEXTURE_PATH "TexturPath"
#define SHADER_PATH "ShaderPath"
#define SOUND_PATH "SoundPath"
#define DATA_PATH "DataPath"

#define KEYPRESS(Key) KeyInput::Get()->KeyPress(key)
#define KEYUP(Key) KeyInput::Get()->KeyUp(key)
#define KEYPDOWN(Key) KeyInput::Get()->KeyDown(key)

//BaseShader
#define STANDARD_COLOR_SHADER "StandardColorShader"
#define STANDARD_UV_SHADER "StandardUVShader"
#define COLLIDER_SHADER "ColliderShader"
#define BUTTON_SHADER "ButtonShader"
#define BAR_SHADER "BarShader"
#define CHECKBOX_SHADER "CheckBoxShader"
#define TILE_SHADER "TileShader"
#define FULLSCREEN_SHADER "FullScreenShader"
#define STANDARD_UV_STATIC_SHADER "StandardUVStaticShader"

//Base InputLayOut
#define POS_COLOR_LAYOUT "PosColorLayOut"
#define POS_UV_LAYOUT "PosUVLayOut"
#define POS_LAYOUT "PosLayOut"

//Sampler
#define LINER_SAMPLER "Linear"

//AlphaBlend
#define ALPHA_BLEND "AlphaBlend"
#define DEPTH_DISABLE "DepthDisable"

template<typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator StartIter = p.begin();
	T::iterator EndIter = p.end();

	for (; StartIter != EndIter ; StartIter++)
	{
		SAFE_DELETE((*StartIter));
	}
	p.clear();
}

template<typename T>
void Safe_Release_VecList(T& p)
{
	T::iterator StartIter = p.begin();
	T::iterator EndIter = p.end();

	for (; StartIter != EndIter; StartIter++)
	{
		SAFE_RELEASE((*StartIter));
	}
}

template<typename T>
void Safe_Release_Map(T& p)
{
	T::iterator StartIter = p.begin();
	T::iterator EndIter = p.end();

	for (; StartIter != EndIter; StartIter++)
	{
		SAFE_RELEASE(StartIter->second);
	}
}

template<typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator StartIter = p.begin();
	T::iterator EndIter = p.end();

	for (; StartIter != EndIter; StartIter++)
	{
		SAFE_DELETE(StartIter->second);
	}
	p.clear();
}

#define CLASS_IN_SINGLE(Type)	\
	private:\
		static Type* Instance;\
	public: \
		static Type* Get();\
		static void Delete();\
	private: \
		Type();\
		~Type();

#define SINGLETON_VAR_INIT(Type) Type* Type::Instance = NULLPTR; \
	Type* Type::Get()\
	{\
		if (Instance == NULLPTR)\
			Instance = new Type;\
		return Instance;\
	}\
	void Type::Delete()\
	{\
		SAFE_DELETE(Instance);\
	}
