#include "stdafx.h"
#include "TimeManager.h"
#include "Timer.h"

JEONG_USING
SINGLETON_VAR_INIT(TimeManager)

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
	unordered_map<string, Timer*>::iterator StartIter = m_TimerMap.begin();
	unordered_map<string, Timer*>::iterator EndIter = m_TimerMap.end();

	for (; StartIter != EndIter; StartIter++)
		SAFE_DELETE(StartIter->second);

	m_TimerMap.clear();
}

bool TimeManager::Init()
{
	CreateTimer("MainTimer");

	return true;
}

bool TimeManager::CreateTimer(const string & TagName)
{
	Timer* newTimer = FindTimer(TagName);

	if (newTimer != NULLPTR)
		return false;

	newTimer = new Timer();
	newTimer->TimerName = TagName;
	newTimer->Init();

	m_TimerMap.insert(make_pair(TagName, newTimer));
	return true;
}

Timer * TimeManager::FindTimer(const string & TagName)
{
	unordered_map<string, Timer*>::iterator FindIter =  m_TimerMap.find(TagName);

	if (FindIter == m_TimerMap.end())
		return NULLPTR;

	return FindIter->second;
}
