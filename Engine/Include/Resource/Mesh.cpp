#include "../stdafx.h"
#include "../Device.h"
#include "Mesh.h"
#include "FBXLoader.h"

#include "Component/Material_Com.h"

JEONG_USING

Mesh::Mesh()
{
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
		//인풋 어셈블러에 도형 위상구조를 지정한다.
		Device::Get()->GetContext()->IASetPrimitiveTopology(m_vecMeshContainer[i]->PrimitiveType);

		//스트라이드 : 해당 버텍스버퍼에서 사용될 크기(사이즈)
		UINT Stride = m_vecMeshContainer[i]->vertexBuffer.vSize;
		UINT Offset = 0;
		//버텍스버퍼를 셋팅한다. (버텍스 쉐이더에 정점정보를 넣는 준비를 한다) (인풋 어셈블러에 넣는다)
		Device::Get()->GetContext()->IASetVertexBuffers(0, 1, &m_vecMeshContainer[i]->vertexBuffer.vBuffer, &Stride, &Offset);
		//인덱스버퍼가 있는지 체크한다.
		if (m_vecMeshContainer[i]->vecIndexBuffer.empty())
			Device::Get()->GetContext()->Draw(m_vecMeshContainer[i]->vertexBuffer.vCount, 0);
		else
		{
			//인덱스버퍼가 있다면 인덱스를 그릴 순서 지정 후 인덱스로 그림.
			for (size_t j = 0; j < m_vecMeshContainer[i]->vecIndexBuffer.size(); j++)
			{
				//쉐이더에 정점정보를 넣는준비를 한다(입력 레지스터에 넣는다)
				Device::Get()->GetContext()->IASetIndexBuffer(m_vecMeshContainer[i]->vecIndexBuffer[j].iBuffer, m_vecMeshContainer[i]->vecIndexBuffer[j].iFormat, 0);
				Device::Get()->GetContext()->DrawIndexed(m_vecMeshContainer[i]->vecIndexBuffer[j].iCount, 0, 0); ///인덱스버퍼의 갯수, 위치(첫번째), 추가되는값(0)
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
	//항상 마지막에 추가된 것을 가져온다.
	MeshContainer* getContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];
	getContainer->vertexBuffer.vCount = vertexCount;
	getContainer->vertexBuffer.vInfo = new char[vertexSize * vertexCount];
	getContainer->vertexBuffer.vUsage = vertexUsage;
	getContainer->vertexBuffer.vSize = vertexSize;

	memcpy(getContainer->vertexBuffer.vInfo, vertexInfo, vertexSize * vertexCount);

	D3D11_BUFFER_DESC bufferDesc = {}; ///버퍼의 정보를 설정한다
	bufferDesc.ByteWidth = vertexSize * vertexCount; ///총 바이트수
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; ///무슨용도로쓸꺼야?
	bufferDesc.Usage = vertexUsage; ///어떻게출력?

	if (vertexUsage == D3D11_USAGE_DYNAMIC)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA SubResourceData = {};
	SubResourceData.pSysMem = getContainer->vertexBuffer.vInfo; ///버텍스버퍼의 정보

	if (FAILED(Device::Get()->GetDevice()->CreateBuffer(&bufferDesc, &SubResourceData, &getContainer->vertexBuffer.vBuffer)))
	{
		TrueAssert(true);
		return false;
	}

	char* Vertices =(char*)vertexInfo;
	Vector3 TempPos;
	//주소값 memcpy (첫번째 주소)
	memcpy(&TempPos, Vertices, sizeof(Vector3));

	m_Min = TempPos;
	m_Max = TempPos;

	for (size_t i = 0; i < vertexCount; i++)
	{
		//주소값을 늘려서 계속 값을 받는다. (void*를 쓰면 안됨)
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

	//중심점
	m_Center = (m_Min + m_Max) * 0.5f;
	//길이
	m_Lenth = m_Max - m_Min;
	//반지름
	m_Radius = m_Lenth.Lenth() * 0.5f;

	return true;
}

bool Mesh::CreateIndexBuffer(void * indexInfo, int indexCount, int indexSize, D3D11_USAGE indexUsage, DXGI_FORMAT indexFormat)
{
	//항상 마지막에 추가된 것을 가져온다.  
	MeshContainer* getContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];

	IndexBuffer TempIndexBuffer;
	TempIndexBuffer.iCount = indexCount;
	TempIndexBuffer.iInfo = new char[indexSize * indexCount];
	TempIndexBuffer.iUsage = indexUsage;
	TempIndexBuffer.iSize = indexSize;
	TempIndexBuffer.iFormat = indexFormat;

	memcpy(TempIndexBuffer.iInfo, indexInfo, indexSize * indexCount);

	D3D11_BUFFER_DESC bufferDesc = {}; ///버퍼의 정보를 설정한다
	bufferDesc.ByteWidth = indexSize * indexCount; ///총 바이트수
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; ///무슨용도로쓸꺼야?
	bufferDesc.Usage = indexUsage; ///어떻게출력?

	if (indexUsage == D3D11_USAGE_DYNAMIC)
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA SubResourceData = {};
	SubResourceData.pSysMem = TempIndexBuffer.iInfo; ///인덱스버퍼의 정보

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
	return m_Material->Clone();
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
			//메모리락을걸고 실행 후 락을 푼다. (컨텍스트 스위칭시 조작방지) Map ~ UnMap
			Device::Get()->GetContext()->Map(m_vecMeshContainer[ContainerIndex]->vertexBuffer.vBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapData);
			{
				memcpy(mapData.pData, vertexInfo, m_vecMeshContainer[ContainerIndex]->vertexBuffer.vSize *m_vecMeshContainer[ContainerIndex]->vertexBuffer.vCount);
			}
			Device::Get()->GetContext()->Unmap(m_vecMeshContainer[ContainerIndex]->vertexBuffer.vBuffer, 0);
		}
			break;
	}//switch
}

