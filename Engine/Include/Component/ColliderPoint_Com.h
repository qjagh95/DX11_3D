#pragma once
#include "Collider_Com.h"

JEONG_BEGIN

class JEONG_DLL ColliderPoint_Com : public Collider_Com
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
	ColliderPoint_Com* Clone() override;

	void SetInfo(const Vector3& value) { m_Virtual = value; }
	Vector3 GetInfo() const { return m_WorldInfo; }

private:
	Vector3 m_Virtual;
	Vector3 m_WorldInfo;

protected:
	ColliderPoint_Com();
	ColliderPoint_Com(const ColliderPoint_Com& CopyCollider);
	~ColliderPoint_Com();

public:
	friend class GameObject;
};

JEONG_END

