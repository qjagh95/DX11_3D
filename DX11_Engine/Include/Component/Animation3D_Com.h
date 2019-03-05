#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class JEONG_DLL Animation3D_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Animation3D_Com* Clone() override;
	void AfterClone() override;

protected:
	Animation3D_Com();
	Animation3D_Com(const Animation3D_Com& CopyData);
	~Animation3D_Com();

public:
	friend class GameObject;
};

JEONG_END
