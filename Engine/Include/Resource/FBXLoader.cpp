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
	FBX�� �������� ���� �����̴�.
	�׷��� �ʿ���� �����͵��� �ֱ⶧���� ���� �ʿ��� �����͸� �̴°����̴�.
	���� �ʿ��� �����͸� �̾Ƽ� Bineryȭ ���� �ε��ӵ��� ������ �ϴ°��� ��ǥ.

	1. Manager���� - �ֻ��� (�ϳ��� �־ �ȴ�)
	2. IOSetting ����
	3. Importer ����
	4. Scene����
	
	Importer�� ���� �����͸� Scene�� �Ѱ��ش�.
	Scene�� �׷������� (Ʈ��)
	
	Scene���� �ڽĳ��� �Ӽ��� ���� ������ �� ������ �°� �����͸� �Է��Ѵ�.

	FbxScene
	FBX�� ��� ������ ������ �ִ� ��.

	Scene���� Read�� Material��
	Mesh�ȿ��ִ� Material���� ������ �ٸ������̴�(?)

	ù��° ReadMaterial������ ��� Material������ �������� ���̰� (for; MaterialCount)
	Mesh���� Material�� ������ ��� Material�� ���ϴ°��ΰ��� �����ϴ� ���̴�.
	*/

	m_Manager = FbxManager::Create();
	FbxIOSettings* pIos = FbxIOSettings::Create(m_Manager, IOSROOT);
	FbxImporter* pImporter = FbxImporter::Create(m_Manager, "");
	m_Scene = FbxScene::Create(m_Manager, "");

	// FbxManager�� �����Ѵ�.
	m_Manager->SetIOSettings(pIos);

	// �ش� ��ο� �ִ� fbx ������ �о���� ������ ������.
	pImporter->Initialize(FullPath, -1, m_Manager->GetIOSettings());

	// ������ ���� ������ FbxScene�� ��带 �����Ѵ�.
	pImporter->Import(m_Scene);

	//Max�� Axis�ý����� Y�� Z�� �ٲ���ִ�. ���� �ڵ�� ���� �ǹ� ����.
	//������ Max Axis������ Ƣ�� ����
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
	�ڼ��� ������ �ʹ� ����ϴ�.
	������ �����ؼ� ���� ������ �����͵��� ������ Property�� �ִٰ� �����ϸ� �ȴ�.

	Factor�� ������ ���̴�. ������ ������

	�� �Լ������� ���� ��ǥ�� Material�����͸� XML�� �̾Ƴ���.
	1. FBXRead
	2. Read�� Data -> XMLWrite
	*/

	//���������� ������� �ݺ����� ������ ������ ���Ϳ� �־��ش�
	for (int i = 0; i < scene->GetMaterialCount(); ++i)
	{
		FbxSurfaceMaterial* getMaterial = scene->GetMaterial(i);
		FBXMaterial* newMaterial = new FBXMaterial();
		
		newMaterial->MaterialName = getMaterial->GetName();

		//����Ʈ�� �ִٸ� Diffuse�� �����´�.
		if (getMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) == true)
		{
			//FbxSurfaceMaterial �̳��� �θ��.
			FbxSurfaceLambert* getLambert = (FbxSurfaceLambert*)getMaterial;
			newMaterial->Diffuse = ToColor(getLambert->Diffuse, getLambert->DiffuseFactor);
		}

		//Phong�� �ִٸ� Spcular�� �����´�. (Phong�� Spcular���� ������ �ֱ⶧�� ���� �� �� ����)
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
		case FbxNodeAttribute::eNull: //�� �����͵� �ȿ� Joint�����Ͱ� ���� ���� ����
			Check = true;
			break;

		case FbxNodeAttribute::eMarker: //�Ӹ����� �г����� ǥ���� Joint
			Check = true;
			break;

		case FbxNodeAttribute::eSkeleton: //Joint�� ����
			Check = true;
			break;

		case FbxNodeAttribute::eMesh: //Mesh ��ü
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

	//�ﰢ������
	for (size_t i = 0; i < getMesh->GetPolygonCount(); ++i)
	{
		int polygon_size = getMesh->GetPolygonSize(i);
		
		//������ 3���� �ƴ϶�� ��
		if (polygon_size != 3)
			TrueAssert(true);

		//�ﰢ�� �ȿ� �������� ���Դ�.
		//GL��ǥ������̶� �ε����� �Ųٷ� ������ �츮��ǥ��� �����
		for (size_t j = polygon_size - 1; j >= 0; --j) // Winding Order CCW(RH) -> CW(LH)
		{
			//����ȭ���������� ���� �����͵��� �ε����� ��ĥ �س���
			FBXVertex* newVertex = new FBXVertex();

			//Vertex�� Index (0 1 2 3 .. ��� �ε����� VertexData�� ����Ų��.)
			auto ControlPointIndex = getMesh->GetPolygonVertex(i, j);
			newVertex->ControlPointIndex = ControlPointIndex;

			//polygon_vertex_idx = �ε����� �ε���
			//0 1 2 / 2 1 3 <- �ε���
			//0 1 2 3 4 5 <- �ε����� �ε���
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

	// Mesh_Part �߰�
	for (int i = 0; i < Scene->GetMaterialCount(); ++i)
	{
		FbxSurfaceMaterial* getSurfaceMaterial = Scene->GetMaterial(i);
		string material_name = getSurfaceMaterial->GetName();

		vector<FBXVertex> gather;
		for (size_t i = 0; i < newMeshData->Vertices.size(); i++)
		{
			FBXVertex* CurVertex = newMeshData->Vertices[i];
			
			//�̸��� ������ �ִ´�. Scene�� Material�� ���Ҽ�����
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
			newMeshPart->Indices.push_back(newMeshPart->Indices.size()); // �ϴ��� �ε��� ����ȭ ����
		}
		newMeshData->MeshPart.push_back(newMeshPart);
	}
}

Matrix FBXLoader::ToDirectX()
{
	//Y�� 180�� ������ ���
	//�߰������� X������ -90�� ȸ�����������. �ϴ��� �̴��
	Matrix a;
	Matrix b;
	a = a.RotationY(180.0f);
	b = b.Scaling(-1.0f, 1.0f, 1.0f); //X���� ���������� (��������ǥ��)

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
	//A�� ������ 1��ä���� �����ϰڴ�.
	result.x = (float)_v.mData[0];
	result.y = (float)_v.mData[1];
	result.z = (float)_v.mData[2];
	result.w = 1.0f;

	return result;
}

Vector4 FBXLoader::ToColor(const FbxPropertyT<FbxDouble3>& _color, const FbxPropertyT<FbxDouble>& _factor)
{
	//a or w���� Factor�� ä�� �ְڴ�.
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
	//�� ������
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

	//�̷����ϸ� Z�� Y�� ��ҵ��� ��������.
	return ToDirectX() * result * ToDirectX();
}

string FBXLoader::GetTextureFileName(const FbxProperty& Property)
{
	string FileName;

	//�������� üũ�Ѵ�.
	if (Property.IsValid() == true)
	{
		//Src - �� �ؿ��ִ� �͵�
		if (Property.GetSrcObjectCount() > 0)
		{
			FbxFileTexture* getTexture = Property.GetSrcObject<FbxFileTexture>();

			if (getTexture != NULLPTR)
				FileName = getTexture->GetFileName();
		}
	}

	//���� �ϳ������⶧���� �ϳ��� �������ش�.
	return FileName;
}

string FBXLoader::GetMaterialName(FbxMesh* Mesh, int PolygonIndex, int PolygonVertexIndex, int ControlPointIndex)
{
	string result;
	FbxNode* getNode = Mesh->GetNode();
	// ���ӿ����� ���� ������ 0�� ���̾ ���
	FbxLayerElementMaterial* getLayerElement = Mesh->GetLayer(0)->GetMaterials();

	if (getNode == NULLPTR)
		return result;
	if (getLayerElement == NULLPTR)
		return result;

	//���θ�� : �ﰢ������ ��������,������ �ε�������,�ε�������, Scene�� Material�� ��� �����ִ�.
	//AllSame : ���� ���� Scene�� Material�� ����
	//Ref��� : ���������Ұ��ΰ�? (��û�� �ϴ°��̶�� �����, Get�Լ��� �Ἥ �Ұ��ΰ� �ƴϸ� ���������� �����ð��ΰ�)
	FbxLayerElement::EMappingMode mapping_mode = getLayerElement->GetMappingMode();
	FbxLayerElement::EReferenceMode reference_mode = getLayerElement->GetReferenceMode();

	int ResultIndex = -1;

	//�ɼǿ����� �ε��� ����
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

	//�� �ɼ��� ���ٸ� �߰��� �ε����迭�� �ϳ� ���ִµ� �ű�ȿ��ִ� ��ȣ�� Scene�� Material�� ��Ī��Ų��.
	if (reference_mode == FbxLayerElement::eIndexToDirect)
		ResultIndex = getLayerElement->GetIndexArray().GetAt(ResultIndex); 

	// material�� direct array�� ���� ������ �Ұ���
	//Node���� Material������ �ִµ� �̳��� Scene�� Material�� �����ϰ� �ִ�.
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

	//GL��ǥ��� UV�� V�� �������ִ�. �ٽ� �������ش�.
	Result.y = 1.0f - Result.y;

	return Result;
}

//void FBXLoader::LoadMesh(FbxNode * pNode)
//{
//	//���������� Mesh�� �ε��ϴ� �Լ�.
//
//	//����� �Ӽ��� �����´�.
//	FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();
//
//	//�Ӽ��� Mesh Ÿ���̸� �ε�.
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
//	// MeshContainer�� ����� �����̳ʺ� ������ �����̳�&����� ��
//	// �ε��� ������ ���ͼ� �޽��� �����ؾ� �Ѵ�.
//	FBXMeshContainer* pContainer = new FBXMeshContainer();
//	pContainer->isBump = false;
//	m_vecMeshContainer.push_back(pContainer);
//
//	// ControlPoint �� ��ġ������ ��� �ִ� �迭�̴�.
//	// �� �迭�� ������ �� ������ ������ �ȴ�.
//	int	iVtxCount = pMesh->GetControlPointsCount();
//
//	// ���������� FbxVector4Ÿ���� �迭�� �����ϰ� �ֱ� ������ �迭�� 
//	// �����ּҸ� ���´�.
//	FbxVector4*	pVtxPos = pMesh->GetControlPoints();
//
//	// �����̳ʰ� ������ �ִ� ���� �������� ��������ŭ resize ���ش�.
//	pContainer->vecPos.resize(iVtxCount);
//	pContainer->vecNormal.resize(iVtxCount);
//	pContainer->vecUV.resize(iVtxCount);
//	pContainer->vecTangent.resize(iVtxCount);
//	pContainer->vecBinormal.resize(iVtxCount);
//
//	for (int i = 0; i < iVtxCount; ++i)
//	{
//		// y�� z���� �ٲ�� �ֱ� ������ �������ش�.
//		pContainer->vecPos[i].x = pVtxPos[i].mData[0];
//		pContainer->vecPos[i].y = pVtxPos[i].mData[2];
//		pContainer->vecPos[i].z = pVtxPos[i].mData[1];
//	}
//
//	// ������ ���� ���´�.
//	int	iPolygonCount = pMesh->GetPolygonCount();
//	UINT	iVtxID = 0;
//
//	// ���� ���� ���´�.
//	int	iMtrlCount = pMesh->GetNode()->GetMaterialCount();
//
//	// ���� ���� �� ����� ���̱� ������ ���� ����ŭ resize �Ѵ�.
//	pContainer->vecIndices.resize(iMtrlCount);
//
//	// ���� ������ ���´�.
//	FbxGeometryElementMaterial*	pMaterial = pMesh->GetElementMaterial();
//	int iCount = pMesh->GetElementMaterialCount();
//	// �ﰢ�� ����ŭ �ݺ��Ѵ�.
//	for (int i = 0; i < iPolygonCount; ++i)
//	{
//		// �� �������� �����ϴ� ������ ���� ���´�.
//		// �ﰢ���� ��� 3���� ���´�.
//		int	iPolygonSize = pMesh->GetPolygonSize(i);
//
//		int	iIdx[3] = {};
//
//		for (int j = 0; j < iPolygonSize; ++j)
//		{
//			// ���� �ﰢ���� �����ϰ� �ִ� ���ؽ����� �������� �ε�����
//			// ���Ѵ�.
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