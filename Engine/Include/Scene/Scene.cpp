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

#include "../Component/Light_Com.h"

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
	 
	m_MainCameraObject = CreateCamera("MainCamera", Vector3(1.0f, -1.0f, -5.0f), CT_PERSPECTIVE, (float)Device::Get()->GetWinSize().Width, (float)Device::Get()->GetWinSize().Height, 60.0f, 0.3f, 1000.0f);
	m_MainCameraTransform = m_MainCameraObject->GetTransform();
	m_MainCamera = m_MainCameraObject->FindComponentFromType<JEONG::Camera_Com>(CT_CAMERA);

	m_UICameraObject = CreateCamera("UICamera", Vector3(0.0f, 0.0f, -0.6f), CT_ORTHO, (float)Device::Get()->GetWinSize().Width, (float)Device::Get()->GetWinSize().Height, 60.0f, 0.0f, 1000.0f);
	m_UICameraTransform = m_UICameraObject->GetTransform();
	m_UICamera = m_UICameraObject->FindComponentFromType<JEONG::Camera_Com>(CT_CAMERA);

	SortLayer();

	Layer* Default = FindLayer("Default");
	GameObject*	newLightObject = GameObject::CreateObject("GlobalLight", Default);
	newLightObject->GetTransform()->SetWorldPos(1.0f, 1.0f, 1.0f);

	Light_Com* newLight = newLightObject->AddComponent<Light_Com>("GlobalLight");
	newLight->SetLightType(LT_POINT);
	newLight->SetLightRange(5.0f);
	newLight->SetLightAttenuation(Vector3(0.2f, 0.2f, 0.2f));

	KeyInput::Get()->AddKey("Up", 'W');
	KeyInput::Get()->AddKey("Down", 'S');
	KeyInput::Get()->AddKey("Left", 'A');
	KeyInput::Get()->AddKey("Right", 'D');
	KeyInput::Get()->AddKey("Front", VK_SPACE);
	KeyInput::Get()->AddKey("Back", VK_BACK);

	SAFE_RELEASE(newLight);
	SAFE_RELEASE(newLightObject);
	SAFE_RELEASE(Default);
	
	return true;
}

int JEONG::Scene::Input(float DeltaTime)
{
#ifdef _DEBUG
	LightDebug(DeltaTime);
#endif

	list<JEONG::SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<JEONG::SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

	for (; StartIter != EndIter ; )
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

	for (; StartIter != EndIter ; StartIter++)
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

		if(getObject != NULLPTR)
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

void JEONG::Scene::LightDebug(float DeltaTime)
{
	ImGui::Text("GlobalLight");
	ImGui::BeginTabBar("AA");
	ImGui::EndTabBar();

	static int GlobalLightType = 0;

	GameObject* getObject = FindObject("GlobalLight");
	Light_Com* getLight = getObject->FindComponentFromType<Light_Com>(CT_LIGHT);

	const char* Items[4] = { "Direction", "Point", "Spot", "BomiSpot" };
	ImGui::Text("LightType");
	ImGui::Combo("", &getLight->m_tLightInfo.LightType, Items, 4);

	ImGui::Text("LightInfo");

	ImGui::SliderFloat4("Ambient", (float*)&getLight->m_tLightInfo.Ambient, 0.0f, 1.0f);
	ImGui::SliderFloat4("Diffuse", (float*)&getLight->m_tLightInfo.Diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat4("Specular", (float*)&getLight->m_tLightInfo.Spcular, 0.0f, 1.0f);
	ImGui::SliderFloat3("Direction", (float*)&getLight->m_tLightInfo.Direction, 0.0f, 1.0f);
	ImGui::SliderFloat3("Attenuation", (float*)&getLight->m_tLightInfo.Attenuation, 0.0f, 1.0f);
	ImGui::SliderFloat("Range", (float*)&getLight->m_tLightInfo.Range, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", (float*)&getLight->m_tLightInfo.FallOff, 0.0f, 1.0f);
	ImGui::SliderFloat("InAngle", (float*)&getLight->m_tLightInfo.InAngle , 0.0f, 1.0f);
	ImGui::SliderFloat("OutAngle", (float*)&getLight->m_tLightInfo.OutAngle, 0.0f, 1.0f);

	ImGui::Text("Trasnform");

	ImGui::SliderFloat3("Pos", (float*)&getLight->m_tLightInfo.Pos, 0.0f, 20.0f);
	SAFE_RELEASE(getObject);
	SAFE_RELEASE(getLight);

	ImGui::BeginTabBar("BB");
	ImGui::EndTabBar();
}

JEONG::GameObject * JEONG::Scene::FindCamera(const string & TagName)
{
	unordered_map<string, JEONG::GameObject*>::iterator FindIter = m_CameraMap.find(TagName);

	if (FindIter == m_CameraMap.end())
		return NULLPTR;

	return FindIter->second;
}
