#include "stdafx.h"
#include "UIBase_Com.h"
#include "ColliderRect_Com.h"

JEONG_USING

UIBase_Com::UIBase_Com()
{
	m_ComType = CT_UI;
}

UIBase_Com::UIBase_Com(const UIBase_Com & CopyData)
	:Component_Base(CopyData)
{
	m_UIType = CopyData.m_UIType;
}

UIBase_Com::~UIBase_Com()
{
}

bool UIBase_Com::Init()
{
	return true;
}

int UIBase_Com::Input(float DeltaTime)
{
	return 0;
}

int UIBase_Com::Update(float DeltaTime)
{
	return 0;
}

int UIBase_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void UIBase_Com::Collision(float DeltaTime)
{
}

void UIBase_Com::CollisionLateUpdate(float DeltaTime)
{
}

void UIBase_Com::Render(float DeltaTime)
{
}

UIBase_Com * UIBase_Com::Clone()
{
	return new UIBase_Com(*this);
}
