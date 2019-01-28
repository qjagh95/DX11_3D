#include "../stdafx.h"
#include "RenderManager.h"
#include "RenderTarget.h"
#include "BlendState.h"

#include "../GameObject.h"
#include "../Device.h"
#include "../Render/ShaderManager.h"
#include "../Component/Light_Com.h"
#include "../Resource/Sampler.h"

#include "DepthStancilState.h"
#include "Render/Shader.h"

#include "Resource/ResourceManager.h"

JEONG_USING
SINGLETON_VAR_INIT(JEONG::RenderManager)

JEONG::RenderManager::RenderManager()
	:m_CreateState(NULLPTR)
{
	m_GameMode = GM_3D;
	m_isDeferred = true;
	m_DepthDisable = NULLPTR;
	m_GBufferSampler = NULLPTR;
	m_LightAccDirShader = NULLPTR;

}

JEONG::RenderManager::~RenderManager()
{
	ShaderManager::Delete();
	Safe_Release_Map(m_RenderStateMap);
	
	unordered_map<string, RenderTarget*>::iterator StartIter = m_RenderTargetMap.begin();
	unordered_map<string, RenderTarget*>::iterator EndIter = m_RenderTargetMap.end();

	for (; StartIter != EndIter; StartIter++)
		SAFE_DELETE(StartIter->second);

	m_RenderTargetMap.clear();

	for (int i = 0; i < RG_END; ++i)
	{
		for (int j = 0; j < m_RenderGroup[i].Size; ++j)
		{
			SAFE_RELEASE(m_RenderGroup[i].ObjectList[j]);
		}
		m_RenderGroup[i].Size = 0;
	}

	Safe_Delete_Map(m_MultiTargetMap);

	SAFE_RELEASE(m_DepthDisable);
	SAFE_RELEASE(m_GBufferSampler);
	SAFE_RELEASE(m_LightAccDirShader);
}

bool JEONG::RenderManager::Init()
{
	if (ShaderManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	AddBlendTargetDesc(TRUE);
	CreateBlendState(ALPHA_BLEND);
	//조명 합산 연산을 위해서 Blend옵션 조정.
	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	CreateBlendState(ACC_BLEND);
	CreateDepthStencilState(DEPTH_DISABLE, FALSE);

	m_DepthDisable = FindRenderState(DEPTH_DISABLE);

	Vector3	vPos;
	vPos.x = Device::Get()->GetWinSize().Width - 100.0f;

	if (CreateRenderTarget("PostEffect", DXGI_FORMAT_R8G8B8A8_UNORM, vPos, Vector3(100.0f, 100.0f, 1.0f), true) == false)
	{
		TrueAssert(true);
		return false;
	}

	// Albedo
	vPos.x = 0.0f;
	if (CreateRenderTarget("Albedo", DXGI_FORMAT_R32G32B32A32_FLOAT, Vector3::Zero, Vector3(100.0f, 100.0f, 1.0f), true) == false)
	{
		TrueAssert(true);
		return false;
	}

	vPos.x = 0.0f;
	vPos.y = 100.0f;
	if (CreateRenderTarget("Normal", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.0f, 100.0f, 1.0f), true) == false)
	{
		TrueAssert(true);
		return false;
	}

	// Depth
	vPos.x = 0.0f;
	vPos.y = 200.0f;
	if (CreateRenderTarget("Depth", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.0f, 100.10f, 1.0f), true) == false)
	{
		TrueAssert(true);
		return false;
	}

	// Material
	vPos.x = 0.0f;
	vPos.y = 300.0f;
	if (CreateRenderTarget("Material", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.0f, 100.0f, 1.0f), true) == false)
	{
		TrueAssert(true);
		return false;
	}

	AddMultiTarget("GBuffer", "Albedo");
	AddMultiTarget("GBuffer", "Normal");
	AddMultiTarget("GBuffer", "Depth");
	AddMultiTarget("GBuffer", "Material");

	// Light Dif
	vPos.x = 100.0f;
	vPos.y = 0.0f;
	if (CreateRenderTarget("LightAccDif", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.0f, 100.0f, 1.0f), true) == false)
		return false;

	// Light Spc
	vPos.x = 100.0f;
	vPos.y = 100.0f;
	if (CreateRenderTarget("LightAccSpc", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.0f, 100.0f, 1.0f), true) == false)
		return false;

	AddMultiTarget("LightAcc", "Albedo");
	AddMultiTarget("LightAcc", "Normal");

	m_GBufferSampler = ResourceManager::Get()->FindSampler(POINT_SAMPLER);
	m_LightAccDirShader = ShaderManager::Get()->FindShader(LIGHT_DIR_ACC_SHADER);

	return true;
}

void JEONG::RenderManager::AddBlendTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphaBlend, D3D11_BLEND destAlphaBlend, D3D11_BLEND_OP blendAlphaOp, UINT8 iWriteMask)
{
	if (m_CreateState == NULLPTR)
		m_CreateState = new BlendState();

	m_CreateState->AddTargetDesc(bEnable, srcBlend, destBlend,blendOp, srcAlphaBlend, destAlphaBlend, blendAlphaOp,iWriteMask);
}

bool JEONG::RenderManager::CreateDepthStencilState(const string & KeyName, BOOL bDepthEnable, D3D11_DEPTH_WRITE_MASK eMask, D3D11_COMPARISON_FUNC eDepthFunc, BOOL bStencilEnable, UINT8 iStencilReadMask, UINT8 iStencilWriteMask, D3D11_DEPTH_STENCILOP_DESC tFrontFace, D3D11_DEPTH_STENCILOP_DESC tBackFace)
{
	DepthStancilState* newState = (DepthStancilState*)FindRenderState(KeyName);

	if (newState != NULLPTR)
		return false;

	newState = new DepthStancilState();

	if (newState->CreateState(bDepthEnable, eMask, eDepthFunc, bStencilEnable, iStencilReadMask, iStencilWriteMask, tFrontFace, tBackFace) == false)
	{
		SAFE_RELEASE(newState);
		return false;
	}

	m_RenderStateMap.insert(make_pair(KeyName, newState));

	return true;
}

bool JEONG::RenderManager::CreateRenderTarget(const string & KeyName, DXGI_FORMAT TargetFormat, const Vector3 & Pos, const Vector3 & Scale, bool isDebugDraw, const Vector4 & ClearColor, DXGI_FORMAT DepthFormat)
{
	RenderTarget* newTarget = FindRenderTarget(KeyName);

	if (newTarget != NULLPTR)
		return false;

	newTarget = new RenderTarget();

	if (newTarget->CreateRenderTarget(TargetFormat, Pos, Scale, DepthFormat) == false)
	{
		SAFE_DELETE(newTarget);
		return false;
	}

	newTarget->SetClearColor(ClearColor);
	newTarget->SetDrawDebug(isDebugDraw);

	m_RenderTargetMap.insert(make_pair(KeyName, newTarget));

	return true;
}

bool JEONG::RenderManager::CreateBlendState(const string & KeyName, BOOL bAlphaCoverage, BOOL bIndependent)
{
	if (m_CreateState == NULLPTR)
		return false;

	if (m_CreateState->CreateState(bAlphaCoverage, bIndependent) == false)
	{
		SAFE_RELEASE(m_CreateState);
		return false;
	}

	m_RenderStateMap.insert(make_pair(KeyName, m_CreateState));
	m_CreateState = NULLPTR;

	return true;
}

JEONG::RenderState * JEONG::RenderManager::FindRenderState(const string & KeyName)
{
	unordered_map<string, JEONG::RenderState*>::iterator FindIter = m_RenderStateMap.find(KeyName);

	if (FindIter == m_RenderStateMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

JEONG::RenderTarget * JEONG::RenderManager::FindRenderTarget(const string & KeyName)
{
	unordered_map<string, JEONG::RenderTarget*>::iterator FindIter = m_RenderTargetMap.find(KeyName);

	if (FindIter == m_RenderTargetMap.end())
		return NULLPTR;

	return FindIter->second;
}

void JEONG::RenderManager::AddRenderObject(JEONG::GameObject * object)
{
	RENDER_GROUP group = object->GetRenderGroup();
	
	if (group == RG_LIGHT)
	{
		if (m_LightGroup.Size == m_LightGroup.Capacity)
		{
			m_LightGroup.Capacity *= 2;

			GameObject** newObject = new GameObject*[m_RenderGroup[group].Capacity];
			{
				memcpy(newObject, m_LightGroup.ObjectList, sizeof(GameObject*) * m_LightGroup.Size);
			}
			SAFE_DELETE_ARRARY(m_LightGroup.ObjectList);

			m_LightGroup.ObjectList = newObject;
		}

		m_LightGroup.ObjectList[m_LightGroup.Size] = object;
		m_LightGroup.Size++;
	}
	else
	{
		if (m_RenderGroup[group].Size == m_RenderGroup[group].Capacity)
		{
			m_RenderGroup[group].Capacity *= 2;

			GameObject** newObject = new GameObject*[m_RenderGroup[group].Capacity];
			{
				memcpy(newObject, m_RenderGroup[group].ObjectList, sizeof(GameObject*) * m_RenderGroup[group].Size);
			}
			SAFE_DELETE_ARRARY(m_RenderGroup[group].ObjectList);

			m_RenderGroup[group].ObjectList = newObject;
		}

		m_RenderGroup[group].ObjectList[m_RenderGroup[group].Size] = object;
		m_RenderGroup[group].Size++;
	}
}

void JEONG::RenderManager::Render(float DeltaTime)
{
	Render3D(DeltaTime);
}

bool JEONG::RenderManager::AddMultiTarget(const string & MultiKey, const string & TargetKey)
{
	MultiRenderTarget* newMulti = FindMultiTarget(MultiKey);

	if (newMulti == NULLPTR)
	{
		newMulti = new MultiRenderTarget();
		newMulti->DepthView = NULLPTR;

		m_MultiTargetMap.insert(make_pair(MultiKey, newMulti));
	}

	RenderTarget* getTarget = FindRenderTarget(TargetKey);

	if (getTarget == NULLPTR)
		return false;

	newMulti->vecTargetView.push_back(getTarget);

	return true;
}

bool JEONG::RenderManager::AddMultiTargetDepth(const string & MultiKey, const string & TargetKey)
{
	MultiRenderTarget* newMulti = FindMultiTarget(MultiKey);

	if (newMulti == NULLPTR)
	{
		newMulti = new MultiRenderTarget();
		newMulti->DepthView = NULLPTR;
	}

	RenderTarget* getTarget = FindRenderTarget(TargetKey);

	if (getTarget == NULLPTR)
		newMulti->DepthView = NULLPTR;
	else
		newMulti->DepthView = getTarget->GetDepthView();

	return true;
}

void JEONG::RenderManager::SetMultiTarget(const string & MultiKey)
{
	MultiRenderTarget* getMRT = FindMultiTarget(MultiKey);

	if (getMRT == NULLPTR)
		return;

	if (getMRT->vecOldTargetView.empty())
		getMRT->vecOldTargetView.resize(getMRT->vecTargetView.size());

	Device::Get()->GetContext()->OMGetRenderTargets((UINT)getMRT->vecTargetView.size(), &getMRT->vecOldTargetView[0], &getMRT->OldDepthView);

	ID3D11DepthStencilView*	pDepth = getMRT->DepthView;

	if (pDepth == NULLPTR)
		pDepth = getMRT->OldDepthView;

	vector<ID3D11RenderTargetView*> vecTarget;

	for (size_t i = 0; i < getMRT->vecTargetView.size(); i++)
		vecTarget.push_back(getMRT->vecTargetView[i]->GetRenderTargetView());

	Device::Get()->GetContext()->OMSetRenderTargets((UINT)getMRT->vecTargetView.size(), &vecTarget[0], pDepth);
}

void JEONG::RenderManager::ResetMultiTarget(const string & MultiKey)
{
	MultiRenderTarget* getMRT = FindMultiTarget(MultiKey);

	if (getMRT == NULLPTR)
		return;

	Device::Get()->GetContext()->OMSetRenderTargets((UINT)getMRT->vecOldTargetView.size(), &getMRT->vecOldTargetView[0], getMRT->OldDepthView);

	for (size_t i = 0; i < getMRT->vecOldTargetView.size(); ++i)
		SAFE_RELEASE(getMRT->vecOldTargetView[i]);
	
	SAFE_RELEASE(getMRT->OldDepthView);
}

JEONG::MultiRenderTarget * JEONG::RenderManager::FindMultiTarget(const string & MultiKey)
{
	unordered_map<string, MultiRenderTarget*>::iterator FindIter = m_MultiTargetMap.find(MultiKey);

	if (FindIter == m_MultiTargetMap.end())
		return NULLPTR;

	return FindIter->second;
}

void RenderManager::ClearMultiTarget(const string & MultiKey, float ClearColor[4])
{
	MultiRenderTarget* getMRT = FindMultiTarget(MultiKey);

	if (getMRT == NULLPTR)
		return;

	for (size_t i = 0; i < getMRT->vecTargetView.size(); ++i)
	{
		getMRT->vecTargetView[i]->SetClearColor(ClearColor);
		getMRT->vecTargetView[i]->ClearTarget();
	}

	if (getMRT->DepthView != NULLPTR)
		Device::Get()->GetContext()->ClearDepthStencilView(getMRT->DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void JEONG::RenderManager::Render2D(float DeltaTime)
{
}

void JEONG::RenderManager::Render3D(float DeltaTime)
{
	//if (m_isDeferred == false)
	//	ForwardRender(DeltaTime);
	//else
		DeferredRender(DeltaTime);

	unordered_map<string, RenderTarget*>::iterator	StartIter = m_RenderTargetMap.begin();
	unordered_map<string, RenderTarget*>::iterator	EndIter = m_RenderTargetMap.end();

	for (; StartIter != EndIter; ++StartIter)
		StartIter->second->Render(DeltaTime);
}

void JEONG::RenderManager::DeferredRender(float DeltaTime)
{
	// GBuffer를 만들어준다.
	RenderGBuffer(DeltaTime);

	// UI부터~출력
	for (int i = RG_UI; i < RG_END; ++i)
	{
		for (int j = 0; j < m_RenderGroup[i].Size; ++j)
			m_RenderGroup[i].ObjectList[j]->Render(DeltaTime);

		m_RenderGroup[i].Size = 0;
	}

	for (int i = RG_LANDSCAPE; i < RG_END; ++i)
		m_RenderGroup[i].Size = 0;

	m_LightGroup.Size = 0;
}

void JEONG::RenderManager::RenderGBuffer(float DeltaTime)
{
	// GBuffer MRT로 타겟을 교체한다.
	float ClearColor[4] = {};
	ClearMultiTarget("GBuffer", ClearColor);
	SetMultiTarget("GBuffer");

	//RenderTarget* getTarget = FindRenderTarget("Albedo");

	for (int i = RG_LANDSCAPE; i <= RG_NORMAL; ++i)
	{
		for (int j = 0; j < m_RenderGroup[i].Size; ++j)
			m_RenderGroup[i].ObjectList[j]->Render(DeltaTime);
	}
	  
	ResetMultiTarget("GBuffer");

	//getTarget->RenderFullScreen();
}

void RenderManager::RenderLightAcc(float DeltaTime)
{
	float	fClearColor[4] = {};
	ClearMultiTarget("LightAcc", fClearColor);
	SetMultiTarget("LightAcc");

	m_LightAccDirShader->SetShader();

	RenderState* pAccBlend = FindRenderState(ACC_BLEND);

	pAccBlend->SetState();
	m_DepthDisable->SetState();

	m_GBufferSampler->SetSamplerState(10);

	// GBuffer를 얻어온다.
	MultiRenderTarget*	getGBuffer = FindMultiTarget("GBuffer");

	getGBuffer->vecTargetView[1]->SetShader(11);
	getGBuffer->vecTargetView[2]->SetShader(12);

	for (int i = 0; i < m_LightGroup.Size; ++i)
	{
		Light_Com* getLight = m_LightGroup.ObjectList[i]->FindComponentFromType<Light_Com>(CT_LIGHT);

		switch (getLight->GetLightType())
		{
		case LT_DIRECTION:
			RenderLightDirection(DeltaTime, getLight);
			break;
		case LT_POINT:
			RenderLightPoint(DeltaTime, getLight);
			break;
		case LT_SPOT:
			RenderLightSpot(DeltaTime, getLight);
			break;
		}

		SAFE_RELEASE(getLight);
	}

	getGBuffer->vecTargetView[1]->ResetShader(11);
	getGBuffer->vecTargetView[2]->ResetShader(12);

	m_DepthDisable->ResetState();
	pAccBlend->ResetState();
	ResetMultiTarget("LightAcc");

	SAFE_RELEASE(pAccBlend);
}

void RenderManager::RenderLightDirection(float DeltaTime, Light_Com * Light)
{
	Light->UpdateCBuffer();

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	Device::Get()->GetContext()->IASetInputLayout(NULLPTR);

	UINT iOffset = 0;
	Device::Get()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Device::Get()->GetContext()->IASetVertexBuffers(0, 0, NULLPTR, 0, &iOffset);
	Device::Get()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	Device::Get()->GetContext()->Draw(4, 0);
}

void RenderManager::RenderLightPoint(float DeltaTime, Light_Com * Light)
{
}

void RenderManager::RenderLightSpot(float DeltaTime, Light_Com * Light)
{
}

void RenderManager::LightBlend(float DeltaTime)
{
}

void JEONG::RenderManager::ForwardRender(float DeltaTime)
{
	RenderTarget* getTarget = FindRenderTarget("PostEffect");
	getTarget->ClearTarget();
	getTarget->SetTarget();
	{
		if (m_LightGroup.Size > 0)
		{
			Light_Com* getCom = m_LightGroup.ObjectList[0]->FindComponentFromType<Light_Com>(CT_LIGHT);
			getCom->UpdateCBuffer();

			SAFE_RELEASE(getCom);
		}

		for (int i = 0; i <= RG_ALPHA3; ++i)
		{
			for (int j = 0; j < m_RenderGroup[i].Size; ++j)
				m_RenderGroup[i].ObjectList[j]->Render(DeltaTime);

			m_RenderGroup[i].Size = 0;
		}
	}
	getTarget->ResetTarget();

	getTarget->RenderFullScreen();

	// UI부터~출력
	for (int i = RG_UI; i < RG_END; ++i)
	{
		for (int j = 0; j < m_RenderGroup[i].Size; ++j)
			m_RenderGroup[i].ObjectList[j]->Render(DeltaTime);

		m_RenderGroup[i].Size = 0;
	}

	m_LightGroup.Size = 0;
}
