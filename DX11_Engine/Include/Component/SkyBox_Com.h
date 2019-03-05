#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class JEONG_DLL SkyBox_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	SkyBox_Com* Clone() override;
	void AfterClone() override;

private:
	SkyCBuffer m_CBuffer;

protected:
	SkyBox_Com();
	SkyBox_Com(const SkyBox_Com& CopyData);
	~SkyBox_Com();

public:
	friend class GameObject;
	friend class Scene;
};

JEONG_END