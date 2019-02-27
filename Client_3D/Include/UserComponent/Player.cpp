#include "ClientHeader.h"
#include "Player.h"
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>
#include <Component/Transform_Com.h>

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
	return true;
}

int Player_Com::Input(float DeltaTime)
{
	return 0;
}

int Player_Com::Update(float DeltaTime)
{
	if (KeyInput::Get()->KeyDown("MoveLeft"))
	{
		SoundManager::Get()->SoundPlay("Effect", ST_3D);
	}
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
