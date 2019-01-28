#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class JEONG_DLL UIBase_Com : public Component_Base
{
public:
	virtual bool Init() override;
	virtual int Input(float DeltaTime) override;
	virtual int Update(float DeltaTime) override;
	virtual int LateUpdate(float DeltaTime) override;
	virtual void Collision(float DeltaTime) override;
	virtual void CollisionLateUpdate(float DeltaTime) override;
	virtual void Render(float DeltaTime) override;
	virtual UIBase_Com* Clone() override;
	virtual void AfterClone() override {};

	UI_TYPE GetType() const { return m_UIType; }
	void SetUIType(UI_TYPE eType) { m_UIType = eType; }

protected:
	UI_TYPE m_UIType;

protected:
	UIBase_Com();
	UIBase_Com(const UIBase_Com& CopyData);
	virtual ~UIBase_Com();

public:
	friend class GameObject;
};

JEONG_END