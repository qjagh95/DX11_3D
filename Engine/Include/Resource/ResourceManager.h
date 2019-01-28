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
	bool CreateSampler(const string& KeyName, D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MODE eU = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE eV = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE eW = D3D11_TEXTURE_ADDRESS_WRAP);

private:
	unordered_map<string, Mesh*> m_MeshMap;
	unordered_map<string, Texture*> m_TextureMap;
	unordered_map<string, Sampler*> m_SamplerMap;

public:
	Mesh* FindMesh(const string& KeyName);
	Texture* FindTexture(const string& KeyName);
	Sampler* FindSampler(const string& KeyName);

public:
	CLASS_IN_SINGLE(ResourceManager)
};

JEONG_END

