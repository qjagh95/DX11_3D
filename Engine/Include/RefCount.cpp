#include "stdafx.h"
#include "RefCount.h"

JEONG_USING

RefCount::RefCount()
{
	ReferanceCount = 1;
	isActive = true;
	isShow = true;
}

RefCount::~RefCount()
{    
}

int RefCount::Release()
{
	ReferanceCount--;

	if (ReferanceCount == 0)
	{
		delete this;
		return 0;
	}
	return ReferanceCount;
}
