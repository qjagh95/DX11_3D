#include "stdafx.h"
#include "Light_Com.h"

JEONG_USING

Light_Com::Light_Com()
{
}

Light_Com::Light_Com(const Light_Com & CopyData)
	:Component_Base(CopyData)
{
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
