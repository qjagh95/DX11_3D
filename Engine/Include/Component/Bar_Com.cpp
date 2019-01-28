#include "stdafx.h"
#include "Bar_Com.h"
#include "Renderer_Com.h"
#include "Material_Com.h"
#include "ColliderRect_Com.h"
#include "Collider_Com.h"
#include "Animation2D_Com.h"
#include "Transform_Com.h"

#include "../GameObject.h"

JEONG_USING

JEONG::Bar_Com::Bar_Com()
	:m_RectCollider(NULLPTR)
{
	SetTag("Bar");
	m_Percent = 0.0f;
}

JEONG::Bar_Com::Bar_Com(const Bar_Com & CopyData)
	:Component_Base(CopyData)
{
	m_Dir = CopyData.m_Dir;
	m_MinValue = CopyData.m_MinValue;
	m_MaxValue = CopyData.m_MaxValue;
	m_Value = CopyData.m_Value;

	m_Scale = CopyData.m_Scale;
	m_ValueLenth = CopyData.m_ValueLenth;
}

JEONG::Bar_Com::~Bar_Com()
{
	SAFE_RELEASE(m_RectCollider);
}

bool JEONG::Bar_Com::Init()
{
	SetDir(BD_LEFT);

	m_MinValue = 0.0f;
	m_MaxValue = 100.0f;
	m_Value = 100.0f;
	m_Percent = 0.0f;

	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("BarRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "Bar", TEXT("Bar.png"));
	SAFE_RELEASE(MaterialComponent);

	m_RectCollider = m_Object->AddComponent<ColliderRect_Com>("BarBody");
	m_RectCollider->SetInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(200.0f, 30.0f, 0.0f));
	m_RectCollider->SetCollsionCallback(CCT_DOING, this, &Bar_Com::MouseHit);

	return true;
}

int JEONG::Bar_Com::Input(float DeltaTime)
{
	return 0;
}

int JEONG::Bar_Com::Update(float DeltaTime)
{
	Vector3 tempScale = m_Scale;
	Vector3 TempVar;

	switch (m_Dir)
	{
		case BD_LEFT:
		{
			tempScale.x *= m_Percent;
			m_Transform->SetWorldScale(tempScale);
			break;
		}
		case BD_RIGHT:
		{
			tempScale.x *= m_Percent;
			TempVar = tempScale - m_Scale;

			m_Transform->SetWorldScale(tempScale);
			m_RectCollider->SetInfo(TempVar, tempScale);
			break;

		}
		case BD_UP:
		{
			tempScale.y *= m_Percent;
			TempVar = tempScale - m_Scale;

			m_Transform->SetWorldScale(tempScale);
			m_RectCollider->SetInfo(TempVar, tempScale);
			break;

		}
		case BD_DOWN:
		{
			tempScale.y *= m_Percent;
			m_Transform->SetWorldScale(tempScale);
			break;
		}
	}

	return 0;
}

int JEONG::Bar_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void JEONG::Bar_Com::Collision(float DeltaTime)
{
}

void JEONG::Bar_Com::CollisionLateUpdate(float DeltaTime)
{
}

void JEONG::Bar_Com::Render(float DeltaTime)
{
}

JEONG::Bar_Com * JEONG::Bar_Com::Clone()
{
	return new JEONG::Bar_Com(*this);
}

void JEONG::Bar_Com::AfterClone()
{
	m_RectCollider = (ColliderRect_Com*)FindComponentFromType<ColliderRect_Com>(CT_COLLIDER);
}

void JEONG::Bar_Com::SetDir(BAR_DIR dir)
{
	m_Dir = dir;

	//줄어드는 방향에 따라서 피봇을 조정한다.
	switch (m_Dir)
	{
		case BD_LEFT:
			m_Transform->SetWorldPivot(0.0f, 0.0f, 0.0f);
			break;
		case BD_RIGHT:
			m_Transform->SetWorldPivot(1.0f, 0.0f, 0.0f);
			break;
		case BD_UP:
			m_Transform->SetWorldPivot(0.0f, 1.0f, 0.0f);
			break;
		case BD_DOWN:
			m_Transform->SetWorldPivot(0.0f, 0.0f, 0.0f);
			break;
	}
}

void JEONG::Bar_Com::SetValue(float Value)
{
	m_Value = Value;

	if (m_Value < m_MinValue)
		m_Value = m_MinValue;

	else if (m_Value > m_MaxValue)
		m_Value = m_MaxValue;

	m_ValueLenth = m_MaxValue - m_MinValue;
	m_Percent = (m_Value - m_MinValue) / m_ValueLenth;
}

void JEONG::Bar_Com::AddValue(float Value)
{
	m_Value += Value;

	if (m_Value < m_MinValue)
		m_Value = m_MinValue;

	else if (m_Value > m_MaxValue)
		m_Value = m_MaxValue;

	//비율을 구한다. HP값이 -일 수도있다.
	m_ValueLenth = m_MaxValue - m_MinValue;
	//HP가 무조건 0이라는 법은 없으므로 이렇게 구했다.
	m_Percent = (m_Value - m_MinValue) / m_ValueLenth;
}

void JEONG::Bar_Com::SetMinMaxValue(float minValue, float maxValue)
{
	m_MinValue = minValue;
	m_MaxValue = maxValue;

	if (m_Value < m_MinValue)
		m_Value = m_MinValue;

	else if (m_Value > m_MaxValue)
		m_Value = m_MaxValue;

	m_ValueLenth = m_MaxValue - m_MinValue;
	m_Percent = (m_Value - m_MinValue) / m_ValueLenth;
}

void JEONG::Bar_Com::SetScale(const Vector3 & Scale)
{
	m_Scale = Scale;
	m_Transform->SetWorldScale(m_Scale);
}

void JEONG::Bar_Com::SetScale(float x, float y, float z)
{
	m_Scale = Vector3(x, y, z);
	m_Transform->SetWorldScale(Vector3(x, y, z));
}

void JEONG::Bar_Com::MouseHit(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
}
