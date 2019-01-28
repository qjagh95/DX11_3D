#include "stdafx.h"
#include "CheckBox_Com.h"
#include "Renderer_Com.h"
#include "Material_Com.h"
#include "ColliderRect_Com.h"
#include "Transform_Com.h"

#include "../GameObject.h"
#include "../KeyInput.h"

JEONG_USING

CheckBox_Com::CheckBox_Com()
{
	m_UIType = UT_CHECKBOX;
	m_CheckState = CBS_FALSE;
	SetTag("CheckBox");
	m_CheckBoxCBuffer = {0};
}

CheckBox_Com::CheckBox_Com(const CheckBox_Com & CopyData) : Button_Com(CopyData)
{
	m_CheckState = CopyData.m_CheckState;
	m_CheckBoxCBuffer = CopyData.m_CheckBoxCBuffer;
}

CheckBox_Com::~CheckBox_Com()
{
}

bool CheckBox_Com::Init()
{
	m_Transform->SetWorldScale(16.0f, 16.0f, 0.0f);

	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("CheckBoxRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	RenderComponent->SetShader(CHECKBOX_SHADER);
	RenderComponent->CreateRendererCBuffer("CheckBoxCBuffer", sizeof(CheckBoxCBuffer));
	RenderComponent->SetScreenRender(true);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "CheckBox", TEXT("CheckBox.png"));
	SAFE_RELEASE(MaterialComponent);

	ColliderRect_Com* RectColl = m_Object->AddComponent<ColliderRect_Com>("CheckBoxBody");
	RectColl->SetInfo(Vector3::Zero, Vector3(16.0f, 16.0f, 0.0f));
	RectColl->SetCollisionGroup("UI");
	RectColl->SetCollsionCallback(CCT_DOING, this, &CheckBox_Com::MouseHit);
	RectColl->SetCollsionCallback(CCT_END, this, &CheckBox_Com::MouseOut);
	SAFE_RELEASE(RectColl);

	m_CheckBoxCBuffer.CheckBoxColor = Vector4::White;
	m_CheckBoxCBuffer.isCheck = false;

	return true;
}

int CheckBox_Com::Input(float DeltaTime)
{
	if (m_ButtonState == BS_MOUSEOVER || m_ButtonState == BS_CLICK)
	{
		if (KeyInput::Get()->KeyUp("LButton"))
		{
			if(m_CheckState == CBS_TRUE)
				m_CheckState = CBS_FALSE;
			else if(m_CheckState == CBS_FALSE)
				m_CheckState = CBS_TRUE;

			if (m_CheckButtonCallBack != NULLPTR)
				m_CheckButtonCallBack(DeltaTime);
		}
	}
	return 0;
}

int CheckBox_Com::Update(float DeltaTime)
{
	return 0;
}

int CheckBox_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void CheckBox_Com::Collision(float DeltaTime)
{
}

void CheckBox_Com::CollisionLateUpdate(float DeltaTime)
{
	if (m_CheckState == CBS_TRUE)
		m_CheckBoxCBuffer.isCheck = (int)true;
	else if (m_CheckState == CBS_FALSE)
		m_CheckBoxCBuffer.isCheck = (int)false;
}

void CheckBox_Com::Render(float DeltaTime)
{
	Renderer_Com* getRender = FindComponentFromType<Renderer_Com>(CT_RENDER);
	getRender->UpdateRendererCBuffer("CheckBoxCBuffer", &m_CheckBoxCBuffer, sizeof(CheckBoxCBuffer));
	SAFE_RELEASE(getRender);
}

CheckBox_Com * CheckBox_Com::Clone()
{
	return new CheckBox_Com(*this);
}

void CheckBox_Com::AfterClone()
{
	ColliderRect_Com* RectColl = m_Object->AddComponent<ColliderRect_Com>("CheckBoxBody");
	RectColl->SetInfo(Vector3::Zero, m_Transform->GetWorldScale());
	SAFE_RELEASE(RectColl);
}

void CheckBox_Com::MouseHit(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MouseWindow")
	{
		m_ButtonState = BS_MOUSEOVER;
	}
}

void CheckBox_Com::MouseOut(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MouseWindow")
	{
		m_ButtonState = BS_NORMAL;
	}
}

