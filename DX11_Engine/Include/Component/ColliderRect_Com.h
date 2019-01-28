#pragma once
#include "Collider_Com.h"

JEONG_BEGIN

class JEONG_DLL ColliderRect_Com : public Collider_Com
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
	ColliderRect_Com* Clone() override;

	BoxInfo GetInfo() const { return m_WorldInfo; }
	void SetInfo(const Vector3& Min, const Vector3& Max);

private:
	BoxInfo m_Virtual;
	BoxInfo m_WorldInfo;

protected:
	ColliderRect_Com();
	ColliderRect_Com(const ColliderRect_Com& CopyCollider);
	~ColliderRect_Com();

public:
	friend class GameObject;
};

JEONG_END