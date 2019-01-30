#include "stdafx.h"
#include "Material_Com.h"
#include "Animation2D_Com.h"

#include "../GameObject.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/Sampler.h"
#include "../Resource/Texture.h"

#include "../Render/ShaderManager.h"

JEONG_USING

SubsetMaterial::~SubsetMaterial()
{
}

Material_Com::Material_Com()
{
	m_ComType = CT_MATERIAL;
}

Material_Com::Material_Com(const Material_Com& copyData)
	:Component_Base(copyData)
{
	m_vecMaterial.clear();

	for (size_t i = 0; i < copyData.m_vecMaterial.size(); i++)
	{
		vector<SubsetMaterial*> TempVec;
		m_vecMaterial.push_back(TempVec);

		for (size_t j = 0; j < copyData.m_vecMaterial[i].size(); j++)
		{
			SubsetMaterial* newMaterial = new SubsetMaterial();
			newMaterial->MatrialInfo = copyData.m_vecMaterial[i][j]->MatrialInfo;

			//TODO : 터짐
			for (size_t a = 0; a < copyData.m_vecMaterial[i][j]->vecDiffuseSampler.size(); a++)
			{
				newMaterial->vecDiffuseSampler[a] = copyData.m_vecMaterial[i][j]->vecDiffuseSampler[a];

				if (newMaterial->vecDiffuseSampler[a] != NULLPTR)
					newMaterial->vecDiffuseSampler[a]->AddRefCount();
			}
			for (size_t b = 0; b < copyData.m_vecMaterial[i][j]->vecDiffuseTexture.size(); b++)
			{
				newMaterial->vecDiffuseTexture[b] = copyData.m_vecMaterial[i][j]->vecDiffuseTexture[b];

				if (newMaterial->vecDiffuseTexture[b] != NULLPTR)
					newMaterial->vecDiffuseTexture[b]->AddRefCount();
			}

			for (size_t c = 0; c < copyData.m_vecMaterial[i][j]->vecNormalSampler.size(); c++)
			{
				newMaterial->vecNormalSampler[c] = copyData.m_vecMaterial[i][j]->vecNormalSampler[c];

				if (newMaterial->vecNormalSampler[c] != NULLPTR)
					newMaterial->vecNormalSampler[c]->AddRefCount();
			}
			for (size_t d = 0; d < copyData.m_vecMaterial[i][j]->vecNormalTexture.size(); d++)
			{
				newMaterial->vecNormalTexture[d] = copyData.m_vecMaterial[i][j]->vecNormalTexture[d];

				if (newMaterial->vecNormalTexture[d] != NULLPTR)
					newMaterial->vecNormalTexture[d]->AddRefCount();
			}

			for (size_t e = 0; e < copyData.m_vecMaterial[i][j]->vecSpecularSampler.size(); e++)
			{
				newMaterial->vecSpecularSampler[e] = copyData.m_vecMaterial[i][j]->vecSpecularSampler[e];

				if (newMaterial->vecSpecularSampler[e] != NULLPTR)
					newMaterial->vecSpecularSampler[e]->AddRefCount();
			}
			for (size_t f = 0; f < copyData.m_vecMaterial[i][j]->vecSpecularTexture.size(); f++)
			{
				newMaterial->vecSpecularTexture[f] = copyData.m_vecMaterial[i][j]->vecSpecularTexture[f];

				if (newMaterial->vecSpecularTexture[f] != NULLPTR)
					newMaterial->vecSpecularTexture[f]->AddRefCount();
			}

			m_vecMaterial[i].push_back(newMaterial);
		}
	}
}

Material_Com::~Material_Com()
{
	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterial[i].size(); ++j)
		{
			Safe_Release_VecList(m_vecMaterial[i][j]->vecDiffuseSampler);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecDiffuseTexture);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecNormalSampler);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecNormalTexture);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecSpecularSampler);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecSpecularTexture);

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
	//컨테이너가 없다면 하나 추가한다.
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}
	//서브셋이 없다면 추가한다.
	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	//텍스쳐 셋팅준비
	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];
	Safe_Release_VecList(getMaterial->vecDiffuseTexture);

	if (getMaterial->vecDiffuseTexture.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseTexture.resize(RegisterNumber + 1);
		
	getMaterial->vecDiffuseTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

void Material_Com::SetDiffuseTexture(int RegisterNumber, const string & KeyName, const TCHAR * FileName, const string & PathKey, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecDiffuseTexture.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseTexture.resize(RegisterNumber + 1);

	SAFE_RELEASE(getMaterial->vecDiffuseTexture[0]);

	if (RegisterNumber > 0)
		SAFE_RELEASE(getMaterial->vecDiffuseTexture[RegisterNumber]);

	//텍스쳐가 없다면 맵에 추가한 후
	//가져다 쓴다.
	ResourceManager::Get()->CreateTexture(KeyName, FileName, PathKey);
	getMaterial->vecDiffuseTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

void Material_Com::SetDiffuseTexture(int RegisterNumber, Texture * pTexture, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecDiffuseTexture.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseTexture.resize(RegisterNumber + 1);

	getMaterial->vecDiffuseTexture[RegisterNumber] = pTexture;
}

void Material_Com::SetDiffuseTextureFromFullPath(int RegisterNumber, const string & KeyName, const TCHAR * FullPath, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
	{
		SubsetMaterial* newMaterial = new SubsetMaterial();
		m_vecMaterial[Container].push_back(newMaterial);
	}

	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];
	ResourceManager::Get()->CreateTextureFromFullPath(KeyName, FullPath);

	if (getMaterial->vecDiffuseTexture.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseTexture.resize(RegisterNumber + 1);

	getMaterial->vecDiffuseTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

void Material_Com::SetNormalTexture(int RegisterNumber, const string & KeyName, const TCHAR * FileName, const string & PathKey, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecNormalTexture.capacity() <= RegisterNumber)
		getMaterial->vecNormalTexture.resize(RegisterNumber + 1);

	SAFE_RELEASE(getMaterial->vecNormalTexture[0]);

	if (RegisterNumber > 0)
		SAFE_RELEASE(getMaterial->vecNormalTexture[RegisterNumber]);

	//텍스쳐가 없다면 맵에 추가한 후
	//가져다 쓴다.
	ResourceManager::Get()->CreateTexture(KeyName, FileName, PathKey);
	getMaterial->vecNormalTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

void Material_Com::SetNormalTextureFromFullPath(int RegisterNumber, const string & KeyName, const TCHAR * FullPath, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
	{
		SubsetMaterial* newMaterial = new SubsetMaterial();
		m_vecMaterial[Container].push_back(newMaterial);
	}

	SubsetMaterial *getMaterial = m_vecMaterial[Container][Subset];
	ResourceManager::Get()->CreateTextureFromFullPath(KeyName, FullPath);

	if (getMaterial->vecNormalTexture.capacity() <= RegisterNumber)
		getMaterial->vecNormalTexture.resize(RegisterNumber + 1);

	getMaterial->vecNormalTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

void Material_Com::SetNormalSampler(int RegisterNumber, const string & KeyName, int Container, int Subset)
{
	//Sampler를 셋팅한다.
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
	{
		SubsetMaterial* newMaterial = new SubsetMaterial();
		m_vecMaterial[Container].push_back(newMaterial);
	}

	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecNormalSampler.capacity() <= RegisterNumber)
		getMaterial->vecNormalSampler.resize(RegisterNumber + 1);

	//기본정보가 지정된 샘플러를 찾아온다.
	getMaterial->vecNormalSampler[RegisterNumber] = ResourceManager::Get()->FindSampler(KeyName);
}

void Material_Com::SetSpecularTexture(int RegisterNumber, const string & KeyName, const TCHAR * FileName, const string & PathKey, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecSpecularTexture.capacity() <= RegisterNumber)
		getMaterial->vecSpecularTexture.resize(RegisterNumber + 1);

	SAFE_RELEASE(getMaterial->vecSpecularTexture[0]);

	if (RegisterNumber > 0)
		SAFE_RELEASE(getMaterial->vecSpecularTexture[RegisterNumber]);

	//텍스쳐가 없다면 맵에 추가한 후
	//가져다 쓴다.
	ResourceManager::Get()->CreateTexture(KeyName, FileName, PathKey);
	getMaterial->vecSpecularTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

void Material_Com::SetSpecularTextureFromFullPath(int RegisterNumber, const string & KeyName, const TCHAR * FullPath, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
	{
		SubsetMaterial* newMaterial = new SubsetMaterial();
		m_vecMaterial[Container].push_back(newMaterial);
	}

	SubsetMaterial *getMaterial = m_vecMaterial[Container][Subset];
	ResourceManager::Get()->CreateTextureFromFullPath(KeyName, FullPath);

	if (getMaterial->vecSpecularTexture.capacity() <= RegisterNumber)
		getMaterial->vecSpecularTexture.resize(RegisterNumber + 1);

	getMaterial->vecSpecularTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

void Material_Com::SetSpecularSampler(int RegisterNumber, const string & KeyName, int Container, int Subset)
{
	//Sampler를 셋팅한다.
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
	{
		SubsetMaterial* newMaterial = new SubsetMaterial();
		m_vecMaterial[Container].push_back(newMaterial);
	}

	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecSpecularSampler.capacity() <= RegisterNumber)
		getMaterial->vecSpecularSampler.resize(RegisterNumber + 1);

	//기본정보가 지정된 샘플러를 찾아온다.
	getMaterial->vecSpecularSampler[RegisterNumber] = ResourceManager::Get()->FindSampler(KeyName);
}

//이 함수는 아직 안들어옴.
void Material_Com::SetDiffuseSampler(int RegisterNumber, const string & KeyName, int Container, int Subset)
{
	//Sampler를 셋팅한다.
	if (Container >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
	{
		SubsetMaterial* newMaterial = new SubsetMaterial();
		m_vecMaterial[Container].push_back(newMaterial);
	}

	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecDiffuseSampler.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseSampler.resize(RegisterNumber + 1);

	//기본정보가 지정된 샘플러를 찾아온다.
	getMaterial->vecDiffuseSampler[RegisterNumber] = ResourceManager::Get()->FindSampler(KeyName);
}

void Material_Com::ClearContainer()
{
	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterial[i].size(); ++j)
		{
			Safe_Release_VecList(m_vecMaterial[i][j]->vecDiffuseSampler);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecDiffuseTexture);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecNormalSampler);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecNormalTexture);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecSpecularSampler);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecSpecularTexture);

			SAFE_DELETE(m_vecMaterial[i][j]);
		}
		m_vecMaterial[i].clear();
	}
	m_vecMaterial.clear();
}

SubsetMaterial* Material_Com::CreateSubSet()
{
	SubsetMaterial* newSubset = new SubsetMaterial();
	newSubset->vecDiffuseSampler.push_back(ResourceManager::Get()->FindSampler(LINER_SAMPLER));

	return newSubset;
}

//쉐이더에 셋팅한다.
void Material_Com::SetShader(int Container, int Subset)
{
	SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	ShaderManager::Get()->UpdateCBuffer("Material", &getMaterial->MatrialInfo);

	for (size_t i = 0; i < getMaterial->vecDiffuseTexture.size(); i++)
	{
		if (getMaterial->vecDiffuseTexture[i] != NULLPTR)
			getMaterial->vecDiffuseTexture[i]->SetShaderResource((int)i); //PSSetResourceView
	}

	for (size_t i = 0; i < getMaterial->vecDiffuseSampler.size(); i++)
	{
		if (getMaterial->vecDiffuseSampler[i] != NULLPTR)
			getMaterial->vecDiffuseSampler[i]->SetSamplerState((int)i); //PSSetResourceView
	}

	for (size_t i = 0; i < getMaterial->vecNormalTexture.size(); i++)
	{
		if (getMaterial->vecNormalTexture[i] != NULLPTR)
			getMaterial->vecNormalTexture[i]->SetShaderResource((int)i); //PSSetResourceView
	}

	for (size_t i = 0; i < getMaterial->vecNormalSampler.size(); i++)
	{
		if (getMaterial->vecNormalSampler[i] != NULLPTR)
			getMaterial->vecNormalSampler[i]->SetSamplerState((int)i); //PSSetResourceView
	}

	for (size_t i = 0; i < getMaterial->vecSpecularTexture.size(); i++)
	{
		if (getMaterial->vecSpecularTexture[i] != NULLPTR)
			getMaterial->vecSpecularTexture[i]->SetShaderResource((int)i); //PSSetResourceView
	}

	for (size_t i = 0; i < getMaterial->vecSpecularSampler.size(); i++)
	{
		if (getMaterial->vecSpecularSampler[i] != NULLPTR)
			getMaterial->vecSpecularSampler[i]->SetSamplerState((int)i); //PSSetResourceView
	}
}
