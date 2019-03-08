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
//	vector<UINT> indecies;
//	vector<Vertex3D> vertecis;
//};
//
//// 0. write material xml
//// 1. Read joint Data
//// 2. Read Mesh Data
//
//// TODO: ����_1
////FBX�ȿ� �ִ� ���� �� ���� �� �͵鸸 �̾Ƴ������� ����ü
//struct JEONG_DLL FBXMaterial
//{
//	string MaterialName;
//
//	Vector4 Diffuse;
//	string DiffuseTexName;  //��� ���� �ؽ��� �̸�
//
//	Vector4 Spcular;
//	string SpcularTexName; //��� ���� �ؽ��� �̸�
//	float Spcular_Exp;	   //����(shinese)
//
//	string BumpTexName;
//};
//
//struct JEONG_DLL FBXJoint 
//{
//	string Name;
//	Matrix BindPoseInv;
//};
//
//struct JEONG_DLL FBXVertex
//{
//	string MaterialName; //���� ��� material�� �Ҽӵ��ִ°�
//	int ControlPointIndex; //���� �ε��� (VertexIndex)
//	Vertex3D Vertex3D; //����������
//};
//
//struct JEONG_DLL FBXMeshPart
//{
//	string MaterialName;
//
//	vector<Vertex3D> Vertices;
//	vector<uint32_t> Indices;
//};
//
//struct JEONG_DLL FBXMesh
//{
//	string Name;
//
//	int JointIndex;
//	FbxMesh* pMesh;
//
//	vector<FBXVertex*> Vertices;
//	vector<FBXMeshPart*> MeshPart;
//};
//
//struct JEONG_DLL FBXSkeleton
//{
//	vector<FBXJoint*> vecJoints;
//};
//
//class JEONG_DLL FBXLoader
//{
//public:
//	// TODO: ����_2
//	static Matrix ToDirectX();
//
//	static Vector2 ToVector2(FbxVector2 const& _v);
//	static Vector3 ToVector3(FbxVector4 const& _v);
//	static Vector4 ToColor(FbxVector4 const& _v);
//	static Vector4 ToColor(FbxPropertyT<FbxDouble3> const& _color, FbxPropertyT<FbxDouble> const& _factor);
//	static Matrix ToMatrix(FbxAMatrix const& _m);
//
//	static string GetTextureFileName(const FbxProperty& Property);
//	static string GetMaterialName(FbxMesh* Mesh, int PolygonIndex, int PolygonVertexIndex, int ControlPointIndex);
//	static Vector2 GetUV(FbxMesh* Mesh, int ControlPointIndex, int UVIndex);
//
//	bool LoadFbx(const char* FullPath);
//
//	// ExportMaterial
//	//		Read
//	//		Write
//
//private:
//	// TODO: ����_3
//	void ReadMaterial(FbxScene* scene);
//	void WriteMaterialXML(const string& FileName, const string& PathKey = MESH_DATA_PATH);
//	void ReadJoint(FbxNode* Node);
//	void ReadJointData(FbxMesh* mesh);
//	void ReadMesh(FbxScene* Scene, FbxNode* Node, int JointIndex);
//
//private:
//	FbxManager*	m_Manager;
//	FbxScene* m_Scene;
//
//	// TODO: ����_4
//	//vector<vector<FbxMaterial*>> m_vecMaterials;
//	vector<FBXMeshContainer*> m_vecMeshContainer;
//	vector<FBXMaterial*> m_vecMaterials;
//	vector<FBXMesh*> m_vecMeshs;
//	FBXSkeleton m_vecSkeleton;
//	unordered_map<int, vector<pair<int, float>>> m_JointWeightMap;
//
//	FbxAxisSystem m_AxisSystem;
//	bool m_isRightHand;
//
//public:
//	FBXLoader();
//	~FBXLoader();
//
//public:
//	friend class Mesh;
//};

// Bone
struct JEONG_DLL FbxBone
{
	string	strName;
	int		iDepth;
	int		iParentIndex;
	FbxAMatrix	matOffset;
	FbxAMatrix	matBone;
};

// Key Frame
struct JEONG_DLL FbxKeyFrame
{
	FbxAMatrix	matTransform;
	double		dTime;
};

// Bone Key Frame
struct JEONG_DLL FbxBoneKeyFrame
{
	int		iBoneIndex;
	vector<FbxKeyFrame>	vecKeyFrame;
};

// Fbx Animation Clip
struct JEONG_DLL FbxAnimationClip
{
	string			strName;
	FbxTime			tStart;
	FbxTime			tEnd;
	FbxLongLong		lTimeLength;
	FbxTime::EMode	eTimeMode;
	vector<FbxBoneKeyFrame>	vecBoneKeyFrame;
};

struct JEONG_DLL FbxWeight
{
	int		iIndex;
	double	dWeight;
};

struct JEONG_DLL FbxMaterial
{
	Vector4		vDif;
	Vector4		vAmb;
	Vector4		vSpc;
	Vector4		vEmv;
	float		fSpecularPower;
	float		fTransparencyFactor;
	float		fShininess;
	string		strDifTex;
	string		strBumpTex;
	string		strSpcTex;
};

struct JEONG_DLL FBXMeshContainer
{
	vector<Vector3>	vecPos;
	vector<Vector3>	vecNormal;
	vector<Vector2>	vecUV;
	vector<Vector3>	vecTangent;
	vector<Vector3>	vecBinormal;
	vector<Vector4>	vecBlendWeight;
	vector<Vector4>	vecBlendIndex;
	vector<vector<UINT>>	vecIndices;
	unordered_map<int, vector<FbxWeight>>	mapWeights;
	bool				bBump;
	bool				bAnimation;

	FBXMeshContainer()
	{
		bBump = false;
		bAnimation = false;
	}
};

enum JEONG_DLL FBX_LOAD_TYPE
{
	FLT_MESH,
	FLT_ALL
};

class JEONG_DLL FBXLoader
{
	friend class Mesh;
	friend class Animation3D_Com;

private:
	FbxManager* m_pManager;
	FbxScene* m_pScene;

private:
	vector<vector<FbxMaterial*>> m_vecMaterials;
	vector<FBXMeshContainer*> m_vecMeshContainer;
	vector<FbxBone*> m_vecBones;
	FbxArray<FbxString*> m_NameArr;
	vector<FbxAnimationClip*> m_vecClip;
	bool m_bMixamo;
	int	m_iLoadType;

public:
	const vector<vector<FbxMaterial*>>* GetMaterials() { return &m_vecMaterials;}
	const vector<FBXMeshContainer*>* GetMeshContainers() { return &m_vecMeshContainer; }
	const vector<FbxBone*>* GetBones() const { return &m_vecBones; }
	const vector<FbxAnimationClip*>* GetClips()	const { return &m_vecClip; }

public:
	bool LoadFbx(const char* _pFullPath, FBX_LOAD_TYPE eType = FLT_ALL);

private:
	void Triangulate(FbxNode* pNode);
	void LoadMaterial(FbxSurfaceMaterial * pMtrl);
	Vector4 GetMaterialColor(FbxSurfaceMaterial * pMtrl, const char * pPropertyName, const char * pPropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial * pMtrl, const char * pPropertyName);
	string GetMaterialTexture(FbxSurfaceMaterial * pMtrl, const char * pPropertyName);

	void LoadMesh(FbxNode* pNode);
	void LoadMesh(FbxMesh* pMesh);

	void LoadNormal(FbxMesh * pMesh, FBXMeshContainer* pContainer, int iVtxID, int iControlIndex);
	void LoadUV(FbxMesh * pMesh, FBXMeshContainer* pContainer, int iUVID, int iControlIndex);
	void LoadTangent(FbxMesh * pMesh, FBXMeshContainer* pContainer, int iVtxID, int iControlIndex);
	void LoadBinormal(FbxMesh * pMesh, FBXMeshContainer* pContainer, int iVtxID, int iControlIndex);

private:
	void LoadAnimationClip();
	void LoadBone(FbxNode* pNode);
	void LoadBoneRecursive(FbxNode* pNode, int iDepth, int iIndex, int iParent);
	void LoadAnimation(FbxMesh* pMesh, FBXMeshContainer* pContainer);
	FbxAMatrix GetTransform(FbxNode * pNode);
	int FindBoneFromName(const string & strName);
	void LoadWeightAndIndex(FbxCluster * pCluster, int iBoneIndex, FBXMeshContainer* pContainer);
	void LoadOffsetMatrix(FbxCluster * pCluster,const FbxAMatrix & matTransform, int iBoneIndex, FBXMeshContainer* pContainer);
	void LoadTimeTransform(FbxNode * pNode, FbxCluster * pCluster, const FbxAMatrix & matTransform, int iBoneIndex);
	void ChangeWeightAndIndices(FBXMeshContainer* pContainer);

private:
	FBXLoader();
	~FBXLoader();
};

JEONG_END