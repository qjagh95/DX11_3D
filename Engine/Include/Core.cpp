#include "stdafx.h"
#include "Core.h"
#include "Device.h"
#include "Timer.h"

#include "Resource/Mesh.h"

#include "Render\Shader.h"


JEONG_USING
SINGLETON_VAR_INIT(Core)
bool Core::m_isLoop = true;
WPARAM Core::m_wParam;

Core::Core()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(390);  
	ZeroMemory(ClearColor, sizeof(float) * 4);
	PathManager::Get();
}

Core::~Core()
{
	Device::Delete();
	ResourceManager::Delete();
	ShaderManager::Delete();
	PathManager::Delete();
	SceneManager::Delete();
	TimeManager::Delete();
	CollsionManager::Delete();
	KeyInput::Delete();
	FontManager::Delete();
	SoundManager::Delete();
	StaticManager::Delete();
	RenderManager::Delete();
	ExcelManager::Delete();

	CoUninitialize();
}

bool Core::Init(HINSTANCE hInst, unsigned int Width, unsigned int Height, const TCHAR * TitleName, const TCHAR * ClassName, int iIconID, int iSmallIconID, bool bWindowMode)
{
	m_hIstance = hInst;
	m_WinSize.Width = Width;
	m_WinSize.Height = Height;

	Register(ClassName, iIconID ,iSmallIconID);
	CreateWnd(TitleName, ClassName);

	LoadLibrary(TEXT("libxl.dll"));
	
	return Init(m_hIstance,m_hWnd, Width, Height, bWindowMode);
}

bool Core::Init(HINSTANCE hInst, HWND hWnd, unsigned int Width, unsigned int Height, bool bWindowMode)
{
	m_hIstance = hInst;
	m_hWnd = hWnd;
	m_WinSize.Width = Width;
	m_WinSize.Height = Height;

	//컴객체 초기화.
	CoInitializeEx(NULLPTR, COINIT_MULTITHREADED);

	//DirectX11 Device초기화
	if (Device::Get()->Init(hWnd, Width, Height, bWindowMode) == false)
	{
		TrueAssert(true);
		return false;
	}

	if (PathManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (SoundManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (TimeManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (ResourceManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (RenderManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (KeyInput::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (SceneManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (CollsionManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (FontManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (StaticManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	if (ExcelManager::Get()->Init() == false)
	{
		TrueAssert(true);
		return false;
	}

	SetClearColor(0, 150, 255, 0);

	return true;
}

int Core::Run()
{
	MSG msg;

	while (m_isLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();

			if (KeyInput::Get()->KeyDown("SystemPause"))
				TrueAssert(true);
		}
	}
	return (int)msg.wParam;
}

void Core::Logic()
{
	Timer* getTimer = TimeManager::Get()->FindTimer("MainTimer");
	getTimer->Update();

	float Time = getTimer->GetDeltaTime();

	Input(Time);
	Update(Time);
	LateUpdate(Time);
	Collsion(Time);
	CollsionLateUpdate(Time);
	Render(Time);
}

void Core::SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	ClearColor[0] = r / 255.0f;
	ClearColor[1] = g / 255.0f;
	ClearColor[2] = b / 255.0f;
	ClearColor[3] = a / 255.0f;
}

void Core::Register(const TCHAR * ClassName, int iIconID, int iSmallIconID)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Core::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hIstance;
	wcex.hIcon = LoadIcon(m_hIstance, MAKEINTRESOURCE(iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULLPTR;
	wcex.lpszClassName = ClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iSmallIconID));

	RegisterClassEx(&wcex);
}

void Core::CreateWnd(const TCHAR * TitleName, const TCHAR * ClassName)
{
	m_hWnd = CreateWindow(ClassName, TitleName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, m_WinSize.Width, m_WinSize.Height, NULLPTR, NULLPTR, m_hIstance, NULLPTR);

	if (!m_hWnd)
		return;

	RECT rc = { 0, 0, (LONG)m_WinSize.Width, (LONG)m_WinSize.Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			m_isLoop = false;
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int Core::Input(float DeltaTime)
{
	KeyInput::Get()->Update(DeltaTime);
	SceneManager::Get()->Input(DeltaTime);
	return 0;
}

int Core::Update(float DeltaTime)
{
	SceneManager::Get()->Update(DeltaTime);

	if (KeyInput::Get()->KeyDown("SystemPause"))
		system("pause");

	return 0;
}

int Core::LateUpdate(float DeltaTime)
{
	SceneManager::Get()->LateUpdate(DeltaTime);
	return 0;
}

int Core::Collsion(float DeltaTime)
{
	SceneManager::Get()->Collision(DeltaTime);
	
	return 0;
}

int Core::CollsionLateUpdate(float DeltaTime)
{
	SceneManager::Get()->CollsionLateUpdate(DeltaTime);
	return 0;
}

void Core::Render(float DeltaTime)
{
	Device::Get()->Clear(ClearColor);
	{		
		SceneManager::Get()->Render(DeltaTime);
		RenderManager::Get()->Render(DeltaTime);

		KeyInput::Get()->RenderMouse(DeltaTime);
	}
	Device::Get()->Present();
}

void Core::SetGameMode(GAME_MODE eMode)
{
	m_GameMode = eMode;
	RenderManager::Get()->SetGameMode(eMode);
}
