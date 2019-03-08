#include "stdafx.h"
#include "FreeCamera_Com.h"
#include "Transform_Com.h"

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
	m_Speed = 50.0f;
	return true;
}

int FreeCamera_Com::Input(float DeltaTime)
{
	return 0;
}

int FreeCamera_Com::Update(float DeltaTime)
{
	if (KeyInput::Get()->KeyPress("MoveLeft"))
		m_Object->GetTransform()->Move(AXIS_X, -m_Speed, DeltaTime);

	else if (KeyInput::Get()->KeyPress("MoveRight"))
		m_Object->GetTransform()->Move(AXIS_X, m_Speed, DeltaTime);

	if (KeyInput::Get()->KeyPress("MoveUp"))
		m_Object->GetTransform()->Move(AXIS_Y, m_Speed, DeltaTime);

	else if (KeyInput::Get()->KeyPress("MoveDown"))
		m_Object->GetTransform()->Move(AXIS_Y, -m_Speed, DeltaTime);

	if (KeyInput::Get()->KeyPress("Space"))
		m_Object->GetTransform()->Move(AXIS_Z, -m_Speed, DeltaTime);

	else if (KeyInput::Get()->KeyPress("Shift"))
		m_Object->GetTransform()->Move(AXIS_Z, m_Speed, DeltaTime);

	if (KeyInput::Get()->KeyPress("RButton"))
		m_Object->GetTransform()->Rotation(Vector3(KeyInput::Get()->GetMouseGap().y * 0.1f, -KeyInput::Get()->GetMouseGap().x * 0.1f, 0.0f));

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
