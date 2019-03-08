#include "../stdafx.h"
#include "../Device.h"
#include "Mesh.h"
#include "FBXLoader.h"

#include "Component/Material_Com.h"
#include "Component/Animation3D_Com.h"

JEONG_USING

Mesh::Mesh()
{
	m_Animation = NULLPTR;
	m_Material = NULLPTR;
}

Mesh::~Mesh()
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); i++)
	{
		SAFE_RELEASE(m_vecMeshContainer[i]->vertexBuffer.vBuffer);
		SAFE_DELETE_ARRARY(m_vecMeshContainer[i]->vertexBuffer.vInfo);

		for (size_t j = 0; j < m_vecMeshContainer[i]->vecIndexBuffer.size(); j++)
		{
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIndexBuffer[j].iBuffer);
			SAFE_DELETE_ARRARY(m_vecMeshContainer[i]->vecIndexBuffer[j].iInfo);
		}
		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();

	SAFE_RELEASE(m_Material);
}

void Mesh::Render()
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); i++)
	{
		//��ǲ ������� ���� �������� �����Ѵ�.
		Device::Get()->GetContext()->IASetPrimitiveTopology(m_vecMeshContainer[i]->PrimitiveType);

		//��Ʈ���̵� : �ش� ���ؽ����ۿ��� ���� ũ��(������)
		UINT Stride = m_vecMeshContainer[i]->vertexBuffer.vSize;
		UINT Offset = 0;
		//���ؽ����۸� �����Ѵ�. (���ؽ� ���̴��� ���������� �ִ� �غ� �Ѵ�) (��ǲ ������� �ִ´�)
		Device::Get()->GetContext()->IASetVertexBuffers(0, 1, &m_vecMeshContainer[i]->vertexBuffer.vBuffer, &Stride, &Offset);
		//�ε������۰� �ִ��� üũ�Ѵ�.
		if (m_vecMeshContainer[i]->vecIndexBuffer.empty())
			Device::Get()->GetContext()->Draw(m_vecMeshContainer[i]->vertexBuffer.vCount, 0);
		else
		{
			//�ε������۰� �ִٸ� �ε����� �׸� ���� ���� �� �ε����� �׸�.
			for (size_t j = 0; j < m_vecMeshContainer[i]->vecIndexBuffer.size(); j++)
			{
				//���̴��� ���������� �ִ��غ� �Ѵ�(�Է� �������Ϳ� �ִ´�)
				Device::Get()->GetContext()->IASetIndexBuffer(m_vecMeshContainer[i]->vecIndexBuffer[j].iBuffer, m_vecMeshContainer[i]->vecIndexBuffer[j].iFormat, 0);
				Device::Get()->GetContext()->DrawIndexed(m_vecMeshContainer[i]->vecIndexBuffer[j].iCount, 0, 0); ///�ε��������� ����, ��ġ(ù��°), �߰��Ǵ°�(0)
			} 
		}//else
	}
}

void Mesh::Render(int Container, int Subset)
{
	Device::Get()->GetContext()->IASetPrimitiveTopology(m_vecMeshContainer[Container]->PrimitiveType);

	UINT Stride = m_vecMeshContainer[Container]->vertexBuffer.vSize;
	UINT Offset = 0;

	Device::Get()->GetContext()->IASetVertexBuffers(0, 1, &m_vecMeshContainer[Container]->vertexBuffer.vBuffer, &Stride, &Offset);

	if (m_vecMeshContainer[Container]->vecIndexBuffer.empty())
		Device::Get()->GetContext()->Draw(m_vecMeshContainer[Container]->vertexBuffer.vCount, 0);
	else
	{
		Device::Get()->GetContext()->IASetIndexBuffer(m_vecMeshContainer[Container]->vecIndexBuffer[Subset].iBuffer, m_vecMeshContainer[Container]->vecIndexBuffer[Subset].iFormat, 0);
		Device::Get()->GetContext()->DrawIndexed(m_vecMeshContainer[Container]->vecIndexBuffer[Subset].iCount, 0, 0);
	}
}

bool Mesh::LoadMesh(const string & KeyName, const TCHAR * FileName, const string & PathKey)
{
	TCHAR	strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = PathManager::Get()->FindPath(PathKey);

	if (pPath)
		lstrcpy(strFullPath, pPath);
	lstrcat(strFullPath, FileName);

	return LoadMeshFromFullPath(KeyName, strFullPath);
}

bool Mesh::LoadMeshFromFullPath(const string & KeyName, const TCHAR * FullPath)
{
	m_TagName = KeyName;

	char strFullPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_UTF8, 0, FullPath, -1, strFullPath, lstrlen(FullPath), 0, 0);

	char strExt[_MAX_EXT] = {};
	_splitpath_s(strFullPath, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);

	_strupr_s(strExt);

	if (strcmp(strExt, ".FBX") == 0)
	{
		FBXLoader loader;

		if (loader.LoadFbx(strFullPath) == false)
			return false;

		return ConvertFbx(&loader, strFullPath);
	}

	return LoadFullPath(strFullPath);	
}

bool Mesh::CreateMesh(const string & TagName, const string & ShaderKeyName, const string & LayOutKeyName, void * vertexInfo, int vertexCount, int vertexSize, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitiveType, void * indexInfo, int indexCount, int indexSize, D3D11_USAGE indexUsage, DXGI_FORMAT indexFormat)
{
	SetTag(TagName);
	m_ShaderKeyName = ShaderKeyName;
	m_LayOutKeyName = LayOutKeyName;

	MeshContainer* newContainer = new MeshContainer();
	newContainer->PrimitiveType = primitiveType;

	m_vecMeshContainer.push_back(newContainer);

	if (CreateVertexBuffer(vertexInfo, vertexCount, vertexSize, vertexUsage) == false)
	{
		TrueAssert(true);
		return false;
	}

	if (indexInfo != NULLPTR)
	{
		if (CreateIndexBuffer(indexInfo, indexCount, indexSize, indexUsage, indexFormat) == false)
		{
			TrueAssert(true);
			return false;
		}
	}

	return true;
}

bool Mesh::CreateVertexBuffer(void * vertexInfo, int vertexCount, int vertexSize, D3D11_USAGE vertexUsage)
{
	//�׻� �������� �߰��� ���� �����´�.
	MeshContainer* getContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];
	getContainer->vertexBuffer.vCount = vertexCount;
	getContainer->vertexBuffer.vInfo = new char[vertexSize * vertexCount];
	getContainer->vertexBuffer.vUsage = vertexUsage;
	getContainer->vertexBuffer.vSize = vertexSize;

	memcpy(getContainer->vertexBuffer.vInfo, vertexInfo, vertexSize * vertexCount);

	D3D11_BUFFER_DESC bufferDesc = {}; ///������ ������ �����Ѵ�
	bufferDesc.ByteWidth = vertexSize * vertexCount; ///�� ����Ʈ��
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; ///�����뵵�ξ�����?
	bufferDesc.Usage = vertexUsage; ///������?

	if (vertexUsage == D3D11_USAGE_DYNAMIC)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA SubResourceData = {};
	SubResourceData.pSysMem = getContainer->vertexBuffer.vInfo; ///���ؽ������� ����

	if (FAILED(Device::Get()->GetDevice()->CreateBuffer(&bufferDesc, &SubResourceData, &getContainer->vertexBuffer.vBuffer)))
	{
		TrueAssert(true);
		return false;
	}

	char* Vertices =(char*)vertexInfo;
	Vector3 TempPos;
	//�ּҰ� memcpy (ù��° �ּ�)
	memcpy(&TempPos, Vertices, sizeof(Vector3));

	m_Min = TempPos;
	m_Max = TempPos;

	for (size_t i = 0; i < vertexCount; i++)
	{
		//�ּҰ��� �÷��� ��� ���� �޴´�. (void*�� ���� �ȵ�)
		memcpy(&TempPos, Vertices + (vertexSize * i), sizeof(Vector3));

		if (m_Min.x > TempPos.x)
			m_Min.x = TempPos.x;

		if (m_Min.y > TempPos.y)
			m_Min.y = TempPos.y;

		if (m_Min.z > TempPos.z)
			m_Min.z = TempPos.z;

		//////////////////////////

		if (m_Max.x < TempPos.x)
			m_Max.x = TempPos.x;

		if (m_Max.y < TempPos.y)
			m_Max.y = TempPos.y;

		if (m_Max.z < TempPos.z)
			m_Max.z = TempPos.z;
	}

	//�߽���
	m_Center = (m_Min + m_Max) * 0.5f;
	//����
	m_Lenth = m_Max - m_Min;
	//������
	m_Radius = m_Lenth.Lenth() * 0.5f;

	return true;
}

bool Mesh::CreateIndexBuffer(void * indexInfo, int indexCount, int indexSize, D3D11_USAGE indexUsage, DXGI_FORMAT indexFormat)
{
	//�׻� �������� �߰��� ���� �����´�.  
	MeshContainer* getContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];

	IndexBuffer TempIndexBuffer;
	TempIndexBuffer.iCount = indexCount;
	TempIndexBuffer.iInfo = new char[indexSize * indexCount];
	TempIndexBuffer.iUsage = indexUsage;
	TempIndexBuffer.iSize = indexSize;
	TempIndexBuffer.iFormat = indexFormat;

	memcpy(TempIndexBuffer.iInfo, indexInfo, indexSize * indexCount);

	D3D11_BUFFER_DESC bufferDesc = {}; ///������ ������ �����Ѵ�
	bufferDesc.ByteWidth = indexSize * indexCount; ///�� ����Ʈ��
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; ///�����뵵�ξ�����?
	bufferDesc.Usage = indexUsage; ///������?

	if (indexUsage == D3D11_USAGE_DYNAMIC)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA SubResourceData = {};
	SubResourceData.pSysMem = TempIndexBuffer.iInfo; ///�ε��������� ����

	if (FAILED(Device::Get()->GetDevice()->CreateBuffer(&bufferDesc, &SubResourceData, &TempIndexBuffer.iBuffer)))
	{
		TrueAssert(true);
		return false;
	}

	getContainer->vecIndexBuffer.push_back(TempIndexBuffer);

	return true;
}

Material_Com * Mesh::CloneMaterial()
{
	if (m_Material == NULLPTR)
		return NULLPTR;

	return m_Material->Clone();
}

Animation3D_Com * Mesh::CloneAnimation()
{
	if (m_Animation == NULLPTR)
		return NULLPTR;

	return m_Animation->Clone();
}

void Mesh::UpdateVertexBuffer(void * vertexInfo, int ContainerIndex)
{
	if (ContainerIndex < 0 || ContainerIndex >= m_vecMeshContainer.size())
		return;

	VertexColor* Temp = (VertexColor*)m_vecMeshContainer[ContainerIndex]->vertexBuffer.vInfo;

	switch (m_vecMeshContainer[ContainerIndex]->vertexBuffer.vUsage)
	{
		case D3D11_USAGE_DEFAULT:
			Device::Get()->GetContext()->UpdateSubresource(m_vecMeshContainer[ContainerIndex]->vertexBuffer.vBuffer, 0, NULLPTR, vertexInfo, 0, 0);
			break;
		case D3D11_USAGE_DYNAMIC:
		{
			D3D11_MAPPED_SUBRESOURCE mapData;
			//�޸𸮶����ɰ� ���� �� ���� Ǭ��. (���ؽ�Ʈ ����Ī�� ���۹���) Map ~ UnMap
			Device::Get()->GetContext()->Map(m_vecMeshContainer[ContainerIndex]->vertexBuffer.vBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapData);
			{
				memcpy(mapData.pData, vertexInfo, m_vecMeshContainer[ContainerIndex]->vertexBuffer.vSize *m_vecMeshContainer[ContainerIndex]->vertexBuffer.vCount);
			}
			Device::Get()->GetContext()->Unmap(m_vecMeshContainer[ContainerIndex]->vertexBuffer.vBuffer, 0);
		}
			break;
	}//switch
}

bool Mesh::Save(const string & FileName, const string & PathKey)
{
	return false;
}

bool Mesh::SaveFullPath(const char * pFullPath)
{
	return false;
}

bool Mesh::Load(const string & FileName, const string & PathKey)
{
	return false;
}

bool Mesh::LoadFullPath(const char * pFullPath)
{
	return false;
}

bool Mesh::ConvertFbx(FBXLoader * pLoader, const char * pFullPath)
{
	const vector<FBXMeshContainer*>*	pvecContainer = pLoader->GetMeshContainers();
	const vector<vector<FbxMaterial*>>*	pvecMaterials = pLoader->GetMaterials();

	vector<FBXMeshContainer*>::const_iterator iter;
	vector<FBXMeshContainer*>::const_iterator iterEnd = pvecContainer->end();

	vector<bool> vecEmptyIndex;

	for (iter = pvecContainer->begin(); iter != iterEnd; ++iter)
	{
		MeshContainer*	pContainer = new MeshContainer();

		m_LayOutKeyName = VERTEX3D_LAYOUT;
		m_vecMeshContainer.push_back(pContainer);

		int	iVtxSize = 0;

		// ������ ���� ���
		if ((*iter)->bBump)
		{
			if ((*iter)->bAnimation)
				m_ShaderKeyName = STANDARD_BUMP_ANIM_SHADER;

			else
				m_ShaderKeyName = STANDARD_BUMP_SHADER;
		}

		// ������ ���� ���
		else
		{
			if ((*iter)->bAnimation)
				m_ShaderKeyName = STANDARD_TEX_NORMAL_ANIM_SHADER;

			else
				m_ShaderKeyName = STANDARD_TEX_NORMAL_SHADER;
		}

		vector<Vertex3D>	vecVtx;
		iVtxSize = sizeof(Vertex3D);

		for (size_t i = 0; i < (*iter)->vecPos.size(); ++i)
		{
			Vertex3D	tVtx = {};

			tVtx.Pos = (*iter)->vecPos[i];
			tVtx.Normal = (*iter)->vecNormal[i];
			tVtx.UV = (*iter)->vecUV[i];

			if (!(*iter)->vecTangent.empty())
				tVtx.Tangent = (*iter)->vecTangent[i];

			if (!(*iter)->vecBinormal.empty())
				tVtx.Binormal = (*iter)->vecBinormal[i];

			if (!(*iter)->vecBlendWeight.empty())
			{
				tVtx.Weight = (*iter)->vecBlendWeight[i];
				tVtx.Index = (*iter)->vecBlendIndex[i];
			}

			vecVtx.push_back(tVtx);
		}

		pContainer->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		if (!CreateVertexBuffer(&vecVtx[0], (int)vecVtx.size(), iVtxSize, D3D11_USAGE_DEFAULT))
			return false;

		// �ε������� ����
		for (size_t i = 0; i < (*iter)->vecIndices.size(); ++i)
		{
			if ((*iter)->vecIndices[i].empty())
			{
				vecEmptyIndex.push_back(false);
				continue;
			}

			vecEmptyIndex.push_back(true);

			if (!CreateIndexBuffer(&(*iter)->vecIndices[i][0], (int)(*iter)->vecIndices[i].size(), 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT))
				return false;
		}
	}

	// ���� ������ �о�´�.
	const vector<vector<FbxMaterial*>>*	pMaterials = pLoader->GetMaterials();

	vector<vector<FbxMaterial*>>::const_iterator iterM;
	vector<vector<FbxMaterial*>>::const_iterator iterMEnd = pMaterials->end();

	if (!pMaterials->empty())
	{
		// ���� ����� ���� Ŭ������ �����Ѵ�.
		m_Material = new Material_Com();

		if (!m_Material->Init())
		{
			SAFE_RELEASE(m_Material);
			return NULL;
		}

		m_Material->ClearContainer();
	}

	int	iContainer = 0;
	for (iterM = pMaterials->begin(); iterM != iterMEnd; ++iterM, ++iContainer)
	{
		for (size_t i = 0; i < (*iterM).size(); ++i)
		{
			// �ε��� ���۰� ������� ��쿡�� ������ �߰����� �ʴ´�.
			if (!vecEmptyIndex[i])
				continue;

			// ���� ������ ���´�.
			FbxMaterial*	pMtrl = (*iterM)[i];
			m_Material->SetMaterial(pMtrl->vDif, pMtrl->vAmb, pMtrl->vSpc, pMtrl->fShininess, pMtrl->vEmv, iContainer, (int)i);

			// �̸��� �ҷ��´�.
			char strName[MAX_PATH] = {};
			_splitpath_s(pMtrl->strDifTex.c_str(), NULL, 0, NULL, 0, strName, MAX_PATH, NULL, 0);

			TCHAR	strPath[MAX_PATH] = {};

#ifdef UNICODE
			MultiByteToWideChar(CP_ACP, 0, pMtrl->strDifTex.c_str(), -1, strPath, (int)pMtrl->strDifTex.length());
#endif // UNICODE

			m_Material->SetDiffuseSampler(0, LINER_SAMPLER, iContainer, (int)i);
			m_Material->SetDiffuseTextureFromFullPath(0, strName, strPath, iContainer, (int)i);

			if (!pMtrl->strBumpTex.empty())
			{
				memset(strName, 0, MAX_PATH);
				_splitpath_s(pMtrl->strBumpTex.c_str(), NULL, 0, NULL, 0, strName, MAX_PATH, NULL, 0);

				memset(strPath, 0, sizeof(wchar_t) * MAX_PATH);

				MultiByteToWideChar(CP_ACP, 0, pMtrl->strBumpTex.c_str(), -1, strPath, (int)pMtrl->strBumpTex.length());

				m_Material->SetNormalSampler(0, LINER_SAMPLER, iContainer, (int)i);
				m_Material->SetNormalTextureFromFullPath(1, strName, strPath, iContainer, (int)i);
			}

			if (!pMtrl->strSpcTex.empty())
			{
				memset(strName, 0, MAX_PATH);
				_splitpath_s(pMtrl->strSpcTex.c_str(), NULL, 0, NULL, 0, strName, MAX_PATH, NULL, 0);

				memset(strPath, 0, sizeof(wchar_t) * MAX_PATH);

				MultiByteToWideChar(CP_ACP, 0, pMtrl->strSpcTex.c_str(), -1, strPath, (int)pMtrl->strSpcTex.length());

				m_Material->SetSpecularSampler(0, LINER_SAMPLER, iContainer, (int)i);
				m_Material->SetSpecularTextureFromFullPath(2, strName, strPath, iContainer, (int)i);
			}
		}
	}

	m_Lenth = m_Max - m_Min;

	m_Center = (m_Max + m_Min) / 2.f;
	m_Radius = m_Lenth.Lenth() / 2.f;

	// �ִϸ��̼� ó��
	const vector<FbxBone*>*	pvecBone = pLoader->GetBones();

	if (pvecBone->empty() == false)
	{
		SAFE_RELEASE(m_Animation);

		m_Animation = new Animation3D_Com();

		if (!m_Animation->Init())
		{
			SAFE_RELEASE(m_Animation);
			return false;
		}

		//// �� ����ŭ �ݺ��Ѵ�.
		vector<FbxBone*>::const_iterator	iterB;
		vector<FbxBone*>::const_iterator	iterBEnd = pvecBone->end();

		for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
		{
			Bone* pBone = new Bone();

			pBone->strName = (*iterB)->strName;
			pBone->iDepth = (*iterB)->iDepth;
			pBone->iParentIndex = (*iterB)->iParentIndex;

			float fMat[4][4];

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
					fMat[i][j] = (float)((*iterB)->matOffset.mData[i].mData[j]);
			}

			pBone->matOffset = new Matrix();
			*pBone->matOffset = fMat;

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
					fMat[i][j] = (float)((*iterB)->matBone.mData[i].mData[j]);
			}

			pBone->matBone = new Matrix;
			*pBone->matBone = fMat;

			m_Animation->AddBone(pBone);
		}

		m_Animation->CreateBoneTexture();

		// �ִϸ��̼� Ŭ���� �߰��Ѵ�.
		const vector<FbxAnimationClip*>* pvecClip = pLoader->GetClips();

		// Ŭ���� �о�´�.
		vector<FbxAnimationClip*>::const_iterator	iterC;
		vector<FbxAnimationClip*>::const_iterator	iterCEnd = pvecClip->end();

		for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
			m_Animation->AddClip(AO_LOOP, *iterC);
	}
	else
		m_Animation = NULLPTR;

	char	strFullPath[MAX_PATH] = {};
	strcpy_s(strFullPath, pFullPath);
	int	iPathLength = (int)strlen(strFullPath);
	memcpy(&strFullPath[iPathLength - 3], "msh", 3);

	SaveFullPath(strFullPath);

	wchar_t* tFullPath = {};
	tFullPath = CA2W(strFullPath);

	if (m_Animation)
	{
		memcpy(&strFullPath[iPathLength - 3], "bne", 3);
		m_Animation->SaveBoneFullPath(tFullPath);

		memcpy(&strFullPath[iPathLength - 3], "anm", 3);
		m_Animation->SaveFullPath(tFullPath);
	}

	return true;
}

