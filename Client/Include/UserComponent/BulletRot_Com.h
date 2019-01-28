#pragma once
#include "UserComponent/UserComponent_Base.h"
JEONG_BEGIN

class BulletRot_Com : public UserComponent_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	BulletRot_Com* Clone() override;

	void SetTarget(GameObject* target);
	void SetRotAngle(float Var) { RotAngle = Var; }
	void SetMulti(float time) { Time = time; }

private:
	GameObject* Target;
	float RotAngle;
	float MoveSpeed;
	float Time;
	bool isGap;

protected:
	BulletRot_Com();
	BulletRot_Com(const BulletRot_Com& userCom);
	~BulletRot_Com();

public:
	friend class GameObject;
};

JEONG_END

