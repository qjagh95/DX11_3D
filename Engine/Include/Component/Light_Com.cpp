#include "stdafx.h"
#include "Light_Com.h"
#include "Transform_Com.h"
#include "../Render/ShaderManager.h"

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

	m_ComType = CT_LIGHT;
}

Light_Com::Light_Com(const Light_Com & CopyData)
	:Component_Base(CopyData)
{
	m_tLightInfo = CopyData.m_tLightInfo;
}

Light_Com::~Light_Com()
{
}

bool Light_Com::Init()
{
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