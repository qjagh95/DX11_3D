#include "stdafx.h"
#include "ThreadManager.h"
#include "Thread.h"

JEONG_USING
SINGLETON_VAR_INIT(ThreadManager)

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
	unordered_map<string, CRITICAL_SECTION>::iterator StartIter = m_CriMap.begin();
	unordered_map<string, CRITICAL_SECTION>::iterator EndIter = m_CriMap.end();

	for (; StartIter != EndIter; StartIter++)
		DeleteCriticalSection(&StartIter->second);

	m_CriMap.clear();

	Safe_Delete_Map(m_ThreadMap);
}

bool ThreadManager::Init()
{
	return true;
}

bool ThreadManager::DeleteThread(const string & ThreadName)
{
	unordered_map<string, Thread*>::iterator FindIter = m_ThreadMap.find(ThreadName);

	if (FindIter == m_ThreadMap.end())
		return false;

	SAFE_DELETE(FindIter->second);
	m_ThreadMap.erase(FindIter);

	return true;
}

bool ThreadManager::CritcalCreate(const string & CritcalName)
{
	if (FindCritcal(CritcalName) != NULLPTR)
		return false;

	CRITICAL_SECTION newCritcal;
	InitializeCriticalSection(&newCritcal);

	m_CriMap.insert(make_pair(CritcalName, newCritcal));
	return true;
}

Thread * ThreadManager::FindThread(const string & ThreadName)
{
	unordered_map<string, Thread*>::iterator FindIter = m_ThreadMap.find(ThreadName);

	if (FindIter == m_ThreadMap.end())
		return NULLPTR;

	return FindIter->second;
}

CRITICAL_SECTION* ThreadManager::FindCritcal(const string & CritcalName)
{
	unordered_map<string, CRITICAL_SECTION>::iterator FindIter = m_CriMap.find(CritcalName);

	if (FindIter == m_CriMap.end())
		return NULLPTR;

	return &FindIter->second;
}
