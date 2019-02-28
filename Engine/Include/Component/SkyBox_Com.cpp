#include "../stdafx.h"
#include "SkyBox_Com.h"
#include "Renderer_Com.h"
#include "Transform_Com.h"
#include "Material_Com.h"

JEONG_USING

SkyBox_Com::SkyBox_Com()
{
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
	//m_Transform->SetWorldScale(10000000.0f, 10000000.0f, 10000000.0f);
	//m_Transform->Update(0.0f);

	//Renderer_Com* newRenderer = m_Object->AddComponent<Renderer_Com>("Render");
	//newRenderer->InitMaterial();
	//newRenderer->SetMesh(SPHERE_VOLUM);
	//newRenderer->SetRenderState(DEPTH_LESS_EQUAL);
	//newRenderer->SetRenderState(CULL_NONE);

	//Material_Com* newMat = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	//newMat->SetDiffuseTexture(10, "SkyDefault", TEXT("Sky.dds"));
	//newMat->SetDiffuseSampler(10, LINER_SAMPLER);	

	//SAFE_RELEASE(newRenderer);
	//SAFE_RELEASE(newMat);

	return true;
}

int SkyBox_Com::Input(float DeltaTime)
{
	return 0;
}

int SkyBox_Com::Update(float DeltaTime)
{
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
}

SkyBox_Com * SkyBox_Com::Clone()
{
	return new SkyBox_Com(*this);
}

void SkyBox_Com::AfterClone()
{
}
