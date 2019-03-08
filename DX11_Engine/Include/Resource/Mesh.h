#pragma once

#include "../RefCount.h"

JEONG_BEGIN

struct JEONG_DLL VertexBuffer
{
	ID3D11Buffer* vBuffer;  ///버퍼객체
	void* vInfo;			///버텍스 정보를 담을 void* 
	int vCount;				///버텍스 갯수
	int vSize;				///버텍스 사이즈
	D3D11_USAGE vUsage;		///어떻게 출력?? (동적출력, 수정불가출력...)

	VertexBuffer() : vBuffer(NULLPTR), vInfo(NULLPTR), vCount(0), vSize(0), vUsage(D3D11_USAGE_DEFAULT) {}
};

struct JEONG_DLL IndexBuffer
{
	ID3D11Buffer* iBuffer;		///버퍼객체
	void* iInfo;				///인덱스 정보를 담을 void* 
	int iCount;					///인덱스 갯수
	int iSize;					///인덱스 사이즈
	D3D11_USAGE iUsage;			///어떻게 출력?? (동적출력, 수정불가출력...)
	DXGI_FORMAT iFormat;		///포맷 (R16 = 16비트) 하나의 메쉬당 인덱스 버퍼의 갯수를 65535개 사용하겠다.

	//DXGI_FORMAT_R16_UINT (16비트) 2바이트 갯수만큼 정점갯수 지정. (65535개)
	IndexBuffer() : iBuffer(NULLPTR), iInfo(NULLPTR), iCount(0), iSize(0), iUsage(D3D11_USAGE_DEFAULT), iFormat(DXGI_FORMAT_R16_UINT) {}
};

//사람으로 치자면 팔, 몸통, 다리, 머리 -> 컨테이너
//(팔)이라는 컨테이너 안에 상단 중단 하단의 서브셋
//-> 큰 버텍스 버퍼안에 인덱스버퍼가 여러개있는꼴.
struct JEONG_DLL MeshContainer
{
	VertexBuffer vertexBuffer;
	vector<IndexBuffer> vecIndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveType;
};

class Material_Com;
class FBXLoader;
class Animation3D_Com;
class Mesh : public RefCount
{
public:
	bool CreateMesh(const string& KeyName, const string& ShaderKeyName, const string& LayOutKeyName, void* vertexInfo, int vertexCount, int vertexSize, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitiveType, void* indexInfo = NULLPTR, int indexCount = 0,	int indexSize = 0, D3D11_USAGE indexUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT);
	void Render();
	void Render(int Container, int Subset);

	bool LoadMesh(const string& KeyName, const TCHAR* FileName, const string& PathKey = MESH_DATA_PATH);
	bool LoadMeshFromFullPath(const string& KeyName, const TCHAR* FullPath);
	string GetShaderKey() const { return m_ShaderKeyName; }
	string GetLayOutKey() const { return m_LayOutKeyName; }
	void* GetVertexInfo(int Index) const { return m_vecMeshContainer[Index]->vertexBuffer.vInfo; }
	Material_Com* CloneMaterial();
	Animation3D_Com* CloneAnimation();

	void UpdateVertexBuffer(void* vertexInfo, int ContainerIndex = 0);

	size_t GetContainerCount() const { return m_vecMeshContainer.size(); }
	size_t GetSubsetCount(int Container) const { return m_vecMeshContainer[Container]->vecIndexBuffer.size(); }

	Vector3 GetMin() const { return m_Min; }
	Vector3 GetMax() const { return m_Max; }
	Vector3 GetCenter() const { return m_Center; }
	Vector3 GetLenth() const { return m_Lenth; }
	float GetRadius() const { return m_Radius; }

	bool Save(const string& FileName, const string& PathKey = MESH_PATH);
	bool SaveFullPath(const char* pFullPath);
	bool Load(const string& FileName, const string& PathKey = MESH_DATA_PATH);
	bool LoadFullPath(const char* pFullPath);

private:
	bool ConvertFbx(FBXLoader* pLoader, const char* pFullPath);

private:
	vector<MeshContainer*> m_vecMeshContainer;
	string m_ShaderKeyName;
	string m_LayOutKeyName;
	Vector3 m_Center;
	Vector3 m_Min;
	Vector3 m_Max;
	Vector3 m_Lenth;
	Material_Com* m_Material;
	Animation3D_Com* m_Animation;
	float m_Radius;

private:
	Mesh();
	~Mesh();

	bool CreateVertexBuffer(void* vertexInfo , int vertexCount , int vertexSize, D3D11_USAGE vertexUsage);
	bool CreateIndexBuffer(void* indexInfo, int indexCount, int indexSize, D3D11_USAGE indexUsage, DXGI_FORMAT indexFormat);

public:
	friend class ResourceManager;
};

JEONG_END

