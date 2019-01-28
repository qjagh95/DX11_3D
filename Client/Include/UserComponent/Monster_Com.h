#pragma once
#include "UserComponent/UserComponent_Base.h"
#include "Component/ColliderRect_Com.h"

JEONG_USING

class Monster_Com : public UserComponent_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Monster_Com* Clone() override;

	void BulletHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);
	void BulletRotHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

private:
	GameObject* Target;
	string TargetTag;
	float TimeVar;

protected:
	Monster_Com();
	Monster_Com(const Monster_Com& userCom);
	~Monster_Com();

public:
	friend class GameObject;
};

