#include "stdafx.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneComponent.h"

#include "../GameObject.h"
#include "../Device.h"
#include "../CollsionManager.h"
#include "../KeyInput.h"

#include "../Render/RenderManager.h"

#include "../Component/Camera_Com.h"
#include "../Component/Transform_Com.h"

#include "../Component/Light_Com.h"

JEONG_USING

Scene::Scene()
{
	m_LightObject = NULLPTR;
	m_Light = NULLPTR;
}

Scene::~Scene()
{
	GameObject::DestroyProtoType(this);

	Safe_Release_Map(m_CameraMap);
	Safe_Release_VecList(m_LayerList);
	Safe_Release_VecList(m_SceneComponentList);

	SAFE_RELEASE(m_MainCamera);
	SAFE_RELEASE(m_MainCameraObject);
	SAFE_RELEASE(m_UICamera);
	SAFE_RELEASE(m_UICameraObject);

	SAFE_RELEASE(m_Light);
	SAFE_RELEASE(m_LightObject);
}

bool Scene::Init()
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

	Layer* Default = FindLayerNoneCount("Default");

	m_LightObject = GameObject::CreateObject("GlobalLight", Default);
	m_LightObject->GetTransform()->SetWorldPos(1.0f, 1.0f, 1.0f);

	m_Light = m_LightObject->AddComponent<Light_Com>("GlobalLight");
	m_Light->SetLightType(LT_DIRECTION);
	m_Light->SetLightRange(5.0f);
	m_Light->SetLightAttenuation(Vector3(0.2f, 0.2f, 0.2f));

	return true;
}

int Scene::Input(float DeltaTime)
{
#ifdef _DEBUG
	LightDebug(DeltaTime);
#endif

	list<SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

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

	list<Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<Layer*>::iterator EndIter1 = m_LayerList.end();

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

int Scene::Update(float DeltaTime)
{
	list<SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

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

	list<Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<Layer*>::iterator EndIter1 = m_LayerList.end();

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

int Scene::LateUpdate(float DeltaTime)
{
	list<SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

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

	list<Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<Layer*>::iterator EndIter1 = m_LayerList.end();

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

void Scene::Collision(float DeltaTime)
{
	list<SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

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

	list<Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<Layer*>::iterator EndIter1 = m_LayerList.end();

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

void Scene::CollisionLateUpdate(float DeltaTime)
{
	list<SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

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

	list<Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<Layer*>::iterator EndIter1 = m_LayerList.end();

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

void Scene::Render(float DeltaTime)
{
	list<SceneComponent*>::iterator StartIter = m_SceneComponentList.begin();
	list<SceneComponent*>::iterator EndIter = m_SceneComponentList.end();

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

	list<Layer*>::iterator StartIter1 = m_LayerList.begin();
	list<Layer*>::iterator EndIter1 = m_LayerList.end();

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

void Scene::AddLayer(const string & TagName, int ZOrder)
{
	Layer* newLayer = new JEONG::Layer();
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

void Scene::ChangeLayerZOrder(const string & TagName, int ZOrder)
{
	list<Layer*>::iterator StartIter = m_LayerList.begin();
	list<Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
		{
			(*StartIter)->SetZOrder(ZOrder);
			return;
		}
	}
}

void Scene::SortLayer()
{
	m_LayerList.sort(Scene::SortLayerFunc);
}

void Scene::SetEnableLayer(const string & TagName, bool isShow)
{
	list<Layer*>::iterator StartIter = m_LayerList.begin();
	list<Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter ; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
		{
			(*StartIter)->SetIsShow(isShow);
			return;
		}
	}
}

void Scene::SetLayerDie(const string & TagName, bool isActive)
{
	list<Layer*>::iterator StartIter = m_LayerList.begin();
	list<Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
			(*StartIter)->SetIsActive(isActive);
	}
}

Layer * Scene::FindLayer(const string & TagName)
{
	list<Layer*>::iterator StartIter = m_LayerList.begin();
	list<Layer*>::iterator EndIter = m_LayerList.end();

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

Layer * Scene::FindLayerNoneCount(const string & TagName)
{
	list<Layer*>::iterator StartIter = m_LayerList.begin();
	list<Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
		{
			return (*StartIter);
		}
	}
	return NULLPTR;
}

bool Scene::SortLayerFunc(const JEONG::Layer * Src, const JEONG::Layer * Dest)
{
	return Src->GetZOrder() < Dest->GetZOrder();
}

GameObject * Scene::FindObject(const string & TagName)
{
	list<Layer*>::iterator StartIter = m_LayerList.begin();
	list<Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		GameObject* getObject = (*StartIter)->FindObject(TagName);

		if(getObject != NULLPTR)
			return getObject;
	}
	return NULLPTR;
}

GameObject * Scene::FindObjectNoneCount(const string & TagName)
{
	list<Layer*>::iterator StartIter = m_LayerList.begin();
	list<Layer*>::iterator EndIter = m_LayerList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		GameObject* getObject = (*StartIter)->FindObjectNoneCount(TagName);

		if (getObject != NULLPTR)
			return getObject;
	}
	return NULLPTR;
}

GameObject * Scene::CreateCamera(const string & TagName, const Vector3 & Pos, CAMERA_TYPE eType, float Width, float Height, float ViewAngle, float Near, float Far)
{
	GameObject* newCameraObject = FindCamera(TagName);

	if (newCameraObject != NULLPTR)
		return newCameraObject;

	newCameraObject = GameObject::CreateObject(TagName);
	newCameraObject->GetTransform()->SetWorldPos(Pos);

	Camera_Com* newCameraCom = newCameraObject->AddComponent<Camera_Com>(TagName);
	newCameraCom->SetCameraInfo(eType, Width, Height, ViewAngle, Near, Far);
	SAFE_RELEASE(newCameraCom);

	newCameraObject->AddRefCount();

	m_CameraMap.insert(make_pair(TagName, newCameraObject));

	return newCameraObject;
}

void Scene::ChangeCamera(const string & TagName)
{
	GameObject* getCamera = FindCamera(TagName);

	if (getCamera == NULLPTR)
		return;

	SAFE_RELEASE(m_MainCamera);
	SAFE_RELEASE(m_MainCameraTransform);
	SAFE_RELEASE(m_MainCameraObject);

	m_MainCameraObject = getCamera;
	m_MainCameraTransform = getCamera->GetTransform();
	m_MainCamera = getCamera->FindComponentFromType<Camera_Com>(CT_CAMERA);
}

void Scene::LightDebug(float DeltaTime)
{
	ImGui::Text("GlobalLight");
	ImGui::BeginTabBar("AA");
	ImGui::EndTabBar();

	static int GlobalLightType = 0;

	const char* Items[4] = { "Direction", "Point", "Spot", "BomiSpot" };
	ImGui::Text("LightType");
	ImGui::Combo("", &m_Light->m_tLightInfo.LightType, Items, 4);

	ImGui::Text("LightInfo");

	ImGui::SliderFloat4("Ambient", (float*)&m_Light->m_tLightInfo.Ambient, -1.0f, 1.0f);
	ImGui::SliderFloat4("Diffuse", (float*)&m_Light->m_tLightInfo.Diffuse, -1.0f, 1.0f);
	ImGui::SliderFloat3("Specular", (float*)&m_Light->m_tLightInfo.Spcular, -1.0f, 1.0f);
	ImGui::SliderFloat3("Direction", (float*)&m_Light->m_tLightInfo.Direction, -1.0f, 1.0f);
	ImGui::SliderFloat3("Attenuation", (float*)&m_Light->m_tLightInfo.Attenuation, -1.0f, 1.0f);
	ImGui::SliderFloat("Range", (float*)&m_Light->m_tLightInfo.Range, 0.0f, 20.0f);
	ImGui::SliderFloat("FallOff", (float*)&m_Light->m_tLightInfo.FallOff, -1.0f, 10.0f);

	ImGui::Text("LightPos");

	ImGui::SliderFloat3("Pos", (float*)&m_Light->m_tLightInfo.Pos, -20.0f, 20.0f);

	ImGui::BeginTabBar("BB");
	ImGui::EndTabBar();

	ImGui::Text("Camera");

	static Vector3 CameraPos;
	ImGui::SliderFloat3("CameraPos", (float*)&CameraPos, -100.0f, 100.0f);
	m_MainCameraTransform->SetWorldPos(CameraPos);

	ImGui::BeginTabBar("Camera");
	ImGui::EndTabBar();
}

GameObject * Scene::FindCamera(const string & TagName)
{
	unordered_map<string, GameObject*>::iterator FindIter = m_CameraMap.find(TagName);

	if (FindIter == m_CameraMap.end())
		return NULLPTR;

	return FindIter->second;
}
