#include "stdafx.h"
#include "GUIManager.h"

JEONG_USING
SINGLETON_VAR_INIT(JEONG::GUIManager)
using namespace ImGui;

JEONG::GUIManager::GUIManager()
{
	m_isShow = true;
}

JEONG::GUIManager::~GUIManager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

void JEONG::GUIManager::CreateImGui(HWND hWnd, ID3D11Device * Device, ID3D11DeviceContext * DeviceConstext)
{
#ifdef _DEBUG
	IMGUI_CHECKVERSION();
	CreateContext();
	StyleColorsClassic();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(Device, DeviceConstext);
#endif
}

void JEONG::GUIManager::ImGuiBegin(const char * TitleName)
{
	if (m_isShow == false)
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	NewFrame();

	Begin(TitleName);
}

void JEONG::GUIManager::ImGuiEnd()
{
	if (m_isShow == false)
		return;

	End();
	 
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(GetDrawData());
}