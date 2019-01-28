#include "stdafx.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneComponent.h"

#include "../GameObject.h"
#include "../Device.h"
#include "../CollsionManager.h"
#include "../KeyInput.h"

#include "../Component/Camera_Com.h"
#include "../Component/Transform_Com.h"

JEONG_USING

JEONG::Scene::Scene()
{
}

JEONG::Scene::~Scene()
{
	JEONG::GameObject::DestroyProtoType(this);

	Safe_Release_Map(m_CameraMap);
	Safe_Release_VecList(m_LayerList);
	Safe_Release_VecList(m_SceneComponentList);

	SAFE_RELEASE(m_MainCamera);
	SAFE_RELEASE(m_MainCameraObject);
	SAFE_RELEASE(m_UICamera);
	SAFE_RELEASE(m_UICameraObject);
}

bool JEONG::Scene::Init()
{
	AddLayer("BackGround", INT_MIN);
	AddLayer("Tile", 0);
	AddLayer("Default", 2);
	AddLayer("UI", INT_MAX);

	m_MainCameraObject = CreateCamera("MainCamera", Vector3(0.0f, 0.0f, -200.0f), CT_PERSPECTIVE, (float)Device::Get()->GetWinSize().Width, (float)Device::Get()->GetWinSize().Height, 60.0f, 0.03f, 1000.0f);
	m_MainCameraTransform = m_MainCameraObject->GetTransform();
	m_MainCamera = m_MainCameraObject->FindComponentFromType<JEONG::Camera_Com>(CT_CAMERA);

	m_UICameraObject = CreateCamera("UICamera", Vector3(0.0f, 0.0f, 0.0f), CT_ORTHO, (float)Device::Get()->GetWinSize().Width, (float)Device::Get()->GetWinSize().Height, 60.0f, 0.0f, 1000.0f);
	m_UICameraTransform = m_UICameraObject->GetTransform();
	m_UICamera = m_UICameraObject->FindComponentFromType<JEONG::Camera_Com>(CT_CAMERA);

	//CreateCamera함수안에서 카메라 컴포넌트 생성 후 AddComponent

	SortLayer();

	return true;
}

int JEONG::Scene::Input(float DeltaTime)
{
	list<JEONG::SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<JEONG::SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_SceneComponentList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}

		(*StartIter)->Input(DeltaTime);
		StartIter++;
	}

	list<JEONG::Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter1 = m_LayerList.end();

	for (; StartIter1 != EndIter1; )
	{
		if ((*StartIter1)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter1));
			StartIter1 = m_LayerList.erase(StartIter1);
			continue;
		}
		else if ((*StartIter1)->GetIsShow() == false)
		{
			StartIter1++;
			continue;
		}

		(*StartIter1)->Input(DeltaTime);
		StartIter1++;
	}

	m_MainCameraObject->Input(DeltaTime);

	return 0;
}

int JEONG::Scene::Update(float DeltaTime)
{
	list<JEONG::SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<JEONG::SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_SceneComponentList.erase(StartIter);
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

	list<JEONG::Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter1 = m_LayerList.end();

	for (; StartIter1 != EndIter1; )
	{
		if ((*StartIter1)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter1));
			StartIter1 = m_LayerList.erase(StartIter1);
			continue;
		}
		else if ((*StartIter1)->GetIsShow() == false)
		{
			StartIter1++;
			continue;
		}
		(*StartIter1)->Update(DeltaTime);
		StartIter1++;
	}

	m_MainCameraObject->Update(DeltaTime);
	return 0;
}

int JEONG::Scene::LateUpdate(float DeltaTime)
{
	list<JEONG::SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<JEONG::SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_SceneComponentList.erase(StartIter);
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

	list<JEONG::Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter1 = m_LayerList.end();

	for (; StartIter1 != EndIter1; )
	{
		if ((*StartIter1)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter1));
			StartIter1 = m_LayerList.erase(StartIter1);
			continue;
		}
		else if ((*StartIter1)->GetIsShow() == false)
		{
			StartIter1++;
			continue;
		}

		(*StartIter1)->LateUpdate(DeltaTime);
		StartIter1++;
	}
	m_MainCameraObject->LateUpdate(DeltaTime);
	return 0;
}

void JEONG::Scene::Collision(float DeltaTime)
{
	list<JEONG::SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<JEONG::SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_SceneComponentList.erase(StartIter);
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

	list<JEONG::Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter1 = m_LayerList.end();

	for (; StartIter1 != EndIter1; )
	{
		if ((*StartIter1)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter1));
			StartIter1 = m_LayerList.erase(StartIter1);
			continue;
		}
		else if ((*StartIter1)->GetIsShow() == false)
		{
			StartIter1++;
			continue;
		}

		(*StartIter1)->Collision(DeltaTime);
		StartIter1++;
	}

	KeyInput::Get()->UpdateMousePos();
	CollsionManager::Get()->Collsion(DeltaTime);
}

void JEONG::Scene::CollisionLateUpdate(float DeltaTime)
{
	list<JEONG::SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<JEONG::SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_SceneComponentList.erase(StartIter);
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

	list<JEONG::Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter1 = m_LayerList.end();

	for (; StartIter1 != EndIter1; )
	{
		if ((*StartIter1)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter1));
			StartIter1 = m_LayerList.erase(StartIter1);
			continue;
		}
		else if ((*StartIter1)->GetIsShow() == false)
		{
			StartIter1++;
			continue;
		}

		(*StartIter1)->CollisionLateUpdate(DeltaTime);
		StartIter1++;
	}
}

void JEONG::Scene::Render(float DeltaTime)
{
	list<JEONG::SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<JEONG::SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

	for (; StartIter != EndIter; )
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_SceneComponentList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->GetIsShow() == false)
		{
			StartIter++;
			continue;
		}
		(*StartIter)->Render(DeltaTime);
		StartIter++;
	}

	list<JEONG::Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter1 = m_LayerList.end();

	for (; StartIter1 != EndIter1; )
	{
		if ((*StartIter1)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter1));
			StartIter1 = m_LayerList.erase(StartIter1);
			continue;
		}
		else if ((*StartIter1)->GetIsShow() == false)
		{
			StartIter1++;
			continue;
		}
		(*StartIter1)->Render(DeltaTime);
		StartIter1++;
	}
}

void JEONG::Scene::AddLayer(const string & TagName, int ZOrder)
{
	JEONG::Layer* newLayer = new JEONG::Layer();
	newLayer->m_Scene = this;
	newLayer->SetTag(TagName);

	if (newLayer->Init() == false)
	{
		SAFE_RELEASE(newLayer);
		return;
	}

	m_LayerList.push_back(newLayer);
	newLayer->SetZOrder(ZOrder);
	SortLayer();
}

void JEONG::Scene::ChangeLayerZOrder(const string & TagName, int ZOrder)
{
	list<JEONG::Layer*>::iterator StartIter = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
		{
			(*StartIter)->SetZOrder(ZOrder);
			return;
		}
	}
}

void JEONG::Scene::SortLayer()
{
	m_LayerList.sort(Scene::SortLayerFunc);
}

void JEONG::Scene::SetEnableLayer(const string & TagName, bool isShow)
{
	list<JEONG::Layer*>::iterator StartIter = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
		{
			(*StartIter)->SetIsShow(isShow);
			return;
		}
	}
}

void JEONG::Scene::SetLayerDie(const string & TagName, bool isActive)
{
	list<JEONG::Layer*>::iterator StartIter = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
			(*StartIter)->SetIsActive(isActive);
	}
}

JEONG::Layer * JEONG::Scene::FindLayer(const string & TagName)
{
	list<JEONG::Layer*>::iterator StartIter = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
		{
			(*StartIter)->AddRefCount();
			return (*StartIter);
		}
	}
	return NULLPTR;
}

bool JEONG::Scene::SortLayerFunc(const JEONG::Layer * Src, const JEONG::Layer * Dest)
{
	return Src->GetZOrder() < Dest->GetZOrder();
}

JEONG::GameObject * JEONG::Scene::FindObject(const string & TagName)
{
	list<JEONG::Layer*>::iterator StartIter = m_LayerList.begin();
	list<JEONG::Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		JEONG::GameObject* getObject = (*StartIter)->FindObject(TagName);

		if (getObject != NULLPTR)
			return getObject;
	}
	return NULLPTR;
}

JEONG::GameObject * JEONG::Scene::CreateCamera(const string & TagName, const Vector3 & Pos, CAMERA_TYPE eType, float Width, float Height, float ViewAngle, float Near, float Far)
{
	JEONG::GameObject* newCameraObject = FindCamera(TagName);

	if (newCameraObject != NULLPTR)
		return newCameraObject;

	newCameraObject = JEONG::GameObject::CreateObject(TagName);
	newCameraObject->GetTransform()->SetWorldPos(Pos);

	Camera_Com* newCameraCom = newCameraObject->AddComponent<Camera_Com>(TagName);
	newCameraCom->SetCameraInfo(eType, Width, Height, ViewAngle, Near, Far);
	SAFE_RELEASE(newCameraCom);

	newCameraObject->AddRefCount();

	m_CameraMap.insert(make_pair(TagName, newCameraObject));

	return newCameraObject;
}

void JEONG::Scene::ChangeCamera(const string & TagName)
{
	JEONG::GameObject* getCamera = FindCamera(TagName);

	if (getCamera == NULLPTR)
		return;

	SAFE_RELEASE(m_MainCamera);
	SAFE_RELEASE(m_MainCameraTransform);
	SAFE_RELEASE(m_MainCameraObject);

	m_MainCameraObject = getCamera;
	m_MainCameraTransform = getCamera->GetTransform();
	m_MainCamera = getCamera->FindComponentFromType<JEONG::Camera_Com>(CT_CAMERA);
}

JEONG::GameObject * JEONG::Scene::FindCamera(const string & TagName)
{
	unordered_map<string, JEONG::GameObject*>::iterator FindIter = m_CameraMap.find(TagName);

	if (FindIter == m_CameraMap.end())
		return NULLPTR;

	return FindIter->second;
}
