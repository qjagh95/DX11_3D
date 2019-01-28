#include "stdafx.h"
#include "RefCount.h"

JEONG_USING

JEONG::RefCount::RefCount(): ReferanceCount(1), isActive(true), isShow(true)
{
}

JEONG::RefCount::~RefCount()
{    
}

int JEONG::RefCount::Release()
{
	ReferanceCount--;

	if (ReferanceCount == 0)
	{
		delete this;
		return 0;
	}
	return ReferanceCount;
}
