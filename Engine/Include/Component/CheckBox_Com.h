#pragma once
#include "Button_Com.h"

JEONG_BEGIN
class Collider_Com;
class JEONG_DLL CheckBox_Com : public Button_Com
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	CheckBox_Com* Clone() override;
	void AfterClone() override;

	void SetCheckBoxCallBack(void*(pFunc)(float)) { m_CheckButtonCallBack = bind(pFunc, placeholders::_1); }
	void SetCheckBoxCallBack(function<void(float)> pFunc) { m_CheckButtonCallBack = pFunc; }
	template<typename T>
	void SetCheckBoxCallBack(void(T::*pFunc)(float), T* Object){ m_CheckButtonCallBack = bind(pFunc, Object, placeholders::_1); }

	void MouseHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);
	void MouseOut(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

	CHECKBOX_STATE GetCheckState() const { return m_CheckState; }
	void SetCheckState(CHECKBOX_STATE state) { m_CheckState = state; }
	BUTTON_STATE GetButtonState() const { return m_ButtonState; }

protected:
	CHECKBOX_STATE m_CheckState;
	CheckBoxCBuffer m_CheckBoxCBuffer;

private:
	function<void(float)> m_CheckButtonCallBack;
	bool m_isOver;

protected:
	CheckBox_Com();
	CheckBox_Com(const CheckBox_Com& CopyData);
	~CheckBox_Com();

public:
	friend class GameObject;
	friend class RadioButton_Com;
};

JEONG_END