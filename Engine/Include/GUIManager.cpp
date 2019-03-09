#include "stdafx.h"
#include "GUIManager.h"
#include "GameObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Component/Transform_Com.h"
#include "Component/Material_Com.h"

JEONG_USING
SINGLETON_VAR_INIT(GUIManager)
using namespace ImGui;

GUIManager::GUIManager()
{
	m_isShow = true;
}

GUIManager::~GUIManager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

void GUIManager::CreateImGui(HWND hWnd, ID3D11Device * Device, ID3D11DeviceContext * DeviceConstext)
{
	IMGUI_CHECKVERSION();
	CreateContext();
	StyleColorsClassic();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(Device, DeviceConstext);
}

void GUIManager::ImGuiBegin(const char * TitleName)
{
	if (m_isShow == false)
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	NewFrame();

	Begin(TitleName);

	Debug();
}

void GUIManager::ImGuiEnd()
{
	if (m_isShow == false)
		return;

	End();
	 
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(GetDrawData());
}

void GUIManager::Debug()
{
	//static float RotX;
	//static float RotY;
	//static float RotZ;

	//static Vector3 Pos = Vector3(0.0f, 1.0f, 1.0f);

	//Scene* getScene = SceneManager::Get()->GetCurScene();
	//GameObject* getObject = getScene->FindObject("Pyramid");

	//if (getObject == NULLPTR)
	//{
	//	SAFE_RELEASE(getScene);
	//	SAFE_RELEASE(getObject);
	//	return;
	//}

	//Material_Com* getMat = getObject->FindComponentFromTypeNoneCount<Material_Com>(CT_MATERIAL);
	//ImGui::Text("Pyramid");

	//ImGui::SliderFloat("RotX", &RotX, 0.0f, 10.0f);
	//ImGui::SliderFloat("RotY", &RotY, 0.0f, 10.0f);
	//ImGui::SliderFloat("RotZ", &RotZ, 0.0f, 10.0f);
	//ImGui::SliderFloat4("pDiffuse", (float*)&getMat->m_vecMaterial[0][0]->MatrialInfo.Diffuse, 0.0f, 10.0f);
	//ImGui::SliderFloat4("pAmbient", (float*)&getMat->m_vecMaterial[0][0]->MatrialInfo.Ambient, 0.0f, 10.0f);
	//ImGui::SliderFloat3("pSpecular", (float*)&getMat->m_vecMaterial[0][0]->MatrialInfo.Specular, 0.0f, 10.0f);
	//ImGui::SliderFloat("pSpecularPower", (float*)&getMat->m_vecMaterial[0][0]->MatrialInfo.Specular.w, 0.0f, 100.0f);

	//ImGui::SliderFloat3("PyramidPos", (float*)&Pos, 0.0f, 10.0f);

	//getObject->GetTransform()->RotationZ(RotZ);
	//getObject->GetTransform()->RotationX(RotX);
	//getObject->GetTransform()->RotationY(RotY);
	//getObject->GetTransform()->SetWorldPos(Pos);

	//ImGui::BeginTabBar("Pyramid");
	//ImGui::EndTabBar();

	//SAFE_RELEASE(getObject);
}
