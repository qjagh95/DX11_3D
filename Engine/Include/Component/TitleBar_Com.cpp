#include "stdafx.h"
#include "TitleBar_Com.h"
#include "Renderer_Com.h"
#include "Material_Com.h"
#include "ColliderRect_Com.h"
#include "Transform_Com.h"

#include "../GameObject.h"
#include "../KeyInput.h"

JEONG_USING

TitleBar_Com::TitleBar_Com()
{
	m_ComType = CT_TITLEBAR;
	SetTag("TitleBar");
	m_isOver = false;
}

TitleBar_Com::TitleBar_Com(const TitleBar_Com & CopyData)
	:Component_Base(CopyData)
{
	m_isOver = false;
}

TitleBar_Com::~TitleBar_Com()
{
	SAFE_RELEASE(m_TitleObject);
}

bool TitleBar_Com::Init()
{
	Transform_Com* objectTrans = m_Object->GetTransform();

	m_TitleObject = GameObject::CreateObject("TitleObject", m_Layer);
	m_TitleObject->GetTransform()->SetWorldPivot(0.0f, 0.0f, 0.0f);
	m_TitleObject->GetTransform()->SetWorldScale(Vector3(200.0f, 50.0f, 0.0f));

	Renderer_Com* RenderComponent = m_TitleObject->AddComponent<Renderer_Com>("TitleBarRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_TitleObject->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "TitleBar", TEXT("Button.png"));
	SAFE_RELEASE(MaterialComponent);

	ColliderRect_Com* RectColl = m_TitleObject->AddComponent<ColliderRect_Com>("TitleBarBody");
	RectColl->SetInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(200.0f, 50.0f, 0.0f));
	RectColl->SetCollsionCallback(CCT_DOING, this, &TitleBar_Com::MouseHit);
	RectColl->SetCollsionCallback(CCT_END, this, &TitleBar_Com::MouseOut);
	SAFE_RELEASE(RectColl);

	return true;
}

int TitleBar_Com::Input(float DeltaTime)
{
	return 0;
}

int TitleBar_Com::Update(float DeltaTime)
{
	Transform_Com* mainObjectTrans = m_Object->GetTransform();
	m_TitleObject->GetTransform()->SetWorldPos(mainObjectTrans->GetWorldPos().x - mainObjectTrans->GetWorldScale().x / 2.0f, mainObjectTrans->GetWorldPos().y + mainObjectTrans->GetWorldScale().y, 0.0f);

	if (m_isOver == true)
	{
		if (KeyInput::Get()->KeyPress("LButton"))
		{
			//타이틀 오브젝트 이동
			m_TitleObject->GetTransform()->Move(Vector3(KeyInput::Get()->GetMouseGap().x, KeyInput::Get()->GetMouseGap().y, 0.0f), 1.0f);
			//메인 오브젝트 이동.
			mainObjectTrans->Move(Vector3(KeyInput::Get()->GetMouseGap().x, KeyInput::Get()->GetMouseGap().y, 0.0f), 1.0f);
		}
	}

	return 0;
}

int TitleBar_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void TitleBar_Com::Collision(float DeltaTime)
{
}

void TitleBar_Com::CollisionLateUpdate(float DeltaTime)
{
	
}

void TitleBar_Com::Render(float DeltaTime)
{
}

TitleBar_Com * TitleBar_Com::Clone()
{
	return new TitleBar_Com(*this);
}

void TitleBar_Com::AfterClone()
{
}

void TitleBar_Com::MouseHit(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MouseWorld")
	{
		m_isOver = true;
	}
}

void TitleBar_Com::MouseOut(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MouseWorld")
	{
		m_isOver = false;
	}
}
