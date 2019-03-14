#include "../stdafx.h"
#include "SkyBox_Com.h"
#include "Renderer_Com.h"
#include "Transform_Com.h"
#include "Material_Com.h"

#include "Render/ShaderManager.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

JEONG_USING

SkyBox_Com::SkyBox_Com()
{
	m_CBuffer = {};
	m_ComType = CT_SKY;
}

SkyBox_Com::SkyBox_Com(const SkyBox_Com & CopyData)
	:Component_Base(CopyData)
{
}	

SkyBox_Com::~SkyBox_Com()
{
}

bool SkyBox_Com::Init()
{
	/*
	실제 게임에선 구를 크게 그리지않는다.
	구가 크다면 맵이 크다고 가정할때 정점갯수가 많이 들어가야 하늘이 각지게 보이지않기때문이다.
	그러면 카메라를 따라다니는 작은 구 하나를 만들고 각종 쉐이더 기법을 사용하여 하늘처럼 보이게 한다.
	*/

	/*
	물론 큐브텍스쳐가 있다면 구를 크게그려서 큐브텍스쳐를 샘플링하여 
	구현하는 것도 쉽게 가능하다.
	*/

	m_Object->SetRenderGroup(RG_SKY);
	m_Transform->SetWorldScale(1.0f, 1.0f, 1.0f);

	Renderer_Com* newRenderer = m_Object->AddComponent<Renderer_Com>("SkyRender");
	newRenderer->InitMaterial();
	newRenderer->SetMesh(SPHERE_VOLUM);
	newRenderer->SetShader(SKY_BOX_SHADER);
	newRenderer->SetRenderState(CULL_NONE);
	newRenderer->SetRenderState(DEPTH_DISABLE);
	//newRenderer->SetRenderState(FRONT_COUNT_WISE);

	m_Object->FindComponentFromTypeNoneCount<Material_Com>(CT_MATERIAL)->SetMaterial(Vector4::White, Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector4::White, 3.2f, Vector4::White);
	SAFE_RELEASE(newRenderer);

	return true;
}

int SkyBox_Com::Input(float DeltaTime)
{
	return 0;
}

int SkyBox_Com::Update(float DeltaTime)
{
	//카메라를 따라다닌다.
	m_Transform->SetWorldPos(SceneManager::Get()->GetCurScene()->GetMainCameraTransform()->GetWorldPos());
	return 0;
}

int SkyBox_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void SkyBox_Com::Collision(float DeltaTime)
{
}

void SkyBox_Com::CollisionLateUpdate(float DeltaTime)
{
}

void SkyBox_Com::Render(float DeltaTime)
{
	ShaderManager::Get()->UpdateCBuffer("SkyCBuffer", &m_CBuffer);
}

SkyBox_Com * SkyBox_Com::Clone()
{
	return new SkyBox_Com(*this);
}

void SkyBox_Com::AfterClone()
{
}
