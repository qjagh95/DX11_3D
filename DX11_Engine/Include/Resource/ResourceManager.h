#pragma once

JEONG_BEGIN

class Mesh;
class Texture;
class Sampler;
class JEONG_DLL ResourceManager
{
public:
	bool Init();
	bool CreateMesh(const string& KeyName, const string& ShaderKeyName, const string& LayOutKeyName, void* vertexInfo, int vertexCount, int vertexSize, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitiveType, void* indexInfo = NULLPTR, int indexCount = 0, int indexSize = 0, D3D11_USAGE indexUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT);
	bool CreateTexture(const string& KeyName, const TCHAR* FileName, const string& PathKey = TEXTURE_PATH);
	bool CreateTextureFromFullPath(const string& KeyName, const TCHAR* FullPath);
	bool LoadMesh(const string& KeyName, const TCHAR* pFileName,const string& strPathKey = FBX_PATH);
	bool LoadMeshFromFullPath(const string& KeyName, const TCHAR* pFullPath);
	bool CreateSampler(const string& KeyName, D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MODE eU = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE eV = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE eW = D3D11_TEXTURE_ADDRESS_WRAP);

	void CreateSphereVolum(const string& KeyName, float Radius, int StackSlice, int SliceCount);
	void CreateCapsulVolum(const string& KeyName, float Radius, float Height, int StackSlice, int SliceCount);
	void CreateCornVolum(const string& KeyName, float Radius, float Height, int StackSlice, int SliceCount);
	void CreateCylinderVolum(const string& KeyName, float Radius, int Height, int SliceCount);

	Mesh* FindMesh(const string& KeyName);
	Mesh* FindMeshNoneCount(const string& KeyName);
	Texture* FindTexture(const string& KeyName);
	Sampler* FindSampler(const string& KeyName);
	Sampler* FindSamplerNoneCount(const string& KeyName);

private:
	unordered_map<string, Mesh*> m_MeshMap;
	unordered_map<string, Texture*> m_TextureMap;
	unordered_map<string, Sampler*> m_SamplerMap;

public:
	CLASS_IN_SINGLE(ResourceManager)
};

JEONG_END

