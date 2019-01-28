#include "ClientHeader.h"
#include "Core.h"
#include "resource.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include "SceneMain/MainScene.h"
#include "SceneMain/StartScene.h"

JEONG_USING

int APIENTRY wWinMain(_In_ HINSTANCE hIstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	if (Core::Get()->Init(hIstance, 1280, 720, TEXT("JeongRealEngine"), TEXT("JeongRealEngine"), IDI_ICON1, IDI_ICON1) == false)
	{
		Core::Delete();
		return 0;
	}	
	//DLL¿¡¼­ »ý¼ºµÈ ½Ì±ÛÅæ°´Ã¼¿Í ¿©±â¼­ »ý¼ºµÈ ½Ì±ÛÅæ°´Ã¼´Â ¼­·Î ´Ù¸¥°´Ã¼ÀÓ.

	Core::Get()->SetGameMode(GM_3D);

	//MainSceneÃß°¡
	SceneManager::Get()->AddSceneComponent<MainScene>("MainScene");

	//Run¿¡¼­ ¸Þ¼¼Áö ¹«ÇÑ·çÇÁ¸¦(·ÎÁ÷) µ·´Ù.
	//²¨Áö¸é Delete, Result = 0
	//½Ì±ÛÅæ »èÁ¦

	int Result = Core::Get()->Run();
	Core::Delete();

	return Result;
}