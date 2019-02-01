#pragma once
#include "Component_Base.h"
JEONG_BEGIN

//Transform_Com에서는 World행렬을 만들기위하여 S R T결합만 해준다!
//SIMD 수학연산 Matrix를 사용할것이므로 16바이트 정렬.
class GameObject;
class Component_Base;
class JEONG_DLL __declspec(align(16)) Transform_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Transform_Com* Clone() override;

	Vector3 GetLocalSacle() const { return m_LocalScale; }
	Vector3 GetLocalRotation() const { return m_LocalRotation; }
	Vector3 GetLocalPos() const { return m_LocalPos; }
	Vector3 GetLocalAxis(AXIS eAxis) const { return m_LocalAxis[eAxis]; }
	const Vector3* GetLocalAxis() const { return m_LocalAxis; }
	Matrix GetLocalMatrix()	const { return m_MatLocal; }
	Matrix GetLocalRotMatrix() const { return m_MatWorldRotation; }
	Vector3 GetDeltaMove() const { return m_DeltaMove; }

	Vector3 GetWorldScale() const { return m_WorldScale; }
	Vector3 GetWorldRotation() const { return m_WorldRotation; }
	float GetWorldRotationX() const { return m_WorldRotation.x; }
	float GetWorldRotationY() const { return m_WorldRotation.y; }
	float GetWorldRotationZ() const { return m_WorldRotation.z; }
	Vector3 GetWorldPos() const { return m_WorldPos; }
	Vector3 GetWorldAxis(AXIS eAxis) const { return m_WorldAxis[eAxis]; }
	const Vector3* GetWorldAxis() const { return m_WorldAxis; }
	float GetAngle(GameObject* Target);
	float GetAngle(Transform_Com* Target);
	Vector3 GetPivot() const { return m_Pivot; }
	Matrix GetWorldMatrix()	const { return m_MatWorld; }
	Matrix GetParentMatrix() const { return m_MatParent; }
	Matrix GetWorldPosMatrix() const { return m_MatWorldPos; }
	Matrix GetWorldRotMatrix() const { return m_MatWorldRotation; }
	Matrix GetWorldScaleMatrix() const { return m_MatWorldScale; }

	//안움직이는 놈인가용?
	void SetIsStatic(bool isStatic) { m_isStatic = isStatic; }
	void SetIsUpdate(bool value) { m_isUpdate = value; }

	void SetLocalScale(const Vector3& vScale);
	void SetLocalScale(float x, float y, float z);
	void SetLocalRotation(const Vector3& vRot);
	void SetLocalRotation(float x, float y, float z);
	void SetLocalRotX(float x);
	void SetLocalRotY(float y);
	void SetLocalRotZ(float z);
	void SetLocalPos(const Vector3& vPos);
	void SetLocalPos(float x, float y, float z);
	void ComputeLocalAxis();

	void SetWorldScale(const Vector3& vScale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void SetWorldRot(const Vector3& Rot);
	void SetWorldPos(const Vector3& vPos);
	void SetWorldPos(float x, float y, float z);
	void SetWorldPivot(const Vector3& vPos);
	void SetWorldPivot(float x, float y, float z);
	void ComputeWorldAxis();

	void Move(AXIS eAxis, float Speed);
	void Move(AXIS eAxis, float Speed, float DeltaTime);
	void Move(const Vector3& vDir, float Speed);
	void Move(const Vector3& vDir, float Speed, float DeltaTime);
	void Move(const Vector3& vMove);
	void RotationX(float x);
	void RotationX(float x, float DeltaTime);
	void RotationY(float y);
	void RotationY(float y, float DeltaTime);
	void RotationZ(float z);
	void RotationZ(float z, float DeltaTime);
	void Rotation(const Vector3& vRot, float DeltaTime);
	void Rotation(const Vector3& vRot);

	void LookAt(GameObject* object, AXIS eAxis = AXIS_Z);
	void LookAt(Component_Base* component, AXIS eAxis = AXIS_Z);
	void LookAt(const Vector3& Vec, AXIS eAxis = AXIS_Z);

	void SetParentFlag(int Flag);
	void AddParentFlag(TRANSFORM_PARENT_FLAG Flag);
	void DeleteParentFlag(TRANSFORM_PARENT_FLAG Flag);
	void DeleteParentFlag();

	void ScaleParent();
	void PosParent();

	void SetParentPos(const Matrix& parentPos);
	void SetParentRot(const Matrix& parentRot);
	void SetParentScale(const Matrix& parentScale);

	Matrix GetParentPos() const;
	Matrix GetParentRot() const;
	Matrix GetParentScale() const;

	Matrix GetScaleDelta() const { return m_DeltaScale; }
	Matrix GetPosDelta() const { return m_DeltaPos; }
	Matrix GetRotDelta() const { return m_DeltaRot; }

private:
	//행렬연산은 연산자체가 무겁기 때문에 연산을 해줄때만 해주겠다.
	bool m_isStatic;	///가만히 있는놈은 연산할 필요가 없다
	bool m_isUpdate;	///움직일놈만 업데이트하겠다.
	int m_ParentFlag;

	//Rocal
	Vector3 m_LocalScale;
	Vector3 m_LocalRotation;
	Vector3 m_LocalPos;
	Vector3 m_LocalAxis[AXIS_MAX];

	Matrix m_MatLocalScale;
	Matrix m_MatLocalPos;
	Matrix m_MatLocalRotation;
	Matrix m_MatLocalRotationX;
	Matrix m_MatLocalRotationY;
	Matrix m_MatLocalRotationZ;
	Matrix m_MatLocal;

	//World
	Vector3 m_WorldScale;
	Vector3 m_WorldRotation;
	Vector3 m_WorldPos;
	Vector3 m_WorldAxis[AXIS_MAX];

	Vector3 m_Pivot;
	Vector3 m_DeltaMove;

	Matrix m_MatWorldScale;
	Matrix m_MatWorldPos;
	Matrix m_MatWorldRotation;
	Matrix m_MatWorldRotationX;
	Matrix m_MatWorldRotationY;
	Matrix m_MatWorldRotationZ;
	Matrix m_MatWorld;
	Matrix m_MatParent;

	Matrix m_ParentScale;
	Matrix m_ParentPos;
	Matrix m_ParentRot;

	Matrix m_DeltaScale;
	Matrix m_DeltaRot;
	Matrix m_DeltaPos;

private:
	Transform_Com();
	Transform_Com(const Transform_Com& copyObject);
	~Transform_Com() override;

public:
	friend class GameObject;
};

JEONG_END
