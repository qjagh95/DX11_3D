#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class JEONG_DLL FreeCamera_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void Render(float DeltaTime) override;
	FreeCamera_Com* Clone() override;

	void SetCameraSpeed(float Speed) { m_Speed = Speed; }

private:
	float m_Speed;

protected:
	FreeCamera_Com();
	FreeCamera_Com(const FreeCamera_Com& CopyData);
	~FreeCamera_Com();

public:
	friend class GameObject;
};

JEONG_END
