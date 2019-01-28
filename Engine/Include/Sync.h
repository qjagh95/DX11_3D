#pragma once

#include "stdafx.h"

JEONG_BEGIN
class Sync
{
public:
	Sync(CRITICAL_SECTION* Critcal)
	{
		m_Critcal = Critcal;
		EnterCriticalSection(m_Critcal);
	}
	~Sync() 
	{
		LeaveCriticalSection(m_Critcal);
	}

private:
	CRITICAL_SECTION* m_Critcal;
};

JEONG_END
