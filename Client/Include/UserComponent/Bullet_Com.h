#pragma once
#include "UserComponent/UserComponent_Base.h"
#include "../ClientHeader.h"
JEONG_USING

class Bullet_Com : public UserComponent_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Bullet_Com* Clone() override;

	void SetMoveSpeed(float Var) { MoveSpeed = Var; }

private:
	float MoveSpeed;

protected:
	Bullet_Com();
	Bullet_Com(const Bullet_Com& userCom);
	~Bullet_Com();

public:
	friend class GameObject;
};

