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
}

bool EditScene::Init()
{
	return true;
}

int EditScene::Update(float DeltaTime)
{
	MainFrame* mainFrame = (MainFrame*)AfxGetMainWnd();
	EditorForm*	editorForm = mainFrame->GetEditorForm();

	return 0;
}

