#include "ClientHeader.h"
#include "EditScene.h"

EditScene::EditScene()
{
}

EditScene::~EditScene()
{
}

bool EditScene::Init()
{
	Layer* BackLayer = m_Scene->FindLayer("BackGround");
	Camera_Com*	mainCamera = m_Scene->GetMainCamera();
	mainCamera->SetCameraType(CT_ORTHO);
	mainCamera->SetNear(0.0f);
	
	GameObject* ColorObject = GameObject::CreateObject("ColorObject", BackLayer);

	Renderer_Com* bgRender = ColorObject->AddComponent<Renderer_Com>("BgRender");
	bgRender->SetMesh("TextureRect");
	bgRender->SetScreenRender(true);
	SAFE_RELEASE(bgRender);

	Material_Com* bgMaterial = ColorObject->FindComponentFromType<Material_Com>(CT_MATERIAL);
	bgMaterial->SetMaterial(Vector4::Gainsboro);

	SAFE_RELEASE(bgMaterial);
	SAFE_RELEASE(ColorObject);
	SAFE_RELEASE(BackLayer);

	return true;
}

int EditScene::Input(float DeltaTime)
{
	return 0;
}

int EditScene::Update(float DeltaTime)
{
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
