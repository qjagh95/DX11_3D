#pragma once
#include "UIBase_Com.h"

JEONG_BEGIN

class Collider_Com;
class JEONG_DLL Button_Com : public UIBase_Com
{
public:
	virtual bool Init() override;
	virtual int Input(float DeltaTime) override;
	virtual int Update(float DeltaTime) override;
	virtual int LateUpdate(float DeltaTime) override;
	virtual void Collision(float DeltaTime) override;
	virtual void CollisionLateUpdate(float DeltaTime) override;
	virtual void Render(float DeltaTime) override;
	virtual Button_Com* Clone() override;
	virtual void AfterClone() override;
	
	void Disable();
	void SetNormal();
	void SetStateColor(BUTTON_STATE eState, const Vector4& Color);
	void SetStateColor(BUTTON_STATE eState, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void MouseHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);
	void MouseOut(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

	void SetCallBack(void*(pFunc)(float)) { m_ButtonCallBack = bind(pFunc, placeholders::_1); }
	void SetCallBack(function<void(float)> pFunc) { m_ButtonCallBack = pFunc; }
	template<typename T>
	void SetCallBack(void(T::*pFunc)(float),T* Object)
	{
		m_ButtonCallBack = bind(pFunc, Object, placeholders::_1);
	}

protected:
	BUTTON_STATE m_ButtonState;
	function<void(float)> m_ButtonCallBack;
	Vector4 m_ButtonColor[BS_MAX];
	ButtonCBuffer m_ButtonCBuffer;

protected:
	Button_Com();
	Button_Com(const Button_Com& CopyData);
	virtual ~Button_Com();

public:
	friend class GameObject;
};

JEONG_END
