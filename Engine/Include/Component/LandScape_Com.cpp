#include "stdafx.h"
#include "LandScape_Com.h"

JEONG_USING

LandScape_Com::LandScape_Com()
{
	m_CBuffer = {};
	m_NaviMesh = NULLPTR;
}

LandScape_Com::LandScape_Com(const LandScape_Com & CopyData)
	:Component_Base(CopyData)
{
	*this = CopyData;
	ReferanceCount = 1;
}

LandScape_Com::~LandScape_Com()
{
}

bool LandScape_Com::Init()
{
	return true;
}

int LandScape_Com::Input(float DeltaTime)
{
	return 0;
}

int LandScape_Com::Update(float DeltaTime)
{
	return 0;
}

int LandScape_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void LandScape_Com::Collision(float DeltaTime)
{
}

void LandScape_Com::CollisionLateUpdate(float DeltaTime)
{
}

void LandScape_Com::Render(float DeltaTime)
{
}

LandScape_Com * LandScape_Com::Clone()
{
	return new LandScape_Com(*this);
}

void LandScape_Com::CreateGrid(const string & KeyName, int XCount, int ZCount)
{
	m_XCount = XCount;
	m_ZCount = ZCount;

	for (int y = 0; y < ZCount; y++)
	{
		for (int x = 0; x < XCount; x++)
		{

		}
	}

}

void LandScape_Com::ComputeTangent()
{
}

void LandScape_Com::ComputeNormal()
{
}
