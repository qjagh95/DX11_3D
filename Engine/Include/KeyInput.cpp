#include "stdafx.h"
#include "KeyInput.h"
#include "GameObject.h"
#include "Core.h"
#include "Device.h"
#include "CollsionManager.h"

#include "Component/Transform_Com.h"
#include "Component/Renderer_Com.h"
#include "Component/Material_Com.h"
#include "Component/Animation2D_Com.h"
#include "Component/Camera_Com.h"
#include "Component/ColliderPoint_Com.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

JEONG_USING
SINGLETON_VAR_INIT(KeyInput)
bool KeyInput::m_isMosueShow = false;

KeyInput::KeyInput()
	:m_MouseObject(NULLPTR), m_MouseWorldPoint(NULLPTR), m_ShowCursor(false)
{

}

JEONG::KeyInput::~KeyInput()
{
	SAFE_RELEASE(m_MouseWindowPoint);
	SAFE_RELEASE(m_MouseWorldPoint);
	SAFE_RELEASE(m_MouseObject);

	Safe_Delete_Map(m_KeyAxisMap);
	Safe_Delete_Map(m_KeyActionMap);

	if (m_Keyboard)
	{
		m_Keyboard->Unacquire();
		SAFE_RELEASE(m_Keyboard);
	}

	SAFE_RELEASE(m_Input);
}

bool KeyInput::Init()
{
	if (FAILED(DirectInput8Create(Core::Get()->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Input, NULLPTR)))
		return false;

	if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULLPTR)))
		return false;

	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	//창이 활성화 됬을대만 키 옵션을 받는 옵션. (안되넹)
	//if (FAILED(m_Keyboard->SetCooperativeLevel(Core::Get()->GetHwnd(), DISCL_EXCLUSIVE)))
	//	return false;

	if (FAILED(m_Keyboard->Acquire()))
		return false;

	m_MouseObject = JEONG::GameObject::CreateObject("MouseObject");
	m_MouseObject->GetTransform()->SetWorldScale(Vector3(31.0f, 32.0f, 0.0f));
	m_MouseObject->GetTransform()->SetWorldPivot(Vector3(0.0f, 1.0f, 0.0f));

	Renderer_Com* MouseRender = m_MouseObject->AddComponent<Renderer_Com>("MouseRenderer");
	MouseRender->SetMesh("TextureRect");
	MouseRender->SetRenderState(ALPHA_BLEND);
	MouseRender->SetShader(STANDARD_UV_STATIC_SHADER);
	MouseRender->SetScreenRender(true);
	SAFE_RELEASE(MouseRender);

	Material_Com* MouseMeterial = m_MouseObject->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MouseMeterial->SetDiffuseTexture(0, "Mouse", TEXT("Mouse/Default/0.png"));
	MouseMeterial->SetDiffuseSampler(0, LINER_SAMPLER);
	SAFE_RELEASE(MouseMeterial);

	m_MouseWindowPoint = m_MouseObject->AddComponent<ColliderPoint_Com>("MouseWindow");
	m_MouseWindowPoint->SetCollisionGroup("UI");
	m_MouseWindowPoint->SetMyTypeName("MouseWindow");

	m_MouseWorldPoint = m_MouseObject->AddComponent<ColliderPoint_Com>("MouseWorld");
	m_MouseWorldPoint->SetMyTypeName("MouseWorld");

	ShowCursor(TRUE);
	return true;
}

void KeyInput::Update(float DeltaTime)
{
	ReadKeyBoard();

	for (size_t i = 0; i < m_KeyList.size(); ++i)
	{
		unsigned char CurKey = m_KeyList[i];

		if (m_Key[CurKey] & 0x80)
		{
			if (!m_KeyPress[CurKey] && !m_KeyDown[CurKey])
				m_KeyPress[CurKey] = true;
			else
			{
				m_KeyPress[CurKey] = false;
				m_KeyDown[CurKey] = true;
			}
		}

		else if (m_KeyPress[CurKey] || m_KeyDown[CurKey])
		{
			m_KeyPress[CurKey] = false;
			m_KeyDown[CurKey] = false;
			m_KeyUp[CurKey] = true;
		}

		else if (m_KeyUp[CurKey])
			m_KeyUp[CurKey] = false;
	}

	auto iter = m_KeyAxisMap.begin();
	auto iterEnd = m_KeyAxisMap.end();

	for (; iter != iterEnd; ++iter)
	{
		for (size_t i = 0; i < iter->second->KeyList.size(); ++i)
		{
			KeyScale* CurKeyScale = iter->second->KeyList[i];

			float fScale = 0.0f;
			if (m_Key[CurKeyScale->Key] & 0x80 && iter->second->isFunctionBind)
			{
				fScale = CurKeyScale->Scale;
				iter->second->Func(fScale, DeltaTime);
			}
		}
	}

	unsigned char cSKey[SKT_MAX] = { DIK_LSHIFT, DIK_LCONTROL, DIK_LALT };
	bool bSKeyState[SKT_MAX] = {};

	for (int i = 0; i < SKT_MAX; ++i)
	{
		if (m_Key[cSKey[i]] & 0x80)
			bSKeyState[i] = true;
		else
			bSKeyState[i] = false;
	}

	auto iter2 = m_KeyActionMap.begin();
	auto iter2End = m_KeyActionMap.end();

	for (; iter2 != iter2End; ++iter2)
	{
		for (size_t i = 0; i < iter2->second->KeyList.size(); ++i)
		{
			bool bSKeyEnable[SKT_MAX] = { false, false, false };
			ActionKey* CurActionKey = iter2->second->KeyList[i];

			for (int i = 0; i < SKT_MAX; ++i)
			{
				if (CurActionKey->isSKey[i])
				{
					if (bSKeyState[i])
						bSKeyEnable[i] = true;
					else
						bSKeyEnable[i] = false;
				}
				else
				{
					if (bSKeyState[i])
						bSKeyEnable[i] = false;
					else
						bSKeyEnable[i] = true;
				}
			}

			if (m_KeyDown[CurActionKey->Key] && bSKeyEnable[SKT_CONTROL] && bSKeyEnable[SKT_SHIFT] && bSKeyEnable[SKT_ALT])
			{
				if (CurActionKey->Down == false && CurActionKey->Press == false)
				{
					CurActionKey->Down = true;
					CurActionKey->Press = true;
				}
				else
					CurActionKey->Down = false;
			}

			else if (CurActionKey->Down || CurActionKey->Press)
			{
				CurActionKey->Down = false;
				CurActionKey->Press = false;
				CurActionKey->Up = true;
			}
			else if (CurActionKey->Up)
				CurActionKey->Up = false;

			if (iter2->second->KeyState & KEY_DOWN && CurActionKey->Down && iter2->second->isFunctionBind[AT_DOWN])
				iter2->second->Func[AT_DOWN](DeltaTime);

			if (iter2->second->KeyState & KEY_PRESS && CurActionKey->Press && iter2->second->isFunctionBind[AT_PRESS])
				iter2->second->Func[AT_PRESS](DeltaTime);

			if (iter2->second->KeyState & KEY_UP && CurActionKey->Up && iter2->second->isFunctionBind[AT_UP])
				iter2->second->Func[AT_UP](DeltaTime);
		}
	}

	Scene* curScene = SceneManager::Get()->GetCurScene();
	m_CameraPos = curScene->GetMainCameraTransform()->GetWorldPos();

	POINT tempPos;
	RECT ScreenRect;

	GetClientRect(Core::Get()->GetHwnd(), &ScreenRect);
	GetCursorPos(&tempPos);
	ScreenToClient(Core::Get()->GetHwnd(), &tempPos);

	Vector2 DevicePos = Vector2((float)tempPos.x, (float)tempPos.y);

	//좌표계가 반대니까 빼준다.
	DevicePos.y = Device::Get()->GetWinSize().Height - DevicePos.y;

	DevicePos.x *= Device::Get()->GetWindowToDeviceRatio().x;
	DevicePos.y *= Device::Get()->GetWindowToDeviceRatio().y;

	m_MouseGap.x = DevicePos.x - m_MouseScreenPos.x;
	m_MouseGap.y = DevicePos.y - m_MouseScreenPos.y;

	m_MouseScreenPos.x = (float)DevicePos.x;
	m_MouseScreenPos.y = (float)DevicePos.y;

	m_MouseWorldPos.x = DevicePos.x + m_CameraPos.x;
	m_MouseWorldPos.y = DevicePos.y + m_CameraPos.y;

	m_MouseObject->GetTransform()->SetWorldPos((float)m_MouseScreenPos.x, (float)m_MouseScreenPos.y, 0.0f);
	m_MouseObject->Update(DeltaTime);

	if (m_isMosueShow == false)
	{
		if (m_ShowCursor == false && (m_MouseScreenPos.x <= 0.0f && m_MouseScreenPos.x >= Device::Get()->GetWinSize().Width || m_MouseScreenPos.y <= 0.0f && m_MouseScreenPos.y >= Device::Get()->GetWinSize().Height))
		{
			m_ShowCursor = true;
			while (ShowCursor(TRUE) != 0) {}
		}

		else if (m_ShowCursor == true && m_MouseScreenPos.x >= 0.0f && m_MouseScreenPos.x <= Device::Get()->GetWinSize().Width && m_MouseScreenPos.y >= 0.0f && m_MouseScreenPos.y <= Device::Get()->GetWinSize().Height)
		{
			m_ShowCursor = false;
			while (ShowCursor(FALSE) >= 0) {}
		}
	}

	SAFE_RELEASE(curScene);
}

void KeyInput::RenderMouse(float DeltaTime)
{
	m_MouseObject->Render(DeltaTime);
}

Vector3 KeyInput::GetMouseWorldPos() const
{
	return Vector3(m_MouseWorldPos.x, m_MouseWorldPos.y, 0.0f);
}

void KeyInput::ChangeMouseScene(JEONG::Scene * pScene)
{
	m_MouseObject->SetScene(pScene);
}

void KeyInput::UpdateMousePos()
{
	m_MouseWorldPoint->SetInfo(m_CameraPos);
	m_MouseObject->LateUpdate(1.0f);
}

void KeyInput::DeleteActionKey(const string & KeyName)
{
	BindAction* getAction = FindAction(KeyName);

	if (getAction == NULLPTR)
		return;
	
	m_KeyActionMap.erase(KeyName);
}

void KeyInput::DeleteAxisKey(const string & KeyName)
{
	BindAxis* getAxis = FindAxis(KeyName);

	if (getAxis == NULLPTR)
		return;

	m_KeyAxisMap.erase(KeyName);
}

BindAxis* KeyInput::FindAxis(const string& Name)
{
	auto FindIter = m_KeyAxisMap.find(Name);

	if (FindIter == m_KeyAxisMap.end())
		return NULLPTR;

	return FindIter->second;
}

BindAction* KeyInput::FindAction(const string& Name)
{
	auto FindIter = m_KeyActionMap.find(Name);

	if (FindIter == m_KeyActionMap.end())
		return NULLPTR;

	return FindIter->second;
}

bool KeyInput::ReadKeyBoard()
{
	HRESULT	result = m_Keyboard->GetDeviceState(256, m_Key);

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_Keyboard->Acquire();

		else
			return false;
	}

	return true;
}