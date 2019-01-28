#pragma once
#include "Collider_Com.h"

JEONG_BEGIN

class JEONG_DLL ColliderCircle_Com : public Collider_Com
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	bool Collsion(Collider_Com* Dest, float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	ColliderCircle_Com* Clone() override;

	CircleInfo GetInfo() const { return m_WorldInfo; }
	void SetInfo(const float& Radius) { m_VirtualInfo.Radius = Radius; }

private:
	CircleInfo m_WorldInfo;
	CircleInfo m_VirtualInfo;

protected:
	ColliderCircle_Com();
	ColliderCircle_Com(const ColliderCircle_Com& CopyData);
	~ColliderCircle_Com();

public:
	friend class GameObject;
};

JEONG_END