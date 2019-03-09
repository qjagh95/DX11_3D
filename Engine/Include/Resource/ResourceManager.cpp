#include "../stdafx.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Sampler.h"

JEONG_USING
SINGLETON_VAR_INIT(ResourceManager)

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	//TODO : 릭
	Safe_Release_Map(m_MeshMap);
	Safe_Release_Map(m_TextureMap);
	Safe_Release_Map(m_SamplerMap);
}

bool ResourceManager::Init()
{
	VertexColor ColorTri[3] =
	{
		//투영공간은 -1 ~ 0의 범위를 갖는다.
		VertexColor(Vector3(0.0f, 0.5f, 0.0f), Vector4::Chartreuse),
		VertexColor(Vector3(0.5f, -0.5f , 0.0f), Vector4::DarkOrchid),
		VertexColor(Vector3(-0.5f, -0.5f, 0.0f), Vector4::DarkGreen),
	};

	unsigned short IndexTri[3] = { 0, 1, 2 };

	VertexColor ColorRect[4] =
	{
		//투영공간은 -1 ~ 0의 범위를 갖는다.
		VertexColor(Vector3(0.0f, 1.0f, 0.0f), Vector4::Red),
		VertexColor(Vector3(1.0f, 1.0f, 0.0f), Vector4::MediumSpringGreen),
		VertexColor(Vector3(0.0f, 0.0f, 0.0f), Vector4::Purple),
		VertexColor(Vector3(1.0f, 0.0f, 0.0f), Vector4::SteelBlue)
	};

	unsigned short IndexRect[6] = { 0, 1, 3, 0, 3, 2 };

	CreateMesh("ColorTri", STANDARD_COLOR_SHADER, POS_COLOR_LAYOUT, ColorTri, 3, sizeof(VertexColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexTri, 3, 2);
	CreateMesh("ColorRect", STANDARD_COLOR_SHADER, POS_COLOR_LAYOUT, ColorRect, 4, sizeof(VertexColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexRect, 6, 2);

	//UV정보를 위한 버텍스 구조체 초기화
	VertexUV UVRect[4] =
	{
		VertexUV(Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),
		VertexUV(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
		VertexUV(Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)),
		VertexUV(Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f))
	};

	CreateMesh("TextureRect", STANDARD_UV_SHADER, POS_UV_LAYOUT, UVRect, 4, sizeof(VertexUV), D3D11_USAGE_DEFAULT,	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexRect, 6, 2);

	Vector3	DebugColliderPos[5] =
	{
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(1.0f, 1.0f, 0.0f),
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f)
	};

	CreateMesh("ColliderRect", COLLIDER_SHADER, POS_LAYOUT, DebugColliderPos, 5, sizeof(Vector3), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Vector3	DebugColliderCirclePos[37] = {};

	float Angle = 0.0f;
	for (int i = 0; i < 37; i++)
	{
		DebugColliderCirclePos[i] = Vector3(cosf(DegreeToRadian(Angle)), sinf(DegreeToRadian(Angle)), 0.0f);
		Angle += 10.0f;
	}

	CreateMesh("ColliderCircle", COLLIDER_SHADER, POS_LAYOUT, DebugColliderCirclePos, 37, sizeof(Vector3), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Vector3	PyramidPos[5] =
	{
		Vector3(0.0f, 0.5f, 0.0f),  //0
		Vector3(-0.5f, -0.5f, 0.5f), //1
		Vector3(0.5f, -0.5f, 0.5f), //2
		Vector3(0.5f, -0.5f, -0.5f), //3
		Vector3(-0.5f, -0.5f, -0.5f) //4
	};

	VertexNormalColor NormalRect[4] =
	{
		//투영공간은 -1 ~ 0의 범위를 갖는다.
		VertexNormalColor(Vector3(-500.0f, 500.0f, 0.0f),Vector3(0.0f,0.0f,-1.0f), Vector4::Red),
		VertexNormalColor(Vector3(500.0f, 500.0f, 0.0f),Vector3(0.0f,0.0f,-1.0f), Vector4::MediumSpringGreen),
		VertexNormalColor(Vector3(500.0f, -500.0f, 0.0f),Vector3(0.0f,0.0f,-1.0f), Vector4::SteelBlue),
		VertexNormalColor(Vector3(-500.0f, -500.0f, 0.0f),Vector3(0.0f,0.0f,-1.0f), Vector4::Purple)
	};

	unsigned short IndexRect12[6] = { 0, 1, 3, 3, 1, 2 };
	CreateMesh("NormalRect", STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, NormalRect, 4, sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexRect12, 6, 2, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT);

	Vector3	PlaneNormal[4];
	Vector3	Edge1, Edge2;

	Edge1 = PyramidPos[4] - PyramidPos[0];
	Edge2 = PyramidPos[1] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[0] = Edge1.Cross(Edge2);
	PlaneNormal[0].Normalize();

	Edge1 = PyramidPos[1] - PlaneNormal[0];
	Edge2 = PyramidPos[2] - PlaneNormal[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[1] = Edge1.Cross(Edge2);
	PlaneNormal[1].Normalize();

	Edge1 = PlaneNormal[2] - PyramidPos[0];
	Edge2 = PlaneNormal[3] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[2] = Edge1.Cross(Edge2);
	PlaneNormal[2].Normalize();

	Edge1 = PyramidPos[3] - PyramidPos[0];
	Edge2 = PyramidPos[4] - PyramidPos[0];
	Edge1.Normalize();
	Edge2.Normalize();
	PlaneNormal[3] = Edge1.Cross(Edge2);
	PlaneNormal[3].Normalize();

	Vector3	Normal[4];
	Normal[0] = (PlaneNormal[0] + PlaneNormal[1]);
	Normal[0].Normalize();

	Normal[1] = (PlaneNormal[1] + PlaneNormal[2]);
	Normal[1].Normalize();

	Normal[2] = (PlaneNormal[2] + PlaneNormal[3]);
	Normal[2].Normalize();

	Normal[3] = (PlaneNormal[3] + PlaneNormal[0]);
	Normal[3].Normalize();

	VertexNormalColor Pyramid[9] =
	{
		VertexNormalColor(PyramidPos[0], Vector3(0.0f, 1.0f, 0.0f),  Vector4::Bisque),
		VertexNormalColor(PyramidPos[1], Normal[0], Vector4::Cornsilk),
		VertexNormalColor(PyramidPos[2], Normal[1], Vector4::DarkGreen),
		VertexNormalColor(PyramidPos[3], Normal[2], Vector4::DarkOrange),
		VertexNormalColor(PyramidPos[4], Normal[3], Vector4::Bisque),
		VertexNormalColor(PyramidPos[1], Vector3(0.0f, -1.0f, 0.0f), Vector4::White),
		VertexNormalColor(PyramidPos[2], Vector3(0.0f, -1.0f, 0.0f), Vector4::White),
		VertexNormalColor(PyramidPos[3], Vector3(0.0f, -1.0f, 0.0f), Vector4::White),
		VertexNormalColor(PyramidPos[4], Vector3(0.0f, -1.0f, 0.0f), Vector4::White)
	};

	int	PyramidIdx[18] = {0, 4, 1, 2, 0, 1, 3, 0, 2, 4, 0, 3, 8, 6, 5, 7, 6, 8 };
	//{0, 4, 1}
	//{2, 1, 0}
	//{3, 0, 2}
	//{4, 0, 3}
	//{8, 6, 5}
	//{7, 6, 8}
	CreateMesh("Pyramid", STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, Pyramid, 9, sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, PyramidIdx, 18, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	CreateSphereVolum(SPHERE_VOLUM, 0.5f, 64, 128);
	CreateCylinderVolum(CYLINDER_VOLUM, 0.5f, 3, 32);
	CreateCapsulVolum(CAPSUL_VOLUM, 0.5f, 3, 64, 128);
	CreateCornVolum(CORN_VOLUM, 0.5f, 0.5f, 64, 128);
	CreateSphereVolum(SPHERE_SKY, 0.5f, 64, 128, SKY_BOX_SHADER, POS_LAYOUT);

	CreateSampler(LINER_SAMPLER);
	//디퍼드에 최적화된 샘플러 = 포인트(픽셀값을 1:1매칭시켜서 가져온다)
	CreateSampler(POINT_SAMPLER, D3D11_FILTER_MIN_MAG_MIP_POINT);
	
	return true;
}
bool ResourceManager::SamplerInit()
{
	CreateSampler(LINER_SAMPLER);
	CreateSampler(POINT_SAMPLER, D3D11_FILTER_MIN_MAG_MIP_POINT);

	return true;
}

bool ResourceManager::CreateMesh(const string & KeyName, const string & ShaderKeyName, const string & LayOutKeyName, void * vertexInfo, int vertexCount, int vertexSize, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitiveType, void * indexInfo, int indexCount, int indexSize, D3D11_USAGE indexUsage, DXGI_FORMAT indexFormat)
{
	Mesh* newMesh = FindMesh(KeyName);

	if (newMesh != NULLPTR)
	{
		SAFE_RELEASE(newMesh);
		return false;
	}
	
	newMesh = new Mesh();

	if (newMesh->CreateMesh(KeyName, ShaderKeyName, LayOutKeyName, vertexInfo, vertexCount, vertexSize, vertexUsage, primitiveType, indexInfo, indexCount, indexSize, indexUsage, indexFormat) == false)
	{
		SAFE_RELEASE(newMesh);
		return false;
	}

	m_MeshMap.insert(make_pair(KeyName, newMesh));

	return true;
}

bool ResourceManager::CreateTexture(const string & KeyName, const TCHAR * FileName, const string & PathKey)
{
	Texture* newTexture = FindTexture(KeyName);

	if (newTexture != NULLPTR)
	{
		SAFE_RELEASE(newTexture);
		return false;
	}

	newTexture = new Texture();

	if (newTexture->LoadTexture(KeyName, FileName, PathKey) == false)
	{
		SAFE_RELEASE(newTexture);
		return false;
	}

	m_TextureMap.insert(make_pair(KeyName, newTexture));
	return true;
}

bool ResourceManager::CreateTexture(const string & KeyName, const vector<const TCHAR*>& vecFileName, const string & PathKey)
{
	Texture* pTexture = FindTexture(KeyName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new Texture();

	if (!pTexture->LoadTexture(KeyName, vecFileName, PathKey))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_TextureMap.insert(make_pair(KeyName, pTexture));

	return true;
}

bool ResourceManager::CreateTextureFromFullPath(const string & KeyName, const TCHAR * FullPath)
{
	Texture* newTexture = FindTexture(KeyName);

	if (newTexture != NULLPTR)
	{
		SAFE_RELEASE(newTexture);
		return false;
	}

	newTexture = new Texture();

	if (newTexture->LoadTextureFromFullPath(KeyName, FullPath) == false)
	{
		SAFE_RELEASE(newTexture);
		return false;
	}

	m_TextureMap.insert(make_pair(KeyName, newTexture));

	return true;
}

bool ResourceManager::LoadMesh(const string & KeyName, const TCHAR * pFileName, const string & strPathKey)
{
	Mesh* newMesh = FindMesh(KeyName);

	if (newMesh)
	{
		SAFE_RELEASE(newMesh);
		return false;
	}

	newMesh = new Mesh();

	if (newMesh->LoadMesh(KeyName, pFileName, strPathKey) == false)
	{
		SAFE_RELEASE(newMesh);
		return false;
	}

	m_MeshMap.insert(make_pair(KeyName, newMesh));

	return true;
}

bool ResourceManager::LoadMeshFromFullPath(const string & KeyName, const TCHAR * pFullPath)
{
	Mesh* newMesh = FindMesh(KeyName);

	if (newMesh)
	{
		SAFE_RELEASE(newMesh);
		return false;
	}

	newMesh = new Mesh();

	if (newMesh->LoadMeshFromFullPath(KeyName, pFullPath) == false)
	{
		SAFE_RELEASE(newMesh);
		return false;
	}

	m_MeshMap.insert(make_pair(KeyName, newMesh));
	return true;
}

bool ResourceManager::CreateSampler(const string & KeyName, D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE eU, D3D11_TEXTURE_ADDRESS_MODE eV, D3D11_TEXTURE_ADDRESS_MODE eW)
{
	Sampler* newSampler = FindSampler(KeyName);

	if (newSampler != NULLPTR)
	{
		SAFE_RELEASE(newSampler);
		return false;
	}

	newSampler = new Sampler();

	if (newSampler->CreateSampler(KeyName, eFilter, eU, eV, eW) == false)
	{
		SAFE_RELEASE(newSampler);
		return false;
	}

	m_SamplerMap.insert(make_pair(KeyName, newSampler));

	return true;
}

void ResourceManager::CreateSphereVolum(const string& KeyName, float Radius, int StackSlice, int SliceCount, const string& ShaderKeyName, const string& InputLatoutKeyName)
{ 
	vector<VertexNormalColor> vecVertexData;

	//ㅡ자로 짜른다 (최대180도)
	float Phi = JEONG_PI / StackSlice;
	//안쪽으로 원통형으로 자른다 (최대360도)
	float Theta = JEONG_PI * 2.0f / SliceCount;

	for (int i = 0; i <= StackSlice; i++)
	{
		for (size_t j = 0; j < SliceCount; j++)
		{
			VertexNormalColor newVertex;

			if(SliceCount / 2 > j) //StackSlice 기준 위쪽
				newVertex.m_Pos = Vector3(Radius * sin(Phi * i) * cos(Theta * j), Radius * cos(Phi * i), Radius * sin(Phi * i) * sin(Theta * j));
			else //아래쪽
				newVertex.m_Pos = Vector3(Radius * sin(Phi * i) * -cos(Theta * j - JEONG_PI), Radius * cos(Phi * i), Radius * sin(Theta * i) * -sin(Theta * j - JEONG_PI));

			newVertex.m_Normal = newVertex.m_Pos;
			newVertex.m_Normal.Normalize();
			newVertex.m_Color = Vector4::SkyBlue;

			vecVertexData.push_back(newVertex);
		}
	}

	vector<unsigned int> vecIndex;

	for (int i = 0; i <= StackSlice; ++i)
	{
		for (int j = 0; j < SliceCount; ++j)
		{
			unsigned int index[6];

			index[0] = SliceCount * i + j;
			index[1] = SliceCount * (i + 1) + j + 1;
			index[2] = SliceCount * (i + 1) + j;
			index[3] = SliceCount * i + j;
			index[4] = SliceCount * i + j + 1;
			index[5] = SliceCount * (i + 1) + j + 1;


			//마지막에 인덱스가 맞지않기때문에 다시 처음으로 맞춰준다.
			if (j == SliceCount - 1)
			{
				index[1] = SliceCount * i + j + 1;
				index[4] = SliceCount * (i - 1) + j + 1;
				index[5] = SliceCount * i + j + 1;
			}

			vecIndex.push_back(index[0]);
			vecIndex.push_back(index[1]);
			vecIndex.push_back(index[2]);
			vecIndex.push_back(index[3]);
			vecIndex.push_back(index[4]);
			vecIndex.push_back(index[5]);
		}
	}

	CreateMesh(KeyName, ShaderKeyName,InputLatoutKeyName,&vecVertexData[0], (int)vecVertexData.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
}

void ResourceManager::CreateCapsulVolum(const string & KeyName, float Radius, float Height, int StackSlice, int SliceCount)
{
	vector<VertexNormalColor> vecVertex{};

	auto stack_count = StackSlice;

	//짝수라면 홀수로 만든다. (중앙점 찾기가 쉬움)
	if (stack_count % 2 == 0)
		stack_count -= 1;

	float phi = JEONG_PI / stack_count;
	float theta = JEONG_PI * 2.0f / SliceCount;

	for (auto i = 0; i <= stack_count; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			VertexNormalColor vertex{};

			auto y = Radius * cos(phi * i);

			if (j < SliceCount / 2)
				vertex.m_Pos = Vector3{ Radius * sin(phi * i) * cos(theta * j), y, Radius * sin(phi * i) * sin(theta * j) };
			else
				vertex.m_Pos = Vector3{ Radius * sin(phi * i) * -cos(theta * j - JEONG_PI), y, Radius * sin(phi * i) * -sin(theta * j - JEONG_PI) };

			vertex.m_Normal = vertex.m_Pos;
			vertex.m_Normal.Normalize();

			//중앙을 찾아서 위라면 높이고 아래면 내린다.
			if (i <= stack_count / 2)
				vertex.m_Pos.y += Height * 0.5f;
			else
				vertex.m_Pos.y += -Height * 0.5f;

			vertex.m_Color = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			vecVertex.push_back(vertex);
		}
	}

	vector<unsigned int> vecIndex;

	for (auto i = 0; i < stack_count; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			unsigned int index[6];

			index[0] = SliceCount * i + j;
			index[1] = SliceCount * (i + 1) + j + 1;
			index[2] = SliceCount * (i + 1) + j;
			index[3] = SliceCount * i + j;
			index[4] = SliceCount * i + j + 1;
			index[5] = SliceCount * (i + 1) + j + 1;

			//마지막에 인덱스가 맞지않기때문에 다시 처음으로 맞춰준다.
			if (j == SliceCount - 1)
			{
				index[1] = SliceCount * i + j + 1;
				index[4] = SliceCount * (i - 1) + j + 1;
				index[5] = SliceCount * i + j + 1;
			}

			vecIndex.push_back(index[0]);
			vecIndex.push_back(index[1]);
			vecIndex.push_back(index[2]);
			vecIndex.push_back(index[3]);
			vecIndex.push_back(index[4]);
			vecIndex.push_back(index[5]);
		}
	}

	CreateMesh(KeyName, STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, &vecVertex[0], (int)vecVertex.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
}

void ResourceManager::CreateCylinderVolum(const string & KeyName, float Radius, int Height, int SliceCount)
{
	vector<VertexNormalColor> vecVertex;

	//돌려야 되니 360도
	auto theta = JEONG_PI * 2.0f / SliceCount;

	//윗면 아랫면 2개
	for (auto i = 0; i < 2; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			VertexNormalColor vertex;

			//구면좌표계를 보면 x = Radius * cos(세타), z는 Radius * sin(세타)
			if (j < SliceCount / 2)
				vertex.m_Pos = Vector3{ Radius * cos(theta * j), (0.5f - i) * Height, Radius * sin(theta * j) };
			else
				vertex.m_Pos = Vector3{ Radius * -cos(theta * j - JEONG_PI), (0.5f - i) * Height, Radius * -sin(theta * j - JEONG_PI) };

			//X, Z의 Normal을 들고있는 정점
			vertex.m_Normal = vertex.m_Pos;
			vertex.m_Normal.y = 0.0f;
			vertex.m_Normal.Normalize();
			vertex.m_Color = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			vecVertex.push_back(vertex);

			//윗, 아랫면의 Y의 Normal을 들고 있는 정점
			//그지같네 시발롬아? 이놈때문에 밑에 인덱스가 꼬임.
			if (0 == i)
				vertex.m_Normal = Vector3(0.0f, 1.0f, 0.0f);
			else if (1 == i)
				vertex.m_Normal = Vector3(0.0f, -1.0f, 0.0f);

			//정점을 2개추가한다.
			vecVertex.push_back(vertex);
		}
	}

	vector<unsigned int> vecIndex{};

	// 윗면
	for (auto i = 0; i < SliceCount - 2; ++i)
	{
		unsigned int index[3];

		index[0] = 1;
		index[1] = 1 + (i + 2) * 2; //* 2 = 위 아래 두개.
		index[2] = 1 + (i + 1) * 2;

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
	}

	// 옆면
	for (auto i = 0; i < SliceCount; ++i)
	{
		unsigned int index[6];

		index[0] = i * 2;
		index[1] = (i + SliceCount + 1) * 2;
		index[2] = (i + SliceCount) * 2;
		index[3] = i * 2;
		index[4] = (i + 1) * 2;
		index[5] = (i + SliceCount + 1) * 2;

		if (i == SliceCount - 1)
		{
			index[1] = (i + 1) * 2;
			index[4] = (i - SliceCount + 1) * 2;
			index[5] = (i + 1) * 2;
		}

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
		vecIndex.push_back(index[3]);
		vecIndex.push_back(index[4]);
		vecIndex.push_back(index[5]);
	}

	// 아랫면
	for (auto i = 0; i < SliceCount - 2; ++i)
	{
		unsigned int index[3];

		index[0] = 1 + SliceCount * 2;
		index[1] = 1 + SliceCount * 2 + (i + 1) * 2;
		index[2] = 1 + SliceCount * 2 + (i + 2) * 2;

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
	}

	CreateMesh(KeyName, STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, &vecVertex[0], (int)vecVertex.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
}

void ResourceManager::CreateCornVolum(const string & KeyName, float Radius, float Height, int StackSlice, int SliceCount)
{
	vector<VertexNormalColor> vecVertex{};

	//구를 45도 잘랐다.
	float phi = (JEONG_PI / 4.0f) / StackSlice;
	float theta = JEONG_PI * 2.0f / SliceCount;

	for (auto i = 0; i <= StackSlice; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			VertexNormalColor vertex{};

			if (j < SliceCount / 2)
				vertex.m_Pos = Vector3{ Radius * sin(phi * i) * cos(theta * j), Radius * cos(phi * i), Radius * sin(phi * i) * sin(theta * j) };
			else
				vertex.m_Pos = Vector3{ Radius * sin(phi * i) * -cos(theta * j - JEONG_PI), Radius * cos(phi * i), Radius * sin(phi * i) * -sin(theta * j - JEONG_PI) };

			vertex.m_Normal = vertex.m_Pos;
			vertex.m_Normal.Normalize();
			vertex.m_Color = Vector4{ (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f };

			vertex.m_Pos.y += Height;

			vecVertex.push_back(vertex);
		}
	}

	vecVertex.push_back(VertexNormalColor{ Vector3::Zero, Vector3(0.0f, -1.0f, 0.0f), Vector4::White});

	vector<unsigned int> vecIndex;

	//원을 말아준다.
	for (auto i = 0; i < StackSlice; ++i)
	{
		for (auto j = 0; j < SliceCount; ++j)
		{
			unsigned int index[6];

			index[0] = SliceCount * i + j;
			index[1] = SliceCount * (i + 1) + j + 1;
			index[2] = SliceCount * (i + 1) + j;
			index[3] = SliceCount * i + j;
			index[4] = SliceCount * i + j + 1;
			index[5] = SliceCount * (i + 1) + j + 1;

			if (j == SliceCount - 1)
			{
				index[1] = SliceCount * i + j + 1;
				index[4] = SliceCount * (i - 1) + j + 1;
				index[5] = SliceCount * i + j + 1;
			}

			vecIndex.push_back(index[0]);
			vecIndex.push_back(index[1]);
			vecIndex.push_back(index[2]);
			vecIndex.push_back(index[3]);
			vecIndex.push_back(index[4]);
			vecIndex.push_back(index[5]);
		}
	}

	//밑에점과 삼각형으로 연결한다.
	for (auto i = 0; i < SliceCount; ++i)
	{
		unsigned int index[3]{};

		index[0] = SliceCount * StackSlice + i;
		index[1] = SliceCount * StackSlice + i + 1;
		index[2] = (unsigned int)vecVertex.size() - 1;

		if (i == SliceCount - 1)
			index[1] = SliceCount * (StackSlice - 1) + i + 1;

		vecIndex.push_back(index[0]);
		vecIndex.push_back(index[1]);
		vecIndex.push_back(index[2]);
	}

	CreateMesh(KeyName, STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, &vecVertex[0], (int)vecVertex.size(), sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecIndex[0], (int)vecIndex.size(), sizeof(unsigned int), D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);
}

Mesh* ResourceManager::FindMesh(const string & TagName)
{
	unordered_map<string, Mesh*>::iterator FindIter = m_MeshMap.find(TagName);

	if (FindIter == m_MeshMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

Mesh * ResourceManager::FindMeshNoneCount(const string & KeyName)
{
	unordered_map<string, Mesh*>::iterator FindIter = m_MeshMap.find(KeyName);

	if (FindIter == m_MeshMap.end())
		return NULLPTR;

	return FindIter->second;
}

Texture * ResourceManager::FindTexture(const string & KeyName)
{
	unordered_map<string, Texture*>::iterator FindIter = m_TextureMap.find(KeyName);

	if (FindIter == m_TextureMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

Sampler * ResourceManager::FindSampler(const string & KeyName)
{
	unordered_map<string, Sampler*>::iterator FindIter = m_SamplerMap.find(KeyName);

	if (FindIter == m_SamplerMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

Sampler * ResourceManager::FindSamplerNoneCount(const string & KeyName)
{
	unordered_map<string, Sampler*>::iterator FindIter = m_SamplerMap.find(KeyName);

	if (FindIter == m_SamplerMap.end())
		return NULLPTR;

	return FindIter->second;
}
