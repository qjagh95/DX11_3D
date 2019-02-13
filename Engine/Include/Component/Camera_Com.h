#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class Transform_Com;
class JEONG_DLL __declspec(align(16)) Camera_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Camera_Com* Clone() override;

	void SetCameraType(CAMERA_TYPE eType);
	void SetCameraInfo(CAMERA_TYPE eType, float Width, float Height, float ViewAngle, float Near, float Far);
	void SetWidth(float Width);
	void SetHeight(float Height);
	void SetViewAngle(float Angle);
	void SetNear(float Near);
	void SetFar(float Far);

	Matrix GetViewMatrix() const;
	Matrix GetProjection() const;

	void SetTarget(GameObject* pTarget);
	void SetTarget(Component_Base* pTarget);

	float GetFar() const { return m_Far; }

private:
	Matrix m_View;
	Matrix m_Projection;
	CAMERA_TYPE m_CameraType;
	float m_Width;
	float m_Height;
	float m_ViewAngle;
	float m_Near;
	float m_Far;

	Transform_Com* m_Target;

protected:
	Camera_Com();
	Camera_Com(const Camera_Com& CopyData);
	~Camera_Com();

public:
	friend class GameObject;
};

JEONG_END
