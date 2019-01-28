#include "ClientHeader.h"
#include "Player.h"

Player_Com::Player_Com()
{
}

Player_Com::Player_Com(const Player_Com & CopyData)
	:UserComponent_Base(CopyData)
{
}

Player_Com::~Player_Com()
{
}

bool Player_Com::Init()
{
	KeyInput::Get()->AddBindAxis("Move", this, &Player_Com::Move);
	KeyInput::Get()->AddKeyScale("Move", DIK_W, 1.0f);
	KeyInput::Get()->AddKeyScale("Move", DIK_S, -1.0f);

	KeyInput::Get()->AddBindAction("Fire", KEY_PRESS, this, &Player_Com::Fire);
	KeyInput::Get()->AddKeyAction("Fire", DIK_SPACE);

	bool bSKey[SKT_MAX] = { true, true, true };
	KeyInput::Get()->AddBindAction("Fire1", KEY_PRESS, this, &Player_Com::Fire1);
	KeyInput::Get()->AddBindAction("Fire1", KEY_UP, this, &Player_Com::Fire1Release);
	KeyInput::Get()->AddKeyAction("Fire1", DIK_SPACE, bSKey);

	return true;
}

int Player_Com::Input(float DeltaTime)
{
	return 0;
}

int Player_Com::Update(float DeltaTime)
{
	return 0;
}

int Player_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Player_Com::Collision(float DeltaTime)
{
}

void Player_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Player_Com::Render(float DeltaTime)
{
}

Player_Com * Player_Com::Clone()
{
	return new Player_Com(*this);
}

void Player_Com::AfterClone()
{
}

void Player_Com::Move(float Scale, float DeltaTime)
{
	OutputDebugString(TEXT("Move\n"));
}

void Player_Com::Fire(float DeltaTime)
{
	OutputDebugString(TEXT("Fire\n"));
}

void Player_Com::Fire1(float DeltaTime)
{
	OutputDebugString(TEXT("Fire1\n"));
}

void Player_Com::Fire1Release(float DeltaTime)
{
	OutputDebugString(TEXT("KeyUp\n"));
}
