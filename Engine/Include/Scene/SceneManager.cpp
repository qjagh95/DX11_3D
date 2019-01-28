#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"

#include "../KeyInput.h"

JEONG_USING
SINGLETON_VAR_INIT(JEONG::SceneManager)

JEONG::SceneManager::SceneManager()
	:m_CurScene(NULLPTR), m_NextScene(NULLPTR), m_isChange(false)
{
}

JEONG::SceneManager::~SceneManager()
{
	SAFE_RELEASE(m_CurScene);
	SAFE_RELEASE(m_NextScene);
}

bool JEONG::SceneManager::Init()
{
	m_CurScene = new JEONG::Scene();

	if (m_CurScene->Init() == false)
		return false;

	JEONG::KeyInput::Get()->ChangeMouseScene(m_CurScene);

	return true;
}

int JEONG::SceneManager::Input(float DeltaTime)
{
	m_CurScene->Input(DeltaTime);
	return ChangeScene();
}

int JEONG::SceneManager::Update(float DeltaTime)
{
	m_CurScene->Update(DeltaTime);
	return ChangeScene();
}

int JEONG::SceneManager::LateUpdate(float DeltaTime)
{
	m_CurScene->LateUpdate(DeltaTime);
	return ChangeScene();
}

int JEONG::SceneManager::Collision(float DeltaTime)
{
	m_CurScene->Collision(DeltaTime);
	return ChangeScene();
}

int JEONG::SceneManager::CollsionLateUpdate(float DeltaTime)
{
	m_CurScene->CollisionLateUpdate(DeltaTime);
	return ChangeScene();
}

void JEONG::SceneManager::Render(float DeltaTime)
{
	m_CurScene->Render(DeltaTime);
}

JEONG::Scene * JEONG::SceneManager::GetCurScene() const
{
	m_CurScene->AddRefCount();

	return m_CurScene;
}

JEONG::Scene * JEONG::SceneManager::GetNextScene() const
{
	m_NextScene->AddRefCount();

	return m_NextScene;
}

void JEONG::SceneManager::AddLayer(const string & TagName, int ZOrder, bool isCurrent)
{
	if (isCurrent == true)
		m_CurScene->AddLayer(TagName, ZOrder);
	else
		m_NextScene->AddLayer(TagName, ZOrder);
}

void JEONG::SceneManager::ChangeLayerZOrder(const string & TagName, int ZOrder, bool isCurrent)
{
	if (isCurrent == true)
		m_CurScene->ChangeLayerZOrder(TagName, ZOrder);
	else
		m_NextScene->ChangeLayerZOrder(TagName, ZOrder);
}

Layer * JEONG::SceneManager::FindLayer(const string & TagName, bool isCurrent)
{
	if (isCurrent == true)
		return m_CurScene->FindLayer(TagName);
	else
		return m_NextScene->FindLayer(TagName);
}

GameObject * JEONG::SceneManager::FindObject(const string & TagName)
{
	GameObject* getObject = m_CurScene->FindObject(TagName);
	
	if (getObject != NULLPTR)
		return getObject;
	else if (getObject == NULLPTR)
		return NULLPTR;

	return m_NextScene->FindObject(TagName);
}

void JEONG::SceneManager::CreateNextScene(bool isChange)
{
	SAFE_RELEASE(m_NextScene);

	m_NextScene = new JEONG::Scene();
	m_NextScene->Init();
	m_isChange = isChange;
}

void JEONG::SceneManager::SetIsChange(bool isChange)
{
	m_isChange = isChange;
}

int JEONG::SceneManager::ChangeScene()
{
	if (m_NextScene != NULLPTR && m_isChange == true)
	{
		SAFE_RELEASE(m_CurScene);
		m_CurScene = m_NextScene;
		m_NextScene = NULLPTR;
		m_isChange = false;

		JEONG::KeyInput::Get()->ChangeMouseScene(m_CurScene);
		JEONG::StaticManager::Get()->ChangeScene(m_CurScene);
		return 1;
	}

	return 0;
}
