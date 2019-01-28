#include "Collider_Com.h"

JEONG_BEGIN

class JEONG_DLL ColliderOBB2D_Com : public Collider_Com
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
	ColliderOBB2D_Com* Clone() override;

	OBB2DInfo GetInfo() const { return m_WorldInfo; }
	void SetInfo(const Vector3& Center, Vector3 Axis[2], float Lenth[2]);
private:
	OBB2DInfo m_Virtual;
	OBB2DInfo m_WorldInfo;

protected:
	ColliderOBB2D_Com();
	ColliderOBB2D_Com(const ColliderOBB2D_Com& CopyCollider);
	~ColliderOBB2D_Com();

public:
	friend class GameObject;
};

JEONG_END