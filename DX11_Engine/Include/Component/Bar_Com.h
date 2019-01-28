#pragma once
#include "Component_Base.h"
JEONG_BEGIN

class Collider_Com;
class ColliderRect_Com;
class JEONG_DLL Bar_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Bar_Com* Clone() override;
	void AfterClone() override;
	
	void SetDir(BAR_DIR dir);
	void SetMinValue(float minValue) { m_MinValue = minValue; }
	void SetMaxValue(float maxValue) { m_MaxValue = maxValue; }
	void SetValue(float Value);
	void AddValue(float Value);
	void SetMinMaxValue(float minValue, float maxValue);

	void SetScale(const Vector3& Scale);
	void SetScale(float x, float y, float z);

	float GetVal() const { return m_Value; }
	float GetPercent() const { return m_Percent; }

	void MouseHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

private:
	BAR_DIR m_Dir;
	float m_MinValue;
	float m_MaxValue;
	float m_Value;
	float m_ValueLenth;
	float m_Percent;

	ColliderRect_Com* m_RectCollider;
	Vector3 m_Scale;

protected:
	Bar_Com();
	Bar_Com(const Bar_Com& CopyData);
	~Bar_Com();

public:
	friend class GameObject;
};

JEONG_END
