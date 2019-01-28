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

#include "../UserComponent/Player_Com.h"
#include "../UserComponent/Bullet_Com.h"
#include "../UserComponent/BulletRot_Com.h"
#include "../UserComponent/BulletBoom_Com.h"

#include "../UserComponent/Monster_Com.h"

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

	GameObject*	pStageObj = GameObject::CreateObject("StageObj", TileLayer);
	Stage2D_Com* pStage = pStageObj->AddComponent<Stage2D_Com>("Stage");
	pStage->CreateTileMap(100, 100, Vector3::Zero, Vector3(160.0f, 80.0f, 1.0f), STT_ISO);
	pStage->SetLineOn(true);
	SAFE_RELEASE(pStage);
	SAFE_RELEASE(pStageObj);

	GameObject* MonsterObject = GameObject::CreateObject("Monster", Default);
	Monster_Com* monster_Com = MonsterObject->AddComponent<Monster_Com>("Monster_Com");
	SAFE_RELEASE(MonsterObject);
	SAFE_RELEASE(monster_Com);

	GameObject* PlayerObject = GameObject::CreateObject("Player", Default, true);
	if (PlayerObject->EmptyComponent() == true)
	{
		Player_Com* player_Com = PlayerObject->AddComponent<Player_Com>("Player_Com");
		SAFE_RELEASE(player_Com);
	}

	mainCamera->SetTarget(PlayerObject);
	SAFE_RELEASE(mainCamera);
	SAFE_RELEASE(PlayerObject);

	GameObject* BulletObject = GameObject::CreateProtoType("Bullet_Clone", false);
	Bullet_Com* bullet_Com = BulletObject->AddComponent<Bullet_Com>("Bullet_Com");
	SAFE_RELEASE(bullet_Com);
	SAFE_RELEASE(BulletObject);

	GameObject* BulletObject2 = GameObject::CreateProtoType("BulletRot_Clone", false);
	BulletRot_Com* bullet_Com2 = BulletObject2->AddComponent<BulletRot_Com>("BulletRot_Com");
	SAFE_RELEASE(BulletObject2);
	SAFE_RELEASE(bullet_Com2);

	GameObject* BoomObject = GameObject::CreateProtoType("Boom", false);
	BulletBoom_Com* bulletBoom_Com = BoomObject->AddComponent<BulletBoom_Com>("Boom_Com");
	SAFE_RELEASE(BoomObject);
	SAFE_RELEASE(bulletBoom_Com);

	GameObject*	TestPixelColl = GameObject::CreateObject("PixelColl", Default);
	Renderer_Com* PixelRenderer = TestPixelColl->AddComponent<Renderer_Com>("PixelCollRenderer");
	PixelRenderer->SetMesh("TextureRect");

	Material_Com* pixelMaterial = TestPixelColl->FindComponentFromType<Material_Com>(CT_MATERIAL);
	pixelMaterial->SetDiffuseTexture(0, "TestPixelColl", TEXT("PixelCollider.bmp"));

	ColliderPixel_Com* ColliderPixel = TestPixelColl->AddComponent<ColliderPixel_Com>("TestPixel");
	Pixel24	tPixel = { 255, 0, 255 };
	ColliderPixel->SetInfo(Vector3(0.0f, 0.0f, 0.0f), tPixel, "PixelCollider.bmp");
	Transform_Com* pTransform = TestPixelColl->GetTransform();
	pTransform->SetWorldPos(500.0f, 200.0f, 0.0f);
	pTransform->SetWorldScale(200.0f, 50.0f, 1.0f);
	SAFE_RELEASE(PixelRenderer);
	SAFE_RELEASE(pixelMaterial);
	SAFE_RELEASE(ColliderPixel);
	SAFE_RELEASE(TestPixelColl);

	GameObject* BarObject = GameObject::CreateObject("TestBar", UILayer);
	BarObject->GetTransform()->SetWorldPos(200.0f, 600.0f, 0.0f);

	m_TestBar = BarObject->AddComponent<UIBar_Com>("HpBar");
	m_TestBar->SetDir(BD_LEFT);
	m_TestBar->SetValue(100.0f);
	m_TestBar->SetScale(100.0f, 30.0f, 0.0f);
	SAFE_RELEASE(BarObject);

	GameObject* SlotObject1 = GameObject::CreateObject("Slot1", UILayer);
	GameObject* SlotObject2 = GameObject::CreateObject("Slot2", UILayer);
	GameObject* SlotObject3 = GameObject::CreateObject("Slot2", UILayer);
	SlotObject1->GetTransform()->SetWorldPos(Vector3(1030.0f, 300.0f, 0.0f));
	SlotObject2->GetTransform()->SetWorldPos(Vector3(1060.0f, 300.0f, 0.0f));
	SlotObject3->GetTransform()->SetWorldPos(Vector3(1090.0f, 300.0f, 0.0f));

	IconSlot_Com* SlotCom1 = SlotObject1->AddComponent<IconSlot_Com>("Slot1");
	SlotCom1->SetSlotIndex(0);
	IconSlot_Com* SlotCom2 = SlotObject2->AddComponent<IconSlot_Com>("Slot2");
	SlotCom2->SetSlotIndex(1);
	IconSlot_Com* SlotCom3 = SlotObject3->AddComponent<IconSlot_Com>("Slot3");
	SlotCom3->SetSlotIndex(2);

	SAFE_RELEASE(SlotObject1);
	SAFE_RELEASE(SlotObject2);
	SAFE_RELEASE(SlotObject3);
	SAFE_RELEASE(SlotCom1);
	SAFE_RELEASE(SlotCom2);
	SAFE_RELEASE(SlotCom3);

	GameObject* IconObject1 = GameObject::CreateObject("Icon1", UILayer);
	GameObject* IconObject2 = GameObject::CreateObject("Icon2", UILayer);
	IconObject1->GetTransform()->SetWorldPos(Vector3(400.0f, 100.0f, 0.0f));
	IconObject2->GetTransform()->SetWorldPos(Vector3(500.0f, 100.0f, 0.0f));

	UICon_Com* IconCom1 = IconObject1->AddComponent<UICon_Com>("Icon1");
	UICon_Com* IconCom2 = IconObject2->AddComponent<UICon_Com>("Icon2");
	SAFE_RELEASE(IconObject1);
	SAFE_RELEASE(IconCom1);
	SAFE_RELEASE(IconCom2);

	Material_Com* material = IconObject2->FindComponentFromType<Material_Com>(CT_MATERIAL);
	material->SetDiffuseTexture(0, "Icon2", TEXT("Icon2.png"));
	SAFE_RELEASE(material);
	SAFE_RELEASE(IconObject2);

	GameObject* TextObject1 = GameObject::CreateObject("Text1", UILayer);
	m_TestText = TextObject1->AddComponent<Text_Com>("Text1");
	m_TestText->SetText(L"¤·¤µ¤·?");
	m_TestText->SetTextType(TRT_UI);
	m_TestText->SetFont(L"±Ã¼­Ã¼", 20.0f);
	m_TestText->SetColor(Vector4::LightPink);
	m_TestText->SetRenderArea(0, 0, 200, 200);
	m_TestText->SetIsShow(true);
	TextObject1->GetTransform()->SetWorldPos(Vector3(300.0f, 300.0f, 0));
	//loadThread->AddLoadingCount();
	SAFE_RELEASE(TextObject1);

	GameObject* newCheck = GameObject::CreateObject("CheckBox", UILayer);
	newCheck->GetTransform()->SetWorldPos(300.0f, 360.0f, 0.0f);
	CheckBox_Com* CheckCom = newCheck->AddComponent<CheckBox_Com>("CheckBox");
	SAFE_RELEASE(newCheck);
	SAFE_RELEASE(CheckCom);

	GameObject* TestObject = GameObject::CreateObject("TestObject", Default);
	TestObject->GetTransform()->SetWorldPos(300.0f, 0.0f, 0.0f);
	TestObject->GetTransform()->SetWorldScale(100.0f, 100.0f, 1.0f);

	Renderer_Com* TestRender = TestObject->AddComponent<Renderer_Com>("TestRender");
	TestRender->SetMesh("TextureRect");
	TestRender->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(TestRender);

	Material_Com* TestMater = TestObject->FindComponentFromType<Material_Com>(CT_MATERIAL);
	TestMater->SetDiffuseTexture(0, "aaa", TEXT("aaa.png"));
	SAFE_RELEASE(TestMater);
	SAFE_RELEASE(TestObject);

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

	if (GetAsyncKeyState(VK_HOME) & 0x8000)
	{
		SceneManager::Get()->CreateNextScene();
		SceneManager::Get()->AddSceneComponent<StaticTestScene>("StaticTestScene", false);
	}

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