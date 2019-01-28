#pragma once
JEONG_BEGIN

struct JEONG_DLL KeyInfo
{
	string KeyName;
	vector<__int64> vecKey;
	bool KeyDown;
	bool KeyPress;
	bool KeyUp;
};

class GameObject;
class Scene;
class ColliderPoint_Com;
class JEONG_DLL KeyInput
{
public:
	bool Init();
	void Update(float DeltaTime);
	void RenderMouse(float DeltaTime);
	bool KeyDown(const string& Name);
	bool KeyPress(const string& Name);
	bool KeyUp(const string& Name);

	Vector2 GetMouseScreenPos() const { return m_MouseScreenPos; }
	Vector3 GetMouseWorldPos() const;
	Vector2 GetMouseGap() const { return m_MouseGap; }
	void ChangeMouseScene(Scene* pScene);
	void UpdateMousePos();
	GameObject* GetMouseObject() const { return m_MouseObject; }

	void SetEquipObject(GameObject* object);
	GameObject* GetEquipObject() const { return m_EquipObject; }
	void ResetEquipObject();
	bool GetisEquipObject() const { return m_isEquip; }
	void SetShowCursor(bool Value) { m_isMosueShow = Value; }

	template <typename T>
	bool AddKey(const T& value)
	{
		if (m_NewKey == NULLPTR)
		{
			m_NewKey = new KeyInfo();
			m_NewKey->KeyDown = false;
			m_NewKey->KeyPress = false;
			m_NewKey->KeyUp = false;
		}

		//타입이름 가져옴.
		const char* typeName = typeid(T).name();

		if (strcmp(typeName, "int") == 0 || strcmp(typeName, "char") == 0)
			m_NewKey->vecKey.push_back((__int64)value);
		else
		{
			m_NewKey->KeyName = value;
			m_KeyMap.insert(make_pair(m_NewKey->KeyName, m_NewKey));
		}

		return true;
	}

	template <typename T, typename ... Types>
	bool AddKey(const T& value, Types ... Args)
	{
		if (m_NewKey == NULLPTR)
		{
			m_NewKey = new KeyInfo();
			m_NewKey->KeyDown = false;
			m_NewKey->KeyPress = false;
			m_NewKey->KeyUp = false;
		}

		//타입이름 가져옴.
		const char* typeName = typeid(T).name();

		if (strcmp(typeName, "int") == 0 || strcmp(typeName, "char") == 0)
			m_NewKey->vecKey.push_back((__int64)value);
		else
		{
			m_NewKey->KeyName = value;
			m_KeyMap.insert(make_pair(m_NewKey->KeyName, m_NewKey));
		}

		AddKey(Args...);

		if (m_NewKey != NULLPTR)
			m_NewKey = NULLPTR;

		return true;
	}

private:
	KeyInfo* FindKey(const string& Name);

private:
	unordered_map<string, KeyInfo*> m_KeyMap;
	KeyInfo* m_NewKey;
	Vector2 m_MouseScreenPos;	//화면상의 좌표 (0 ~ 1280, 0 ~ 720)
	Vector2 m_MouseWorldPos;	//화면내의 좌표
	Vector2 m_MouseGap;			//화면상 좌표와 화면 내의 좌표의 차이
	GameObject* m_MouseObject;
	ColliderPoint_Com* m_MouseWorldPoint;
	ColliderPoint_Com* m_MouseWindowPoint;
	bool m_ShowCursor;
	GameObject* m_EquipObject;
	bool m_isEquip;
	Vector3 m_CameraPos;
	static bool m_isMosueShow;

public:
	CLASS_IN_SINGLE(KeyInput)
};

JEONG_END


