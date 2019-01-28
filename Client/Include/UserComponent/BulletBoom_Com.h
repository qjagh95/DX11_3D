#pragma once
#include "UserComponent/UserComponent_Base.h"
#include "Component/Animation2D_Com.h"

JEONG_USING

class BulletBoom_Com : public UserComponent_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	BulletBoom_Com* Clone() override;

protected:
	BulletBoom_Com();
	BulletBoom_Com(const BulletBoom_Com& userCom);
	~BulletBoom_Com();

	Animation2D_Com* myAnimation;

public:
	friend class GameObject;
};

