#pragma once
#include "Component_Base.h"
#include "../Resource/ResourceManager.h"

JEONG_BEGIN

class Texture;
class Sampler;
struct JEONG_DLL SubsetMaterial
{
	Material MatrialInfo;

	Texture* DiffuseTexture;
	int TextureRegister;

	Sampler* DiffuseSampler;
	int SamplerRegister;

	SubsetMaterial() : DiffuseTexture(NULLPTR), TextureRegister(-1), DiffuseSampler(NULLPTR), SamplerRegister(-1)
	{
		DiffuseSampler = ResourceManager::Get()->FindSampler(LINER_SAMPLER);
	}

};

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
	void SetDiffuseTextureFromFullPath(int RegisterNumber, const string& KeyName, const TCHAR* FullPath, int Container = 0, int Subset = 0);
	void SetSampler(int RegisterNumber, const string& KeyName, int Container = 0, int Subset = 0);
	void SetMaterial(int Container = 0, int Subset = 0);

private:
	vector<vector<SubsetMaterial*>> m_vecMaterial;

private:
	Material_Com();
	Material_Com(const Material_Com& copyData);
	~Material_Com();

public:
	friend class GameObject;
};

JEONG_END

