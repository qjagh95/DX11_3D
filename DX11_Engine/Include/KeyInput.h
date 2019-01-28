#pragma once
JEONG_BEGIN

struct JEONG_DLL KeyScale
{
	unsigned char Key;
	float Scale;
};

struct JEONG_DLL BindAxis
{
	string	strName;
	bool isFunctionBind;
	function<void(float, float)> Func;
	vector<KeyScale*> KeyList;

	BindAxis() : isFunctionBind(false) {}
	~BindAxis()	{ Safe_Delete_VecList(KeyList);	}
};

struct JEONG_DLL ActionKey
{
	unsigned char Key;
	bool isSKey[SKT_MAX];
	bool Press;
	bool Down;
	bool Up;

	ActionKey() 
	{
		ZeroMemory(isSKey, sizeof(bool) * SKT_MAX);
		Press = false;
		Down = false;
		Up = false;
	}
};

struct JEONG_DLL BindAction
{
	string	strName;
	bool isFunctionBind[AT_MAX];
	function<void(float)> Func[AT_MAX];
	vector<ActionKey*> KeyList;
	int	KeyState;

	BindAction()
	{
		KeyState = 0;
		memset(isFunctionBind, 0, sizeof(bool) * AT_MAX);
	}

	~BindAction() { Safe_Delete_VecList(KeyList); }
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

	Vector2 GetMouseScreenPos() const { return m_MouseScreenPos; }
	Vector3 GetMouseWorldPos() const;
	Vector2 GetMouseGap() const { return m_MouseGap; }
	void ChangeMouseScene(Scene* pScene);
	void UpdateMousePos();
	GameObject* GetMouseObject() const { return m_MouseObject; }
	void SetShowCursor(bool Value) { m_isMosueShow = Value; }

	BindAxis* FindAxis(const string& Name);
	BindAction* FindAction(const string& Name);

private:
	bool ReadKeyBoard();

private:
	IDirectInput8* m_Input;
	IDirectInputDevice8* m_Keyboard;

	unsigned char m_Key[256];
	bool m_KeyPress[256];
	bool m_KeyDown[256];
	bool m_KeyUp[256];

	vector<unsigned char> m_KeyList;
	unordered_map<string, BindAxis*> m_KeyAxisMap;
	unordered_map<string, BindAction*> m_KeyActionMap;

public:
	void AddKeyScale(const string& strName, unsigned char cKey, float fScale)
	{
		BindAxis* pBind = FindAxis(strName);

		if (!pBind)
		{
			pBind = new BindAxis();
			m_KeyAxisMap.insert(make_pair(strName, pBind));
		}

		KeyScale* pKeyScale = new KeyScale();

		pKeyScale->Scale = fScale;
		pKeyScale->Key = cKey;

		pBind->KeyList.push_back(pKeyScale);

		vector<unsigned char>::iterator	iter;
		vector<unsigned char>::iterator	iterEnd = m_KeyList.end();

		bool	bFind = false;
		for (iter = m_KeyList.begin(); iter != iterEnd; ++iter)
		{
			if (*iter == cKey)
			{
				bFind = true;
				break;
			}
		}

		if (!bFind)
			m_KeyList.push_back(cKey);
	}

	BindAxis* FuncBindAxis(const string& strName, void(*pFunc)(float, float))
	{
		BindAxis* pBind = FindAxis(strName);

		if (!pBind)
		{
			pBind = new BindAxis();

			m_KeyAxisMap.insert(make_pair(strName, pBind));
		}

		pBind->strName = strName;
		pBind->isFunctionBind = true;
		pBind->Func = bind(pFunc, placeholders::_1, placeholders::_2);

		return pBind;
	}

	template <typename T>
	BindAxis* FuncBindAxis(const string& strName, T* pObj, void(T::*pFunc)(float, float))
	{
		BindAxis*	pBind = FindAxis(strName);

		if (!pBind)
		{
			pBind = new BindAxis();

			m_KeyAxisMap.insert(make_pair(strName, pBind));
		}

		pBind->strName = strName;
		pBind->isFunctionBind = true;
		pBind->Func = bind(pFunc, pObj, placeholders::_1, placeholders::_2);

		return pBind;
	}

public:
	void AddKeyAction(const string& strName, unsigned char cKey,
		bool* pSKey = nullptr)
	{
		BindAction*	pBind = FindAction(strName);

		if (!pBind)
		{
			pBind = new BindAction();
			m_KeyActionMap.insert(make_pair(strName, pBind));
		}

		ActionKey*	pKeyAction = new ActionKey;

		pKeyAction->Key = cKey;

		if (pSKey)
			memcpy(pKeyAction->isSKey, pSKey, sizeof(bool) * SKT_MAX);

		pBind->KeyList.push_back(pKeyAction);

		vector<unsigned char>::iterator	iter;
		vector<unsigned char>::iterator	iterEnd = m_KeyList.end();

		bool	bFind = false;
		for (iter = m_KeyList.begin(); iter != iterEnd; ++iter)
		{
			if (*iter == cKey)
			{
				bFind = true;
				break;
			}
		}

		if (!bFind)
			m_KeyList.push_back(cKey);
	}

	BindAction* FuncBindAction(const string& strName, KEY_STATE eKeyState,
		void(*pFunc)(float))
	{
		BindAction*	pBind = FindAction(strName);

		if (!pBind)
		{
			pBind = new BindAction();

			m_KeyActionMap.insert(make_pair(strName, pBind));
		}

		ACTION_KEY_FUNCTION_TYPE	eType;

		switch (eKeyState)
		{
		case KEY_PRESS:
			eType = AT_PRESS;
			break;
		case KEY_DOWN:
			eType = AT_DOWN;
			break;
		case KEY_UP:
			eType = AT_UP;
			break;
		}

		pBind->strName = strName;
		pBind->isFunctionBind[eType] = true;
		pBind->Func[eType] = bind(pFunc, placeholders::_1);
		pBind->KeyState |= eKeyState;

		return pBind;
	}

	template <typename T>
	BindAction* FuncBindAction(const string& strName, KEY_STATE eKeyState,
		T* pObj, void(T::*pFunc)(float))
	{
		BindAction*	pBind = FindAction(strName);

		if (!pBind)
		{
			pBind = new BindAction();

			m_KeyActionMap.insert(make_pair(strName, pBind));
		}

		ACTION_KEY_FUNCTION_TYPE eType;

		switch (eKeyState)
		{
		case KEY_PRESS:
			eType = AT_PRESS;
			break;
		case KEY_DOWN:
			eType = AT_DOWN;
			break;
		case KEY_UP:
			eType = AT_UP;
			break;
		}

		pBind->strName = strName;
		pBind->isFunctionBind[eType] = true;
		pBind->KeyState |= eKeyState;
		pBind->Func[eType] = bind(pFunc, pObj, placeholders::_1);

		return pBind;
	}


private:
	Vector2 m_MouseScreenPos;	//È­¸é»óÀÇ ÁÂÇ¥ (0 ~ 1280, 0 ~ 720)
	Vector2 m_MouseWorldPos;	//È­¸é³»ÀÇ ÁÂÇ¥
	Vector2 m_MouseGap;			//È­¸é»ó ÁÂÇ¥¿Í È­¸é ³»ÀÇ ÁÂÇ¥ÀÇ Â÷ÀÌ
	GameObject* m_MouseObject;
	ColliderPoint_Com* m_MouseWorldPoint;
	ColliderPoint_Com* m_MouseWindowPoint;
	bool m_ShowCursor;
	Vector3 m_CameraPos;
	static bool m_isMosueShow;

public:
	CLASS_IN_SINGLE(KeyInput)
};

JEONG_END