#pragma once
#include "../RefCount.h"
#include "DirectXTex.h"

JEONG_BEGIN

class JEONG_DLL Texture : public RefCount
{
public:
	bool LoadTexture(const string& TextureName, const TCHAR* FileName, const string& PathKey = TEXTURE_PATH);
	bool LoadTexture(const string& TextureName, const vector<const TCHAR*>& FileNames, const string& PathKey = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const string& TextureName, const TCHAR* FullPath);
	bool LoadTextureFromFullPath(const string& TextureName, const vector<const TCHAR*>& FullPaths);
	void SetShaderResource(int RegisterNumber);
	size_t GetTextureCount() const { return m_vecImage.size(); }

	size_t GetWidth() const;
	size_t GetHeight()	const;
	const uint8_t* GetPixels() const;
	size_t GetPixelSize() const;
	const vector<TCHAR*>* GetFullPath() const { return &m_vecFullPath; }

private:
	bool CreateShaderResource();
	bool CreateShaderResourceArray();

private:
	///DirectXTex���� �����Ǵ� �̹����Լ�
	///�̹����� �������� �� �ֱ� �빮�� vector
	vector<ScratchImage*> m_vecImage;				
	///�̹����� �׳� �̹����� �ȼ������� ������ ���� ���̴�.
	///�� �ȼ������� �����ٰ� ���̴��� �Ѱܼ� �ű⼭ ������ �� ���������� ����Ѵ�.
	///�׷��⶧���� �Ʒ����� ������ �ʿ��ϴ�.
	ID3D11ShaderResourceView* m_ShaderResourceView;
	TCHAR m_FullPath[MAX_PATH];
	vector<TCHAR*> m_vecFullPath;

private:	
	Texture();
	~Texture();

public:
	friend class ResourceManager;
	friend class Material_Com;
};

JEONG_END

