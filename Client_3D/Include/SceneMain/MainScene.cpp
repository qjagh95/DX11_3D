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
	:m_TestBar(NULLPTR)
{
}

MainScene::~MainScene()
{
	SAFE_RELEASE(m_TestBar);
	SAFE_RELEASE(m_TestText);
}

bool MainScene::Init()
{
	Camera_Com* mainCamera = m_Scene->GetMainCamera();
	mainCamera->SetCameraType(CT_ORTHO);
	mainCamera->SetNear(0.0f);

	KeyInput::Get()->SetShowCursor(false);

	Layer* BackLayer = m_Scene->FindLayer("BackGround");
	Layer* Default = m_Scene->FindLayer("Default");
	Layer* UILayer = m_Scene->FindLayer("UI");
	Layer* TileLayer = m_Scene->FindLayer("Tile");

	GameObject* BackObject = GameObject::CreateObject("BackObject", BackLayer);
	BackColor_Com* BackCom = BackObject->AddComponent<BackColor_Com>("BackColor");

	Vector4 A = ExcelManager::Get()->ReadVector4Data("BackColor", 0, 0);
	BackCom->SetBackColor(ExcelManager::Get()->ReadVector4Data("BackColor", 0, 0));

	ExcelManager::Get()->ReadVector4Data("BaclColor", 0, 0);

	SAFE_RELEASE(BackCom);
	SAFE_RELEASE(BackObject);

	SAFE_RELEASE(Default);
	SAFE_RELEASE(UILayer);
	SAFE_RELEASE(TileLayer);
	SAFE_RELEASE(BackLayer);

	return true;
}

int MainScene::Input(float DeltaTime)
{
	if (GetAsyncKeyState(VK_F7) & 0x8000)
		m_TestBar->AddValue(-40.0f * DeltaTime);
	if (GetAsyncKeyState(VK_F8) & 0x8000)
		m_TestBar->AddValue(40.0f * DeltaTime);

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