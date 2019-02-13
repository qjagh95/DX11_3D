#pragma once
#include "fbxsdk.h"

JEONG_BEGIN

//재질정보를 FBXLib에 맞춰서 쉽게 빼기 위한 구조체
struct JEONG_DLL FbxMaterial
{
	Vector4		Diffuse;			///반사재질정보
	Vector4		Ambient;			///반사재질정보
	Vector4		Spcular;			///반사재질정보
	Vector4		Emissive;			///자체발광 재질 정보	
	float		SpecularPower;      ///이건알지
	float		Shininess;          ///SpcularPower와 같은개념인데...잘모르겠다
	float		TransparencyFactor; ///투명도 비율??
	string		DiffuseTexName;     ///난반사 텍스쳐 이름 (FBXLoad시 자동으로 튀어나옴)
	string		BumpTexName;        ///범프(노말) 텍스쳐 이름
	string		SpcTexName;         ///Spcluar 텍스쳐 이름
};

struct JEONG_DLL FBXMeshContainer
{
	vector<Vector3>	vecPos;
	vector<Vector3>	vecNormal;
	vector<Vector2>	vecUV;
	vector<Vector3>	vecTangent;
	vector<Vector3>	vecBinormal;
	vector<vector<UINT>> vecIndices;
	//vector<Vector4> vecBlendWeight;
	//vector<Vector4> vecBlendIndex;
	//unordered_map<int, vector<FBXWEIGHT>>	mapWeights;
	bool isBump;
	//bool isAnimation;

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