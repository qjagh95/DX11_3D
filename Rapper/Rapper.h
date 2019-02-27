#pragma once

using namespace System;
#include <Scene/SceneManager.h>
#include <Scene/Scene.h>
#include <Type.h>
#include <Flag.h>
#include <EditScene.h>

namespace Rapper 
{
	public ref class CoreRapper
	{
	public:
		void Init(IntPtr hWnd)
		{
			auto window = hWnd.ToInt64();
			JEONG::Core::Get()->EditInit(0, (HWND)window, 1024, 480, true);

			JEONG::Core::Get()->SetGameMode(JEONG::GM_3D);
			JEONG::SceneManager::Get()->AddSceneComponent<JEONG::EditScene>("EditScene");
		}
		void Logic()
		{
			JEONG::Core::Get()->Logic();
		}

		void Delete()
		{
			JEONG::Core::Get()->EditDelete();
		}
	};
}
