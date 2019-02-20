#pragma once
#include "fbxsdk.h"

JEONG_BEGIN

//���������� FBXLib�� ���缭 ���� ���� ���� ����ü
//struct JEONG_DLL FbxMaterial
//{
//	Vector4		Diffuse;			///�ݻ���������
//	Vector4		Ambient;			///�ݻ���������
//	Vector4		Spcular;			///�ݻ���������
//	Vector4		Emissive;			///��ü�߱� ���� ����	
//	float		SpecularPower;      ///�̰Ǿ���
//	float		Shininess;          ///SpcularPower�� ���������ε�...�߸𸣰ڴ�
//	float		TransparencyFactor; ///���� ����??
//	string		DiffuseTexName;     ///���ݻ� �ؽ��� �̸� (FBXLoad�� �ڵ����� Ƣ���)
//	string		BumpTexName;        ///����(�븻) �ؽ��� �̸�
//	string		SpcTexName;         ///Spcluar �ؽ��� �̸�
//};

//struct JEONG_DLL FBXMeshContainer
//{
//	vector<Vector3>	vecPos;
//	vector<Vector3>	vecNormal;
//	vector<Vector2>	vecUV;
//	vector<Vector3>	vecTangent;
//	vector<Vector3>	vecBinormal;
//	vector<vector<UINT>> vecIndices;
//	//vector<Vector4> vecBlendWeight;
//	//vector<Vector4> vecBlendIndex;
//	//unordered_map<int, vector<FBXWEIGHT>>	mapWeights;
//	bool isBump;
//	//bool isAnimation;
//
//	FBXMeshContainer()
//	{
//		isBump = false;
//		//isAnimation = false;
//	}
//};

// 0. write material xml
// 1. Read joint Data
// 2. Read Mesh Data

// TODO: ����_1
//FBX�ȿ� �ִ� ���� �� ���� �� �͵鸸 �̾Ƴ������� ����ü
struct JEONG_DLL FBXMaterial
{
	string MaterialName;

	Vector4 Diffuse;
	string DiffuseTexName;  //��� ���� �ؽ��� �̸�

	Vector4 Spcular;
	string SpcularTexName; //��� ���� �ؽ��� �̸�
	float Spcular_Exp;	   //����(shinese)

	string BumpTexName;
};

struct JEONG_DLL FBXJoint
{
	string Name;

	int Index;
	int ParentIndex;

	Matrix LocalTransform; //�θ�Joint�� ���������� ��ȯ��� (���ϸ� �θ�ΰ���)
	Matrix GlobalTransform; //3DMax�󿡼��� ���� ���������� ��ȯ��� (���ϸ� Max��ǥ���� World�ΰ���)
};

struct JEONG_DLL FBXVertex
{
	string MaterialName; //���� ��� material�� �Ҽӵ��ִ°�
	int ControlPointIndex; //���� �ε��� (VertexIndex)
	Vertex3D Vertex3D; //����������
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
	FbxMesh* Mesh;

	vector<FBXVertex*> Vertices;
	vector<FBXMeshPart*> MeshPart;
};

class JEONG_DLL FBXLoader
{
public:
	// TODO: ����_2
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
	// TODO: ����_3
	void ReadMaterial(FbxScene* scene);
	void WriteMaterialXML(const string& FileName, const string& PathKey = FBX_DATA_PATH);
	void ReadJoint(FbxScene* Scene, FbxNode* Node, int Index, int ParentIndex);
	void ReadMesh(FbxScene* Scene, FbxNode* Node, int JointIndex);
	void ReadBinormal();
	void ReadTangent();

	//void Triangulate(FbxNode* pNode);
	//void LoadMaterial(FbxSurfaceMaterial * pMtrl);
	//Vector4 GetMaterialColor(FbxSurfaceMaterial * pMtrl,const char * pPropertyName, const char * pPropertyFactorName);
	//double GetMaterialFactor(FbxSurfaceMaterial * pMtrl,const char * pPropertyName);
	//string GetMaterialTexture(FbxSurfaceMaterial * pMtrl, const char * pPropertyName);

	//void LoadMesh(FbxNode* pNode);
	//void LoadMesh(FbxMesh* pMesh);

	//void LoadNormal(FbxMesh * pMesh, FBXMeshContainer* pContainer, int iVtxID,int iControlIndex);
	//void LoadUV(FbxMesh * pMesh, FBXMeshContainer* pContainer,int iUVID, int iControlIndex);
	//void LoadTangent(FbxMesh * pMesh, FBXMeshContainer* pContainer, int iVtxID, int iControlIndex);
	//void LoadBinormal(FbxMesh * pMesh, FBXMeshContainer* pContainer,int iVtxID, int iControlIndex);
	
private:
	FbxManager*	m_Manager;
	FbxScene* m_Scene;

	// TODO: ����_4
	//vector<vector<FbxMaterial*>> m_vecMaterials;
	//vector<FBXMeshContainer*> m_vecMeshContainer;
	vector<FBXMaterial*> m_vecMaterials;
	vector<FBXJoint*> m_vecJoints;
	vector<FBXMesh*> m_vecMeshs;

public:
	FBXLoader();
	~FBXLoader();

public:
	friend class Mesh;
};

JEONG_END