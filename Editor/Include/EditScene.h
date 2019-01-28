#pragma once
#include <Scene/SceneComponent.h>

class EditScene : public SceneComponent
{
public:
	bool Init() override;
	int Update(float DeltaTime) override;

public:
	EditScene();
	~EditScene();
};

