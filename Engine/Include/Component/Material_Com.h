#pragma once
#include "Component_Base.h"

JEONG_BEGIN

struct TextureSet
{
	int m_RegisterNumber;
	class Texture* m_Texture;
	class Sampler* m_Sampler;
};

class Texture;
class Sampler;
struct JEONG_DLL SubsetMaterial
{
	MaterialCbuffer MatrialInfo;

	vector<Texture*> vecDiffuseTexture;
	vector<Sampler*> vecDiffuseSampler;

	vector<Texture*> vecNormalTexture;
	vector<Sampler*> vecNormalSampler;

	vector<Texture*> vecSpecularTexture;
	vector<Sampler*> vecSpecularSampler;

	SubsetMaterial() { MatrialInfo.Specular.w = 3.2f; }
	~SubsetMaterial() {}
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

	void SetMaterial(const Vector4& Diffuse, const Vector4& Ambient, const Vector4& Specular, float SpecularPower, const Vector4& Emissive, int Container = 0, int Subset = 0);
	void SetDiffuseTexture(int RegisterNumber, const string& KeyName, int Container = 0, int Subset = 0);
	void SetDiffuseTexture(int RegisterNumber, const string& KeyName, const TCHAR* FileName, const string& PathKey = TEXTURE_PATH, int Container = 0, int Subset = 0);
	void SetDiffuseTexture(int RegisterNumber, Texture* pTexture, int Container = 0, int Subset = 0);
	void SetDiffuseTextureFromFullPath(int RegisterNumber, const string& KeyName, const TCHAR* FullPath, int Container = 0, int Subset = 0);
	void SetNormalTexture(int RegisterNumber, const string& KeyName, const TCHAR* FileName, const string& PathKey = TEXTURE_PATH,int Container = 0, int Subset = 0);
	void SetNormalTextureFromFullPath(int RegisterNumber, const string& strKey,	const TCHAR* FullPath, int Container = 0, int Subset = 0);
	void SetNormalSampler(int RegisterNumber, const string& KeyName,int Container = 0, int Subset = 0);

	void SetSpecularTexture(int iRegister, const string& strKey, const TCHAR* pFileName,const string& strPathKey = TEXTURE_PATH, int Container = 0, int Subset = 0);
	void SetSpecularTextureFromFullPath(int iRegister, const string& strKey,const TCHAR* pFullPath, int Container = 0, int Subset = 0);
	void SetSpecularSampler(int iRegister, const string& strKey,int iContainer = 0, int iSubset = 0);

	void SetDiffuseSampler(int RegisterNumber, const string& KeyName, int Container = 0, int Subset = 0);
	void SetShader(int Container = 0, int Subset = 0);
	MaterialCbuffer GetMaterialInfo() const { return m_vecMaterial[0][0]->MatrialInfo; }
	void ClearContainer();

private:
	//�޽��� �����̳ʿ� ����°������� �������ֱ⶧���� ���������� �� �������� �޶��� �� �ִ�.
	vector<vector<SubsetMaterial*>> m_vecMaterial; 

private:
	Material_Com();
	Material_Com(const Material_Com& copyData);
	~Material_Com();

	SubsetMaterial* CreateSubSet();

public:
	friend class GameObject;
	friend class GUIManager;
	friend class Mesh;
};

JEONG_END

