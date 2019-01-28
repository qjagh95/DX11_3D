#pragma once

JEONG_BEGIN

class JEONG_DLL GUIManager
{
public:
	void CreateImGui(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceConstext);
	void ImGuiBegin(const char* TitleName);
	void ImGuiEnd();

	bool m_isShow;

public:
	CLASS_IN_SINGLE(GUIManager)
};

JEONG_END