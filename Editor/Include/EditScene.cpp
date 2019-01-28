#include "EditorHeader.h"
#include "EditScene.h"
#include "MainFrame.h"
#include "EditorForm.h"
#include "resource.h"
#include <Component/Tile2D_Com.h>
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

	m_BackColorCom->SetBackColor(Vector4((float)editorForm->m_BackColorR, (float)editorForm->m_BackColorG, (float)editorForm->m_BackColorB, (float)editorForm->m_BackColorA));

	Stage2D_Com* TileStage = editorForm->GetStageComponent();

	if (TileStage != NULLPTR)
	{
		if (KeyInput::Get()->KeyPress("LButton"))
		{
			Vector3	MouseWorld = KeyInput::Get()->GetMouseWorldPos();
			ChangeTile(MouseWorld, editorForm, TileStage);
		}

		if (KeyInput::Get()->KeyDown("TileOption"))
		{
			if (editorForm->m_TileOptionBox.GetCurSel() == 0)
				editorForm->m_TileOptionBox.SetCurSel(1);
			else
				editorForm->m_TileOptionBox.SetCurSel(0);
		}
	}

	if (KeyInput::Get()->KeyDown("TileTab"))
	{
		int Index = editorForm->m_TileImageBox.GetCurSel();

		if (editorForm->m_TileImageBox.GetCount() - 1 == Index)
				editorForm->m_TileImageBox.SetCurSel(0);
		else
			editorForm->m_TileImageBox.SetCurSel(Index + 1);

		editorForm->OnCbnSelchangeTileimageselect();
	}


	if (KeyInput::Get()->KeyDown("PrevTileTab"))
	{
		int Index = editorForm->m_TileImageBox.GetCurSel();

		if (editorForm->m_TileImageBox.GetCurSel() == 0)
			editorForm->m_TileImageBox.SetCurSel(editorForm->m_TileImageBox.GetCount() - 1);
		else
			editorForm->m_TileImageBox.SetCurSel(Index - 1);

		editorForm->OnCbnSelchangeTileimageselect();
	}

	if (KeyInput::Get()->KeyDown("CameraZero"))
	{
		FreeCamera_Com* getFree = m_Scene->GetMainCameraObject()->FindComponentFromType<FreeCamera_Com>(CT_FREECAMERA);
		editorForm->AddWorkText(L"카메라가 0, 0위치로 이동합니다");

		getFree->GetTransform()->SetWorldPos(0.0f, 0.0f, 0.0f);

		SAFE_RELEASE(getFree);
	}

	return 0;
}

void EditScene::ChangeTile(const Vector3 & mPos, EditorForm* form, Stage2D_Com* TileStage)
{
	switch (form->GetTileType())
	{
		case STT_TILE:
		{
			if (form->GetTileOption() == T2D_NORMAL)
			{
				form->OnEnChangeCreatetilecount();

				TileStage->SetMoveMesh(mPos, STT_TILE);
				TileStage->SetTileOption(mPos, T2D_NORMAL);
			}
			else
			{
				form->OnEnChangeCreatetilecount();

				TileStage->SetNoMoveMesh(mPos, STT_TILE);
				TileStage->SetTileOption(mPos, T2D_NOMOVE);
			}
		}
		break;

		case STT_ISO:
		{
			if (form->GetTileOption() == T2D_NORMAL)
			{
				form->OnEnChangeCreatetilecount();

				TileStage->SetMoveMesh(mPos, STT_ISO);
				TileStage->SetTileOption(mPos, T2D_NORMAL);
			}
			else
			{
				form->OnEnChangeCreatetilecount();

				TileStage->SetNoMoveMesh(mPos, STT_ISO);
				TileStage->SetTileOption(mPos, T2D_NOMOVE);
			}
		}
		break;
	}
}
