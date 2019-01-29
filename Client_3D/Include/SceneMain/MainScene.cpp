#include "ClientHeader.h"
#include "MainScene.h"
#include "StartScene.h"
#include "StaticTestScene.h"
#include "GameObject.h"

#include "Scene/Scene.h"
#include "scene/Layer.h"

#include "UserComponent/Player.h"

#include <Component/Renderer_Com.h>
#include <Component/Camera_Com.h>
#include <Component/Transform_Com.h>

MainScene::MainScene()

{
}

MainScene::~MainScene()
{

}

bool MainScene::Init()
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
	RenderComponent->SetMesh("SphereVolum");

	GameObject* newObject2 = GameObject::CreateObject("Player", Default);
	Player_Com* newPlayer = newObject2->AddComponent<Player_Com>("Player");

	SAFE_RELEASE(newPlayer);
	SAFE_RELEASE(newObject2);
	SAFE_RELEASE(RenderComponent);
	SAFE_RELEASE(newObject);

	SAFE_RELEASE(Default);
	SAFE_RELEASE(UILayer);
	SAFE_RELEASE(TileLayer);
	SAFE_RELEASE(BackLayer);

	return true;
}

int MainScene::Input(float DeltaTime)
{
	return 0;
}

int MainScene::Update(float DeltaTime)
{
	return 0;
}

int MainScene::LateUpdate(float DeltaTime)
{
	return 0;
}

void MainScene::Collision(float DeltaTime)
{
}

void MainScene::CollisionLateUpdate(float DeltaTime)
{
}

void MainScene::Render(float DeltaTime)
{
}
