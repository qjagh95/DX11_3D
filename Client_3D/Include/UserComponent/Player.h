#pragma once
#include <UserComponent/UserComponent_Base.h>

JEONG_USING

class Player_Com : public UserComponent_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Player_Com* Clone() override;
	void AfterClone() override;

public:
	void Move(float Scale, float DeltaTime);
	void Fire(float DeltaTime);
	void Fire1(float DeltaTime);
	void Fire1Release(float DeltaTime);

protected:
	Player_Com();
	Player_Com(const Player_Com& CopyData);
	~Player_Com();

public:
	friend class GameObject;

};

