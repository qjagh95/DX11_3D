#pragma once
#include "Scene\SceneComponent.h"

JEONG_USING

class StartScene : public SceneComponent
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;

	void StartButtonActive(float DeltaTime);

public:
	StartScene();
	~StartScene();
};

