#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class JEONG_DLL NavigationMesh : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	NavigationMesh* Clone() override;
	void AfterClone() override;

protected:
	NavigationMesh();
	NavigationMesh(const NavigationMesh& CopyData);
	~NavigationMesh();

public:
	friend class GameObject;
};

JEONG_END