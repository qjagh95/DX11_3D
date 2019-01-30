#include "stdafx.h"
#include "FBXLoader.h"

JEONG_USING

FBXLoader::FBXLoader()
{
	m_Manager = NULLPTR;
	m_FBXScene = NULLPTR;
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

	m_FBXScene->Clear();
	m_FBXScene->Destroy();
	m_Manager->Destroy();
}

bool FBXLoader::LoadFbx(const char * pFullPath)
{
	/*

	1. Manager생성 - 최상위
	2. IOSetting 생성
	3. Importer 생성
	4. Scene생성

	Importer가 읽은 데이터를 Scene에 넘겨준다.
	Scene은 그래프구조 (트리)

	Scene에서 자식노드와 속성을 돌며 데이터 내 구조에 맞게 데이터를 입력한다.

	*/

	// FbxManager 객체를 생성한다.
	m_Manager = FbxManager::Create();

	// IOSetting을 생성한다. (InputOutput)
	FbxIOSettings*	pIos = FbxIOSettings::Create(m_Manager, IOSROOT);

	// FbxManager에 지정한다.
	m_Manager->SetIOSettings(pIos);

	// FbxScene을 생성한다.
	m_FBXScene = FbxScene::Create(m_Manager, "");

	// FbxImporter 를 이용해서 fbx 파일로부터 메쉬 정보를 읽어온다.
	FbxImporter* pImporter = FbxImporter::Create(m_Manager, "");

	// 해당 경로에 있는 fbx 파일을 읽어오고 정보를 만들어낸다.
	pImporter->Initialize(pFullPath, -1, m_Manager->GetIOSettings());

	// 위에서 만들어낸 정보를 FbxScene에 노드를 구성한다.
	pImporter->Import(m_FBXScene);

	if (m_FBXScene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
		m_FBXScene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	Triangulate(m_FBXScene->GetRootNode());
	LoadMesh(m_FBXScene->GetRootNode());
	pImporter->Destroy();

	return true;
}

void FBXLoader::Triangulate(FbxNode * pNode)
{
	//사각형을 삼각형으로 쪼개주는 기능,
	//메쉬 똥을 치워주는 기능도 있음.

	// FbxNodeAttribute : FbxNode 속성을 나타내는 인터페이스이다.
	// 노드로부터 얻어올 수 있다.
	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if (pAttr && (pAttr->GetAttributeType() == FbxNodeAttribute::eMesh ||
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbs || //곡선표현 즉 메쉬냐?를 물어봄.
		pAttr->GetAttributeType() == FbxNodeAttribute::eNurbsSurface))
	{
		FbxGeometryConverter converter(m_Manager);
		converter.Triangulate(pAttr, true);
	}

	// 재질정보를 읽는다.
	int	iMtrlCount = pNode->GetMaterialCount();

	if (iMtrlCount > 0)
	{
		vector<FbxMaterial*> vecMtrl;
		m_vecMaterials.push_back(vecMtrl);

		for (int i = 0; i < iMtrlCount; ++i)
		{
			// FbxSurfaceMaterial : Fbx 가 가지고 있는 재질 정보를 담고있다.
			FbxSurfaceMaterial*	pMtrl = pNode->GetMaterial(i);

			if (pMtrl == NULLPTR)
				continue;

			LoadMaterial(pMtrl);
		}
	}

	// 현재 노드의 자식노드 수를 얻어온다.
	int	iChildCount = pNode->GetChildCount();

	for (int i = 0; i < iChildCount; ++i)
		Triangulate(pNode->GetChild(i));
}

void FBXLoader::LoadMaterial(FbxSurfaceMaterial * pMtrl)
{
	// 재질 정보를 저장할 구조체를 생성한다.
	FbxMaterial* pMtrlInfo = new FbxMaterial;

	// 가장 마지막 컨테이너에 재질 정보를 추가한다.
	m_vecMaterials[m_vecMaterials.size() - 1].push_back(pMtrlInfo);

	// Diffuse 정보를 읽어온다.
	pMtrlInfo->Diffuse = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sDiffuse,FbxSurfaceMaterial::sDiffuseFactor);
	pMtrlInfo->Ambient = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sAmbient,FbxSurfaceMaterial::sAmbientFactor);
	pMtrlInfo->Spcular = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
	pMtrlInfo->Emissive = GetMaterialColor(pMtrl, FbxSurfaceMaterial::sEmissive,FbxSurfaceMaterial::sEmissiveFactor);

	pMtrlInfo->SpecularPower = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sSpecularFactor);
	pMtrlInfo->Shininess = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sShininess);
	pMtrlInfo->TransparencyFactor = (float)GetMaterialFactor(pMtrl, FbxSurfaceMaterial::sTransparencyFactor);

	pMtrlInfo->strDifTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sDiffuse);
	pMtrlInfo->strBumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sNormalMap);

	if (pMtrlInfo->strBumpTex.empty())
		pMtrlInfo->strBumpTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sBump);

	pMtrlInfo->strSpcTex = GetMaterialTexture(pMtrl, FbxSurfaceMaterial::sSpecular);
}

Vector4 FBXLoader::GetMaterialColor(FbxSurfaceMaterial * pMtrl, const char * pPropertyName, const char * pPropertyFactorName)
{
	FbxDouble3 vResult(0, 0, 0);
	double dFactor = 0;

	// 인자로 들어온 재질에서 해당 이름을 가진 프로퍼티가 있는지를 판단한다음
	// 얻어온다. 마찬가지로 해당 이름의 프로퍼티 팩터가 있는지를 판단한다음 얻어온다.
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	FbxProperty	tPropertyFactor = pMtrl->FindProperty(pPropertyFactorName);

	// 유효한지 체크한다.
	if (tProperty.IsValid() && tPropertyFactor.IsValid())
	{
		vResult = tProperty.Get<FbxDouble3>();
		dFactor = tPropertyFactor.Get<FbxDouble>();

		// Factor가 1이 아닐 경우 일종의 비율값으로 사용하여
		// 얻어온 vResult 에 모두 곱해준다.
		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return Vector4((float)vResult[0], (float)vResult[1], (float)vResult[2], (float)dFactor);
}

double FBXLoader::GetMaterialFactor(FbxSurfaceMaterial * pMtrl, const char * pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);
	double	dFactor = 0.0;

	if (tProperty.IsValid())
		dFactor = tProperty.Get<FbxDouble>();

	return dFactor;
}

string FBXLoader::GetMaterialTexture(FbxSurfaceMaterial * pMtrl, const char * pPropertyName)
{
	FbxProperty	tProperty = pMtrl->FindProperty(pPropertyName);

	string	str = "";

	if (tProperty.IsValid())
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
	FbxNodeAttribute*	pAttr = pNode->GetNodeAttribute();

	if (pAttr && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
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
	// MeshContainer를 만들고 컨테이너별 정점과 컨테이너&서브셋 별 인덱스 정보를 얻어와서 메쉬를 구성해야 한다.
	FBXMeshContainer* pContainer = new FBXMeshContainer();
	pContainer->isBump = false;
	m_vecMeshContainer.push_back(pContainer);

	// ControlPoint 는 위치정보를 담고 있는 배열이다. 이 배열의 개수는 곧 정점의 개수가 된다.
	int	iVtxCount = pMesh->GetControlPointsCount();

	// 내부적으로 FbxVector4타입의 배열로 저장하고 있기 때문에 배열의 시작주소를 얻어온다.
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
		pContainer->vecPos[i].x = (float)pVtxPos[i].mData[0];
		pContainer->vecPos[i].y = (float)pVtxPos[i].mData[2];
		pContainer->vecPos[i].z = (float)pVtxPos[i].mData[1];
	}

	// 폴리곤 수를 얻어온다.
	int	iPolygonCount = pMesh->GetPolygonCount();

	UINT iVtxID = 0;

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
		// 이 폴리곤을 구성하는 정점의 수를 얻어온다. 삼각형일 경우 3개를 얻어온다.
		int	iPolygonSize = pMesh->GetPolygonSize(i);

		int	iIdx[3] = {};

		for (int j = 0; j < iPolygonSize; ++j)
		{
			// 현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를 구한다.
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
	// 메쉬로부터 ElementNormal 정보를 얻어온다.
	FbxGeometryElementNormal*	pNormal = pMesh->GetElementNormal();

	int	iNormalIndex = iVtxID;

	// MappingMode와 ReferenceMode에 따라서 인덱스로 사용할 정보가 달라진다.
	if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iNormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iNormalIndex = pNormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vNormal = pNormal->GetDirectArray().GetAt(iNormalIndex);

	pContainer->vecNormal[iControlIndex].x = (float)vNormal.mData[0];
	pContainer->vecNormal[iControlIndex].y = (float)vNormal.mData[2];
	pContainer->vecNormal[iControlIndex].z = (float)vNormal.mData[1];
}

void FBXLoader::LoadUV(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iUVID, int iControlIndex)
{
	int iCount = pMesh->GetElementUVCount();
	FbxGeometryElementUV* pUV = pMesh->GetElementUV(0);

	if (pUV == NULLPTR)
		return;

	int	iUVIndex = iUVID;

	if (pUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iUVIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iUVIndex = pUV->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	else if (pUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			//iUVIndex = pUV->GetIndexArray().GetAt(iUVIndex);
		}
		break;
		default:
			break; // other reference modes not shown here!
		}
	}

	FbxVector2	vUV = pUV->GetDirectArray().GetAt(iUVIndex);

	pContainer->vecUV[iControlIndex].x = (float)vUV.mData[0] - (int)vUV.mData[0];
	pContainer->vecUV[iControlIndex].y = (float)(1.f - (vUV.mData[1] - (int)vUV.mData[1]));
}

void FBXLoader::LoadTangent(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementTangent*	pTangent = pMesh->GetElementTangent();

	if (pTangent == NULLPTR)
		return;

	pContainer->isBump = true;

	int	iTangentIndex = iVtxID;

	if (pTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iTangentIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iTangentIndex = pTangent->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vTangent = pTangent->GetDirectArray().GetAt(iTangentIndex);

	pContainer->vecTangent[iControlIndex].x = (float)vTangent.mData[0];
	pContainer->vecTangent[iControlIndex].y = (float)vTangent.mData[2];
	pContainer->vecTangent[iControlIndex].z = (float)vTangent.mData[1];
}

void FBXLoader::LoadBinormal(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iVtxID, int iControlIndex)
{
	FbxGeometryElementBinormal*	pBinormal = pMesh->GetElementBinormal();

	if (pBinormal == NULLPTR)
		return;

	pContainer->isBump = true;

	int	iBinormalIndex = iVtxID;

	if (pBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iVtxID);
			break;
		}
	}

	else if (pBinormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		switch (pBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			iBinormalIndex = iControlIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iBinormalIndex = pBinormal->GetIndexArray().GetAt(iControlIndex);
			break;
		}
	}

	FbxVector4	vBinormal = pBinormal->GetDirectArray().GetAt(iBinormalIndex);

	pContainer->vecBinormal[iControlIndex].x = (float)vBinormal.mData[0];
	pContainer->vecBinormal[iControlIndex].y = (float)vBinormal.mData[2];
	pContainer->vecBinormal[iControlIndex].z = (float)vBinormal.mData[1];
}
