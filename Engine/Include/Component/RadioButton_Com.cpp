#include "stdafx.h"
#include "RadioButton_Com.h"
#include "CheckBox_Com.h"
#include "Transform_Com.h"
#include "Renderer_Com.h"
#include "Material_Com.h"
#include "ColliderRect_Com.h"

#include "../GameObject.h"
#include "../KeyInput.h"

#include "../Scene/Scene.h"
#include "../Scene/Layer.h"

JEONG_USING


RadioButton_Com::RadioSet::RadioSet()
	:RadioObject(NULLPTR), CheckCom(NULLPTR)
{
}

RadioButton_Com::RadioSet::~RadioSet()
{
	SAFE_RELEASE(RadioObject);
	SAFE_RELEASE(CheckCom);
}

RadioButton_Com::RadioButton_Com()
{
	m_UIType = UT_RADIOBUTTON;
	SetTag("RadioButton");
	SelectIndex = INT_MAX;
}

RadioButton_Com::RadioButton_Com(const RadioButton_Com & CopyData)
	:UIBase_Com(CopyData)
{
	for (size_t i = 0; i < CopyData.m_vecRadio.size(); i++)
	{
		RadioSet* newSet = new RadioSet();
			
		GameObject* newObject = CopyData.m_vecRadio[i]->RadioObject;
		CheckBox_Com* newCheck = CopyData.m_vecRadio[i]->CheckCom;

		newSet->CheckCom = newCheck->Clone();
		newSet->RadioObject = newObject->Clone();

		m_vecRadio.push_back(newSet);

		SAFE_RELEASE(newObject);
		SAFE_RELEASE(newCheck);
	}
}

RadioButton_Com::~RadioButton_Com()
{
	for (size_t i = 0; i < m_vecRadio.size(); i++)
		SAFE_DELETE(m_vecRadio[i]);
}

bool RadioButton_Com::Init()
{
	m_Transform->SetWorldScale(16.0f, 16.0f, 0.0f);

	return true;
}

int RadioButton_Com::Input(float DeltaTime)
{
	return 0;
}

int RadioButton_Com::Update(float DeltaTime)
{
	//¤µ¤² ¿Ï¼º
	for (size_t i = 0; i < m_vecRadio.size(); i++)
	{
		if (m_vecRadio[i]->CheckCom->GetCheckState() == CBS_TRUE)
		{
			SelectIndex = (int)i;

			if (SelectIndex < PrevSelect)
				break;
			else
				continue;
		}
	}

	if(PrevSelect != INT_MAX)
		m_vecRadio[PrevSelect]->CheckCom->SetCheckState(CBS_FALSE);
	
	if (SelectIndex != INT_MAX)
	{
		m_vecRadio[SelectIndex]->CheckCom->SetCheckState(CBS_TRUE);
		PrevSelect = SelectIndex;
		SelectIndex = INT_MAX;
	}

	return 0;
}

int RadioButton_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void RadioButton_Com::Collision(float DeltaTime)
{
}

void RadioButton_Com::CollisionLateUpdate(float DeltaTime)
{
}

void RadioButton_Com::Render(float DeltaTime)
{
}

RadioButton_Com * RadioButton_Com::Clone()
{
	return new RadioButton_Com(*this);
}

void RadioButton_Com::AfterClone()
{
}

void RadioButton_Com::CreateRadioGroup(const string & GroupName, int Interval, int ButtonCount, Vector3 StartPos, RADIO_DIR dir)
{
	m_GroupName = GroupName;

	for (size_t i = 0; i < ButtonCount; i++)
	{
		RadioSet* newSet = new RadioSet();
		GameObject* newObject = GameObject::CreateObject("RadioObject", m_Layer);
		CheckBox_Com* newCheck = newObject->AddComponent<CheckBox_Com>("RadioCom");
		Material_Com* getMaterial = newObject->FindComponentFromType<Material_Com>(CT_MATERIAL);
		getMaterial->SetDiffuseTexture(0, "RadioButton", TEXT("RadioButton.png"));

		SAFE_RELEASE(getMaterial);

		newSet->RadioObject = newObject;
		newSet->CheckCom = newCheck;

		float xPos;
		float yPos;

		switch (dir)
		{
			case RD_DOWN:
			{
				yPos = StartPos.y + (float)Interval * (float)i;
				newSet->RadioObject->GetTransform()->SetWorldPos(StartPos.y, yPos, 0.0f);
			}
				break;
			case RD_RIGHT:
			{
				xPos = StartPos.x + (float)Interval * (float)i;
				newSet->RadioObject->GetTransform()->SetWorldPos(xPos, StartPos.y, 0.0f);
			}
				break;
		}

		m_vecRadio.push_back(newSet);
	}
}
