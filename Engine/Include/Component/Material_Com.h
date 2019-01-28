#pragma once
#include "Component_Base.h"
#include "../Resource/ResourceManager.h"

JEONG_BEGIN

class Texture;
class Sampler;
struct JEONG_DLL SubsetMaterial
{
	MaterialCbuffer MatrialInfo;

	vector<JEONG::Texture*> vecDiffuseTexture;
	vector<JEONG::Sampler*> vecDiffuseSampler;

	SubsetMaterial();
	~SubsetMaterial();
};

class Texture;
class JEONG_DLL Material_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Material_Com* Clone() override;

	void SetMaterial(const Vector4& Diffuse, int Container = 0, int Subset = 0);
	void SetDiffuseTexture(int RegisterNumber, const string& KeyName, int Container = 0, int Subset = 0);
	void SetDiffuseTexture(int RegisterNumber, const string& KeyName, const TCHAR* FileName,	const string& PathKey = TEXTURE_PATH, int Container = 0, int Subset = 0);
	void SetDiffuseTexture(int RegisterNumber, Texture* pTexture, int Container = 0, int Subset = 0);
	void SetDiffuseTextureFromFullPath(int RegisterNumber, const string& KeyName, const TCHAR* FullPath, int Container = 0, int Subset = 0);
	void SetSampler(int RegisterNumber, const string& KeyName, int Container = 0, int Subset = 0);
	void SetShader(int Container = 0, int Subset = 0);

private:
	//메쉬도 컨테이너와 서브셋개념으로 나눠져있기때문에 재질정보도 각 부위마다 달라질 수 있다.
	vector<vector<SubsetMaterial*>> m_vecMaterial; 

private:
	Material_Com();
	Material_Com(const Material_Com& copyData);
	~Material_Com();

	SubsetMaterial* CreateSubSet();

public:
	friend class GameObject;
};

JEONG_END

