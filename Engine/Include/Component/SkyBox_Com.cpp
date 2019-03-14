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
	���� ���ӿ��� ���� ũ�� �׸����ʴ´�.
	���� ũ�ٸ� ���� ũ�ٰ� �����Ҷ� ���������� ���� ���� �ϴ��� ������ �������ʱ⶧���̴�.
	�׷��� ī�޶� ����ٴϴ� ���� �� �ϳ��� ����� ���� ���̴� ����� ����Ͽ� �ϴ�ó�� ���̰� �Ѵ�.
	*/

	/*
	���� ť���ؽ��İ� �ִٸ� ���� ũ�Ա׷��� ť���ؽ��ĸ� ���ø��Ͽ� 
	�����ϴ� �͵� ���� �����ϴ�.
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
	//ī�޶� ����ٴѴ�.
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
