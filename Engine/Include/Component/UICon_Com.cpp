#include "stdafx.h"
#include "UICon_Com.h"
#include "Renderer_Com.h"
#include "Material_Com.h"
#include "ColliderRect_Com.h"
#include "Animation2D_Com.h"
#include "Transform_Com.h"
#include "IconSlot_Com.h"

#include "../GameObject.h"
#include "../KeyInput.h"

JEONG_USING

UICon_Com::UICon_Com()
{
	m_UIType = UT_ICON;
	SetTag("Icon");
}

UICon_Com::UICon_Com(const UICon_Com & CopyData)
	:UIBase_Com(CopyData)
{
}

UICon_Com::~UICon_Com()
{
}

bool UICon_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("UIconRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	RenderComponent->SetScreenRender(true);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "UIcon", TEXT("Icon.png"));
	SAFE_RELEASE(MaterialComponent);

	ColliderRect_Com* RectColl = m_Object->AddComponent<ColliderRect_Com>("UIconBody");
	RectColl->SetInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(30.0f, 30.0f, 0.0f));
	RectColl->SetCollisionGroup("UI");
	RectColl->SetCollsionCallback(CCT_DOING, this, &UICon_Com::MouseHit);
	RectColl->SetCollsionCallback(CCT_END, this, &UICon_Com::MouseOut);
	SAFE_RELEASE(RectColl);

	m_Transform->SetWorldPivot(0.5f, 0.0f, 0.0f);
	m_Transform->SetWorldScale(30.0f, 30.0f, 0.0f);

	return true;
}

int UICon_Com::Input(float DeltaTime)
{
	return 0;
}

int UICon_Com::Update(float DeltaTime)
{
	return 0;
}

int UICon_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void UICon_Com::Collision(float DeltaTime)
{
}

void UICon_Com::CollisionLateUpdate(float DeltaTime)
{
}

void UICon_Com::Render(float DeltaTime)
{
}

UICon_Com * UICon_Com::Clone()
{
	return new UICon_Com(*this);
}

void UICon_Com::AfterClone()
{
}

void UICon_Com::Active()
{
}

void UICon_Com::MouseHit(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MouseWindow")
	{

	}
}

void UICon_Com::MouseOut(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "MouseWindow")
	{

	}
}
