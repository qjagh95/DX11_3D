#include "stdafx.h"
#include "SlotManager.h"
#include "GameObject.h"

#include "Component/IconSlot_Com.h"
#include "Component/UICon_Com.h"

JEONG_USING
SINGLETON_VAR_INIT(SlotManager)

SlotManager::SlotManager()
{
}

SlotManager::~SlotManager()
{
	unordered_map<string, vector<IconSlot_Com*>>::iterator StartIter = m_SlotMap.begin();
	unordered_map<string, vector<IconSlot_Com*>>::iterator EndIter = m_SlotMap.end();

	for (; StartIter != EndIter; StartIter++)
	{
		for (size_t i = 0; i < StartIter->second.size(); i++)
		{
			SAFE_RELEASE(StartIter->second[i]);
		}
	}
}

bool SlotManager::Init()
{
	return true;
}

void SlotManager::ChangeSlot(const string & GroupName, size_t Src, size_t Dest)
{
}

bool SlotManager::CreateSlot(const string & GroupName, size_t RowCount, size_t ColumCount)
{
	vector<IconSlot_Com*>* Temp = FindSlot(GroupName);

	if (Temp != NULLPTR)
		return false;

	size_t AllCount = RowCount * ColumCount;

	for (size_t i = 0; i < AllCount; i++)
	{
		//GameObject* newObject;
		//IconSlot_Com* newSlot;
	}

	return true;
}

vector<IconSlot_Com*>* SlotManager::FindSlot(const string & GroupName)
{
	unordered_map<string, vector<IconSlot_Com*>>::iterator FindIter = m_SlotMap.find(GroupName);

	if (FindIter == m_SlotMap.end())
		return NULLPTR;
	
	return &FindIter->second;
}
