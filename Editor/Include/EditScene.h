#pragma once
#include <Scene/SceneComponent.h>
#include <Component/BackColor_Com.h>

class EditScene : public SceneComponent
{
public:
	bool Init() override;
	int Update(float DeltaTime) override;

private:
	BackColor_Com* m_BackColorCom;
	wstring m_Path;

public:
	EditScene();
	~EditScene();
};

