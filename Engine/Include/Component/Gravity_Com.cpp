#include "stdafx.h"
#include "Gravity_Com.h"

JEONG_USING

Gravity_Com::Gravity_Com()
{
}

Gravity_Com::Gravity_Com(const Gravity_Com & CopyData)
	:Component_Base(CopyData)
{
	*this = CopyData;
}

Gravity_Com::~Gravity_Com()
{
}

bool Gravity_Com::Init()
{
	return true;
}

int Gravity_Com::Input(float DeltaTime)
{
	return 0;
}

int Gravity_Com::Update(float DeltaTime)
{
	return 0;
}

int Gravity_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Gravity_Com::Collision(float DeltaTime)
{
}

void Gravity_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Gravity_Com::Render(float DeltaTime)
{
}

Gravity_Com * Gravity_Com::Clone()
{
	return new Gravity_Com(*this);
}

void Gravity_Com::AfterClone()
{
}
