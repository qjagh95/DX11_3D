#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class Collider_Com;
class JEONG_DLL TitleBar_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	TitleBar_Com* Clone() override;
	void AfterClone() override;

	void MouseHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);
	void MouseOut(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

private:
	bool m_isOver;
	GameObject* m_TitleObject;

protected:
	TitleBar_Com();
	TitleBar_Com(const TitleBar_Com& CopyData);
	~TitleBar_Com();

public:
	friend class GameObject;
};

JEONG_END


