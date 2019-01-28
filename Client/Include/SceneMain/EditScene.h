#pragma once
#include "Scene/SceneComponent.h"

JEONG_USING

class EditScene : public SceneComponent
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;

public:
	EditScene();
	~EditScene();
};

