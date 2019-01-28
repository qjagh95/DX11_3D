#include "ClientHeader.h"
#include "MainScene.h"
#include "StartScene.h"
#include "StaticTestScene.h"
#include "GameObject.h"

#include "Scene/Scene.h"
#include "scene/Layer.h"

#include "Component/Component_Base.h"
#include "Component/ColliderPixel_Com.h"
#include "Component/Button_Com.h"
#include "Component/UICon_Com.h"
#include "Component/IconSlot_Com.h"
#include "Component/Stage2D_Com.h"
#include "Component/Tile2D_Com.h"
#include "Component/CheckBox_Com.h"
#include "Component/BackColor_Com.h"


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

	GameObject* newObject = GameObject::CreateObject("new", Default);
	//newObject->GetTransform()->SetWorldRotX(-20.0f);
	//newObject->GetTransform()->SetWorldScale(100.0f, 100.0f, 100.0f);

	Renderer_Com* RenderComponent = newObject->AddComponent<Renderer_Com>("Render");
	RenderComponent->SetMesh("Pyramid");
	RenderComponent->SetShader(STANDARD_NORMAL_COLOR_SHADER);

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