#include "Button_Com.h"

JEONG_BEGIN

class ColliderRect_Com;
class JEONG_DLL UIBar_Com : public Button_Com
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	UIBar_Com* Clone() override;
	void AfterClone() override;

	void SetDir(BAR_DIR dir);
	void SetMinValue(float minValue) { m_MinValue = minValue; }
	void SetMaxValue(float maxValue) { m_MaxValue = maxValue; }
	void SetValue(float Value);
	void AddValue(float Value);
	void SetMinMaxValue(float minValue, float maxValue);

	void SetScale(const Vector3& Scale);
	void SetScale(float x, float y, float z);

	void MouseHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

private:
	BAR_DIR m_Dir;
	float m_MinValue;
	float m_MaxValue;
	float m_Value;
	float m_ValueLenth;
	float m_Percent;

	ColliderRect_Com* m_RectCollider; //마우스를 올렸을때 HP표시..를 위함.
	Vector3 m_Scale;

protected:
	UIBar_Com();
	UIBar_Com(const UIBar_Com& CopyData);
	~UIBar_Com();

public:
	friend class GameObject;
};

JEONG_END