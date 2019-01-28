#include "EditorHeader.h"
#include "EditScene.h"
#include "MainFrame.h"
#include "EditorForm.h"
#include "resource.h"
#include <Component/FreeCamera_Com.h>

EditScene::EditScene()
{
}

EditScene::~EditScene()
{
	SAFE_RELEASE(m_BackColorCom);
}

bool EditScene::Init()
{
	Camera_Com*	pCamera = m_Scene->GetMainCamera();
	pCamera->SetCameraType(CT_ORTHO);
	pCamera->SetNear(0.0f);

	Layer* BackLayer = m_Scene->FindLayer("BackGround");
	Layer* Default = m_Scene->FindLayer("Default");
	Layer* UILayer = m_Scene->FindLayer("UI");
	Layer* TileLayer = m_Scene->FindLayer("Tile");

	GameObject* BackObject = GameObject::CreateObject("BackObject", BackLayer);
	m_BackColorCom = BackObject->AddComponent<BackColor_Com>("BackColor");

	KeyInput::Get()->AddKey("TileOption", VK_CONTROL);
	KeyInput::Get()->AddKey("TileTab", VK_TAB);
	KeyInput::Get()->AddKey("PrevTileTab", VK_OEM_3);
	KeyInput::Get()->AddKey("CameraZero", VK_SPACE);
	KeyInput::Get()->AddKey("Shift", VK_SHIFT);

	SAFE_RELEASE(BackObject);

	SAFE_RELEASE(BackLayer);
	SAFE_RELEASE(Default);
	SAFE_RELEASE(TileLayer);
	SAFE_RELEASE(UILayer);

	RenderManager::Get()->SetIsZoomMode(true);

	return true;
}

int EditScene::Update(float DeltaTime)
{
	MainFrame* mainFrame = (MainFrame*)AfxGetMainWnd();
	EditorForm*	editorForm = mainFrame->GetEditorForm();

	return 0;
}

