#include "stdafx.h"
#include "GameObject.h"
#include "CollsionManager.h"

#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include "Component/Transform_Com.h"
#include "Component/Renderer_Com.h"
#include "StaticManager.h"

#include "BineryReader.h"
#include "BineryWriter.h"

JEONG_USING

unordered_map<JEONG::Scene*, unordered_map<string, JEONG::GameObject*>> JEONG::GameObject::m_ProtoTypeMap;

JEONG::GameObject::GameObject()
	:m_Scene(NULLPTR), m_Layer(NULLPTR), m_Transform(NULLPTR), m_Parent(NULLPTR)
{
	SetTag("GameObject");
	m_RenderGroup = RG_NORMAL;
}

JEONG::GameObject::GameObject(const JEONG::GameObject& copyObject)
{
	*this = copyObject;

	ReferanceCount = 1;

	m_Transform = copyObject.m_Transform->Clone();
	m_Transform->m_Transform = m_Transform;

	m_ComponentList.clear();
	m_FindComList.clear();

	list<JEONG::Component_Base*>::const_iterator StartIter = copyObject.m_ComponentList.begin();
	list<JEONG::Component_Base*>::const_iterator EndIter = copyObject.m_ComponentList.end();

	for (; StartIter != EndIter ; StartIter++)
	{
		JEONG::Component_Base* newComponent = (*StartIter)->Clone();
		newComponent->m_Object = this;
		newComponent->m_Transform = m_Transform;

		m_ComponentList.push_back(newComponent);
	}

	JEONG::Renderer_Com* getRender = FindComponentFromType<JEONG::Renderer_Com>(CT_RENDER);

	if(getRender != NULLPTR)
	{
		getRender->CheckComponent();

		SAFE_RELEASE(getRender);
	}

	m_ChildList.clear();

	list<JEONG::GameObject*>::iterator StartIter1 = m_ChildList.begin();
	list<JEONG::GameObject*>::iterator EndIter1 = m_ChildList.end();

	for (; StartIter1 != EndIter1; StartIter1++)
	{
		GameObject* child = (*StartIter1)->Clone();
		child->m_Parent = this;
		child->m_Transform->SetParentFlag(TPF_POS | TPF_ROT);

		m_ChildList.push_back(child);
	}
}

JEONG::GameObject::~GameObject()
{
	Safe_Release_VecList(m_ChildList);
	SAFE_RELEASE(m_Transform);
	Safe_Release_VecList(m_FindComList);
	Safe_Release_VecList(m_ComponentList);
}

bool JEONG::GameObject::Init()
{
	m_Transform = new JEONG::Transform_Com();
	m_Transform->Init();

	m_Transform->m_Transform = m_Transform;

	return true;
}

int JEONG::GameObject::Input(float DeltaTime)
{
	list<JEONG::Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<JEONG::Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			JEONG::Renderer_Com* pRenderer = FindComponentFromType<JEONG::Renderer_Com>(CT_RENDER);
			if (pRenderer != NULLPTR)
			{
				pRenderer->DeleteComponentCBuffer(*StartIter);
				SAFE_RELEASE(pRenderer);
			}

			SAFE_RELEASE((*StartIter));
			StartIter = m_ComponentList.erase(StartIter);
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

	return 0;
}

int JEONG::GameObject::Update(float DeltaTime)
{
	list<JEONG::Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<JEONG::Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			JEONG::Renderer_Com* pRenderer = FindComponentFromType<JEONG::Renderer_Com>(CT_RENDER);
			if (pRenderer != NULLPTR)
			{
				pRenderer->DeleteComponentCBuffer(*StartIter);
				SAFE_RELEASE(pRenderer);
			}

			SAFE_RELEASE((*StartIter));
			StartIter = m_ComponentList.erase(StartIter);
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

	//내꺼 Transform업데이트를 한다
	m_Transform->Update(DeltaTime);

	//자식이 있다면 자식트랜스폼에 부모의 정보가 저장된 행렬과 자식의 Transform정보를 곱하여
	//자식의 Parent행렬에 넣어준다.
	list<GameObject*>::iterator StartIter1 = m_ChildList.begin();
	list<GameObject*>::iterator EndIter1 = m_ChildList.end();

	Matrix S = m_Transform->GetWorldScaleMatrix() * m_Transform->GetParentScale();
	Matrix R = m_Transform->GetWorldRotMatrix() * m_Transform->GetParentRot();
	Matrix T = m_Transform->GetWorldPosMatrix() * m_Transform->GetParentPos();

	for (; StartIter1 != EndIter1; StartIter1++)
	{
		(*StartIter1)->GetTransform()->SetParentScale(S);
		(*StartIter1)->GetTransform()->SetParentRot(R);
		(*StartIter1)->GetTransform()->SetParentPos(T);
		(*StartIter1)->GetTransform()->SetIsUpdate(true);
	}

	return 0;
}

int JEONG::GameObject::LateUpdate(float DeltaTime)
{
	list<JEONG::Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<JEONG::Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			JEONG::Renderer_Com* pRenderer = FindComponentFromType<JEONG::Renderer_Com>(CT_RENDER);
			if (pRenderer != NULLPTR)
			{
				pRenderer->DeleteComponentCBuffer(*StartIter);
				SAFE_RELEASE(pRenderer);
			}

			SAFE_RELEASE((*StartIter));
			StartIter = m_ComponentList.erase(StartIter);
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

	m_Transform->LateUpdate(DeltaTime);

	list<JEONG::GameObject*>::iterator StartIter1 = m_ChildList.begin();
	list<JEONG::GameObject*>::iterator EndIter1 = m_ChildList.end();

	Matrix S = m_Transform->GetWorldScaleMatrix() * m_Transform->GetParentScale();
	Matrix R = m_Transform->GetWorldRotMatrix() * m_Transform->GetParentRot();
	Matrix T = m_Transform->GetWorldPosMatrix() * m_Transform->GetParentPos();

	for (; StartIter1 != EndIter1; StartIter1++)
	{
		(*StartIter1)->GetTransform()->SetParentScale(S);
		(*StartIter1)->GetTransform()->SetParentRot(R);
		(*StartIter1)->GetTransform()->SetParentPos(T);
		(*StartIter1)->GetTransform()->SetIsUpdate(true);
	}

	return 0;
}

void JEONG::GameObject::Collision(float DeltaTime)
{
	//충돌체 추가한다.
	CollsionManager::Get()->AddCollsion(this);
}

void JEONG::GameObject::CollisionLateUpdate(float DeltaTime)
{
	list<JEONG::Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<JEONG::Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter;)
	{
		JEONG::Renderer_Com* pRenderer = FindComponentFromType<JEONG::Renderer_Com>(CT_RENDER);
		if (pRenderer != NULLPTR)
		{
			pRenderer->DeleteComponentCBuffer(*StartIter);
			SAFE_RELEASE(pRenderer);
		}

		if ((*StartIter)->GetIsActive() == false)
		{
			SAFE_RELEASE((*StartIter));
			StartIter = m_ComponentList.erase(StartIter);
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

void JEONG::GameObject::Render(float DeltaTime)
{
	m_Transform->Render(DeltaTime);

	Renderer_Com* getRender = FindComponentFromType<Renderer_Com>(CT_RENDER);
	
	if (getRender != NULLPTR)
	{
		if (getRender->GetIsActive() == false)
		{
			getRender->Release();
			m_ComponentList.remove(getRender);
		}
		else if (getRender->GetIsShow() == true)
		{
			getRender->Render(DeltaTime);
		}

		SAFE_RELEASE(getRender);
	}

	list<Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->GetIsActive() == false)
		{
			if (getRender != NULLPTR)
			{
				getRender->DeleteComponentCBuffer(*StartIter);
				SAFE_RELEASE(getRender);
			}

			SAFE_RELEASE((*StartIter));
			StartIter = m_ComponentList.erase(StartIter);
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
}

GameObject * GameObject::Clone()
{
	return new GameObject(*this);
}

void GameObject::AfterClone()
{
	list<Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (;StartIter != EndIter; StartIter++)
		(*StartIter)->AfterClone();

}

void GameObject::SetScene(Scene * scene)
{
	m_Scene = scene;
	m_Transform->m_Scene = scene;

	list<Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (;StartIter != EndIter; StartIter++)
		(*StartIter)->m_Scene = scene;
}

void GameObject::SetLayer(Layer * layer)
{
	m_Layer = layer;
	m_LayerName = layer->GetTag();
	m_LayerZOrder = layer->GetZOrder();

	m_Transform->m_Layer = layer;

	list<Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (;StartIter != EndIter; StartIter++)
		(*StartIter)->m_Layer = layer;
}

JEONG::GameObject * JEONG::GameObject::CreateObject(const string & TagName, JEONG::Layer * layer, bool isStaticObject)
{
	GameObject* newObject = StaticManager::Get()->FindStaticObject(TagName);

	if (newObject != NULLPTR)
		return newObject;

	newObject = new GameObject();
	newObject->SetTag(TagName);

	if (isStaticObject == true)
		newObject->AddStaticObject();

	if (newObject->Init() == false)
	{
		SAFE_RELEASE(newObject);
		return NULLPTR;
	}

	//해당 레이어에 오브젝트 추가를 해준다.
	if (layer != NULLPTR)
	{
		if(isStaticObject == false)
			layer->AddObject(newObject);

		else
		{
			Scene* pScene = layer->GetScene();
			newObject->SetScene(pScene);
			newObject->SetLayer(layer);
		}
	}

	return newObject;
}

const list<Component_Base*>* GameObject::GetComponentList() const
{
	return &m_ComponentList;
}

bool GameObject::CheckComponentType(COMPONENT_TYPE eType)
{
	list<Component_Base*>::const_iterator StartIter = m_ComponentList.begin();
	list<Component_Base*>::const_iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetComType() == eType)
			return true;
	}
	return false;
}

Component_Base * GameObject::AddComponent(Component_Base * component)
{
	component->m_Scene = m_Scene;
	component->m_Layer = m_Layer;
	component->m_Transform = m_Transform;
	component->m_Object = this;
	component->AddRefCount();

	switch (component->GetComType())
	{
	case JEONG::CT_UI:
		m_RenderGroup = RG_UI;
		break;
	case JEONG::CT_STAGE2D:
		m_RenderGroup = RG_LANDSCAPE;
		break;
	case JEONG::CT_LIGHT:
		m_RenderGroup = RG_LIGHT;
		break;
	}

	m_ComponentList.push_back(component);

	Renderer_Com* pRender = FindComponentFromType<Renderer_Com>(CT_RENDER);

	if (pRender != NULLPTR)
	{
		pRender->CheckComponent();
		SAFE_RELEASE(pRender);
	}

	return component;
}

GameObject * GameObject::CreateProtoType(const string & TagName, bool isCurrent)
{
	Scene* getScene = NULLPTR;

	if (isCurrent == true)
		getScene = SceneManager::Get()->GetCurScene();
	else
		getScene = SceneManager::Get()->GetNextScene();

	GameObject* newProtoType = FindProtoType(getScene, TagName);
	
	if (newProtoType != NULLPTR)
		return NULLPTR;

	unordered_map<JEONG::Scene*, unordered_map<string, GameObject*>>::iterator FindIter = m_ProtoTypeMap.find(getScene);
	unordered_map<string, JEONG::GameObject*>* pMap = NULLPTR;

	if (FindIter == m_ProtoTypeMap.end())
	{
		unordered_map<string, JEONG::GameObject*> TempMap;

		m_ProtoTypeMap.insert(make_pair(getScene, TempMap));

		FindIter = m_ProtoTypeMap.find(getScene);
	}

	pMap = &FindIter->second;
	SAFE_RELEASE(getScene);

	newProtoType = new GameObject();
	newProtoType->SetTag(TagName);

	if (newProtoType->Init() == false)
	{
		SAFE_RELEASE(newProtoType);
		return NULLPTR;
	}

	newProtoType->AddRefCount();

	pMap->insert(make_pair(TagName, newProtoType));

	return newProtoType;
}

GameObject * GameObject::CreateClone(const string & TagName, const string & ProtoTypeTagName, JEONG::Layer * layer, bool isCurrent)
{
	JEONG::Scene* getScene = NULLPTR;

	if (isCurrent == true)
		getScene = SceneManager::Get()->GetCurScene();
	else
		getScene = SceneManager::Get()->GetNextScene();

	GameObject* newCloneObject = FindProtoType(getScene, ProtoTypeTagName);
	SAFE_RELEASE(getScene);

	if (newCloneObject == NULLPTR)
		return NULLPTR;

	GameObject*	pClone = newCloneObject->Clone();
	pClone->SetTag(TagName);
	pClone->AfterClone();

	if (layer != NULLPTR)
		layer->AddObject(pClone);

	return pClone;
}

void GameObject::DestroyProtoType(JEONG::Scene * scene)
{
	unordered_map<Scene*, unordered_map<string, GameObject*>>::iterator FindIter = m_ProtoTypeMap.find(scene);

	if (FindIter == m_ProtoTypeMap.end())
		return;

	Safe_Release_Map(FindIter->second);

	m_ProtoTypeMap.erase(FindIter);
}

void GameObject::DestroyProtoType(Scene * scene, const string & TagName)
{
	unordered_map<Scene*, unordered_map<string, GameObject*>>::iterator FindIter = m_ProtoTypeMap.find(scene);

	if (FindIter == m_ProtoTypeMap.end())
		return;

	unordered_map<string, GameObject*>::iterator FindIter2 = FindIter->second.find(TagName);

	if (FindIter2 == FindIter->second.end())
		return;

	SAFE_RELEASE(FindIter2->second);
	
	FindIter->second.erase(FindIter2);
}

void JEONG::GameObject::DestroyProtoType()
{
	unordered_map<Scene*, unordered_map<string, GameObject*>>::iterator StartIter = m_ProtoTypeMap.begin();
	unordered_map<Scene*, unordered_map<string, GameObject*>>::iterator EndIter = m_ProtoTypeMap.end();

	for (; StartIter != EndIter; StartIter++)
	{
		Safe_Release_Map(StartIter->second);
	}

	m_ProtoTypeMap.clear();
}

void GameObject::DeleteComponent(COMPONENT_TYPE Type)
{
	list<Component_Base*>::iterator	StartIter = m_ComponentList.begin();
	list<Component_Base*>::iterator	EndIter = m_ComponentList.begin();

	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter)->GetComType() == Type)
		{
			SAFE_RELEASE((*StartIter));
			m_ComponentList.erase(StartIter);
			return;
		}
	}
}

GameObject * GameObject::FindProtoType(Scene * scene, const string & TagName)
{
	unordered_map<JEONG::Scene*, unordered_map<string, JEONG::GameObject*>>::iterator FindIter = m_ProtoTypeMap.find(scene);
	
	if (FindIter == m_ProtoTypeMap.end())
		return NULLPTR;

	unordered_map<string, JEONG::GameObject*>::iterator FindIter2 = FindIter->second.find(TagName);

	if (FindIter2 == FindIter->second.end())
		return NULLPTR;

	return FindIter2->second;
}

GameObject * GameObject::FindObject(const string & TagName)
{
	return JEONG::SceneManager::Get()->FindObject(TagName);
}

void GameObject::AddChild(GameObject * Child)
{
	Child->m_Parent = this;

	Child->m_Transform->SetParentFlag(TPF_ROT | TPF_POS);
	Child->AddRefCount();

	m_ChildList.push_back(Child);
	m_Layer->AddObject(Child);
}

void JEONG::GameObject::AddStaticObject()
{
	StaticManager::Get()->AddStaticObject(this);
}

void GameObject::Save(BineryWrite & Writer)
{
}

void GameObject::Load(BineryRead & Reader)
{
}

const list<JEONG::Component_Base*>* JEONG::GameObject::FindComponentFromTag(const string& TagName)
{
	Safe_Release_VecList(m_FindComList);
	m_FindComList.clear();

	list<Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter ; StartIter++)
	{
		if ((*StartIter)->GetTag() == TagName)
		{
			(*StartIter)->AddRefCount();
			m_FindComList.push_back(*StartIter);
		}
	}
	return &m_FindComList;
}

const list<Component_Base*>* GameObject::FindComponentFromType(COMPONENT_TYPE type)
{
	Safe_Release_VecList(m_FindComList);
	m_FindComList.clear();

	list<Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetComType() == type)
		{
			(*StartIter)->AddRefCount();
			m_FindComList.push_back(*StartIter);
		}
	}
	return &m_FindComList;
}

const list<Component_Base*>* GameObject::FindComponentFromTypeNoneCount(COMPONENT_TYPE type)
{
	Safe_Release_VecList(m_FindComList);
	m_FindComList.clear();

	list<Component_Base*>::iterator StartIter = m_ComponentList.begin();
	list<Component_Base*>::iterator EndIter = m_ComponentList.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter)->GetComType() == type)
			m_FindComList.push_back(*StartIter);
	}
	return &m_FindComList;
}

void GameObject::SetTransform(JEONG::Transform_Com* transform)
{
	m_Transform = transform;
}
void GameObject::SetRotation(const Vector3& vecRot)
{
	m_Transform->Rotation(vecRot);	
}

void GameObject::SetRotationX(float RotX)
{
	m_Transform->SetWorldRotX(RotX);
}

void GameObject::SetRotationY(float RotY)
{
	m_Transform->SetWorldRotY(RotY);
}

void JEONG::GameObject::SetRotationZ(float RotZ)
{
	m_Transform->SetWorldRotZ(RotZ);
}
