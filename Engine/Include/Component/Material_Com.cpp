#include "stdafx.h"
#include "Material_Com.h"
#include "Animation2D_Com.h"

#include "../GameObject.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/Sampler.h"
#include "../Resource/Texture.h"

#include "../Render/ShaderManager.h"

JEONG::SubsetMaterial::SubsetMaterial()
{

}
JEONG::SubsetMaterial::~SubsetMaterial()
{
	
}

JEONG::Material_Com::Material_Com()
{
	m_ComType = CT_MATERIAL;
}

JEONG::Material_Com::Material_Com(const Material_Com& copyData)
	:Component_Base(copyData)
{
	m_vecMaterial.clear();

	for (size_t i = 0; i < copyData.m_vecMaterial.size(); i++)
	{
		vector<JEONG::SubsetMaterial*> TempVec;
		m_vecMaterial.push_back(TempVec);

		for (size_t j = 0; j < copyData.m_vecMaterial[i].size(); j++)
		{
			JEONG::SubsetMaterial* newMaterial = new JEONG::SubsetMaterial();
			newMaterial->MatrialInfo = copyData.m_vecMaterial[i][j]->MatrialInfo;

			for (size_t a = 0; a < copyData.m_vecMaterial[i][j]->vecDiffuseSampler.size(); a++)
			{
				newMaterial->vecDiffuseSampler[a] = copyData.m_vecMaterial[i][j]->vecDiffuseSampler[a];

				if (newMaterial->vecDiffuseSampler[a] != NULLPTR)
					newMaterial->vecDiffuseSampler[a]->AddRefCount();
			}
			for (size_t b = 0; b < copyData.m_vecMaterial[i][j]->vecDiffuseTexture.size(); b++)
			{
				newMaterial->vecDiffuseTexture[b] = copyData.m_vecMaterial[i][j]->vecDiffuseTexture[b];

				if (newMaterial->vecDiffuseTexture[b]!= NULLPTR)
					newMaterial->vecDiffuseTexture[b]->AddRefCount();
			}

			m_vecMaterial[i].push_back(newMaterial);
		}
	}
}

JEONG::Material_Com::~Material_Com()
{
	for (size_t i = 0; i < m_vecMaterial.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMaterial[i].size(); ++j)
		{
			Safe_Release_VecList(m_vecMaterial[i][j]->vecDiffuseSampler);
			Safe_Release_VecList(m_vecMaterial[i][j]->vecDiffuseTexture);
			SAFE_DELETE(m_vecMaterial[i][j]);
		}
		m_vecMaterial[i].clear();
	}
	m_vecMaterial.clear();
}

bool JEONG::Material_Com::Init()
{
	SetMaterial(Vector4::White);

	return true;
}

int JEONG::Material_Com::Input(float DeltaTime)
{
	return 0;
}

int JEONG::Material_Com::Update(float DeltaTime)
{
	return 0;
}

int JEONG::Material_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void JEONG::Material_Com::Collision(float DeltaTime)
{
}

void JEONG::Material_Com::CollisionLateUpdate(float DeltaTime)
{
}

void JEONG::Material_Com::Render(float DeltaTime)
{
}

JEONG::Material_Com * JEONG::Material_Com::Clone()
{
	return new Material_Com(*this);
}

void JEONG::Material_Com::SetMaterial(const Vector4 & Diffuse, int Container, int Subset)
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
}

void JEONG::Material_Com::SetDiffuseTexture(int RegisterNumber, const string & KeyName, int Container, int Subset)
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

	//텍스쳐 셋팅준비
	JEONG::SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];
	Safe_Release_VecList(getMaterial->vecDiffuseTexture);

	if (getMaterial->vecDiffuseTexture.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseTexture.resize(RegisterNumber + 1);

	getMaterial->vecDiffuseTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

void JEONG::Material_Com::SetDiffuseTexture(int RegisterNumber, const string & KeyName, const TCHAR * FileName, const string & PathKey, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<JEONG::SubsetMaterial*>	newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	JEONG::SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

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

void JEONG::Material_Com::SetDiffuseTexture(int RegisterNumber, Texture * pTexture, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<JEONG::SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
		m_vecMaterial[Container].push_back(CreateSubSet());

	JEONG::SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecDiffuseTexture.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseTexture.resize(RegisterNumber + 1);

	getMaterial->vecDiffuseTexture[RegisterNumber] = pTexture;
}

void JEONG::Material_Com::SetDiffuseTextureFromFullPath(int RegisterNumber, const string & KeyName, const TCHAR * FullPath, int Container, int Subset)
{
	if (Container >= m_vecMaterial.size())
	{
		vector<JEONG::SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
	{
		JEONG::SubsetMaterial* newMaterial = new JEONG::SubsetMaterial();
		m_vecMaterial[Container].push_back(newMaterial);
	}

	JEONG::SubsetMaterial *getMaterial = m_vecMaterial[Container][Subset];
	ResourceManager::Get()->CreateTextureFromFullPath(KeyName, FullPath);

	if (getMaterial->vecDiffuseTexture.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseTexture.resize(RegisterNumber + 1);

	getMaterial->vecDiffuseTexture[RegisterNumber] = ResourceManager::Get()->FindTexture(KeyName);
}

//이 함수는 아직 안들어옴.
void JEONG::Material_Com::SetSampler(int RegisterNumber, const string & KeyName, int Container, int Subset)
{
	//Sampler를 셋팅한다.
	if (Container >= m_vecMaterial.size())
	{
		vector<JEONG::SubsetMaterial*> newVec;
		m_vecMaterial.push_back(newVec);
	}

	if (Subset >= m_vecMaterial[Container].size())
	{
		JEONG::SubsetMaterial* newMaterial = new JEONG::SubsetMaterial();
		m_vecMaterial[Container].push_back(newMaterial);
	}

	JEONG::SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

	if (getMaterial->vecDiffuseSampler.capacity() <= RegisterNumber)
		getMaterial->vecDiffuseSampler.resize(RegisterNumber + 1);

	//기본정보가 지정된 샘플러를 찾아온다.
	getMaterial->vecDiffuseSampler[RegisterNumber] = ResourceManager::Get()->FindSampler(KeyName);
}

//쉐이더에 셋팅한다.
void JEONG::Material_Com::SetShader(int Container, int Subset)
{
	JEONG::SubsetMaterial* getMaterial = m_vecMaterial[Container][Subset];

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
}

JEONG::SubsetMaterial* JEONG::Material_Com::CreateSubSet()
{
	JEONG::SubsetMaterial* newSubset = new JEONG::SubsetMaterial();
	newSubset->vecDiffuseSampler.push_back(ResourceManager::Get()->FindSampler(LINER_SAMPLER));

	return newSubset;
}
