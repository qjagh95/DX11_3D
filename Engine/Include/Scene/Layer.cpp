#include "../stdafx.h"
#include "Layer.h"
#include "Scene.h"

#include "../GameObject.h"
#include "Render/RenderManager.h"

JEONG_USING

JEONG::Layer::Layer()
	:m_ZOrder(-1)
{
}

JEONG::Layer::~Layer()
{
	Safe_Release_VecList(m_ObjectList);
}

bool JEONG::Layer::Init()
{
	return true;
}

int JEONG::Layer::Input(float DeltaTime)
{
	list<JEONG::GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<JEONG::GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->Input(DeltaTime);
		StartIter++;
	}

	return 0;
}

int JEONG::Layer::Update(float DeltaTime)
{
	list<JEONG::GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<JEONG::GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->Update(DeltaTime);
		StartIter++;
	}
	return 0;
}

int JEONG::Layer::LateUpdate(float DeltaTime)
{
	list<JEONG::GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<JEONG::GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->LateUpdate(DeltaTime);
		StartIter++;
	}
	return 0;
}

void JEONG::Layer::Collision(float DeltaTime)
{
	list<JEONG::GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<JEONG::GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->Collision(DeltaTime);
		StartIter++;
	}
}

void JEONG::Layer::CollisionLateUpdate(float DeltaTime)
{
	list<JEONG::GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<JEONG::GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->CollisionLateUpdate(DeltaTime);
		StartIter++;
	}
}

void JEONG::Layer::Render(float DeltaTime)
{
	list<JEONG::GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<JEONG::GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ObjectList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}
		//씬의 Render는 RenderTarget에다가 픽셀정보를 보내준다.
		RenderManager::Get()->AddRenderObject((*StartIter));
		StartIter++;
	}
}

void JEONG::Layer::SetZOrder(int zorder)
{
	m_ZOrder = zorder;
	m_Scene->SortLayer();
}

void JEONG::Layer::AddObject(JEONG::GameObject * object)
{
	object->SetScene(m_Scene);
	object->SetLayer(this);
	object->AddRefCount();

	m_ObjectList.push_back(object);
}

JEONG::GameObject * JEONG::Layer::FindObject(const string & TagName)
{
	list<JEONG::GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<JEONG::GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ( ((*StartIter)->GetTag() == TagName))
		{
			(*StartIter)->AddRefCount();
			return *StartIter;
		}
	}
	return NULLPTR;
}

GameObject * Layer::FindObjectNoneCount(const string & TagName)
{
	list<GameObject*>::iterator StartIter = m_ObjectList.begin();
	list<GameObject*>::iterator EndIter = m_ObjectList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if (((*StartIter)->GetTag() == TagName))
		{
			return *StartIter;
		}
	}
	return NULLPTR;
}
