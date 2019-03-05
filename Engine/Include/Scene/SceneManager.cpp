#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"

#include "../KeyInput.h"

JEONG_USING
SINGLETON_VAR_INIT(JEONG::SceneManager)

SceneManager::SceneManager()
	:m_CurScene(NULLPTR), m_NextScene(NULLPTR), m_isChange(false)
{
}

SceneManager::~SceneManager()
{
	SAFE_RELEASE(m_CurScene);
	SAFE_RELEASE(m_NextScene);
}

bool SceneManager::Init()
{
	m_CurScene = new Scene();

	if (m_CurScene->Init() == false)
		return false;

	KeyInput::Get()->ChangeMouseScene(m_CurScene);

	return true;
}

int SceneManager::Input(float DeltaTime)
{
	m_CurScene->Input(DeltaTime);
	return ChangeScene();
}

int SceneManager::Update(float DeltaTime)
{
	m_CurScene->Update(DeltaTime);
	return ChangeScene();
}

int SceneManager::LateUpdate(float DeltaTime)
{
	m_CurScene->LateUpdate(DeltaTime);
	return ChangeScene();
}

int SceneManager::Collision(float DeltaTime)
{
	m_CurScene->Collision(DeltaTime);
	return ChangeScene();
}

int SceneManager::CollsionLateUpdate(float DeltaTime)
{
	m_CurScene->CollisionLateUpdate(DeltaTime);
	return ChangeScene();
}

void SceneManager::Render(float DeltaTime)
{
	m_CurScene->Render(DeltaTime);
}

Scene * SceneManager::GetCurScene() const
{
	return m_CurScene;
}

Scene * SceneManager::GetCurSceneNoneCount() const
{
	return m_CurScene;
}

Scene * SceneManager::GetNextScene() const
{
	m_NextScene->AddRefCount();

	return m_NextScene;
}

void SceneManager::AddLayer(const string & TagName, int ZOrder, bool isCurrent)
{
	if (isCurrent == true)
		m_CurScene->AddLayer(TagName, ZOrder);
	else
		m_NextScene->AddLayer(TagName, ZOrder);
}

void SceneManager::ChangeLayerZOrder(const string & TagName, int ZOrder, bool isCurrent)
{
	if (isCurrent == true)
		m_CurScene->ChangeLayerZOrder(TagName, ZOrder);
	else
		m_NextScene->ChangeLayerZOrder(TagName, ZOrder);
}

Layer * SceneManager::FindLayer(const string & TagName, bool isCurrent)
{
	if (isCurrent == true)
		return m_CurScene->FindLayer(TagName);
	else
		return m_NextScene->FindLayer(TagName);
}

GameObject * SceneManager::FindObject(const string & TagName)
{
	GameObject* getObject = m_CurScene->FindObject(TagName);
	
	if (getObject != NULLPTR)
		return getObject;
	else if (getObject == NULLPTR)
		return NULLPTR;

	return m_NextScene->FindObject(TagName);
}

void SceneManager::CreateNextScene(bool isChange)
{
	SAFE_RELEASE(m_NextScene);

	m_NextScene = new Scene();
	m_NextScene->Init();
	m_isChange = isChange;
}

void SceneManager::SetIsChange(bool isChange)
{
	m_isChange = isChange;
}

int SceneManager::ChangeScene()
{
	if (m_NextScene != NULLPTR && m_isChange == true)
	{
		SAFE_RELEASE(m_CurScene);
		m_CurScene = m_NextScene;
		m_NextScene = NULLPTR;
		m_isChange = false;

		KeyInput::Get()->ChangeMouseScene(m_CurScene);
		StaticManager::Get()->ChangeScene(m_CurScene);
		return 1;
	}

	return 0;
}
