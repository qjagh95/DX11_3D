#pragma once
#include "Collider_Com.h"

JEONG_BEGIN

class JEONG_DLL ColliderPixel_Com : public Collider_Com
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
	ColliderPixel_Com* Clone() override;

	PixelInfo GetInfo() const { return m_WorldInfo; }
	void SetInfo(const Vector3& Min, const Pixel24& OutColor, const char* FileName, const string& PathKey = TEXTURE_PATH);

private:
	PixelInfo m_Virtual;
	PixelInfo m_WorldInfo;

protected:
	ColliderPixel_Com();
	ColliderPixel_Com(const ColliderPixel_Com& CopyCollider);
	~ColliderPixel_Com();

public:
	friend class GameObject;
};

JEONG_END
