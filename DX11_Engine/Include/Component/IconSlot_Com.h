#pragma once
#include "UIBase_Com.h"

JEONG_BEGIN

class UICon_Com;
class Collider_Com;
class JEONG_DLL IconSlot_Com : public UIBase_Com
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	IconSlot_Com* Clone() override;
	void AfterClone() override;

	void IconHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);
	void IconOut(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);
	void SetSlotIndex(int _index) { m_Index = _index; }

	bool CreateSlot(const string& GroupName, size_t RowCount, size_t ColumCount);

private:
	int m_Index;

protected:
	IconSlot_Com();
	IconSlot_Com(const IconSlot_Com& CopyData);
	~IconSlot_Com();

public:
	friend class GameObject;
	friend class UICon_Com;
	friend class SlotManager;
};

JEONG_END
