#include "stdafx.h"
#include "Animation3D_Com.h"

JEONG_USING

Animation3D_Com::Animation3D_Com()
{
}

Animation3D_Com::Animation3D_Com(const Animation3D_Com& CopyData)
	:Component_Base(CopyData)
{

}

Animation3D_Com::~Animation3D_Com()
{
}

bool Animation3D_Com::Init()
{
	return true;
}

int Animation3D_Com::Input(float DeltaTime)
{
	return 0;
}

int Animation3D_Com::Update(float DeltaTime)
{
	return 0;
}

int Animation3D_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Animation3D_Com::Collision(float DeltaTime)
{
}

void Animation3D_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Animation3D_Com::Render(float DeltaTime)
{
}

Animation3D_Com * Animation3D_Com::Clone()
{
	return new Animation3D_Com(*this);
}

void Animation3D_Com::AfterClone()
{
}
