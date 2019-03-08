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
#define MESH_PATH "MeshPath"
#define MESH_DATA_PATH "MeshDataPath"

//Shader
#define STANDARD_COLOR_SHADER "StandardColorShader"
#define STANDARD_UV_SHADER "StandardUVShader"
#define COLLIDER_SHADER "ColliderShader"
#define BUTTON_SHADER "ButtonShader"
#define BAR_SHADER "BarShader"
#define CHECKBOX_SHADER "CheckBoxShader"
#define TILE_SHADER "TileShader"
#define FULLSCREEN_SHADER "FullScreenShader"
#define STANDARD_UV_STATIC_SHADER "StandardUVStaticShader"
#define STANDARD_NORMAL_COLOR_SHADER "StandardNormalColorShader"
#define LIGHT_DIR_ACC_SHADER "LightDirAccShader"
#define LIGHT_POINT_ACC_SHADER "LightPointAccShader"
#define	LIGHT_SPOT_SHADER "LightSpotAccShader"
#define LIGHT_BLEND_SHADER "LightBlendShader"
#define	STANDARD_BUMP_SHADER "StandardBumpShader"
#define	STANDARD_BUMP_SHADER "StandardBumpShader"
#define STANDARD_BUMP_ANIM_SHADER "StandardBumpAnimShader"
#define STANDARD_TEX_NORMAL_ANIM_SHADER "StandardTexNormalAnimShader"
#define STANDARD_TEX_NORMAL_SHADER "StandardTexNormalShader"
#define	VERTEX3D_SHADER "Vertex3DShader"
#define SKY_BOX_SHADER "SkyBoxShader"

//Base InputLayOut
#define POS_COLOR_LAYOUT "PosColorLayOut"
#define POS_UV_LAYOUT "PosUVLayOut"
#define POS_LAYOUT "PosLayOut"
#define POS_VOLUME_LAYOUT "PosVolumeLayOut"
#define POS_NORMAL_COLOR_LAYOUT "PosNormalColorLayOut"
#define	VERTEX3D_LAYOUT	"Vertex3DLayout"
#define	VERTEX4_LAYOUT	"Vertex4Layout"

//Sampler
#define LINER_SAMPLER "Linear"
#define POINT_SAMPLER "Point"

//BlendState
#define ALPHA_BLEND "AlphaBlend"
#define DEPTH_DISABLE "DepthDisable"
#define ACC_BLEND "AccBlend"
#define ZERO_BLEND "ZeroBlend"
#define ALL_BLEND "AllBlend"
#define CULL_NONE "CullNone"
#define WIRE_FRAME "WireFrame"
#define DEPTH_GRATOR "DepthGrator"
#define DEPTH_LESS "DepthLess"
#define DEPTH_LESS_EQUAL "DepthLessEqual"
#define FRONT_CULL "FrontCull"
#define BACK_CULL "BackCull"
#define FRONT_COUNT_WISE "FrontCountWise"

//VolumMesh
#define SPHERE_VOLUM "SphereVolum"
#define CAPSUL_VOLUM "CapsulVolum"
#define CORN_VOLUM "CornVolum"
#define CYLINDER_VOLUM "CylinderVolum"

//Mesh
#define SPHERE_SKY "Sky"

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
