#include "ClientHeader.h"
#include "StaticTestScene.h"
#include "MainScene.h"
#include "UserComponent/Player_Com.h"

StaticTestScene::StaticTestScene()
{
}

StaticTestScene::~StaticTestScene()
{
}

bool StaticTestScene::Init()
{
	//Layer* Default = m_Scene->FindLayer("Default");
	//Layer* UILayer = m_Scene->FindLayer("UI");
	//Layer* StageLayer = m_Scene->FindLayer("Tile");

	//GameObject* PlayerObject = GameObject::CreateObject("Player", Default, true);

	//if (PlayerObject->EmptyComponent())
	//{
	//	Player_Com* player_Com = PlayerObject->AddComponent<Player_Com>("Player_Com");
	//	SAFE_RELEASE(player_Com);
	//}

	//SAFE_RELEASE(Default);
	//SAFE_RELEASE(StageLayer);
	//SAFE_RELEASE(UILayer);

	return true;
}

int StaticTestScene::Input(float DeltaTime)
{
	return 0;
}

int StaticTestScene::Update(float DeltaTime)
{
	if (GetAsyncKeyState(VK_NEXT) & 0x8000)
	{
		SceneManager::Get()->CreateNextScene();
		SceneManager::Get()->AddSceneComponent<MainScene>("MainScene", false);
	}

	return 0;
}

int StaticTestScene::LateUpdate(float DeltaTime)
{
	return 0;
}

void StaticTestScene::Collision(float DeltaTime)
{
}

void StaticTestScene::CollisionLateUpdate(float DeltaTime)
{
}

void StaticTestScene::Render(float DeltaTime)
{
}
