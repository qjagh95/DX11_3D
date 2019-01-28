#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class JEONG_DLL Light_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Light_Com* Clone() override;
	void AfterClone() override;

protected:
	Light_Com();
	Light_Com(const Light_Com& CopyData);
	~Light_Com();

public:
	friend class GameObject;
};

JEONG_END