#include "stdafx.h"
#include "StaticManager.h"

JEONG_USING
SINGLETON_VAR_INIT(JEONG::StaticManager)

JEONG::StaticManager::StaticManager()
{
}

JEONG::StaticManager::~StaticManager()
{
	Safe_Release_VecList(m_staticObjectList);
}

bool JEONG::StaticManager::Init()
{
	return true;
}

void JEONG::StaticManager::AddStaticObject(GameObject * object)
{
	list<GameObject*>::iterator	StartIter = m_staticObjectList.begin();
	list<GameObject*>::iterator	EndIter = m_staticObjectList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (object == *StartIter)
			return;
	}

	object->AddRefCount();
	m_staticObjectList.push_back(object);
}

void JEONG::StaticManager::ChangeScene(Scene * scene)
{
	list<GameObject*>::iterator	StartIter = m_staticObjectList.begin();
	list<GameObject*>::iterator	EndIter = m_staticObjectList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		Layer* pLayer = scene->FindLayer((*StartIter)->GetLayerName());

		if (pLayer == NULLPTR)
		{
			scene->AddLayer((*StartIter)->GetLayerName(), (*StartIter)->GetLayerZOrder());
			pLayer = scene->FindLayer((*StartIter)->GetLayerName());
		}

		pLayer->AddObject(*StartIter);
		SAFE_RELEASE(pLayer);
	}
}

bool JEONG::StaticManager::CheckStaticObject(const string & TagName)
{
	list<GameObject*>::iterator	StartIter = m_staticObjectList.begin();
	list<GameObject*>::iterator	EndIter = m_staticObjectList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (TagName == (*StartIter)->GetTag())
			return true;
	}

	return false;
}

GameObject * JEONG::StaticManager::FindStaticObject(const string & TagName)
{
	list<GameObject*>::iterator	StartIter = m_staticObjectList.begin();
	list<GameObject*>::iterator	EndIter = m_staticObjectList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (TagName == (*StartIter)->GetTag())
			return *StartIter;
	}

	return NULLPTR;
}
