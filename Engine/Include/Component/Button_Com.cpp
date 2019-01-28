#include "stdafx.h"
#include "Button_Com.h"
#include "Renderer_Com.h"
#include "Material_Com.h"
#include "ColliderRect_Com.h"
#include "Transform_Com.h"

#include "../GameObject.h"
#include "../KeyInput.h"

JEONG_USING

Button_Com::Button_Com()
{
	m_UIType = UT_BUTTON;
	SetTag("UIButton");
}

Button_Com::Button_Com(const Button_Com & CopyData)
	:UIBase_Com(CopyData)
{
	m_ButtonState = BS_NORMAL;
	memcpy(m_ButtonColor, CopyData.m_ButtonColor, sizeof(Vector4) * BS_MAX);
}

Button_Com::~Button_Com()
{
}

bool Button_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("ButtonRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	RenderComponent->SetShader(BUTTON_SHADER);
	RenderComponent->CreateRendererCBuffer("ButtonCBuffer", sizeof(ButtonCBuffer));
	RenderComponent->SetScreenRender(true);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "Button", TEXT("Start.png"));
	SAFE_RELEASE(MaterialComponent);

	ColliderRect_Com* RectColl = m_Object->AddComponent<ColliderRect_Com>("ButtonBody");
	RectColl->SetInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(200.0f, 50.0f, 0.0f));
	RectColl->SetCollisionGroup("UI");
	RectColl->SetCollsionCallback(CCT_DOING, this, &Button_Com::MouseHit);
	RectColl->SetCollsionCallback(CCT_END, this, &Button_Com::MouseOut);
	SAFE_RELEASE(RectColl);

	m_ButtonState = BS_NORMAL;

	m_ButtonColor[BS_DISABLE] = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
	m_ButtonColor[BS_NORMAL] = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
	m_ButtonColor[BS_MOUSEOVER] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_ButtonColor[BS_CLICK] = Vector4(0.7f, 0.7f, 0.7f, 1.0f);

	m_Transform->SetWorldPivot(0.5f, 0.0f, 0.0f);

	return true;
}

int Button_Com::Input(float DeltaTime)
{
	return 0;
}

int Button_Com::Update(float DeltaTime)
{
	return 0;
}

int Button_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Button_Com::Collision(float DeltaTime)
{
}

void Button_Com::CollisionLateUpdate(float DeltaTime)
{
	if (m_ButtonState == BS_MOUSEOVER || m_ButtonState == BS_CLICK)
	{
		if (KeyInput::Get()->KeyPress("LButton"))
			m_ButtonState = BS_CLICK;

		else if (KeyInput::Get()->KeyUp("LButton"))
			m_ButtonCallBack(DeltaTime);
	}
}

void Button_Com::Render(float DeltaTime)
{
	m_ButtonCBuffer.DiffuseColor = m_ButtonColor[m_ButtonState];
	
	Renderer_Com* getRender = FindComponentFromType<Renderer_Com>(CT_RENDER);
	getRender->UpdateRendererCBuffer("ButtonCBuffer", &m_ButtonCBuffer, sizeof(ButtonCBuffer));
	SAFE_RELEASE(getRender);
}

Button_Com * Button_Com::Clone()
{
	return new Button_Com(*this);
}

void Button_Com::AfterClone()
{
	ColliderRect_Com* RectColl = m_Object->AddComponent<ColliderRect_Com>("ButtonBody");
	RectColl->SetInfo(Vector3::Zero, m_Transform->GetWorldScale());
	SAFE_RELEASE(RectColl);
}

void Button_Com::SetStateColor(BUTTON_STATE eState, const Vector4 & Color)
{
	m_ButtonColor[eState] = Color;
}
void Button_Com::SetStateColor(BUTTON_STATE eState, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_ButtonColor[eState] = Vector4(r, g, b, a);
}

void Button_Com::MouseHit(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MouseWindow")
	{
		m_ButtonState = BS_MOUSEOVER;
	}
}

void Button_Com::MouseOut(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MouseWindow")
	{
		m_ButtonState = BS_NORMAL;
	}
}

void Button_Com::Disable()
{
	m_ButtonState = BS_DISABLE;

}
void Button_Com::SetNormal()
{
	m_ButtonState = BS_NORMAL;
}