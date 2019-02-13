#pragma once
#include "fbxsdk.h"

JEONG_BEGIN

//���������� FBXLib�� ���缭 ���� ���� ���� ����ü
struct JEONG_DLL FbxMaterial
{
	Vector4		Diffuse;
	Vector4		Ambient;
	Vector4		Spcular;
	Vector4		Emissive;
	float		SpecularPower;
	float		TransparencyFactor;
	float		Shininess;
	string		DiffuseTexName;
	string		BumpTexName;
	string		SpcTexName;
};

struct JEONG_DLL FBXMeshContainer
{
	vector<Vector3>	vecPos;
	vector<Vector3>	vecNormal;
	vector<Vector2>	vecUV;
	vector<Vector3>	vecTangent;
	vector<Vector3>	vecBinormal;
	//vector<Vector4>	vecBlendWeight;
	//vector<Vector4>	vecBlendIndex;
	vector<vector<UINT>>	vecIndices;
	//unordered_map<int, vector<FBXWEIGHT>>	mapWeights;
	bool				isBump;
	//bool				isAnimation;

	FBXMeshContainer()
	{
		isBump = false;
		//isAnimation = false;
	}
};


class JEONG_DLL FBXLoader
{
public:
	bool LoadFbx(const char* pFullPath);

	const vector<FBXMeshContainer*>* GetMeshContainers() const { return &m_vecMeshContainer; }
	const vector<vector<FbxMaterial*>>* GetMaterials()	const { return &m_vecMaterials; }

private:
	void Triangulate(FbxNode* pNode);
	void LoadMaterial(FbxSurfaceMaterial * pMtrl);
	Vector4 GetMaterialColor(FbxSurfaceMaterial * pMtrl,const char * pPropertyName, const char * pPropertyFactorName);
	double GetMaterialFactor(FbxSurfaceMaterial * pMtrl,const char * pPropertyName);
	string GetMaterialTexture(FbxSurfaceMaterial * pMtrl, const char * pPropertyName);

	void LoadMesh(FbxNode* pNode);
	void LoadMesh(FbxMesh* pMesh);

	void LoadNormal(FbxMesh * pMesh, FBXMeshContainer* pContainer, int iVtxID,int iControlIndex);
	void LoadUV(FbxMesh * pMesh, FBXMeshContainer* pContainer,int iUVID, int iControlIndex);
	void LoadTangent(FbxMesh * pMesh, FBXMeshContainer* pContainer, int iVtxID, int iControlIndex);
	void LoadBinormal(FbxMesh * pMesh, FBXMeshContainer* pContainer,int iVtxID, int iControlIndex);

private:
	FbxManager*	m_Manager;
	FbxScene* m_Scene;

	vector<vector<FbxMaterial*>> m_vecMaterials;
	vector<FBXMeshContainer*> m_vecMeshContainer;

private:
	FBXLoader();
	~FBXLoader();

public:
	friend class Mesh;
};

JEONG_END