#include "stdafx.h"
#include "RenderManager.h"
#include "RenderTarget.h"
#include "BlendState.h"
#include "MultiRenderTarget.h"
#include "DepthStancilState.h"
#include "ResterizerState.h"

#include "Render/Shader.h"
#include "Resource/ResourceManager.h"

#include "../GameObject.h"
#include "../Device.h"
#include "../Render/ShaderManager.h"
#include "../Component/Light_Com.h"
#include "../Component/Camera_Com.h"
#include "../Component/Transform_Com.h"
#include "../Resource/Sampler.h"
#include "../Resource/Mesh.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"


JEONG_USING
SINGLETON_VAR_INIT(JEONG::RenderManager)

RenderManager::RenderManager()
{
	m_GameMode = GM_3D;
	m_isDeferred = true;
	m_isWireFrame = true;
	m_CreateState = NULLPTR;
	m_DepthDisable = NULLPTR;
	m_GBufferSampler = NULLPTR;
	m_LightAccDirShader = NULLPTR;
	m_GBufferMultiTarget = NULLPTR;
	m_LightMultiTarget = NULLPTR;
	m_AddBlend = NULLPTR;
	m_LightAccPointShader = NULLPTR;
	m_LightAccSpotShader = NULLPTR;
	m_LightAccBlendShader = NULLPTR;
	m_FullScreenShader = NULLPTR;
	m_LightBlendTarget = NULLPTR;
	m_LightDiffuseTarget = NULLPTR;
	m_LightSpcularTarget = NULLPTR;
	m_AlbedoTarget = NULLPTR;
	m_SphereVolum = NULLPTR;
	m_CornVolum = NULLPTR;

	m_DepthGrator = NULLPTR;
	m_DepthLess = NULLPTR;
	m_FrontCull = NULLPTR;
	m_BackCull = NULLPTR;
	m_WireFrame = NULLPTR;
	m_CullNone = NULLPTR;
	m_ZeroBlend = NULLPTR;
	m_AllBlend = NULLPTR;

	m_CBuffer = {};
}

RenderManager::~RenderManager()
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
	SAFE_RELEASE(m_AddBlend);

	SAFE_RELEASE(m_LightAccPointShader);
	SAFE_RELEASE(m_LightAccBlendShader);
	SAFE_RELEASE(m_FullScreenShader);

	SAFE_RELEASE(m_DepthDisable);
	SAFE_RELEASE(m_DepthGrator);
	SAFE_RELEASE(m_DepthLess);
	SAFE_RELEASE(m_FrontCull);
	SAFE_RELEASE(m_BackCull);
	SAFE_RELEASE(m_WireFrame);
	SAFE_RELEASE(m_ZeroBlend);
	SAFE_RELEASE(m_CullNone);
	SAFE_RELEASE(m_AllBlend);

	SAFE_RELEASE(m_SphereVolum);
	SAFE_RELEASE(m_CornVolum);
}

bool RenderManager::Init()
{
	if (ShaderManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	D3D11_DEPTH_STENCILOP_DESC First = {};
	First.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	First.StencilFunc = D3D11_COMPARISON_NEVER;

	D3D11_DEPTH_STENCILOP_DESC Second = {};
	Second.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilFunc = D3D11_COMPARISON_ALWAYS; //무조건통과옵션

	//뒷면만 통과하겠다.
	CreateDepthStencilState(DEPTH_GRATOR, TRUE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_GREATER, TRUE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_READ_MASK, First, Second);

	First.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	First.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	First.StencilFunc = D3D11_COMPARISON_EQUAL; //같을때만

	Second.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	Second.StencilFunc = D3D11_COMPARISON_NEVER; //통과 X

	//앞면만 통과시키겠다.
	CreateDepthStencilState(DEPTH_LESS, TRUE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS, TRUE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_READ_MASK, First, Second);
	CreateDepthStencilState(DEPTH_LESS_EQUAL,TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);
	CreateDepthStencilState(DEPTH_DISABLE, FALSE);

	m_CBuffer.ViewPortSize = Vector2((float)Device::Get()->GetWinSize().Width, (float)Device::Get()->GetWinSize().Height);
	m_CBuffer.isDeferred = m_isDeferred;
	m_CBuffer.isWireMode = m_isWireFrame;

	AddBlendTargetDesc(TRUE);
	CreateBlendState(ALPHA_BLEND);
	//조명 합산 연산을 위해서 Blend옵션 조정.
	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE);
	CreateBlendState(ACC_BLEND);

	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, 0);
	CreateBlendState(ZERO_BLEND);

	AddBlendTargetDesc(TRUE, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL);
	CreateBlendState(ALL_BLEND);

	CreateResterizerState(FRONT_CULL, D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	CreateResterizerState(BACK_CULL, D3D11_FILL_SOLID, D3D11_CULL_BACK);
	CreateResterizerState(CULL_NONE, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	CreateResterizerState(WIRE_FRAME, D3D11_FILL_WIREFRAME);
	CreateResterizerState(FRONT_COUNT_WISE, D3D11_FILL_SOLID, D3D11_CULL_BACK, TRUE);

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

	CreateMultiTarget("GBuffer");
	AddMultiRenderTarget("GBuffer", "Albedo");
	AddMultiRenderTarget("GBuffer", "Normal");
	AddMultiRenderTarget("GBuffer", "Depth");
	AddMultiRenderTarget("GBuffer", "Material");

	// Light Dif
	vPos.x = 100.0f;
	vPos.y = 0.0f;
	if (CreateRenderTarget("LightAccDiffuse", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.0f, 100.0f, 1.0f), true) == false)
		return false;

	// Light Spc
	vPos.x = 100.0f;
	vPos.y = 100.0f;
	if (CreateRenderTarget("LightAccSpcular", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.0f, 100.0f, 1.0f), true) == false)
		return false;

	// Light Spc
	vPos.x = 200.0f;
	vPos.y = 0.0f;
	if (CreateRenderTarget("LightBlend", DXGI_FORMAT_R32G32B32A32_FLOAT, vPos, Vector3(100.0f, 100.0f, 1.0f), true) == false)
		return false;

	CreateMultiTarget("LightAcc");
	AddMultiRenderTarget("LightAcc", "LightAccDiffuse");
	AddMultiRenderTarget("LightAcc", "LightAccSpcular");

	m_GBufferSampler = ResourceManager::Get()->FindSamplerNoneCount(POINT_SAMPLER);
	m_LightAccDirShader = ShaderManager::Get()->FindShaderNoneCount(LIGHT_DIR_ACC_SHADER);

	m_AddBlend = FindRenderStateNoneCount(ACC_BLEND);
	m_DepthGrator = FindRenderStateNoneCount(DEPTH_GRATOR);
	m_DepthLess = FindRenderStateNoneCount(DEPTH_LESS);
	m_FrontCull = FindRenderStateNoneCount(FRONT_CULL);
	((DepthStancilState*)m_DepthGrator)->SetStencilRef(1); //스텐실값을 1로 채운다.
	m_BackCull = FindRenderStateNoneCount(BACK_CULL);
	m_WireFrame = FindRenderStateNoneCount(WIRE_FRAME);
	m_CullNone = FindRenderStateNoneCount(CULL_NONE);
	m_ZeroBlend = FindRenderStateNoneCount(ZERO_BLEND);
	m_AllBlend = FindRenderStateNoneCount(ALL_BLEND);
	m_DepthDisable = FindRenderStateNoneCount(DEPTH_DISABLE);

	m_GBufferMultiTarget = FindMultiTarget("GBuffer");
	m_LightMultiTarget = FindMultiTarget("LightAcc");

	m_LightAccDirShader = ShaderManager::Get()->FindShaderNoneCount(LIGHT_DIR_ACC_SHADER);
	m_LightAccPointShader = ShaderManager::Get()->FindShaderNoneCount(LIGHT_POINT_ACC_SHADER);
	m_LightAccBlendShader = ShaderManager::Get()->FindShaderNoneCount(LIGHT_BLEND_SHADER);
	m_LightAccSpotShader = ShaderManager::Get()->FindShaderNoneCount(LIGHT_SPOT_SHADER);
	m_FullScreenShader = ShaderManager::Get()->FindShaderNoneCount(FULLSCREEN_SHADER);

	m_LightDiffuseTarget = FindRenderTarget("LightAccDiffuse");
	m_LightSpcularTarget = FindRenderTarget("LightAccSpcular");
	m_AlbedoTarget = FindRenderTarget("Albedo");
	m_LightBlendTarget = FindRenderTarget("LightBlend");

	m_SphereVolum = ResourceManager::Get()->FindMeshNoneCount(SPHERE_VOLUM);
	m_CornVolum = ResourceManager::Get()->FindMeshNoneCount(CORN_VOLUM);

	return true;
}

void RenderManager::AddBlendTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphaBlend, D3D11_BLEND destAlphaBlend, D3D11_BLEND_OP blendAlphaOp, UINT8 iWriteMask)
{
	if (m_CreateState == NULLPTR)
		m_CreateState = new BlendState();

	m_CreateState->AddTargetDesc(bEnable, srcBlend, destBlend,blendOp, srcAlphaBlend, destAlphaBlend, blendAlphaOp,iWriteMask);
}

bool RenderManager::CreateDepthStencilState(const string & KeyName, BOOL bDepthEnable, D3D11_DEPTH_WRITE_MASK eMask, D3D11_COMPARISON_FUNC eDepthFunc, BOOL bStencilEnable, UINT8 iStencilReadMask, UINT8 iStencilWriteMask, D3D11_DEPTH_STENCILOP_DESC tFrontFace, D3D11_DEPTH_STENCILOP_DESC tBackFace)
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

bool RenderManager::CreateRenderTarget(const string & KeyName, DXGI_FORMAT TargetFormat, const Vector3 & Pos, const Vector3 & Scale, bool isDebugDraw, const Vector4 & ClearColor, DXGI_FORMAT DepthFormat)
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

bool RenderManager::CreateBlendState(const string & KeyName, BOOL bAlphaCoverage, BOOL bIndependent)
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

bool RenderManager::CreateResterizerState(const string & KeyName, D3D11_FILL_MODE eFill, D3D11_CULL_MODE eCull, BOOL bFrontCounterClockwise, int iDepthBias, float fDepthBiasClamp, float fSlopeScaledDepthBias, BOOL bDepthClipEnable, BOOL bScissorEnable, BOOL bMultisampleEnable, BOOL bAntialiasedLineEnable)
{
	ResterizerState* newState = (ResterizerState*)FindRenderState(KeyName);

	if (newState != NULLPTR)
		return false;

	newState = new ResterizerState();

	if (newState->CreateState(eFill, eCull, bFrontCounterClockwise, iDepthBias, fDepthBiasClamp, fSlopeScaledDepthBias, bDepthClipEnable, bScissorEnable, bMultisampleEnable, bAntialiasedLineEnable) == false)
	{
		SAFE_RELEASE(newState);
		return false;
	}

	m_RenderStateMap.insert(make_pair(KeyName, newState));

	return newState;
}

RenderState * RenderManager::FindRenderState(const string & KeyName)
{
	unordered_map<string, RenderState*>::iterator FindIter = m_RenderStateMap.find(KeyName);

	if (FindIter == m_RenderStateMap.end())
		return NULLPTR;

	FindIter->second->AddRefCount();

	return FindIter->second;
}

RenderState * RenderManager::FindRenderStateNoneCount(const string & KeyName)
{
	unordered_map<string, RenderState*>::iterator FindIter = m_RenderStateMap.find(KeyName);

	if (FindIter == m_RenderStateMap.end())
		return NULLPTR;

	return FindIter->second;
}

RenderTarget * RenderManager::FindRenderTarget(const string & KeyName)
{
	unordered_map<string, RenderTarget*>::iterator FindIter = m_RenderTargetMap.find(KeyName);

	if (FindIter == m_RenderTargetMap.end())
		return NULLPTR;

	return FindIter->second;
}

void RenderManager::AddRenderObject(GameObject * object)
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

void RenderManager::Render(float DeltaTime)
{
	m_CBuffer.isWireMode = m_isWireFrame;
	ShaderManager::Get()->UpdateCBuffer("PublicCBuffer", &m_CBuffer);
	Render3D(DeltaTime);

	m_CBuffer.DeltaTime = DeltaTime;
	m_CBuffer.PlusedDeltaTime += DeltaTime;

	//TODO : Far값 임의로 10
	m_CBuffer.Far = 2.0f;
}

bool RenderManager::CreateMultiTarget(const string & MultiKey)
{
	MultiRenderTarget* newTarget = FindMultiTarget(MultiKey);

	if (newTarget != NULLPTR)
		return false;

	newTarget = new MultiRenderTarget();

	m_MultiTargetMap.insert(make_pair(MultiKey, newTarget));
	return true;
}

bool RenderManager::AddMultiRenderTarget(const string & MultiKey, const string & TargetKey)
{
	MultiRenderTarget* getMulti = FindMultiTarget(MultiKey);

	if (getMulti == NULLPTR)
		return false;

	getMulti->AddRenderTargetView(TargetKey);
	return true;
}

bool RenderManager::AddMultiRenderTargetDepthView(const string & MultiKey, const string & TargetKey)
{
	MultiRenderTarget* getMulti = FindMultiTarget(MultiKey);

	if (getMulti == NULLPTR)
		return false;

	getMulti->AddDepthView(TargetKey);
	return true;
}

MultiRenderTarget * RenderManager::FindMultiTarget(const string & MultiKey)
{
	unordered_map<string, MultiRenderTarget*>::iterator FindIter = m_MultiTargetMap.find(MultiKey);

	if (FindIter == m_MultiTargetMap.end())
		return NULLPTR;

	return FindIter->second;
}

void RenderManager::Render3D(float DeltaTime)
{
#ifdef GUI_USING
	//ImGui::Checkbox("WireFrame", &m_isWireFrame);
#endif 

	if (m_isDeferred == false)
		ForwardRender(DeltaTime);
	else
		DeferredRender(DeltaTime);

	unordered_map<string, RenderTarget*>::iterator StartIter = m_RenderTargetMap.begin();
	unordered_map<string, RenderTarget*>::iterator EndIter = m_RenderTargetMap.end();
	 
	//사각형 출력하기만 함
	for (; StartIter != EndIter; ++StartIter)
		StartIter->second->Render(DeltaTime);
}

void RenderManager::DeferredRender(float DeltaTime)
{
	// GBuffer를 만들어준다.
	RenderGBuffer(DeltaTime);
	RenderLightAcc(DeltaTime);
	RenderLightBlend(DeltaTime);
	RenderFullScreen(DeltaTime);

	//UI부터~출력
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

void RenderManager::RenderGBuffer(float DeltaTime)
{
	// GBuffer MRT로 타겟을 교체한다.
	float ClearColor[4] = {Vector4::RoyalBlue.r,Vector4::RoyalBlue.g ,Vector4::RoyalBlue.b ,Vector4::RoyalBlue.a};
	m_GBufferMultiTarget->ClearRenderTarget(ClearColor);
	m_GBufferMultiTarget->SetTarget();

	for (int i = RG_LANDSCAPE; i <= RG_NORMAL; ++i)
	{
		for (int j = 0; j < m_RenderGroup[i].Size; ++j)
			m_RenderGroup[i].ObjectList[j]->Render(DeltaTime);
	}
	  
	m_GBufferMultiTarget->ResetTarget();
}

void RenderManager::RenderLightAcc(float DeltaTime)
{
	float ClearColor[4] = { 0 };

	m_LightMultiTarget->ClearRenderTarget(ClearColor);
	m_LightMultiTarget->SetTarget();
	m_GBufferSampler->SetSamplerState(10);
	m_AddBlend->SetState();
	m_DepthDisable->SetState();
	//GBuffer타겟에서 뽑아져나온 픽셀데이터들을 10번에 셋팅하겠다.
	//LightGBuffer 10번에 쓰겠다고 할때 경고 나온이유
	//Light타겟에선 뽑아져나온 데이터가 없고
	//데이터쓰고있는데 갑자기 10번에 셋팅하겠다고
	//했기때문에 읽기와 쓰기를 동시에 할 수 없도록 셋팅했으므로 경고발생.
	m_GBufferMultiTarget->SetShaderResource(10);
	{
		for (size_t i = 0; i < m_LightGroup.Size; i++)
		{
			Light_Com* getLight = m_LightGroup.ObjectList[i]->FindComponentFromTypeNoneCount<Light_Com>(CT_LIGHT);
			
			switch (getLight->GetLightType())
			{
				case LT_DIRECTION:
					RenderDirectionLight(DeltaTime, getLight);
					break;
				case LT_POINT:
					RenderPointLight(DeltaTime, getLight);
					break;
				case LT_SPOT:
					RenderSpotLight(DeltaTime, getLight);
					break;
			}
		}
	}
	m_GBufferMultiTarget->ResetShaderResource(10);
	m_LightMultiTarget->ResetTarget();
	m_DepthDisable->ResetState();
	m_AddBlend->ResetState();
}

void RenderManager::RenderDirectionLight(float DeltaTime, Light_Com * light)
{
	m_LightAccDirShader->SetShader();

	// 조명 정보를 상수버퍼에 넘겨준다.
	light->UpdateCBuffer();

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	Device::Get()->GetContext()->IASetInputLayout(NULLPTR);

	UINT iOffset = 0;
	Device::Get()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Device::Get()->GetContext()->IASetVertexBuffers(0, 0, NULLPTR, 0, &iOffset);
	Device::Get()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	Device::Get()->GetContext()->Draw(4, 0);
}

void RenderManager::RenderPointLight(float DeltaTime, Light_Com * light)
{
	m_LightAccPointShader->SetShader();
	
	light->UpdateCBuffer();

	Scene* pScene = SceneManager::Get()->GetCurSceneNoneCount();
	Camera_Com* getCamera = NULLPTR;

	Matrix	matScale, matPos;
	matScale.Scaling(light->GetLightInfo().Range, light->GetLightInfo().Range, light->GetLightInfo().Range);
	matPos.Translation(light->GetLightInfo().Pos);

	getCamera = pScene->GetMainCamera();

	TransformCBuffer cBuffer = {};
	cBuffer.World = matScale * matPos;
	cBuffer.View = getCamera->GetViewMatrix();
	cBuffer.Projection = getCamera->GetProjection();

	cBuffer.WV = cBuffer.World * cBuffer.View;
	cBuffer.WVP = cBuffer.WV * cBuffer.Projection;

	cBuffer.World.Transpose();
	cBuffer.View.Transpose();
	cBuffer.Projection.Transpose();
	cBuffer.WV.Transpose();
	cBuffer.WVP.Transpose();

	ShaderManager::Get()->UpdateCBuffer("Transform", &cBuffer);

	m_FrontCull->SetState();
	{
		m_DepthGrator->SetState();
		{
			m_AllBlend->SetState();
			{
				m_SphereVolum->Render();
			}
			m_AllBlend->ResetState();
		}
		m_DepthGrator->ResetState();
	}
	m_FrontCull->ResetState();
	
	m_BackCull->SetState();
	{
		m_ZeroBlend->SetState();
		{
			m_DepthLess->SetState();
			{
				m_SphereVolum->Render();
			}
			m_DepthLess->ResetState();
		}
		m_ZeroBlend->ResetState();
	}
	m_BackCull->ResetState();

	if (m_isWireFrame == false)
		return;

	m_WireFrame->SetState();
	{
		m_SphereVolum->Render();
	}
	m_WireFrame->ResetState();
}

void RenderManager::RenderSpotLight(float DeltaTime, Light_Com * light)
{
	light->UpdateCBuffer();

	m_LightAccSpotShader->SetShader();

	Scene* pScene = SceneManager::Get()->GetCurSceneNoneCount();
	Camera_Com* getCamera = NULLPTR;

	Matrix matScale, matPos, matRot, matLocal;
	matScale.Scaling(light->GetLightInfo().Range, light->GetLightInfo().Range, light->GetLightInfo().Range);
	matPos.Translation(light->GetLightInfo().Pos);
	matRot.Rotation(light->GetTransform()->GetWorldRotation());
	matLocal = light->GetTransform()->GetLocalMatrix();

	getCamera = pScene->GetMainCamera();

	TransformCBuffer cBuffer = {};
	cBuffer.World = matLocal * matScale * matRot * matPos;
	cBuffer.View = getCamera->GetViewMatrix();
	cBuffer.Projection = getCamera->GetProjection();

	cBuffer.WV = cBuffer.World * cBuffer.View;
	cBuffer.WVP = cBuffer.WV * cBuffer.Projection;

	cBuffer.World.Transpose();
	cBuffer.View.Transpose();
	cBuffer.Projection.Transpose();
	cBuffer.WV.Transpose();
	cBuffer.WVP.Transpose();

	ShaderManager::Get()->UpdateCBuffer("Transform", &cBuffer);

	m_FrontCull->SetState();
	{
		m_DepthGrator->SetState();
		{
			m_AllBlend->SetState();
			{
				m_CornVolum->Render();
			}
			m_AllBlend->ResetState();
		}
		m_DepthGrator->ResetState();
	}
	m_FrontCull->ResetState();

	m_BackCull->SetState();
	{
		m_DepthLess->SetState();
		{
			m_ZeroBlend->SetState();
			{
				m_CornVolum->Render();
			}
			m_ZeroBlend->ResetState();
		}
		m_DepthLess->ResetState();
	}
	m_BackCull->ResetState();

	if (m_isWireFrame == false)
		return;

	m_WireFrame->SetState();
	{
		m_CornVolum->Render();
	}
	m_WireFrame->ResetState();
}

void RenderManager::RenderFullScreen(float DeltaTime)
{
	m_DepthDisable->SetState();
	m_LightBlendTarget->SetShader(0);
	m_FullScreenShader->SetShader();
	m_GBufferSampler->SetSamplerState(0);

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	Device::Get()->GetContext()->IASetInputLayout(NULLPTR);

	UINT iOffset = 0;
	Device::Get()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Device::Get()->GetContext()->IASetVertexBuffers(0, 0, NULLPTR, 0, &iOffset);
	Device::Get()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	Device::Get()->GetContext()->Draw(4, 0);

	m_DepthDisable->ResetState();
	m_LightBlendTarget->ResetShader(0);
}

void RenderManager::RenderLightBlend(float DeltaTime)
{
	m_LightBlendTarget->ClearTarget();
	m_LightBlendTarget->SetTarget();

	m_DepthDisable->SetState();
	m_GBufferSampler->SetSamplerState(10);

	m_AlbedoTarget->SetShader(10);
	m_LightDiffuseTarget->SetShader(14);
	m_LightSpcularTarget->SetShader(15);

	m_LightAccBlendShader->SetShader();

	// NULL Buffer로 전체 화면크기의 사각형을 출력한다.
	Device::Get()->GetContext()->IASetInputLayout(NULLPTR);

	UINT iOffset = 0;
	Device::Get()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Device::Get()->GetContext()->IASetVertexBuffers(0, 0, NULLPTR, 0, &iOffset);
	Device::Get()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	Device::Get()->GetContext()->Draw(4, 0);

	m_AlbedoTarget->ResetShader(10);
	m_LightDiffuseTarget->ResetShader(14);
	m_LightSpcularTarget->ResetShader(15);

	m_DepthDisable->ResetState();
	m_LightBlendTarget->ResetTarget();
}

void RenderManager::ForwardRender(float DeltaTime)
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
