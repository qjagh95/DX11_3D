#include "stdafx.h"
#include "Renderer_Com.h"
#include "Transform_Com.h"
#include "Camera_Com.h"
#include "Material_Com.h"

#include "../Device.h"
#include "../GameObject.h"

#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"
#include "../Render/Shader.h"
#include "../Render/RenderState.h"
#include "../Render/ShaderManager.h"
#include "../Render/RenderManager.h"

JEONG_USING

Renderer_Com::Renderer_Com()
	:m_Mesh(NULLPTR), m_Shader(NULLPTR), m_LayOut(NULLPTR), m_Material(NULLPTR), m_isScreenRender(false)
{
	m_ComType = CT_RENDER;
	ZeroMemory(m_RenderState, sizeof(RenderState*) * RS_END);
	ZeroMemory(&m_ComponentCBuffer, sizeof(ComponentCBuffer));

	if (RenderManager::Get()->GetGameMode() == GM_2D)
		SetRenderState(DEPTH_DISABLE); //여기서 2D오브젝트들은 이미 Set을 해줌 알파 XX
}

Renderer_Com::Renderer_Com(const Renderer_Com& copyObject)
	:Component_Base(copyObject)
{
	*this = copyObject;

	if (m_Mesh != NULLPTR)
		m_Mesh->AddRefCount();

	if (m_Shader != NULLPTR)
		m_Shader->AddRefCount();

	for (int i = 0; i < RS_END; ++i)
	{
		if (m_RenderState[i] != NULLPTR)
			m_RenderState[i]->AddRefCount();
	}

	m_Material = NULLPTR;
	m_CBufferMap.clear();

	unordered_map<string, RendererCBuffer*>::const_iterator StartIter = copyObject.m_CBufferMap.begin();
	unordered_map<string, RendererCBuffer*>::const_iterator EndIter = copyObject.m_CBufferMap.end();

	for (; StartIter != EndIter; StartIter++)
	{
		RendererCBuffer* newCBuffer = new RendererCBuffer();
		newCBuffer->Name = StartIter->second->Name;
		newCBuffer->BufferSize = StartIter->second->BufferSize;
		newCBuffer->pBuffer = new char[newCBuffer->BufferSize];

		m_CBufferMap.insert(make_pair(StartIter->first, newCBuffer));
	}
}

Renderer_Com::~Renderer_Com()
{
	SAFE_RELEASE(m_Mesh);
	SAFE_RELEASE(m_Shader);
	SAFE_RELEASE(m_Material);

	for (int i = 0; i < RS_END; ++i)
		SAFE_RELEASE(m_RenderState[i]);

	unordered_map<string, RendererCBuffer*>::iterator StartIter = m_CBufferMap.begin();
	unordered_map<string, RendererCBuffer*>::iterator EndIter = m_CBufferMap.end();

	for (; StartIter != EndIter; StartIter++)
	{
		SAFE_DELETE_ARRARY(StartIter->second->pBuffer);
		SAFE_DELETE(StartIter->second);
	}

	m_CBufferMap.clear();
}

bool Renderer_Com::Init()
{
	CheckComponent();

	return true;
}

int Renderer_Com::Input(float DeltaTime)
{
	return 0;
}

int Renderer_Com::Update(float DeltaTime)
{
	return 0;
}

int Renderer_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Renderer_Com::Collision(float DeltaTime)
{
}

void Renderer_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Renderer_Com::Render(float DeltaTime)
{
	if (m_Material == NULLPTR)
		m_Material = FindComponentFromType<Material_Com>(CT_MATERIAL);

	//업데이트 함수에서 이미 World행렬은 변환이 되었다.
	//랜더러 컴포넌트에서 투영변환(Projection)을하고, 쉐이더에 버퍼정보를 보낸다.(UpdateTransform)
	UpdateTransform();

	for (int i = 0; i < RS_END; i++)
	{
		if (m_RenderState[i] != NULLPTR)
			m_RenderState[i]->SetState();
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	//여기서 아틀라스이미지의 UV값을 쉐이더로 보낸다.
	unordered_map<string, RendererCBuffer*>::iterator StartIter = m_CBufferMap.begin();
	unordered_map<string, RendererCBuffer*>::iterator EndIter = m_CBufferMap.end();

	for (; StartIter != EndIter ; StartIter++)
		ShaderManager::Get()->UpdateCBuffer(StartIter->first, StartIter->second->pBuffer);

	////////////////////////////////////////////////////////////////////////////////////////////
	ShaderManager::Get()->UpdateCBuffer("Component", &m_ComponentCBuffer);

	//버텍스 데이터 덩어리를 쉐이더에 셋팅해주는 역할.
	Device::Get()->GetContext()->IASetInputLayout(m_LayOut);
	m_Shader->SetShader();

	for (size_t i = 0; i < m_Mesh->GetContainerCount(); i++)
	{
		for (size_t j = 0; j < m_Mesh->GetSubsetCount((int)i); j++)
		{
			m_Material->SetShader((int)i, (int)j);			
			m_Mesh->Render((int)i, (int)j);
		}
	}

	for (int i = 0; i < RS_END; i++)
	{
		if (m_RenderState[i] != NULLPTR)
			m_RenderState[i]->ResetState();
	}
}

Renderer_Com * Renderer_Com::Clone()
{
	return new Renderer_Com(*this);
}

void Renderer_Com::AfterClone()
{
	m_Material = FindComponentFromType<Material_Com>(CT_MATERIAL);
}

void Renderer_Com::SetMesh(Mesh * mesh)
{
	SAFE_RELEASE(m_Mesh);
	m_Mesh = mesh;

	if(mesh != NULLPTR)
	{
		mesh->AddRefCount();

		SetShader(mesh->GetShaderKey());
		SetLayOut(mesh->GetLayOutKey());
	}
}

void Renderer_Com::SetMesh(const string & KeyName)
{
	SAFE_RELEASE(m_Mesh);

	m_Mesh = ResourceManager::Get()->FindMesh(KeyName);

	if (m_Mesh != NULLPTR)
	{
		SetShader(m_Mesh->GetShaderKey());
		SetLayOut(m_Mesh->GetLayOutKey());
	}
}

void Renderer_Com::SetMesh(const string & KeyName, const TCHAR * FileName, const string & PathKey)
{
	SAFE_RELEASE(m_Mesh);
	ResourceManager::Get()->LoadMesh(KeyName, FileName, PathKey);
	m_Mesh = ResourceManager::Get()->FindMesh(KeyName);

	if (m_Mesh != NULLPTR)
	{
		SetShader(m_Mesh->GetShaderKey());
		SetLayOut(m_Mesh->GetLayOutKey());

		Material_Com* clone = m_Mesh->CloneMaterial();

		if (clone != NULLPTR)
		{
			m_Material = clone;

			m_Object->DeleteComponent(CT_MATERIAL);
			m_Object->AddComponent(clone);

			SAFE_RELEASE(clone);
		}
	}
}

void Renderer_Com::SetShader(Shader * shader)
{
	SAFE_RELEASE(m_Shader);
	m_Shader = shader;

	if (shader != NULLPTR)
		shader->AddRefCount();
}

void Renderer_Com::SetShader(const string & KeyName)
{
	SAFE_RELEASE(m_Shader);
	m_Shader = ShaderManager::Get()->FindShader(KeyName);
}

void Renderer_Com::SetLayOut(const string & KeyName)
{
	m_LayOut = ShaderManager::Get()->FindInputLayOut(KeyName);
}

void Renderer_Com::SetRenderState(const string & KeyName)
{
	RenderState* getState = RenderManager::Get()->FindRenderState(KeyName);

	if (getState == NULLPTR)
		return;

	//멤버변수 배열에 가져온 타입상수번째에 집어넣는다.
	m_RenderState[getState->GetStateEnum()] = getState;
}

bool Renderer_Com::CreateRendererCBuffer(const string & KeyName, int BufferSize)
{
	RendererCBuffer* newCBuffer = FindCBuffer(KeyName);

	if (newCBuffer != NULLPTR)
		return false;

	newCBuffer = new RendererCBuffer();
	newCBuffer->Name = KeyName;
	newCBuffer->pBuffer = new char[BufferSize];
	newCBuffer->BufferSize = BufferSize;

	m_CBufferMap.insert(make_pair(KeyName, newCBuffer));
	return true;
}

void Renderer_Com::UpdateRendererCBuffer(const string & KeyName, void * pData, int BufferSize)
{
	//여기선 메모리 복사만 해놓고 랜더에서 쉐이더로 넘긴다.
	RendererCBuffer* getCBuffer = FindCBuffer(KeyName);

	if (getCBuffer == NULLPTR)
	{
		getCBuffer = new RendererCBuffer();
		getCBuffer->Name = KeyName;
		getCBuffer->pBuffer = new char[BufferSize];
		getCBuffer->BufferSize = BufferSize;

		m_CBufferMap.insert(make_pair(KeyName, getCBuffer));
	}
	memcpy(getCBuffer->pBuffer, pData, BufferSize);
}

void Renderer_Com::UpdateTransform()
{
	TransformCBuffer cBuffer = {};
	Camera_Com* getCamera = NULLPTR;

	if (m_isScreenRender == true)
		getCamera = m_Scene->GetUICamera();
	else
		getCamera = m_Scene->GetMainCamera();
	
	cBuffer.World = m_Transform->GetWorldMatrix();
	cBuffer.View = getCamera->GetViewMatrix();
	cBuffer.Projection = getCamera->GetProjection();

	cBuffer.WV = cBuffer.World * cBuffer.View;
	cBuffer.WVP = cBuffer.WV * cBuffer.Projection;
	cBuffer.InvProjection = cBuffer.Projection;
	cBuffer.InvProjection.Inverse();

	cBuffer.Pivot = m_Transform->GetPivot();
	cBuffer.Lenth = m_Mesh->GetLenth();

	//쉐이더로 전해주기위하여 전치행렬로 변환한다.
	cBuffer.World.Transpose();
	cBuffer.View.Transpose();
	cBuffer.Projection.Transpose();
	cBuffer.WV.Transpose();
	cBuffer.WVP.Transpose();
	cBuffer.InvProjection.Transpose();

	//미리 정의해놓은 상수버퍼(ID3D11Buffer)를 가져와서 업데이트 시킨다.
	//버텍스쉐이더와 픽셀쉐이더에 위에서 값을 가져온 행렬들을 (상수버퍼) 셋팅해준다. 
	ShaderManager::Get()->UpdateCBuffer("Transform", &cBuffer);
}

RendererCBuffer * Renderer_Com::FindCBuffer(const string & KeyName)
{
	unordered_map<string, RendererCBuffer*>::iterator FindIter = m_CBufferMap.find(KeyName);

	if (FindIter == m_CBufferMap.end())
		return NULLPTR;

	return FindIter->second;
}

void Renderer_Com::CheckComponent()
{
	const list<Component_Base*>* TempList = m_Object->GetComponentList();

	list<Component_Base*>::const_iterator StartIter = TempList->begin();
	list<Component_Base*>::const_iterator EndIter = TempList->end();

	for (;StartIter != EndIter ;StartIter++)
	{
		switch ((*StartIter)->GetComType())
		{
			case CT_ANIMATION2D:
				m_ComponentCBuffer.TextureAnimation2D = 1;
				break;
		}
	}
}

void Renderer_Com::DeleteComponentCBuffer(Component_Base * DeleteCom)
{
	switch (DeleteCom->GetComType())
	{
		case CT_ANIMATION2D:
			m_ComponentCBuffer.TextureAnimation2D = 1;
			break;
	}
}

void Renderer_Com::InitMaterial()
{
	m_Material = AddComponent<Material_Com>("Material");
	CheckComponent();
}
	