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

	CreateMesh("TextureRect", STANDARD_UV_SHADER, POS_UV_LAYOUT, UVRect, 4, sizeof(VertexUV), D3D11_USAGE_DEFAULT,	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IndexRect, 6, 2);
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

	Vector3	IsoTileNomal[5] = {};
	IsoTileNomal[0] = Vector3(0.5f, 1.0f, 0.0f);
	IsoTileNomal[1] = Vector3(1.0f, 0.5f, 0.0f);
	IsoTileNomal[2] = Vector3(0.5f, 0.0f, 0.0f);
	IsoTileNomal[3] = Vector3(0.0f, 0.5f, 0.0f);
	IsoTileNomal[4] = Vector3(0.5f, 1.0f, 0.0f);

	CreateMesh("IsoTileNomal", TILE_SHADER, POS_LAYOUT, IsoTileNomal, 5, sizeof(Vector3), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Vector3	IsoTileNoMove[8] = {};
	IsoTileNoMove[0] = Vector3(0.5f, 1.0f, 0.0f);
	IsoTileNoMove[1] = Vector3(1.0f, 0.5f, 0.0f);
	IsoTileNoMove[2] = Vector3(0.5f, 0.0f, 0.0f);
	IsoTileNoMove[3] = Vector3(0.0f, 0.5f, 0.0f);
	IsoTileNoMove[4] = Vector3(0.5f, 1.0f, 0.0f);
	IsoTileNoMove[5] = Vector3(0.5f, 0.0f, 0.0f);
	IsoTileNoMove[6] = Vector3(1.0f, 0.5f, 0.0f);
	IsoTileNoMove[7] = Vector3(0.0f, 0.5f, 0.0f);

	CreateMesh("IsoTileNomove", TILE_SHADER, POS_LAYOUT, IsoTileNoMove, 8, sizeof(Vector3), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Vector3	TileNoMove[9] =
	{
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(1.0f, 1.0f, 0.0f),
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(1.0f, 1.0f, 0.0f),
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
	};

	CreateMesh("TileNoMove", COLLIDER_SHADER, POS_LAYOUT, TileNoMove, 9, sizeof(Vector3), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);


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
