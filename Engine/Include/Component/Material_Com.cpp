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

void Material_Com::Save(BineryWrite & Writer)
{
	size_t	iContainer = m_vecMaterial.size();
	Writer.WriteData(iContainer);

	for (size_t i = 0; i < iContainer; ++i)
	{
		size_t iSubset = m_vecMaterial[i].size();
		Writer.WriteData(iSubset);

		for (size_t j = 0; j < iSubset; ++j)
		{
			SubsetMaterial*	pSubset = m_vecMaterial[i][j];
			Writer.WriteData(&pSubset->MatrialInfo, sizeof(MaterialCbuffer));

			SaveTextureSet(Writer, pSubset->DiffuseTex);
			SaveTextureSet(Writer, pSubset->NormalTex);
			SaveTextureSet(Writer, pSubset->SpecularTex);

			size_t	iMultiTexCount = pSubset->vecMultiTexture.size();
			Writer.WriteData(iMultiTexCount);

			for (size_t k = 0; k < iMultiTexCount; ++k)
				SaveTextureSet(Writer, &pSubset->vecMultiTexture[k]);
		}
	}
}

void Material_Com::Load(BineryRead & Reader)
{
	ClearContainer();

	size_t	iContainer = 0;
	Reader.ReadData(iContainer);

	for (size_t i = 0; i < iContainer; ++i)
	{
		size_t iSubset = 0;
		Reader.ReadData(iSubset);
		
		vector<SubsetMaterial*>	vecMtrl;
		m_vecMaterial.push_back(vecMtrl);

		for (size_t j = 0; j < iSubset; ++j)
		{
			SubsetMaterial*	pSubset = new SubsetMaterial;
			m_vecMaterial[i].push_back(pSubset);

			Reader.ReadData(&pSubset->MatrialInfo, sizeof(MaterialCbuffer));

			LoadTextureSet(Reader, &pSubset->DiffuseTex);
			LoadTextureSet(Reader, &pSubset->NormalTex);
			LoadTextureSet(Reader, &pSubset->SpecularTex);

			size_t	iMultiTexCount = pSubset->vecMultiTexture.size();

			Reader.ReadData(iMultiTexCount);

			TextureSet* pTexSet = nullptr;
			for (size_t k = 0; k < iMultiTexCount; ++k)
			{
				pTexSet = &pSubset->vecMultiTexture[k];
				LoadTextureSet(Reader, &pTexSet);
			}
		}
	}

}

void Material_Com::SetMaterial(const Vector4& Diffuse, const Vector4& Ambient, const Vector4& Specular, float SpecularPower, const Vector4& Emissive, int Container, int Subset)
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

void Material_Com::AddMultiTex(int iSmpRegister, const string & strSmpKey, int iRegister, const string & strKey, const TCHAR * pFileName, const string & strPathKey, int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
		m_vecMaterial[iContainer].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[iContainer][iSubset];

	if (pMaterial->vecMultiTexture.size() == 4)
		return;

	TextureSet	tSet = {};
	tSet.m_Register = iRegister;
	tSet.m_SamplerRegister = iSmpRegister;

	ResourceManager::Get()->CreateTexture(strKey, pFileName, strPathKey);
	tSet.m_Tex = ResourceManager::Get()->FindTexture(strKey);
	tSet.m_Sampler = ResourceManager::Get()->FindSampler(strKey);

	pMaterial->vecMultiTexture.push_back(tSet);
}

void Material_Com::AddMultiTex(int iSmpRegister, const string & strSmpKey, int iRegister, const string & strKey, const vector<const TCHAR*>& vecFileName, const string & strPathKey, int iContainer, int iSubset)
{
	if (iContainer >= m_vecMaterial.size())
	{
		vector<SubsetMaterial*>	vec;
		m_vecMaterial.push_back(vec);
	}

	if (iSubset >= m_vecMaterial[iContainer].size())
		m_vecMaterial[iContainer].push_back(CreateSubSet());

	SubsetMaterial*	pMaterial = m_vecMaterial[iContainer][iSubset];

	if (pMaterial->vecMultiTexture.size() == 4)
		return;

	TextureSet	tSet = {};

	tSet.m_Register = iRegister;
	tSet.m_SamplerRegister = iSmpRegister;

	ResourceManager::Get()->CreateTexture(strKey, vecFileName, strPathKey);
	tSet.m_Tex = ResourceManager::Get()->FindTexture(strKey);

	tSet.m_Sampler = ResourceManager::Get()->FindSampler(strSmpKey);

	pMaterial->vecMultiTexture.push_back(tSet);
}

void Material_Com::SaveTextureSet(BineryWrite & Writer, TextureSet * pTexture)
{
	bool bTexEnable = false;

	if (pTexture)
	{
		bTexEnable = true;
		Writer.WriteData(bTexEnable);
		Writer.WriteData(pTexture->m_Tex->GetTag());

		// Texture 경로를 얻어온다.
		const vector<TCHAR*>* pPathList = pTexture->m_Tex->GetFullPath();

		size_t iPathCount = pPathList->size();
		Writer.WriteData(iPathCount);

		vector<TCHAR*>::const_iterator	iter;
		vector<TCHAR*>::const_iterator	iterEnd = pPathList->end();

		for (iter = pPathList->begin(); iter != iterEnd; ++iter)
		{
			int	iPathLength = lstrlen(*iter);
			char strPath[MAX_PATH] = {};

#ifdef UNICODE
			WideCharToMultiByte(CP_ACP, 0, *iter, -1, strPath, iPathLength, 0, 0);
#else
			strcpy_s(strPath, *iter);
#endif // UNICODE
			_strupr_s(strPath);

			for (int k = iPathLength - 1; k >= 0; --k)
			{
				if (strPath[k] == '\\' || strPath[k] == '/')
				{
					char	strBin[3];
					strBin[0] = 'N';
					strBin[1] = 'I';
					strBin[2] = 'B';
					bool	bEnable = true;
					for (int l = 1; l < 4; ++l)
					{
						if (strPath[k - l] != strBin[l - 1])
						{
							bEnable = false;
							break;
						}
					}

					if (bEnable)
					{
						char strSavePath[MAX_PATH] = {};
						int	iSaveCount = iPathLength - (k + 1);
						memcpy(strSavePath, &strPath[k + 1], sizeof(char) * iSaveCount);
						Writer.WriteData(strSavePath);
						break;
					}
				}
			}
		}

		// Sampler Key
		Writer.WriteData(pTexture->m_Sampler->GetTag());
		Writer.WriteData(pTexture->m_Register);
		Writer.WriteData(pTexture->m_SamplerRegister);
	}

	else
		Writer.WriteData(bTexEnable);
}

void Material_Com::LoadTextureSet(BineryRead & Reader, TextureSet ** ppTexture)
{
	bool bTexEnable = false;
	Reader.ReadData(bTexEnable);

	if (bTexEnable)
	{
		string strTexKey;
		Reader.ReadData(strTexKey);

		size_t iPathCount = 0;
		Reader.ReadData(iPathCount);

		if (iPathCount == 1)
		{
			string strPath = {};
			Reader.ReadData(strPath);

			TCHAR strLoadPath[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, strPath.c_str(), -1, strLoadPath, (int)strPath.length() * 2);
#else
			strcpy_s(strLoadPath, strPath);
#endif // UNICODE

			*ppTexture = new TextureSet;

			ResourceManager::Get()->CreateTexture(strTexKey, strLoadPath, ROOT_PATH);
			(*ppTexture)->m_Tex = ResourceManager::Get()->FindTexture(strTexKey);
		}

		else
		{
			vector<const TCHAR*> vecPath;
			for (size_t i = 0; i < iPathCount; ++i)
			{
				string strPath = {};
				int	iSaveCount = 0;
				
				Reader.ReadData(strPath);

				TCHAR*	strLoadPath = new TCHAR[MAX_PATH];
				memset(strLoadPath, 0, sizeof(TCHAR) * MAX_PATH);

				strLoadPath = CA2W(strPath.c_str());

				vecPath.push_back(strLoadPath);
			}
			*ppTexture = new TextureSet;

			ResourceManager::Get()->CreateTexture(strTexKey, vecPath, ROOT_PATH);
			(*ppTexture)->m_Tex = ResourceManager::Get()->FindTexture(strTexKey);
		}

		// Sampler Key
		string strSmpKey = {};
		Reader.ReadData(strSmpKey);

		(*ppTexture)->m_Sampler = ResourceManager::Get()->FindSampler(strSmpKey);

		Reader.ReadData((*ppTexture)->m_Register);
		Reader.ReadData((*ppTexture)->m_SamplerRegister);
	}
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
