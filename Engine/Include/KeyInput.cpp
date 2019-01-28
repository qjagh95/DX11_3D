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
SINGLETON_VAR_INIT(JEONG::KeyInput)
bool KeyInput::m_isMosueShow = false;

JEONG::KeyInput::KeyInput()
	:m_NewKey(NULLPTR), m_MouseObject(NULLPTR), m_EquipObject(NULLPTR), m_MouseWorldPoint(NULLPTR) ,m_ShowCursor(false), m_isEquip(false)
{
	
}

JEONG::KeyInput::~KeyInput()
{
	SAFE_RELEASE(m_MouseWindowPoint);
	SAFE_RELEASE(m_MouseWorldPoint);
	SAFE_RELEASE(m_MouseObject);
	SAFE_RELEASE(m_EquipObject);
	Safe_Delete_Map(m_KeyMap);
}

bool JEONG::KeyInput::Init()
{
	AddKey("MoveLeft", 'A');
	AddKey("MoveRight", 'D');
	AddKey("MoveUp", 'W');
	AddKey("MoveDown", 'S');
	AddKey("SystemPause", VK_F12);
	AddKey("LButton", VK_LBUTTON);
	AddKey("RButton", VK_RBUTTON);
	AddKey("MButton", VK_MBUTTON);

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
	SAFE_RELEASE(MouseMeterial);

	m_MouseWindowPoint = m_MouseObject->AddComponent<ColliderPoint_Com>("MouseWindow");
	m_MouseWindowPoint->SetCollisionGroup("UI");
	m_MouseWindowPoint->SetMyTypeName("MouseWindow");

	m_MouseWorldPoint = m_MouseObject->AddComponent<ColliderPoint_Com>("MouseWorld");
	m_MouseWorldPoint->SetMyTypeName("MouseWorld");

	ShowCursor(TRUE);
	return true;
}

void JEONG::KeyInput::Update(float DeltaTime)
{
	JEONG::Scene* curScene = JEONG::SceneManager::Get()->GetCurScene();
	m_CameraPos = curScene->GetMainCameraTransform()->GetWorldPos();

	unordered_map<string, JEONG::KeyInfo*>::iterator StartIter = m_KeyMap.begin();
	unordered_map<string, JEONG::KeyInfo*>::iterator EndIter = m_KeyMap.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		size_t	iCount = 0;
		for (size_t i = 0; i < StartIter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState((int)StartIter->second->vecKey[i]) & 0x8000)
				++iCount;
		}

		if (iCount == StartIter->second->vecKey.size())
		{
			if (StartIter->second->KeyDown == false && StartIter->second->KeyPress == false)
			{
				StartIter->second->KeyDown = true;
				StartIter->second->KeyPress = true;
			}

			else if (StartIter->second->KeyDown == true)
				StartIter->second->KeyDown = false;
		}

		else if (StartIter->second->KeyPress == true)
		{
			StartIter->second->KeyUp = true;
			StartIter->second->KeyDown = false;
			StartIter->second->KeyPress = false;
		}

		else if (StartIter->second->KeyUp == true)
			StartIter->second->KeyUp = false;
	}

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

void JEONG::KeyInput::RenderMouse(float DeltaTime)
{
	//m_MouseObject->Render(DeltaTime);
}

Vector3 KeyInput::GetMouseWorldPos() const
{
	return Vector3(m_MouseWorldPos.x, m_MouseWorldPos.y, 0.0f);
}

void JEONG::KeyInput::ChangeMouseScene(JEONG::Scene * pScene)
{
	m_MouseObject->SetScene(pScene);
}

void JEONG::KeyInput::UpdateMousePos()
{
	m_MouseWorldPoint->SetInfo(m_CameraPos);
	m_MouseObject->LateUpdate(1.0f);
}

bool JEONG::KeyInput::KeyDown(const string & Name)
{
	JEONG::KeyInfo* getKey = FindKey(Name);

	if (getKey == NULLPTR)
		return false;

	return getKey->KeyDown;
}

bool JEONG::KeyInput::KeyPress(const string & Name)
{
	JEONG::KeyInfo* getKey = FindKey(Name);

	if (getKey == NULLPTR)
		return false;

	return getKey->KeyPress;
}

bool JEONG::KeyInput::KeyUp(const string & Name)
{
	JEONG::KeyInfo* getKey = FindKey(Name);

	if (getKey == NULLPTR)
		return false;

	return getKey->KeyUp;
}
   
JEONG::KeyInfo* JEONG::KeyInput::FindKey(const string& Name)
{
	unordered_map<string, JEONG::KeyInfo*>::iterator FindIter = m_KeyMap.find(Name);

	if (FindIter == m_KeyMap.end())
		return NULLPTR;
	
	return FindIter->second;
}
void JEONG::KeyInput::SetEquipObject(JEONG::GameObject * object)
{
	m_EquipObject = object;
	m_isEquip = true;
}

void JEONG::KeyInput::ResetEquipObject()
{
	if (m_EquipObject != NULLPTR)
	{
		m_EquipObject = NULLPTR;
		m_isEquip = false;
	}
}