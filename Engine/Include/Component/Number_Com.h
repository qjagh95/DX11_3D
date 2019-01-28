#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class JEONG_DLL Number_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Number_Com* Clone() override;
	void AfterClone() override;

	void SetNumber(int Value) { m_ResultNumber = Value; }
	void AddNumber(int Value) { m_ResultNumber += Value; }
	int GetNumber() const { return m_ResultNumber; }

private:
	int m_ResultNumber;

protected:
	Number_Com();
	Number_Com(const Number_Com& CopyData);
	~Number_Com();

public:
	friend class GameObject;
};

JEONG_END