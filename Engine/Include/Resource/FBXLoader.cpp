#include "stdafx.h"
#include "FBXLoader.h"

JEONG_USING

FBXLoader::FBXLoader()
{
	m_Manager = NULLPTR;
	m_Scene = NULLPTR;
}

FBXLoader::~FBXLoader()
{
	for (size_t i = 0; i < m_vecMaterials.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterials[i].size(); ++j)
			SAFE_DELETE(m_vecMaterials[i][j]);

		m_vecMaterials[i].clear();
	}

	m_vecMaterials.clear();

	Safe_Delete_VecList(m_vecMeshContainer);

	m_Scene->Clear();
	m_Scene->Destroy();
	m_Manager->Destroy();
}

bool FBXLoader::LoadFbx(const char * pFullPath)
{
	/*
	1. Manager생성 - 최상위 (하나만 있어도 된다)
	2. IOSetting 생성
	3. Importer 생성
	4. Scene생성
	
	Importer가 읽은 데이터를 Scene에 넘겨준다.
	Scene은 그래프구조 (트리)
	
	Scene에서 자식노드와 속성을 돌며 데이터 내 구조에 맞게 데이터를 입력한다.
	*/

	m_Manager = FbxManager::Create();
	FbxIOSettings* pIos = FbxIOSettings::Create(m_Manager, IOSROOT);
	FbxImporter* pImporter = FbxImporter::Create(m_Manager, "");
	m_Scene = FbxScene::Create(m_Manager, "");


	// FbxManager에 지정한다.
	m_Manager->SetIOSettings(pIos);

	// 해당 경로에 있는 fbx 파일을 읽어오고 정보를 만들어낸다.
	pImporter->Initialize(pFullPath, -1, m_Manager->GetIOSettings());

	// 위에서 만들어낸 정보를 FbxScene에 노드를 구성한다.
	pImporter->Import(m_Scene);

	//Max의 Axis시스템은 Y와 Z가 바뀌어있다. 밑의 코드는 딱히 의미 없다.
	//무조건 Max Axis축으로 튀어 나옴
	if (m_Scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
		m_Scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	Triangulate(m_Scene->GetRootNode());
	LoadMesh(m_Scene->GetRootNode());
	pImporter->Destroy();

	/*
	Load Logic
	1. FBX의 모든 Node정보를 삼각형으로 쪼갠 후 각각 삼각형마다 재질정보를 가져와서 로드한다.
	2. 
	*/

	return true;
}

void FBXLoader::Triangulate(FbxNode * pNode)
{
	//정보를 삼각형으로 쪼개주는 역할과 메쉬똥 치우는 역할을 한다.

	// FbxNodeAttribute : FbxNode 속성을 나타내는 인터페이스이다.
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	//Mesh 타입이면 가져온다.
	if (pAttr && (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh || pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs || pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		//노드의 속성을 변환하는 클래스.	
		FbxGeometryConverter converter(m_Manager);
		//쪼갠다.
		converter.Triangulate(pAttr, true);
	}

	// 재질정보가 몇개있는지 읽어온다.
	int	iMtrlCount = pNode->GetMaterialCount();

	if (iMtrlCount > 0)
	{
		vector<FbxMaterial*> vecMtrl;
		m_vecMaterials.push_back(vecMtrl);

		for (int i = 0; i < iMtrlCount; ++i)
		{
			// FbxSurfaceMaterial : 파일이 가지고있는 재질정보를 뽑아낸다.
			FbxSurfaceMaterial*	pMtrl = pNode->GetMaterial(i);

			if (pMtrl == NULLPTR)
				continue;

			LoadMaterial(pMtrl);
		}
	}

	// 현재 노드의 자식노드 수를 얻어온다.
	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
		Triangulate(pNode->GetChild(i)); //재귀형식으로 자식을 순회하면서 데이터뽑는다.
}

void FBXLoader::LoadMaterial(FbxSurfaceMaterial * pMtrl)
{
	//이 함수는 재질정보를 읽어와서 내가만든 구조체에 넣어주고 
	//나중에 그 구조체 정보를 끌어와서 내 구조에 맞게 넣어주기 위함이다.

	FbxMaterial* pMtrlInfo = new FbxMaterial();

	// 가장 마지막 컨테이너에 재질 정보를 추가한다.
	m_vecMaterials[m_vecMaterials.size() - 1].push_back(pMtrlInfo);

	// 각 정보를 읽어온다. (char*는 FBX에 이미 static으로 정해놨다)
	pMtrlInfo->Diffuse = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
	pMtrlInfo->Ambient = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
	pMtrlInfo->Spcular = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	pMtrlInfo->Emissive = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);

	pMtrlInfo->SpecularPower = GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sSpecularFactor);
	pMtrlInfo->Shininess = GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sShininess);
	pMtrlInfo->TransparencyFactor = GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sTransparencyFactor);

	pMtrlInfo->DiffuseTexName = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sDiffuse);

	//범프와 노말은 같은개념이라고 보면 된다.
	pMtrlInfo->BumpTexName = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sNormalMap);

	//노말텍스쳐가 없다면 범프텍스쳐를 읽어온다.
	if (pMtrlInfo->BumpTexName.empty())
		pMtrlInfo->BumpTexName = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sBump);

	pMtrlInfo->SpcTexName = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sSpecular);
}

Vector4 FBXLoader::GetMaterialColor(FbxSurfaceMaterial * pMtrl, const char* pPropertyName, const char* pPropertyFactorName)
{
	//FBX의 재질정보를 가져오기 위한 함수.
	
	FbxDouble3	vResult(0, 0, 0);
	double	dFactor = 0;

	/*
	FbxProperty
	자세한 내용은 너무 방대하다.
	내가 가져올 데이터들은 무조건 Property에 있다고 생각하면 된다.

	Factor는 독립된 놈이다. 연관이 있을뿐
	*/

	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	FbxProperty	tPropertyFactor = pMtrl->FindProperty(pPropertyFactorName);

	// 유효한지 체크한다.
	if (tProperty.IsValid() == true && tPropertyFactor.IsValid() == true)
	{
		vResult = tProperty.Get<FbxDouble3>();
		dFactor = tPropertyFactor.Get<FbxDouble>();

		// Factor는 일종의 비율로 사용한다.
		//Ex) Diffuse가 1인데 Factor는 0.2면 Diffuse는 1.0 * 0.2
		vResult[0] *= dFactor;
		vResult[1] *= dFactor;
		vResult[2] *= dFactor;
	}

	return Vector4((float)vResult[0], (float)vResult[1], (float)vResult[2], (float)dFactor);
}

double FBXLoader::GetMaterialFactor(FbxSurfaceMaterial * pMtrl, const char * pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);

	double	dFactor = 0.0;

	if (tProperty.IsValid() == true)
		dFactor = tProperty.Get<FbxDouble>();

	return dFactor;
}

string FBXLoader::GetMaterialTexture(FbxSurfaceMaterial * pMtrl, const char * pPropertyName)
{
	//재질정보 텍스쳐의 이름을 반환해주는 놈이다.
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);

	string	str = "";

	if (tProperty.IsValid() == true)
	{
		// FbxFileTexture 이 타입이 몇개가 있는지를 얻어오는 함수이다.
		int	iTexCount = tProperty.GetSrcObjectCount<FbxFileTexture>();

		if (iTexCount > 0)
		{
			FbxFileTexture*	pFileTex = tProperty.GetSrcObject<FbxFileTexture>(0);

			if (pFileTex != NULLPTR)
				str = pFileTex->GetFileName();
		}
	}

	return str;
}

void FBXLoader::LoadMesh(FbxNode * pNode)
{
	//본격적으로 Mesh를 로드하는 함수.

	//노드의 속성을 가져온다.
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	//속성이 Mesh 타입이면 로드.
	if (pAttr != NULLPTR && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pMesh = pNode->GetMesh();

		if (pMesh != NULLPTR)
			LoadMesh(pMesh);
	}

	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
		LoadMesh(pNode->GetChild(i));
}

void FBXLoader::LoadMesh(FbxMesh * pMesh)
{
	// MeshContainer를 만들고 컨테이너별 정점과 컨테이너&서브셋 별
	// 인덱스 정보를 얻어와서 메쉬를 구성해야 한다.
	FBXMeshContainer* pContainer = new FBXMeshContainer();
	pContainer->isBump = false;
	m_vecMeshContainer.push_back(pContainer);

	// ControlPoint 는 위치정보를 담고 있는 배열이다.
	// 이 배열의 개수는 곧 정점의 개수가 된다.
	int	iVtxCount = pMesh->GetControlPointsCount();

	// 내부적으로 FbxVector4타입의 배열로 저장하고 있기 때문에 배열의 
	// 시작주소를 얻어온다.
	FbxVector4*	pVtxPos = pMesh->GetControlPoints();

	// 컨테이너가 가지고 있는 정점 정보들을 정점수만큼 resize 해준다.
	pContainer->vecPos.resize(iVtxCount);
	pContainer->vecNormal.resize(iVtxCount);
	pContainer->vecUV.resize(iVtxCount);
	pContainer->vecTangent.resize(iVtxCount);
	pContainer->vecBinormal.resize(iVtxCount);

	for (int i = 0; i < iVtxCount; ++i)
	{
		// y와 z축이 바뀌어 있기 때문에 변경해준다.
		pContainer->vecPos[i].x = pVtxPos[i].mData[0];
		pContainer->vecPos[i].y = pVtxPos[i].mData[2];
		pContainer->vecPos[i].z = pVtxPos[i].mData[1];
	}

	// 폴리곤 수를 얻어온다.
	int	iPolygonCount = pMesh->GetPolygonCount();
	UINT	iVtxID = 0;

	// 재질 수를 얻어온다.
	int	iMtrlCount = pMesh->GetNode()->GetMaterialCount();

	// 재질 수는 곧 서브셋 수이기 때문에 재질 수만큼 resize 한다.
	pContainer->vecIndices.resize(iMtrlCount);

	// 재질 정보를 얻어온다.
	FbxGeometryElementMaterial*	pMaterial = pMesh->GetElementMaterial();
	int iCount = pMesh->GetElementMaterialCount();
	// 삼각형 수만큼 반복한다.
	for (int i = 0; i < iPolygonCount; ++i)
	{
		// 이 폴리곤을 구성하는 정점의 수를 얻어온다.
		// 삼각형일 경우 3개를 얻어온다.
		int	iPolygonSize = pMesh->GetPolygonSize(i);

		int	iIdx[3] = {};

		for (int j = 0; j < iPolygonSize; ++j)
		{
			// 현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를
			// 구한다.
			int	iControlIndex = pMesh->GetPolygonVertex(i, j);

			iIdx[j] = iControlIndex;

			LoadNormal(pMesh, pContainer, iVtxID, iControlIndex);

			LoadUV(pMesh, pContainer, pMesh->GetTextureUVIndex(i, j), iControlIndex);

			LoadTangent(pMesh, pContainer, iVtxID, iControlIndex);

			LoadBinormal(pMesh, pContainer, iVtxID, iControlIndex);

			++iVtxID;
		}

		int	iMtrlID = pMaterial->GetIndexArray().GetAt(i);

		pContainer->vecIndices[iMtrlID].push_back(iIdx[0]);
		pContainer->vecIndices[iMtrlID].push_back(iIdx[2]);
		pContainer->vecIndices[iMtrlID].push_back(iIdx[1]);
	}
}

void FBXLoader::LoadNormal(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iVtxID, int iControlIndex)
{
}

void FBXLoader::LoadUV(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iUVID, int iControlIndex)
{
}

void FBXLoader::LoadTangent(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iVtxID, int iControlIndex)
{
}

void FBXLoader::LoadBinormal(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iVtxID, int iControlIndex)
{
}
