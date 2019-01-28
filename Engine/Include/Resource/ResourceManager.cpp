#include "stdafx.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Sampler.h"

JEONG_USING
SINGLETON_VAR_INIT(ResourceManager)

JEONG::ResourceManager::ResourceManager()
{
}

JEONG::ResourceManager::~ResourceManager()
{
	Safe_Release_Map(m_MeshMap);
	Safe_Release_Map(m_TextureMap);
	Safe_Release_Map(m_SamplerMap);
}

bool JEONG::ResourceManager::Init()
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

	CreateMesh("TextureRect", STANDARD_UV_SHADER, POS_UV_LAYOUT, UVRect, 4, sizeof(VertexUV), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexRect, 6, 2);
	CreateSampler(LINER_SAMPLER);

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

	Vector3	PlaneNormal[4];
	Vector3	Edge1, Edge2;

	Edge1 = PyramidPos[4] - PyramidPos[0];
	Edge2 = PyramidPos[1] - PyramidPos[0];
	Edge1.Nomallize();
	Edge2.Nomallize();
	PlaneNormal[0] = Edge1.Cross(Edge2);
	PlaneNormal[0].Nomallize();

	Edge1 = PyramidPos[1] - PlaneNormal[0];
	Edge2 = PyramidPos[2] - PlaneNormal[0];
	Edge1.Nomallize();
	Edge2.Nomallize();
	PlaneNormal[1] = Edge1.Cross(Edge2);
	PlaneNormal[1].Nomallize();

	Edge1 = PlaneNormal[2] - PyramidPos[0];
	Edge2 = PlaneNormal[3] - PyramidPos[0];
	Edge1.Nomallize();
	Edge2.Nomallize();
	PlaneNormal[2] = Edge1.Cross(Edge2);
	PlaneNormal[2].Nomallize();

	Edge1 = PyramidPos[3] - PyramidPos[0];
	Edge2 = PyramidPos[4] - PyramidPos[0];
	Edge1.Nomallize();
	Edge2.Nomallize();
	PlaneNormal[3] = Edge1.Cross(Edge2);
	PlaneNormal[3].Nomallize();

	Vector3	Normal[4];
	Normal[0] = (PlaneNormal[0] + PlaneNormal[1]);
	Normal[0].Nomallize();

	Normal[1] = (PlaneNormal[1] + PlaneNormal[2]);
	Normal[1].Nomallize();

	Normal[2] = (PlaneNormal[2] + PlaneNormal[3]);
	Normal[2].Nomallize();

	Normal[3] = (PlaneNormal[3] + PlaneNormal[0]);
	Normal[3].Nomallize();

	VertexNormalColor Pyramid[9] =
	{
		VertexNormalColor(PyramidPos[0], Vector3(0.0f, 1.0f, 0.0f),  Vector4::Red),
		VertexNormalColor(PyramidPos[1], Normal[0], Vector4::Green),
		VertexNormalColor(PyramidPos[2], Normal[1], Vector4::Blue),
		VertexNormalColor(PyramidPos[3], Normal[2], Vector4::Yellow),
		VertexNormalColor(PyramidPos[4], Normal[3], Vector4::Magenta),
		VertexNormalColor(PyramidPos[1], Vector3(0.0f, -1.0f, 0.0f), Vector4::Green),
		VertexNormalColor(PyramidPos[2], Vector3(0.0f, -1.0f, 0.0f), Vector4::Blue),
		VertexNormalColor(PyramidPos[3], Vector3(0.0f, -1.0f, 0.0f), Vector4::Yellow),
		VertexNormalColor(PyramidPos[4], Vector3(0.0f, -1.0f, 0.0f), Vector4::Magenta)
	};

	int	PyramidIdx[18] = { 0, 4, 1, 2, 0, 1, 3, 0, 2, 4, 0, 3, 8, 6, 5, 7, 6, 8 };
	//{0, 4, 1}
	//{2, 1, 0}
	//{3, 0, 2}
	//{4, 0, 3}
	//{8, 6, 5}
	//{7, 6, 8}
	CreateMesh("Pyramid", STANDARD_NORMAL_COLOR_SHADER, POS_NORMAL_COLOR_LAYOUT, Pyramid, 9, sizeof(VertexNormalColor), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, PyramidIdx, 18, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT);

	return true;
}

bool JEONG::ResourceManager::CreateMesh(const string & KeyName, const string & ShaderKeyName, const string & LayOutKeyName, void * vertexInfo, int vertexCount, int vertexSize, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitiveType, void * indexInfo, int indexCount, int indexSize, D3D11_USAGE indexUsage, DXGI_FORMAT indexFormat)
{
	JEONG::Mesh* newMesh = FindMesh(KeyName);

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

bool JEONG::ResourceManager::CreateTexture(const string & KeyName, const TCHAR * FileName, const string & PathKey)
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

bool JEONG::ResourceManager::CreateTextureFromFullPath(const string & KeyName, const TCHAR * FullPath)
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

bool JEONG::ResourceManager::CreateSampler(const string & KeyName, D3D11_FILTER eFilter, D3D11_TEXTURE_ADDRESS_MODE eU, D3D11_TEXTURE_ADDRESS_MODE eV, D3D11_TEXTURE_ADDRESS_MODE eW)
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

JEONG::Mesh* JEONG::ResourceManager::FindMesh(const string & TagName)
{
	unordered_map<string, JEONG::Mesh*>::iterator FindIter = m_MeshMap.find(TagName);

	if (FindIter == m_MeshMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

Texture * JEONG::ResourceManager::FindTexture(const string & KeyName)
{
	unordered_map<string, JEONG::Texture*>::iterator FindIter = m_TextureMap.find(KeyName);

	if (FindIter == m_TextureMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

Sampler * JEONG::ResourceManager::FindSampler(const string & KeyName)
{
	unordered_map<string, JEONG::Sampler*>::iterator FindIter = m_SamplerMap.find(KeyName);

	if (FindIter == m_SamplerMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}
