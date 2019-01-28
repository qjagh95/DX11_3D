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

	size_t GetWidth() const { return m_vecImage[0]->GetImage(0, 0, 0)->width; }
	size_t GetHeight()	const { return m_vecImage[0]->GetImage(0, 0, 0)->height; }
	const uint8_t* GetPixels() const { return m_vecImage[0]->GetPixels(); }
	size_t GetPixelSize() const { return m_vecImage[0]->GetPixelsSize(); }

private:
	bool CreateShaderResource();
	bool CreateShaderResourceArray();

private:
	///DirectXTex에서 제공되는 이미지함수
	///이미지가 여러장일 수 있기 대문에 vector
	vector<ScratchImage*> m_vecImage;				
	///이미지는 그냥 이미지의 픽셀정보를 가지고 있을 뿐이다.
	///그 픽셀정보를 가져다가 쉐이더로 넘겨서 거기서 정보를 뜯어서 색상정보를 써야한다.
	///그렇기때문에 아랫놈이 무조건 필요하다.
	ID3D11ShaderResourceView* m_ShaderResourceView;
	TCHAR m_FullPath[MAX_PATH];

private:	
	Texture();
	~Texture();

public:
	friend class ResourceManager;
};

JEONG_END

