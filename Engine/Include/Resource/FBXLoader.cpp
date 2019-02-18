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
	m_Scene->Clear();
	m_Scene->Destroy();
	m_Manager->Destroy();
}

bool FBXLoader::LoadFbx(const char* FullPath)
{
	/*
	FBX는 범용적인 파일 포맷이다.
	그래서 필요없는 데이터들이 있기때문에 내가 필요한 데이터만 뽑는과정이다.
	내가 필요한 데이터만 뽑아서 Binery화 시켜 로딩속도를 빠르게 하는것이 목표.

	1. Manager생성 - 최상위 (하나만 있어도 된다)
	2. IOSetting 생성
	3. Importer 생성
	4. Scene생성
	
	Importer가 읽은 데이터를 Scene에 넘겨준다.
	Scene은 그래프구조 (트리)
	
	Scene에서 자식노드와 속성을 돌며 데이터 내 구조에 맞게 데이터를 입력한다.

	FbxScene
	FBX의 모든 정보를 가지고 있는 놈.

	Scene에서 Read한 Material과
	Mesh안에있는 Material과는 같지만 다른존재이다(?)

	첫번째 ReadMaterial에서는 모든 Material정보를 가져오는 것이고 (for; MaterialCount)
	Mesh에서 Material은 정점이 어느 Material에 속하는것인가를 지정하는 것이다.
	*/

	m_Manager = FbxManager::Create();
	FbxIOSettings* pIos = FbxIOSettings::Create(m_Manager, IOSROOT);
	FbxImporter* pImporter = FbxImporter::Create(m_Manager, "");
	m_Scene = FbxScene::Create(m_Manager, "");

	// FbxManager에 지정한다.
	m_Manager->SetIOSettings(pIos);

	// 해당 경로에 있는 fbx 파일을 읽어오고 정보를 만들어낸다.
	pImporter->Initialize(FullPath, -1, m_Manager->GetIOSettings());

	// 위에서 만들어낸 정보를 FbxScene에 노드를 구성한다.
	pImporter->Import(m_Scene);

	//Max의 Axis시스템은 Y와 Z가 바뀌어있다. 밑의 코드는 딱히 의미 없다.
	//무조건 Max Axis축으로 튀어 나옴
	if (m_Scene->GetGlobalSettings().GetAxisSystem() != FbxAxisSystem::Max)
		m_Scene->GetGlobalSettings().SetAxisSystem(FbxAxisSystem::Max);

	pImporter->Destroy();

	ReadMaterial(m_Scene);
	//WriteMaterialXML();

	return true;
}

void FBXLoader::ReadMaterial(FbxScene* scene)
{	
	/*
	FbxProperty
	자세한 내용은 너무 방대하다.
	간단히 생각해서 내가 가져올 데이터들은 무조건 Property에 있다고 생각하면 된다.

	Factor는 독립된 놈이다. 연관이 있을뿐

	이 함수에서의 최종 목표는 Material데이터를 XML로 뽑아낸다.
	1. FBXRead
	2. Read한 Data -> XMLWrite
	*/

	//재질정보의 갯수대로 반복돌며 가져온 정보를 벡터에 넣어준다
	for (int i = 0; i < scene->GetMaterialCount(); ++i)
	{
		FbxSurfaceMaterial* getMaterial = scene->GetMaterial(i);
		FBXMaterial* newMaterial = new FBXMaterial();
		
		newMaterial->MaterialName = getMaterial->GetName();

		//렘버트가 있다면 Diffuse를 가져온다.
		if (getMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) == true)
		{
			//FbxSurfaceMaterial 이놈이 부모다.
			FbxSurfaceLambert* getLambert = (FbxSurfaceLambert*)getMaterial;
			newMaterial->Diffuse = ToColor(getLambert->Diffuse, getLambert->DiffuseFactor);
		}

		//Phong이 있다면 Spcular를 가져온다. (Phong은 Spcular에만 영향을 주기때문 보통 둘 다 있음)
		if (getMaterial->GetClassId().Is(FbxSurfacePhong::ClassId) == true)
		{
			FbxSurfacePhong* getPhong = (FbxSurfacePhong*)getMaterial;
			newMaterial->Spcular_Exp = (float)getPhong->Shininess;
		}

		newMaterial->DiffuseTexName = GetTextureFileName(getMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse));
		newMaterial->SpcularTexName = GetTextureFileName(getMaterial->FindProperty(FbxSurfaceMaterial::sSpecular));
		newMaterial->BumpTexName = GetTextureFileName(getMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap));
		
		m_vecMaterials.push_back(newMaterial);
	}
}

void FBXLoader::WriteMaterialXML(const string& FileName, const string& PathKey)
{
	tinyxml2::XMLDocument* Document = new tinyxml2::XMLDocument();

	tinyxml2::XMLDeclaration* declaration = Document->NewDeclaration();
	Document->LinkEndChild(declaration);

	tinyxml2::XMLElement* root = Document->NewElement("MaterialData");
	Document->LinkEndChild(root);

	for (auto const& bms_material : m_vecMaterials)
	{
		tinyxml2::XMLElement* newElement = Document->NewElement("Material");
		root->LinkEndChild(newElement);

		tinyxml2::XMLElement* Element = NULLPTR;

		Element = Document->NewElement("Name");
		Element->SetText(bms_material->MaterialName.c_str());
		newElement->LinkEndChild(Element);

		Element = Document->NewElement("Diffuse File");
		Element->SetText(bms_material->DiffuseTexName.c_str());
		newElement->LinkEndChild(Element);

		Element = Document->NewElement("Specular File");
		Element->SetText(bms_material->SpcularTexName.c_str());
		newElement->LinkEndChild(Element);

		Element = Document->NewElement("Normal File");
		Element->SetText(bms_material->BumpTexName.c_str());
		newElement->LinkEndChild(Element);

		{
			Element = Document->NewElement("Diffuse");
			newElement->LinkEndChild(Element);

			tinyxml2::XMLElement* r = Document->NewElement("R");
			r->SetText(bms_material->Diffuse.x);
			Element->LinkEndChild(r);

			tinyxml2::XMLElement* g = Document->NewElement("G");
			g->SetText(bms_material->Diffuse.y);
			Element->LinkEndChild(g);

			tinyxml2::XMLElement* b = Document->NewElement("B");
			b->SetText(bms_material->Diffuse.z);
			Element->LinkEndChild(b);

			tinyxml2::XMLElement* a = Document->NewElement("A");
			a->SetText(bms_material->Diffuse.w);
			Element->LinkEndChild(a);
		}

		{
			Element = Document->NewElement("Specular");
			newElement->LinkEndChild(Element);

			tinyxml2::XMLElement* r = Document->NewElement("R");
			r->SetText(bms_material->Spcular.x);
			Element->LinkEndChild(r);

			tinyxml2::XMLElement* g = Document->NewElement("G");
			g->SetText(bms_material->Spcular.y);
			Element->LinkEndChild(g);

			tinyxml2::XMLElement* b = Document->NewElement("B");
			b->SetText(bms_material->Spcular.z);
			Element->LinkEndChild(b);

			tinyxml2::XMLElement* a = Document->NewElement("A");
			a->SetText(bms_material->Spcular.w);
			Element->LinkEndChild(a);

			tinyxml2::XMLElement* specular_exp = Document->NewElement("Specular Exp");
			a->SetText(bms_material->Spcular_Exp);
			Element->LinkEndChild(specular_exp);
		}
	}

	string FullPath = PathManager::Get()->FindPathMultiByte(PathKey);
	FullPath += FileName;

	Document->SaveFile(FullPath.c_str());
}

void FBXLoader::ReadJoint(FbxScene* Scene, FbxNode* Node, int Index, int ParentIndex)
{
	FbxNodeAttribute* Attribute = Node->GetNodeAttribute();

	if (Attribute != NULLPTR)
	{
		bool Check = false;

		switch (Attribute->GetAttributeType())
		{
		case FbxNodeAttribute::eNull: //잡 데이터들 안에 Joint데이터가 있을 수도 있음
			Check = true;
			break;

		case FbxNodeAttribute::eMarker: //머리위에 닉네임을 표시할 Joint
			Check = true;
			break;

		case FbxNodeAttribute::eSkeleton: //Joint의 집합
			Check = true;
			break;

		case FbxNodeAttribute::eMesh: //Mesh 자체
			Check = true;
			break;
		}

		if (Check == true)
		{
			FBXJoint* newJoint = new FBXJoint();
			newJoint->Name = Node->GetName();
			newJoint->Index = Index;
			newJoint->ParentIndex = ParentIndex;
			newJoint->LocalTransform = ToMatrix(Node->EvaluateLocalTransform());
			newJoint->GlobalTransform = ToMatrix(Node->EvaluateGlobalTransform());

			m_vecJoints.push_back(newJoint);

			if (Attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
				ReadMesh(Scene, Node, Index);
		}
	}

	for (int i = 0; i < Node->GetChildCount(); ++i)
		ReadJoint(Scene, Node->GetChild(i), m_vecJoints.size(), Index);
}

void FBXLoader::ReadMesh(FbxScene* Scene, FbxNode* Node, int JointIndex)
{
	FbxMesh* getMesh = Node->GetMesh();
	vector<FBXVertex*> vertices;

	//삼각형갯수
	for (size_t i = 0; i < getMesh->GetPolygonCount(); ++i)
	{
		int polygon_size = getMesh->GetPolygonSize(i);
		
		//정점이 3개가 아니라면 빾
		if (polygon_size != 3)
			TrueAssert(true);

		//삼각형 안에 정점으로 들어왔다.
		//GL좌표계기준이라서 인덱스를 거꾸로 돌려서 우리좌표계로 맞췄다
		for (size_t j = polygon_size - 1; j >= 0; --j) // Winding Order CCW(RH) -> CW(LH)
		{
			//최적화문제때문에 전부 데이터들을 인덱스로 떡칠 해놨다
			FBXVertex* newVertex = new FBXVertex();

			//Vertex의 Index (0 1 2 3 .. 라는 인덱스가 VertexData를 가르킨다.)
			auto ControlPointIndex = getMesh->GetPolygonVertex(i, j);
			newVertex->ControlPointIndex = ControlPointIndex;

			//polygon_vertex_idx = 인덱스의 인덱스
			//0 1 2 / 2 1 3 <- 인덱스
			//0 1 2 3 4 5 <- 인덱스의 인덱스
			auto PolygonVertexIndex = 3 * i + j;
			newVertex->MaterialName = GetMaterialName(getMesh, i, PolygonVertexIndex, ControlPointIndex);

			Vector3 temp;

			FbxVector4 position = getMesh->GetControlPointAt(ControlPointIndex);
			temp = ToVector3(position);
			newVertex->Vertex3D.Pos = temp.TransformCoord(ToDirectX());

			FbxVector4 normal;
			getMesh->GetPolygonVertexNormal(i, j, normal);
			normal.Normalize();
			temp = ToVector3(normal);
			newVertex->Vertex3D.Normal = temp.TransformNormal(ToDirectX());

			int uv_idx = getMesh->GetTextureUVIndex(i, j);
			newVertex->Vertex3D.UV = GetUV(getMesh, ControlPointIndex, uv_idx);

			vertices.push_back(newVertex);
		}
	}

	FBXMesh* newMeshData = new FBXMesh();
	newMeshData->Name = Node->GetName();
	newMeshData->JointIndex = JointIndex;
	newMeshData->Mesh = getMesh;
	newMeshData->Vertices = vertices;

	m_vecMeshs.push_back(newMeshData);

	// Mesh_Part 추가
	for (int i = 0; i < Scene->GetMaterialCount(); ++i)
	{
		FbxSurfaceMaterial* getSurfaceMaterial = Scene->GetMaterial(i);
		string material_name = getSurfaceMaterial->GetName();

		vector<FBXVertex> gather;
		for (size_t i = 0; i < newMeshData->Vertices.size(); i++)
		{
			FBXVertex* CurVertex = newMeshData->Vertices[i];
			
			//이름이 같으면 넣는다. Scene의 Material에 어디소속인지
			if (material_name == CurVertex->MaterialName)
				gather.push_back(*CurVertex);
		}
		if (gather.empty() == true)
			continue;

		FBXMeshPart* newMeshPart = new FBXMeshPart();
		newMeshPart->MaterialName = material_name;

		for (size_t i = 0; i < gather.size(); i++)
		{
			newMeshPart->Vertices.push_back(gather[i].Vertex3D);
			newMeshPart->Indices.push_back(newMeshPart->Indices.size()); // 일단은 인덱스 최적화 안함
		}
		newMeshData->MeshPart.push_back(newMeshPart);
	}
}

Matrix FBXLoader::ToDirectX()
{
	//Y를 180도 돌리는 행렬
	//추가적으로 X축으로 -90도 회전시켜줘야함. 일단은 이대로
	Matrix a;
	Matrix b;
	a = a.RotationY(180.0f);
	b = b.Scaling(-1.0f, 1.0f, 1.0f); //X축을 뒤집기위함 (오른손좌표계)

	return b * a;
}

Vector2 FBXLoader::ToVector2(const FbxVector2& _v)
{
	Vector2 result{};
	result.x = (float)_v.mData[0];
	result.y = (float)_v.mData[1];

	return result;
}

Vector3 FBXLoader::ToVector3(const FbxVector4& _v)
{
	Vector3 result;
	result.x = (float)_v.mData[0];
	result.y = (float)_v.mData[1];
	result.z = (float)_v.mData[2];

	return result;
}

Vector4 FBXLoader::ToColor(const FbxVector4& _v)
{
	Vector4 result;
	//A는 무조건 1로채워서 리턴하겠다.
	result.x = (float)_v.mData[0];
	result.y = (float)_v.mData[1];
	result.z = (float)_v.mData[2];
	result.w = 1.0f;

	return result;
}

Vector4 FBXLoader::ToColor(const FbxPropertyT<FbxDouble3>& _color, const FbxPropertyT<FbxDouble>& _factor)
{
	//a or w값에 Factor를 채워 넣겠다.
	FbxDouble3 color = _color;

	Vector4 result;
	result.x = (float)color.mData[0];
	result.y = (float)color.mData[1];
	result.z = (float)color.mData[2];
	result.w = (float)_factor;

	return result;
}

Matrix FBXLoader::ToMatrix(FbxAMatrix const& _m)
{
	//행 가져옴
	auto r1 = _m.GetRow(0);
	auto r2 = _m.GetRow(1);
	auto r3 = _m.GetRow(2);
	auto r4 = _m.GetRow(3);

	Matrix result{};
	result._11 = (float)r1.mData[0];
	result._12 = (float)r1.mData[1];
	result._13 = (float)r1.mData[2];
	result._14 = (float)r1.mData[3];

	result._21 = (float)r2.mData[0];
	result._22 = (float)r2.mData[1];
	result._23 = (float)r2.mData[2];
	result._24 = (float)r2.mData[3];

	result._31 = (float)r3.mData[0];
	result._32 = (float)r3.mData[1];
	result._33 = (float)r3.mData[2];
	result._34 = (float)r3.mData[3];

	result._41 = (float)r4.mData[0];
	result._42 = (float)r4.mData[1];
	result._43 = (float)r4.mData[2];
	result._44 = (float)r4.mData[3];

	//이렇게하면 Z와 Y축 요소들이 뒤집힌다.
	return ToDirectX() * result * ToDirectX();
}

string FBXLoader::GetTextureFileName(const FbxProperty& Property)
{
	string FileName;

	//유요한지 체크한다.
	if (Property.IsValid() == true)
	{
		//Src - 내 밑에있는 것들
		if (Property.GetSrcObjectCount() > 0)
		{
			FbxFileTexture* getTexture = Property.GetSrcObject<FbxFileTexture>();

			if (getTexture != NULLPTR)
				FileName = getTexture->GetFileName();
		}
	}

	//보통 하나만쓰기때문에 하나만 리턴해준다.
	return FileName;
}

string FBXLoader::GetMaterialName(FbxMesh* Mesh, int PolygonIndex, int PolygonVertexIndex, int ControlPointIndex)
{
	string result;
	FbxNode* getNode = Mesh->GetNode();
	// 게임에서는 성능 때문에 0번 레이어만 사용
	FbxLayerElementMaterial* getLayerElement = Mesh->GetLayer(0)->GetMaterials();

	if (getNode == NULLPTR)
		return result;
	if (getLayerElement == NULLPTR)
		return result;

	//맵핑모드 : 삼각형마다 정점마다,정점의 인덱스마다,인덱스마다, Scene의 Material의 어디에 속해있다.
	//AllSame : 전부 같은 Scene의 Material을 쓴다
	//Ref모드 : 직접참조할것인가? (요청을 하는것이라고 보면됨, Get함수를 써서 할것인가 아니면 직접적으로 가져올것인가)
	FbxLayerElement::EMappingMode mapping_mode = getLayerElement->GetMappingMode();
	FbxLayerElement::EReferenceMode reference_mode = getLayerElement->GetReferenceMode();

	int ResultIndex = -1;

	//옵션에따른 인덱스 변경
	switch (mapping_mode)
	{
	case FbxLayerElement::eAllSame:
		ResultIndex = 0;
		break;
	case FbxLayerElement::eByPolygon:
		ResultIndex = PolygonIndex;
		break;
	case FbxLayerElement::eByControlPoint:
		ResultIndex = ControlPointIndex;
		break;
	case FbxLayerElement::eByPolygonVertex:
		ResultIndex = PolygonVertexIndex;
		break;
	}

	//이 옵션이 들어간다면 중간에 인덱스배열이 하나 더있는데 거기안에있는 번호가 Scene의 Material과 매칭시킨다.
	if (reference_mode == FbxLayerElement::eIndexToDirect)
		ResultIndex = getLayerElement->GetIndexArray().GetAt(ResultIndex); 

	// material은 direct array로 직접 엑세스 불가능
	//Node에도 Material정보가 있는데 이놈은 Scene의 Material을 참조하고 있다.
	if (getNode->GetMaterial(ResultIndex) != NULLPTR)
		result = getNode->GetMaterial(ResultIndex)->GetName();

	return result;
}

Vector2 FBXLoader::GetUV(FbxMesh * Mesh, int ControlPointIndex, int UVIndex)
{
	Vector2 Result;

	FbxLayerElementUV* getUV = Mesh->GetLayer(0)->GetUVs();

	if (getUV == NULLPTR)
		return Result;

	FbxLayerElement::EMappingMode mapping_mode = getUV->GetMappingMode();
	FbxLayerElement::EReferenceMode reference_mode = getUV->GetReferenceMode();

	int mapping_idx = -1;

	switch (mapping_mode)
	{
	case FbxLayerElement::eByControlPoint:
		mapping_idx = ControlPointIndex;
		break;
	case FbxLayerElement::eByPolygonVertex:
		mapping_idx = UVIndex;
		break;
	}

	if (FbxLayerElement::eIndexToDirect == reference_mode)
		mapping_idx = getUV->GetIndexArray().GetAt(mapping_idx);

	Result.x = (float)getUV->GetDirectArray().GetAt(mapping_idx).mData[0];
	Result.y = (float)getUV->GetDirectArray().GetAt(mapping_idx).mData[1];

	//GL좌표계는 UV중 V가 뒤집혀있다. 다시 뒤집어준다.
	Result.y = 1.0f - Result.y;

	return Result;
}

//void FBXLoader::LoadMesh(FbxNode * pNode)
//{
//	//본격적으로 Mesh를 로드하는 함수.
//
//	//노드의 속성을 가져온다.
//	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();
//
//	//속성이 Mesh 타입이면 로드.
//	if (pAttr != NULLPTR && pAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
//	{
//		FbxMesh* pMesh = pNode->GetMesh();
//
//		if (pMesh != NULLPTR)
//			LoadMesh(pMesh);
//	}
//
//	int	iChildCount = pNode->GetChildCount();
//
//	for (int i = 0; i < iChildCount; ++i)
//		LoadMesh(pNode->GetChild(i));
//}
//
//void FBXLoader::LoadMesh(FbxMesh * pMesh)
//{
//	// MeshContainer를 만들고 컨테이너별 정점과 컨테이너&서브셋 별
//	// 인덱스 정보를 얻어와서 메쉬를 구성해야 한다.
//	FBXMeshContainer* pContainer = new FBXMeshContainer();
//	pContainer->isBump = false;
//	m_vecMeshContainer.push_back(pContainer);
//
//	// ControlPoint 는 위치정보를 담고 있는 배열이다.
//	// 이 배열의 개수는 곧 정점의 개수가 된다.
//	int	iVtxCount = pMesh->GetControlPointsCount();
//
//	// 내부적으로 FbxVector4타입의 배열로 저장하고 있기 때문에 배열의 
//	// 시작주소를 얻어온다.
//	FbxVector4*	pVtxPos = pMesh->GetControlPoints();
//
//	// 컨테이너가 가지고 있는 정점 정보들을 정점수만큼 resize 해준다.
//	pContainer->vecPos.resize(iVtxCount);
//	pContainer->vecNormal.resize(iVtxCount);
//	pContainer->vecUV.resize(iVtxCount);
//	pContainer->vecTangent.resize(iVtxCount);
//	pContainer->vecBinormal.resize(iVtxCount);
//
//	for (int i = 0; i < iVtxCount; ++i)
//	{
//		// y와 z축이 바뀌어 있기 때문에 변경해준다.
//		pContainer->vecPos[i].x = pVtxPos[i].mData[0];
//		pContainer->vecPos[i].y = pVtxPos[i].mData[2];
//		pContainer->vecPos[i].z = pVtxPos[i].mData[1];
//	}
//
//	// 폴리곤 수를 얻어온다.
//	int	iPolygonCount = pMesh->GetPolygonCount();
//	UINT	iVtxID = 0;
//
//	// 재질 수를 얻어온다.
//	int	iMtrlCount = pMesh->GetNode()->GetMaterialCount();
//
//	// 재질 수는 곧 서브셋 수이기 때문에 재질 수만큼 resize 한다.
//	pContainer->vecIndices.resize(iMtrlCount);
//
//	// 재질 정보를 얻어온다.
//	FbxGeometryElementMaterial*	pMaterial = pMesh->GetElementMaterial();
//	int iCount = pMesh->GetElementMaterialCount();
//	// 삼각형 수만큼 반복한다.
//	for (int i = 0; i < iPolygonCount; ++i)
//	{
//		// 이 폴리곤을 구성하는 정점의 수를 얻어온다.
//		// 삼각형일 경우 3개를 얻어온다.
//		int	iPolygonSize = pMesh->GetPolygonSize(i);
//
//		int	iIdx[3] = {};
//
//		for (int j = 0; j < iPolygonSize; ++j)
//		{
//			// 현재 삼각형을 구성하고 있는 버텍스정보 내에서의 인덱스를
//			// 구한다.
//			int	iControlIndex = pMesh->GetPolygonVertex(i, j);
//
//			iIdx[j] = iControlIndex;
//
//			LoadNormal(pMesh, pContainer, iVtxID, iControlIndex);
//			LoadUV(pMesh, pContainer, pMesh->GetTextureUVIndex(i, j), iControlIndex);
//			LoadTangent(pMesh, pContainer, iVtxID, iControlIndex);
//			LoadBinormal(pMesh, pContainer, iVtxID, iControlIndex);
//
//			++iVtxID;
//		}
//
//		int	iMtrlID = pMaterial->GetIndexArray().GetAt(i);
//
//		pContainer->vecIndices[iMtrlID].push_back(iIdx[0]);
//		pContainer->vecIndices[iMtrlID].push_back(iIdx[2]);
//		pContainer->vecIndices[iMtrlID].push_back(iIdx[1]);
//	}
//}
//
//void FBXLoader::LoadNormal(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iVtxID, int iControlIndex)
//{
//}
//
//void FBXLoader::LoadUV(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iUVID, int iControlIndex)
//{
//}
//
//void FBXLoader::LoadTangent(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iVtxID, int iControlIndex)
//{
//}
//
//void FBXLoader::LoadBinormal(FbxMesh * pMesh, FBXMeshContainer * pContainer, int iVtxID, int iControlIndex)
//{
//}