#pragma once
#include "UIBase_Com.h"

JEONG_BEGIN

class CheckBox_Com;
class GameObject;
class JEONG_DLL RadioButton_Com : public UIBase_Com
{
private:
	struct JEONG_DLL RadioSet
	{
		GameObject* RadioObject;
		CheckBox_Com* CheckCom;

		RadioSet();
		~RadioSet();
	};

public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	RadioButton_Com* Clone() override;
	void AfterClone() override;

	void CreateRadioGroup(const string& GroupName ,int Interval, int ButtonCount, Vector3 StartPos, RADIO_DIR dir = RD_RIGHT);
	const string& GetRadioGroupName() const { return m_GroupName; }

private:
	vector<RadioSet*>m_vecRadio;
	string m_GroupName;
	int SelectIndex;
	int PrevSelect = INT_MAX;

protected:
	RadioButton_Com();
	RadioButton_Com(const RadioButton_Com& CopyDaa);
	~RadioButton_Com();
	
public:
	friend class GameObject;
};

JEONG_END
  