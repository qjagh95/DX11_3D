#include "stdafx.h"
#include "FreeCamera_Com.h"

JEONG_USING

FreeCamera_Com::FreeCamera_Com()
{
	m_ComType = CT_FREECAMERA;
}

FreeCamera_Com::FreeCamera_Com(const FreeCamera_Com & CopyData)
	:Component_Base(CopyData)
{
}

FreeCamera_Com::~FreeCamera_Com()
{
}

bool FreeCamera_Com::Init()
{
	m_Speed = 1000.0f;
	return true;
}

int FreeCamera_Com::Input(float DeltaTime)
{
	return 0;
}

int FreeCamera_Com::Update(float DeltaTime)
{
	return 0;
}

int FreeCamera_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void FreeCamera_Com::Collision(float DeltaTime)
{
}

void FreeCamera_Com::Render(float DeltaTime)
{
}

FreeCamera_Com * FreeCamera_Com::Clone()
{
	return new FreeCamera_Com(*this);
}
