#include "stdafx.h"
#include "UIBar_Com.h"
#include "Renderer_Com.h"
#include "Material_Com.h"
#include "ColliderRect_Com.h"
#include "Animation2D_Com.h"
#include "Transform_Com.h"

#include "../GameObject.h"

JEONG_USING

UIBar_Com::UIBar_Com()
	:m_RectCollider(NULLPTR)
{
	m_UIType = UT_BAR;
	SetTag("Bar");
	m_Percent = 0.0f;
}

UIBar_Com::UIBar_Com(const UIBar_Com & CopyData)
	:Button_Com(CopyData)
{
	m_Dir = CopyData.m_Dir;
	m_MinValue = CopyData.m_MinValue;
	m_MaxValue = CopyData.m_MaxValue;
	m_Value = CopyData.m_Value;

	m_Scale = CopyData.m_Scale;
	m_ValueLenth = CopyData.m_ValueLenth;
}

UIBar_Com::~UIBar_Com()
{
	SAFE_RELEASE(m_RectCollider);
}

bool UIBar_Com::Init()
{
	//일단 기본셋팅.
	SetDir(BD_LEFT);

	m_MinValue = 0.0f;
	m_MaxValue = 100.0f;
	m_Value = 100.0f;
	m_Percent = 0.0f;

	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("ButtonRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	RenderComponent->SetScreenRender(true);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "Bar", TEXT("Bar.png"));
	SAFE_RELEASE(MaterialComponent);

	m_RectCollider = m_Object->AddComponent<ColliderRect_Com>("UIBarBody");
	m_RectCollider->SetInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(200.0f, 30.0f, 0.0f));
	m_RectCollider->SetCollisionGroup("UI");
	m_RectCollider->SetCollsionCallback(CCT_DOING, this, &UIBar_Com::MouseHit);

	return true;
}

int UIBar_Com::Input(float DeltaTime)
{
	return 0;
}

int UIBar_Com::Update(float DeltaTime)
{
	Vector3 tempScale = m_Scale;
	Vector3 TempVar;

	switch (m_Dir)
	{
		case BD_LEFT:
		{
			tempScale.x *= m_Percent;
			m_Transform->SetWorldScale(tempScale);

			m_RectCollider->SetInfo(TempVar, tempScale);
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

int UIBar_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void UIBar_Com::Collision(float DeltaTime)
{
}

void UIBar_Com::CollisionLateUpdate(float DeltaTime)
{
}

void UIBar_Com::Render(float DeltaTime)
{

}

UIBar_Com * UIBar_Com::Clone()
{
	return new UIBar_Com(*this);
}
 
void UIBar_Com::AfterClone()
{
	m_RectCollider = (ColliderRect_Com*)FindComponentFromType<ColliderRect_Com>(CT_COLLIDER);
}

void UIBar_Com::SetDir(BAR_DIR dir)
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

void UIBar_Com::SetValue(float Value)
{
	m_Value = Value;

	if (m_Value < m_MinValue)
		m_Value = m_MinValue;

	else if (m_Value > m_MaxValue)
		m_Value = m_MaxValue;

	m_ValueLenth = m_MaxValue - m_MinValue;
	m_Percent = (m_Value - m_MinValue) / m_ValueLenth;
}

void UIBar_Com::AddValue(float Value)
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

void UIBar_Com::SetMinMaxValue(float minValue, float maxValue)
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

void UIBar_Com::SetScale(const Vector3 & Scale)
{
	m_Scale = Scale;
	m_Transform->SetWorldScale(m_Scale);
}

void UIBar_Com::SetScale(float x, float y, float z)
{
	m_Scale = Vector3(x, y, z);
	m_Transform->SetWorldScale(Vector3(x, y, z));
}

void JEONG::UIBar_Com::MouseHit(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
}