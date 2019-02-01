#include "stdafx.h"
#include "Light_Com.h"
#include "Transform_Com.h"
#include "Renderer_Com.h"
#include "Camera_Com.h"

#include "../Render/ShaderManager.h"
#include "../Render/DepthStancilState.h"
#include "../Render/ResterizerState.h"
#include "../Render/RenderManager.h"
#include "../Render/BlendState.h"
#include "../Render/Shader.h"

#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

JEONG_USING

Light_Com::Light_Com()
{
	m_tLightInfo.LightType = LT_DIRECTION;
	m_tLightInfo.Diffuse = Vector4::White;
	m_tLightInfo.Spcular = Vector4::White;
	m_tLightInfo.Ambient = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
	m_tLightInfo.Direction = Vector3(0.0f, 0.0f, 1.0f);
	m_tLightInfo.Range = 0.0f;
	m_tLightInfo.InAngle = 0.0f;
	m_tLightInfo.OutAngle = 0.0f;
	m_tLightInfo.Attenuation = Vector3::Zero;
	m_tLightInfo.FallOff = 0.0f;
	m_tLightInfo.Pos = Vector3::Zero;

	//m_SphereVolum = NULLPTR;
	//m_CornVolum = NULLPTR;
	//m_VolumeObject = NULLPTR;
	//m_LightDir = NULLPTR;
	//m_LightPointDir = NULLPTR;
	//m_WireFrameState = NULLPTR;

	//m_FrontCullState = NULLPTR;
	//m_DepthGreater = NULLPTR;

	m_ComType = CT_LIGHT;

}

Light_Com::Light_Com(const Light_Com & CopyData)
	:Component_Base(CopyData)
{
	m_tLightInfo = CopyData.m_tLightInfo;
}

Light_Com::~Light_Com()
{
	//SAFE_RELEASE(m_VolumeObject);
	//SAFE_RELEASE(m_DepthGreater);
	//SAFE_RELEASE(m_FrontCullState);
	//SAFE_RELEASE(m_WireFrameState);
}

bool Light_Com::Init()
{
	//m_Transform->SetWorldScale(Vector3(3.0f, 3.0f, 3.0f));

	//m_SphereVolum = ResourceManager::Get()->FindMeshNoneCount(SPHERE_VOLUM);
	//m_CornVolum = ResourceManager::Get()->FindMeshNoneCount(CORN_VOLUM);

	//m_VolumeObject = GameObject::CreateObject("VolumeMesh");
	//m_VolumeObject->GetTransform()->SetWorldScale(Vector3(1.0f, 1.0f, 1.0f));
	//m_VolumeObject->SetScene(m_Scene);

	//m_FrontCullState = (ResterizerState*)RenderManager::Get()->FindRenderState(FRONT_CULL);
	//m_DepthGreater = (DepthStancilState*)RenderManager::Get()->FindRenderState(DEPTH_GRATER);
	//m_WireFrameState = (ResterizerState*)RenderManager::Get()->FindRenderState(WIRE_FRAME);

	//m_LightDir = ShaderManager::Get()->FindShaderNoneCount(LIGHT_DIR_ACC_SHADER);
	//m_LightPointDir = ShaderManager::Get()->FindShaderNoneCount(LIGHT_POINT_ACC_SHADER);
	//
	return true;
}

int Light_Com::Input(float DeltaTime)
{
	return 0;
}

int Light_Com::Update(float DeltaTime)
{
	return 0;
}

int Light_Com::LateUpdate(float DeltaTime)
{
	//if (m_tLightInfo.LightType != LT_POINT)
	//	m_tLightInfo.Direction = m_Transform->GetWorldAxis(AXIS_Z);

	if (m_tLightInfo.LightType != LT_DIRECTION)
		m_Transform->SetWorldPos(m_tLightInfo.Pos);

	//if (m_tLightInfo.LightType != LT_DIRECTION)
	//{
	//	m_Transform->SetWorldPos(m_tLightInfo.Pos);
	//	m_VolumeObject->GetTransform()->SetWorldPos(m_tLightInfo.Pos);
	//}

	//if(m_tLightInfo.LightType != LT_POINT)
	//	m_VolumeObject->GetTransform()->LookAt(m_tLightInfo.Direction);

	//m_VolumeObject->GetTransform()->SetWorldScale(m_Transform->GetWorldScale());
	//m_VolumeObject->Update(DeltaTime);

	return 0;
}

void Light_Com::Collision(float DeltaTime)
{
}

void Light_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Light_Com::Render(float DeltaTime)
{
	//m_FrontCullState->SetState();
	//m_DepthGreater->SetState();

	//switch (m_tLightInfo.LightType)
	//{
	//case LT_POINT:
	//	m_LightPointDir->SetShader();
	//	m_SphereVolum->Render();
	//	break;

	//case LT_SPOT:
	//case LT_SPOT_BOMI:
	//	m_LightPointDir->SetShader();
	//	m_CornVolum->Render();
	//	break;
	//}
	//m_DepthGreater->ResetState();
	//m_FrontCullState->ResetState();
}

Light_Com * Light_Com::Clone()
{
	return new Light_Com(*this);
}

void Light_Com::AfterClone()
{
}

void Light_Com::UpdateCBuffer()
{
	//TransformCBuffer cBuffer = {};
	//Camera_Com* getCamera = NULLPTR;

	//getCamera = m_Scene->GetMainCamera();

	//cBuffer.World = m_VolumeObject->GetTransform()->GetWorldMatrix();
	//cBuffer.View = getCamera->GetViewMatrix();
	//cBuffer.Projection = getCamera->GetProjection();

	//cBuffer.WV = cBuffer.World * cBuffer.View;
	//cBuffer.WVP = cBuffer.WV * cBuffer.Projection;

	//cBuffer.World.Transpose();
	//cBuffer.View.Transpose();
	//cBuffer.Projection.Transpose();
	//cBuffer.WV.Transpose();
	//cBuffer.WVP.Transpose();

	//ShaderManager::Get()->UpdateCBuffer("Transform", &cBuffer);
	ShaderManager::Get()->UpdateCBuffer("LightCBuffer", &m_tLightInfo);
}

void Light_Com::SetLightType(LIGHT_TYPE eType)
{
	m_tLightInfo.LightType = eType;
}

void Light_Com::SetLightRange(float Range)
{
	m_tLightInfo.Range = Range;
}

void Light_Com::SetLightAngle(float InAngle, float OutAngle)
{
	m_tLightInfo.InAngle = InAngle;
	m_tLightInfo.OutAngle = OutAngle;
}

void Light_Com::SetLightFallOff(float FallOff)
{
	m_tLightInfo.FallOff = FallOff;
}

void Light_Com::SetLightAttenuation(const Vector3& Attenuation)
{
	m_tLightInfo.Attenuation = Attenuation;
}
void Light_Com::SetLightPos(const Vector3& Pos)
{
	m_tLightInfo.Pos = Pos;
}

void Light_Com::SetLightDirection(const Vector3& Dir)
{
	m_tLightInfo.Direction = Dir;
}

void Light_Com::SetLightInAngle(float InAngle)
{
	m_tLightInfo.InAngle = InAngle;
}

void Light_Com::SetLightOutAngle(float OutAngle)
{
	m_tLightInfo.OutAngle = OutAngle;
}

void Light_Com::SetLightColor(const Vector4& Ambient, const Vector4& Diffuse, const Vector4& Specular)
{
	m_tLightInfo.Ambient = Ambient;
	m_tLightInfo.Diffuse = Diffuse;
	m_tLightInfo.Spcular = Specular;
}