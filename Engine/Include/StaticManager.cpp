#include "stdafx.h"
#include "StaticManager.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"

JEONG_USING
SINGLETON_VAR_INIT(JEONG::StaticManager)

StaticManager::StaticManager()
{
}

StaticManager::~StaticManager()
{
	Safe_Release_VecList(m_staticObjectList);
}

bool StaticManager::Init()
{
	return true;
}

void StaticManager::AddStaticObject(GameObject * object)
{
	vector<GameObject*>::iterator	StartIter = m_staticObjectList.begin();
	vector<GameObject*>::iterator	EndIter = m_staticObjectList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (object == *StartIter)
			return;
	}

	object->AddRefCount();
	m_staticObjectList.push_back(object);
}

void StaticManager::ChangeScene(Scene * scene)
{
	vector<GameObject*>::iterator  StartIter = m_staticObjectList.begin();
	vector<GameObject*>::iterator EndIter = m_staticObjectList.end();

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

bool StaticManager::CheckStaticObject(const string & TagName)
{
	vector<GameObject*>::iterator StartIter = m_staticObjectList.begin();
	vector<GameObject*>::iterator EndIter = m_staticObjectList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (TagName == (*StartIter)->GetTag())
			return true;
	}

	return false;
}

GameObject * StaticManager::FindStaticObject(const string & TagName)
{
	vector<GameObject*>::iterator StartIter = m_staticObjectList.begin();
	vector<GameObject*>::iterator EndIter = m_staticObjectList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (TagName == (*StartIter)->GetTag())
			return *StartIter;
	}

	return NULLPTR;
}
