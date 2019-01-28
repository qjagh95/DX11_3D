#include "stdafx.h"
#include "UserComponent_Base.h"

JEONG_USING

UserComponent_Base::UserComponent_Base()
{
}

UserComponent_Base::UserComponent_Base(const UserComponent_Base & userCom)
	:Component_Base(userCom)
{
}

UserComponent_Base::~UserComponent_Base()
{
}

bool UserComponent_Base::Init()
{
	return false;
}

int UserComponent_Base::Input(float DeltaTime)
{
	return 0;
}

int UserComponent_Base::Update(float DeltaTime)
{
	return 0;
}

int UserComponent_Base::LateUpdate(float DeltaTime)
{
	return 0;
}

void UserComponent_Base::Collision(float DeltaTime)
{
}

void UserComponent_Base::CollisionLateUpdate(float DeltaTime)
{
}

void UserComponent_Base::Render(float DeltaTime)
{
}

UserComponent_Base * UserComponent_Base::Clone()
{
	return new UserComponent_Base(*this);
}
