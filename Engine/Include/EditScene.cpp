#include "stdafx.h"
#include "EditScene.h"
#include "Component/Camera_Com.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"

#include "Component/Renderer_Com.h"
#include "Component/Transform_Com.h"
#include "GameObject.h"

JEONG_USING

EditScene::EditScene()
{
}


EditScene::~EditScene()
{
}

bool EditScene::Init()
{
	Camera_Com* mainCamera = m_Scene->GetMainCamera();
	mainCamera->SetCameraType(CT_PERSPECTIVE);

	Layer* BackLayer = m_Scene->FindLayer("BackGround");
	Layer* Default = m_Scene->FindLayer("Default");
	Layer* UILayer = m_Scene->FindLayer("UI");
	Layer* TileLayer = m_Scene->FindLayer("Tile");

	GameObject* newObject = GameObject::CreateObject("Pyramid", Default);
	newObject->GetTransform()->SetWorldPos(0.0f, 0.0f, 2.0f);

	Renderer_Com* RenderComponent = newObject->AddComponent<Renderer_Com>("Render");
	//RenderComponent->SetMesh("Player", TEXT("Monster4.fbx"));
	RenderComponent->SetMesh("Pyramid");
	RenderComponent->InitMaterial();

	SAFE_RELEASE(RenderComponent);
	SAFE_RELEASE(newObject);

	SAFE_RELEASE(Default);
	SAFE_RELEASE(UILayer);
	SAFE_RELEASE(TileLayer);
	SAFE_RELEASE(BackLayer);

	SAFE_RELEASE(mainCamera);

	return true;
}

int EditScene::Input(float DeltaTime)
{
	return 0;
}

int EditScene::Update(float DeltaTime)
{
	static Vector3 aa;

	aa += Vector3(0, 0, -1.0f * DeltaTime);
	m_Scene->GetMainCameraTransform()->SetWorldPos(aa);

	return 0;
}

int EditScene::LateUpdate(float DeltaTime)
{
	return 0;
}

void EditScene::Collision(float DeltaTime)
{
}

void EditScene::CollisionLateUpdate(float DeltaTime)
{
}

void EditScene::Render(float DeltaTime)
{
}
