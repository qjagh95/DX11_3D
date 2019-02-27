#include "stdafx.h"
#include "Material_Com.h"
#include "Animation2D_Com.h"

#include "../GameObject.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/Sampler.h"
#include "../Resource/Texture.h"

#include "../Render/ShaderManager.h"

JEONG_USING

Material_Com::Material_Com()
{
	m_ComType = CT_MATERIAL;
}

Material_Com::Material_Com(const Material_Com& copyData)
	:Component_Base(copyData)
{
	m_vecMaterial.clear();

	for (size_t i = 0; i < copyData.m_vecMaterial.size(); ++i)
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
		for (size_t j = 0; j < copyData.m_vecMaterial[i].size(); ++j)
		{
			SubsetMaterial*	pMtrl = new SubsetMaterial();

			pMtrl->MatrialInfo = copyData.m_vecMaterial[i][j]->MatrialInfo;

			if (copyData.m_vecMaterial[i][j]->DiffuseTex != NULLPTR)
			{
				pMtrl->DiffuseTex = new TextureSet;
				*pMtrl->DiffuseTex = *copyData.m_vecMaterial[i][j]->DiffuseTex;
				pMtrl->DiffuseTex->m_Tex->AddRefCount();
				pMtrl->DiffuseTex->m_Sampler->AddRefCount();
			}

			if (copyData.m_vecMaterial[i][j]->NormalTex != NULLPTR)
			{
				pMtrl->NormalTex = new TextureSet;
				*pMtrl->NormalTex = *copyData.m_vecMaterial[i][j]->NormalTex;
				pMtrl->NormalTex->m_Tex->AddRefCount();
				pMtrl->NormalTex->m_Sampler->AddRefCount();
			}

			if (copyData.m_vecMaterial[i][j]->SpecularTex)
			{
				pMtrl->SpecularTex = new TextureSet;
				*pMtrl->SpecularTex = *copyData.m_vecMaterial[i][j]->SpecularTex;
				pMtrl->SpecularTex->m_Tex->AddRefCount();
				pMtrl->SpecularTex->m_Sampler->AddRefCount();
			}

			pMtrl->vecMultiTexture = copyData.m_vecMaterial[i][j]->vecMultiTexture;

			for (size_t k = 0; k < pMtrl->vecMultiTexture.size(); ++k)
			{
				if (pMtrl->vecMultiTexture[k].m_Tex)
					pMtrl->vecMultiTexture[k].m_Tex->AddRefCount();

				if (pMtrl->vecMultiTexture[k].m_Sampler)
					pMtrl->vecMultiTexture[k].m_Sampler->AddRefCount();
			}

			m_vecMaterial[i].push_back(pMtrl);
		}
	}

	ReferanceCount = 1;
}

Material_Com::~Material_Com()
{
	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterial[i].size(); ++j)
		{
			for (size_t k = 0; k < m_vecMaterial[i][j]->vecMultiTexture.size(); ++k)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->vecMultiTexture[k].m_Tex);
				SAFE_RELEASE(m_vecMaterial[i][j]->vecMultiTexture[k].m_Sampler);
			}

			if (m_vecMaterial[i][j]->DiffuseTex)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->DiffuseTex->m_Tex);
				SAFE_RELEASE(m_vecMaterial[i][j]->DiffuseTex->m_Sampler);
			}

			if (m_vecMaterial[i][j]->NormalTex)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->NormalTex->m_Tex);
				SAFE_RELEASE(m_vecMaterial[i][j]->NormalTex->m_Sampler);
			}

			if (m_vecMaterial[i][j]->SpecularTex)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->SpecularTex->m_Tex);
				SAFE_RELEASE(m_vecMaterial[i][j]->SpecularTex->m_Sampler);
			}

			SAFE_DELETE(m_vecMaterial[i][j]->DiffuseTex);
			SAFE_DELETE(m_vecMaterial[i][j]->NormalTex);
			SAFE_DELETE(m_vecMaterial[i][j]->SpecularTex);
			SAFE_DELETE(m_vecMaterial[i][j]);
		}

		m_vecMaterial[i].clear();
	}

	m_vecMaterial.clear();
}

bool Material_Com::Init()
{
	SetMaterial(Vector4::White, Vector4::White, Vector4::White, 3.2f, Vector4::White);

	return true;
}

int Material_Com::Input(float DeltaTime)
{
	return 0;
}

int Material_Com::Update(float DeltaTime)
{
	return 0;
}

int Material_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Material_Com::Collision(float DeltaTime)
{
}

void Material_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Material_Com::Render(float DeltaTime)
{
}

Material_Com * Material_Com::Clone()
{
	return new Material_Com(*this);
}

void Material_Com::SetMaterial(const Vector4& Diffuse, const Vector4& Ambient, const Vector4& Specular, float SpecularPower, const Vector4& Emissive, int Container, int Subset)
{
	//컨테이너가 없다면 하나 추가한다.
	if (Container >= m_vecMaterial.size())
	{
		vector<JEONG::SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}
	//서브셋이 없다면 추가한다.
	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	//색상정보셋팅
	m_vecMaterial[Container][Subset]->MatrialInfo.Diffuse = Diffuse;
	m_vecMaterial[Container][Subset]->MatrialInfo.Ambient = Ambient;
	m_vecMaterial[Container][Subset]->MatrialInfo.Specular = Specular;
	m_vecMaterial[Container][Subset]->MatrialInfo.Emissive = Emissive;
	m_vecMaterial[Container][Subset]->MatrialInfo.Specular.w = SpecularPower;
}

void Material_Com::SetDiffuseTexture(int RegisterNumber, const string & KeyName, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->DiffuseTex == NULLPTR)
		pMaterial->DiffuseTex = new TextureSet();

	SAFE_RELEASE(pMaterial->DiffuseTex->m_Tex);
	pMaterial->DiffuseTex->m_Tex = ResourceManager::Get()->FindTexture(KeyName);
	pMaterial->DiffuseTex->m_Register = RegisterNumber;
}

void Material_Com::SetDiffuseTexture(int RegisterNumber, const string & KeyName, const TCHAR * FileName, const string & PathKey, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial* pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->DiffuseTex == NULLPTR)
		pMaterial->DiffuseTex = new TextureSet();

	SAFE_RELEASE(pMaterial->DiffuseTex->m_Tex);
	ResourceManager::Get()->CreateTexture(KeyName, FileName, PathKey);
	pMaterial->DiffuseTex->m_Tex = ResourceManager::Get()->FindTexture(KeyName);
	pMaterial->DiffuseTex->m_Register = RegisterNumber;
}

void Material_Com::SetDiffuseTexture(int RegisterNumber, Texture * pTexture, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->DiffuseTex == NULLPTR)
		pMaterial->DiffuseTex = new TextureSet();

	SAFE_RELEASE(pMaterial->DiffuseTex->m_Tex);
	pTexture->AddRefCount();
	
	pMaterial->DiffuseTex->m_Tex = pTexture;
	pMaterial->DiffuseTex->m_Register = RegisterNumber;
}

void Material_Com::SetDiffuseTextureFromFullPath(int RegisterNumber, const string & KeyName, const TCHAR * FullPath, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->DiffuseTex == NULLPTR)
		pMaterial->DiffuseTex = new TextureSet;

	SAFE_RELEASE(pMaterial->DiffuseTex->m_Tex);
	ResourceManager::Get()->CreateTextureFromFullPath(KeyName, FullPath);
	pMaterial->DiffuseTex->m_Tex = ResourceManager::Get()->FindTexture(KeyName);
	pMaterial->DiffuseTex->m_Register = RegisterNumber;
}

void Material_Com::SetNormalTexture(int RegisterNumber, const string & KeyName, const TCHAR * FileName, const string & PathKey, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->NormalTex == NULLPTR)
		pMaterial->NormalTex = new TextureSet();

	SAFE_RELEASE(pMaterial->NormalTex->m_Tex);
	ResourceManager::Get()->CreateTexture(KeyName, FileName, PathKey);
	pMaterial->NormalTex->m_Tex = ResourceManager::Get()->FindTexture(KeyName);
	pMaterial->NormalTex->m_Register = RegisterNumber;
}

void Material_Com::SetNormalTextureFromFullPath(int RegisterNumber, const string & KeyName, const TCHAR * FullPath, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->NormalTex == NULLPTR)
		pMaterial->NormalTex = new TextureSet;

	SAFE_RELEASE(pMaterial->NormalTex->m_Tex);
	ResourceManager::Get()->CreateTextureFromFullPath(KeyName, FullPath);
	pMaterial->NormalTex->m_Tex = ResourceManager::Get()->FindTexture(KeyName);
	pMaterial->NormalTex->m_Register = RegisterNumber;
}

void Material_Com::SetNormalSampler(int RegisterNumber, const string & KeyName, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->NormalTex == NULLPTR)
		pMaterial->NormalTex = new TextureSet;

	SAFE_RELEASE(pMaterial->NormalTex->m_Sampler);
	pMaterial->NormalTex->m_Sampler = ResourceManager::Get()->FindSampler(KeyName);
	pMaterial->NormalTex->m_SamplerRegister = RegisterNumber;
}

void Material_Com::SetSpecularTexture(int RegisterNumber, const string & KeyName, const TCHAR * FileName, const string & PathKey, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->SpecularTex == NULLPTR)
		pMaterial->SpecularTex = new TextureSet();

	SAFE_RELEASE(pMaterial->SpecularTex->m_Tex);
	ResourceManager::Get()->CreateTexture(KeyName, FileName, PathKey);
	pMaterial->SpecularTex->m_Tex = ResourceManager::Get()->FindTexture(KeyName);
	pMaterial->SpecularTex->m_Register = RegisterNumber;
}

void Material_Com::SetSpecularTextureFromFullPath(int RegisterNumber, const string & KeyName, const TCHAR * FullPath, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->SpecularTex == NULLPTR)
		pMaterial->SpecularTex = new TextureSet;

	SAFE_RELEASE(pMaterial->SpecularTex->m_Tex);
	ResourceManager::Get()->CreateTextureFromFullPath(KeyName, FullPath);
	pMaterial->SpecularTex->m_Tex = ResourceManager::Get()->FindTexture(KeyName);
	pMaterial->SpecularTex->m_Register = RegisterNumber;
}

void Material_Com::SetSpecularSampler(int RegisterNumber, const string & KeyName, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->SpecularTex == NULLPTR)
		pMaterial->SpecularTex = new TextureSet();

	SAFE_RELEASE(pMaterial->SpecularTex->m_Sampler);
	pMaterial->SpecularTex->m_Sampler = ResourceManager::Get()->FindSampler(KeyName);
	pMaterial->SpecularTex->m_SamplerRegister = RegisterNumber;
}

void Material_Com::SetDiffuseSampler(int RegisterNumber, const string & KeyName, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[Container][Subset];

	if (pMaterial->DiffuseTex == NULLPTR)
		pMaterial->DiffuseTex = new TextureSet();

	SAFE_RELEASE(pMaterial->DiffuseTex->m_Sampler);
	pMaterial->DiffuseTex->m_Sampler = ResourceManager::Get()->FindSampler(KeyName);
	pMaterial->DiffuseTex->m_SamplerRegister = RegisterNumber;
}

void Material_Com::ClearContainer()
{
	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterial[i].size(); ++j)
		{
			for (size_t k = 0; k < m_vecMaterial[i][j]->vecMultiTexture.size(); ++k)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->vecMultiTexture[k].m_Tex);
				SAFE_RELEASE(m_vecMaterial[i][j]->vecMultiTexture[k].m_Sampler);
			}

			if (m_vecMaterial[i][j]->DiffuseTex != NULLPTR)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->DiffuseTex->m_Tex);
				SAFE_RELEASE(m_vecMaterial[i][j]->DiffuseTex->m_Sampler);
			}

			if (m_vecMaterial[i][j]->NormalTex != NULLPTR)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->NormalTex->m_Tex);
				SAFE_RELEASE(m_vecMaterial[i][j]->NormalTex->m_Sampler);
			}

			if (m_vecMaterial[i][j]->SpecularTex != NULLPTR)
			{
				SAFE_RELEASE(m_vecMaterial[i][j]->SpecularTex->m_Tex);
				SAFE_RELEASE(m_vecMaterial[i][j]->SpecularTex->m_Sampler);
			}

			SAFE_DELETE(m_vecMaterial[i][j]->DiffuseTex);
			SAFE_DELETE(m_vecMaterial[i][j]->NormalTex);
			SAFE_DELETE(m_vecMaterial[i][j]->SpecularTex);
			SAFE_DELETE(m_vecMaterial[i][j]);
		}

		m_vecMaterial[i].clear();
	}

	m_vecMaterial.clear();
}

SubsetMaterial* Material_Com::CreateSubSet()
{
	SubsetMaterial* newSubset = new SubsetMaterial();

	return newSubset;
}

//쉐이더에 셋팅한다.
void Material_Com::SetShader(int Container, int Subset)
{
	SubsetMaterial*	getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->DiffuseTex != NULLPTR)
	{
		getMaterial->DiffuseTex->m_Tex->SetShaderResource(getMaterial->DiffuseTex->m_Register);
		getMaterial->DiffuseTex->m_Sampler->SetSamplerState(getMaterial->DiffuseTex->m_SamplerRegister);
	}

	if (getMaterial->NormalTex != NULLPTR)
	{
		getMaterial->NormalTex->m_Tex->SetShaderResource(getMaterial->NormalTex->m_Register);
		getMaterial->NormalTex->m_Sampler->SetSamplerState(getMaterial->NormalTex->m_SamplerRegister);
		getMaterial->MatrialInfo.Diffuse.w = 1.0f;
	}
	else
		getMaterial->MatrialInfo.Diffuse.w = 0.0f;

	if (getMaterial->SpecularTex != NULLPTR)
	{
		getMaterial->SpecularTex->m_Tex->SetShaderResource(getMaterial->SpecularTex->m_Register);
		getMaterial->SpecularTex->m_Sampler->SetSamplerState(getMaterial->SpecularTex->m_SamplerRegister);
		getMaterial->MatrialInfo.Ambient.w = 1.0f;
	}
	else
		getMaterial->MatrialInfo.Ambient.w = 0.0f;

	for (size_t i = 0; i < getMaterial->vecMultiTexture.size(); ++i)
	{
		getMaterial->vecMultiTexture[i].m_Tex->SetShaderResource(getMaterial->vecMultiTexture[i].m_Register);
		getMaterial->vecMultiTexture[i].m_Sampler->SetSamplerState(getMaterial->vecMultiTexture[i].m_SamplerRegister);
	}

	ShaderManager::Get()->UpdateCBuffer("Material", &getMaterial->MatrialInfo);
}
