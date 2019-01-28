#include "stdafx.h"
#include "Number_Com.h"

JEONG_USING

Number_Com::Number_Com()
	:m_ResultNumber(0)
{
}

Number_Com::Number_Com(const Number_Com& CopyData)
	:Component_Base(CopyData)
{
}

Number_Com::~Number_Com()
{
}

bool Number_Com::Init()
{
	return true;
}

int Number_Com::Input(float DeltaTime)
{
	return 0;
}

int Number_Com::Update(float DeltaTime)
{
	return 0;
}

int Number_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Number_Com::Collision(float DeltaTime)
{
}

void Number_Com::CollisionLateUpdate(float DeltaTime)
{
	vector<int> num;

	while (m_ResultNumber != 0)
	{
		m_ResultNumber /= 10;
		num.push_back(m_ResultNumber);
	}

	for (size_t i = 0; i < num.size(); i++)
	{
		//咯扁辑 积己贸府
	}
}

void Number_Com::Render(float DeltaTime)
{
}

Number_Com * Number_Com::Clone()
{
	return new Number_Com(*this);
}

void Number_Com::AfterClone()
{
}
