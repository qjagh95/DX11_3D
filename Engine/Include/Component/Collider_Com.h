#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class Shader;
class Mesh;
class RenderState;
class JEONG_DLL Collider_Com : public Component_Base
{
public:
	virtual bool Init() override;
	virtual int Input(float DeltaTime) override;
	virtual int Update(float DeltaTime) override;
	virtual int LateUpdate(float DeltaTime) override;
	virtual void Collision(float DeltaTime) override;
	virtual void CollisionLateUpdate(float DeltaTime) override;
	virtual void Render(float DeltaTime) override;
	virtual bool Collsion(Collider_Com* Dest, float DeltaTime) = 0;
	virtual Collider_Com* Clone() override = 0;

	COLLIDER_TYPE GetCollType() const { return m_CollType; }
	Vector3 GetSectionMin()	const { return m_SectionMin; }
	Vector3 GetSectionMax()	const { return m_SectionMax; }
	bool GetUpdateCollision() const { return m_UpdateCollision; }
	string GetCollisionGroupName() const { return m_CollisionGroupName; }
	const list<int>* GetSectionIndexList() const { return &m_SelectionIndexList; }

	void SetPivot(const Vector3& vPivot) { m_Pivot = vPivot; }
	void SetPivot(float x, float y, float z) { m_Pivot = Vector3(x, y, z); }
	void SetCollisionGroup(const string& CollsionGroupName) { m_CollisionGroupName = CollsionGroupName; }
	void ClearSectionIndex() { m_SelectionIndexList.clear(); }
	void AddCollisionSection(int Section) { m_SelectionIndexList.push_back(Section); }
	void AddPrevCollision(Collider_Com* Dest) { m_PrevCollision.push_back(Dest); }
	bool CheckPrevCollision(Collider_Com* Dest);
	void ErasePrevCollision(Collider_Com* Dest);
	void CheckPrevCollisionInSection(float DeltaTime);
	void PushContinueTypeName(const string& Name) { m_vecContinueTypeName.push_back(Name); }
	void SetMyTypeName(const string& Name) { m_MyTypeName = Name; }
	const string GetMyTypeName() const { return m_MyTypeName; }
	const string GetContinueTypeName(size_t index) const { return m_vecContinueTypeName[index]; }
	size_t GetContinueTypeNameSize() const { return m_vecContinueTypeName.size(); }

	void OnCollsionFirst(Collider_Com* Dest, float DeltaTime);
	void OnCollsionDoing(Collider_Com* Dest, float DeltaTime);
	void OnCollsionEnd(Collider_Com* Dest, float DeltaTime);

	bool CollsionRectToRect(const BoxInfo& Src, const BoxInfo& Dest);
	bool CollsionRectToPoint(const BoxInfo& Src, const Vector3& Dest);
	bool CollsionRectToCircle(const BoxInfo& Src, const CircleInfo& Dest);
	bool CollsionCircleToCircle(CircleInfo& Src, const CircleInfo& Dest);
	bool CollsionCircleToPoint(CircleInfo& Src, Vector3& Dest);
	bool CollsionOBB2DToRect(const OBB2DInfo& Src, const BoxInfo& Dest);
	bool CollsionOBB2DToPoint(const OBB2DInfo& Src, const Vector3& Dest);
	bool CollsionOBB2DToCircle(const OBB2DInfo& Src, const CircleInfo& Dest);
	bool CollsionOBB2DToOBB2D(const OBB2DInfo& Src, const OBB2DInfo& Dest);
	bool CollsionRectToPixel(BoxInfo Src, const PixelInfo& Dest);

	void SetCallback(function<void(Collider_Com*, Collider_Com*, float)> const& _function, COLLSION_CALLBACK_TYPE _type);
	void SetCollsionCallback(COLLSION_CALLBACK_TYPE eType, void(*pFunc)(Collider_Com*, Collider_Com*, float));
	template <typename T>
	void SetCollsionCallback(COLLSION_CALLBACK_TYPE eType, T* pObj, void (T::*pFunc)(Collider_Com*, Collider_Com*, float))
	{
		function<void(Collider_Com*, Collider_Com*, float)>	func;
		func = bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3);
		m_CollisionFunc[eType].push_back(func);
	}

protected:
	COLLIDER_TYPE m_CollType;
	list<Collider_Com*> m_PrevCollision;
	list<function<void(Collider_Com*, Collider_Com*, float)>> m_CollisionFunc[CCT_MAX];
	list<int> m_SelectionIndexList;

	Vector3 m_Pivot;
	Vector3	m_SectionMin;
	Vector3	m_SectionMax;
	string m_CollisionGroupName;
	vector<string> m_vecContinueTypeName;
	string m_MyTypeName;
	bool m_UpdateCollision;

#ifdef _DEBUG
	//디버그모드시 충돌체 출력하기위함.
	Shader*	m_Shader;
	Mesh*	m_Mesh;
	ID3D11InputLayout* m_LayOut;
	RenderState* m_DepthDisable;
	Vector4 m_Color;
#endif 

protected:
	Collider_Com();
	Collider_Com(const Collider_Com& CopyCollider);
	virtual ~Collider_Com() = 0;

public:
	friend class GameObject;
};

JEONG_END