#pragma once
#include "fbxsdk.h"

JEONG_BEGIN

//재질정보를 FBXLib에 맞춰서 쉽게 빼기 위한 구조체
//struct JEONG_DLL FbxMaterial
//{
//	Vector4		Diffuse;			///반사재질정보
//	Vector4		Ambient;			///반사재질정보
//	Vector4		Spcular;			///반사재질정보
//	Vector4		Emissive;			///자체발광 재질 정보	
//	float		SpecularPower;      ///이건알지
//	float		Shininess;          ///SpcularPower와 같은개념인데...잘모르겠다
//	float		TransparencyFactor; ///투명도 비율??
//	string		DiffuseTexName;     ///난반사 텍스쳐 이름 (FBXLoad시 자동으로 튀어나옴)
//	string		BumpTexName;        ///범프(노말) 텍스쳐 이름
//	string		SpcTexName;         ///Spcluar 텍스쳐 이름
//};

struct JEONG_DLL FBXMeshContainer
{
	vector<UINT> indecies;
	vector<Vertex3D> vertecis;
};

// 0. write material xml
// 1. Read joint Data
// 2. Read Mesh Data

// TODO: 광민_1
//FBX안에 있는 정보 중 내가 쓸 것들만 뽑아내기위한 구조체
struct JEONG_DLL FBXMaterial
{
	string MaterialName;

	Vector4 Diffuse;
	string DiffuseTexName;  //까면 나올 텍스쳐 이름

	Vector4 Spcular;
	string SpcularTexName; //까면 나올 텍스쳐 이름
	float Spcular_Exp;	   //지수(shinese)

	string BumpTexName;
};

struct JEONG_DLL FBXJoint 
{
	string Name;
	Matrix BindPoseInv;
};

struct JEONG_DLL FBXVertex
{
	string MaterialName; //내가 어느 material에 소속되있는가
	int ControlPointIndex; //정점 인덱스 (VertexIndex)
	Vertex3D Vertex3D; //정점데이터
};

struct JEONG_DLL FBXMeshPart
{
	string MaterialName;

	vector<Vertex3D> Vertices;
	vector<uint32_t> Indices;
};

struct JEONG_DLL FBXMesh
{
	string Name;

	int JointIndex;
	FbxMesh* pMesh;

	vector<FBXVertex*> Vertices;
	vector<FBXMeshPart*> MeshPart;
};

struct JEONG_DLL FBXSkeleton
{
	vector<FBXJoint*> vecJoints;
};

class JEONG_DLL FBXLoader
{
public:
	// TODO: 광민_2
	static Matrix ToDirectX();

	static Vector2 ToVector2(FbxVector2 const& _v);
	static Vector3 ToVector3(FbxVector4 const& _v);
	static Vector4 ToColor(FbxVector4 const& _v);
	static Vector4 ToColor(FbxPropertyT<FbxDouble3> const& _color, FbxPropertyT<FbxDouble> const& _factor);
	static Matrix ToMatrix(FbxAMatrix const& _m);

	static string GetTextureFileName(const FbxProperty& Property);
	static string GetMaterialName(FbxMesh* Mesh, int PolygonIndex, int PolygonVertexIndex, int ControlPointIndex);
	static Vector2 GetUV(FbxMesh* Mesh, int ControlPointIndex, int UVIndex);

	bool LoadFbx(const char* FullPath);

	// ExportMaterial
	//		Read
	//		Write

private:
	// TODO: 광민_3
	void ReadMaterial(FbxScene* scene);
	void WriteMaterialXML(const string& FileName, const string& PathKey = FBX_DATA_PATH);
	void ReadJoint(FbxNode* Node);
	void ReadJointData(FbxMesh* mesh);
	void ReadMesh(FbxScene* Scene, FbxNode* Node, int JointIndex);

private:
	FbxManager*	m_Manager;
	FbxScene* m_Scene;

	// TODO: 광민_4
	//vector<vector<FbxMaterial*>> m_vecMaterials;
	vector<FBXMeshContainer*> m_vecMeshContainer;
	vector<FBXMaterial*> m_vecMaterials;
	vector<FBXMesh*> m_vecMeshs;
	FBXSkeleton m_vecSkeleton;
	unordered_map<int, vector<pair<int, float>>> m_JointWeightMap;

	FbxAxisSystem m_AxisSystem;
	bool m_isRightHand;

public:
	FBXLoader();
	~FBXLoader();

public:
	friend class Mesh;
};

JEONG_END