#pragma once
#include "UIBase_Com.h"

JEONG_BEGIN

class IconSlot_Com;
class Collider_Com;
class JEONG_DLL UICon_Com : public UIBase_Com
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	UICon_Com* Clone() override;
	void AfterClone() override;

	void Active();
	
	void MouseHit(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);
	void MouseOut(Collider_Com* Src, Collider_Com* Dest, float DeltaTime);

private:

protected:
	UICon_Com();
	UICon_Com(const UICon_Com& CopyDaa);
	~UICon_Com();

public:
	friend class GameObject;
	friend class IconSlot_Com;
	friend class SlotManager;
};

JEONG_END
