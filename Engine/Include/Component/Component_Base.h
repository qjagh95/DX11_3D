#pragma once
#include "../RefCount.h"
JEONG_BEGIN

class Scene;
class Layer;
class GameObject;
class Transform_Com;
class BineryRead;
class BineryWrite;
class JEONG_DLL Component_Base : public RefCount
{
public:
	virtual bool Init() = 0;
	virtual int Input(float DeltaTime);
	virtual int Update(float DeltaTime);
	virtual int LateUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void CollisionLateUpdate(float DeltaTime);
	virtual void Render(float DeltaTime);
	virtual Component_Base* Clone() = 0;
	virtual void AfterClone() {}
	virtual void Save(BineryWrite& Writer);
	virtual void Load(BineryRead& Reader);

public:
	Scene* GetScene() const { return m_Scene; }
	Layer* GetLayer() const { return m_Layer; }
	GameObject* GetGameObject() const;
	Transform_Com* GetTransform() const;
	COMPONENT_TYPE GetComType() const { return m_ComType; }
	bool CheckComponentFromType(COMPONENT_TYPE eType);

	const list<Component_Base*>* FindComponentFromTag(const string& TagName);
	const list<Component_Base*>* FindComponentFromType(COMPONENT_TYPE type);

	template<typename T>
	T* AddComponent(const string& TagName)
	{
		return m_Object->AddComponent<T>(TagName);
	}

	template<typename T>
	T* FindComponentFromTag(const string& TagName)
	{
		return m_Object->FindComponentFromTag<T>(TagName);
	}

	template<typename T>
	T* FindComponentFromType(COMPONENT_TYPE type)
	{
		return m_Object->FindComponentFromType<T>(type);
	}

protected:
	Scene* m_Scene;
	Layer* m_Layer;
	GameObject* m_Object;
	Transform_Com* m_Transform;
	COMPONENT_TYPE m_ComType;

protected:
	Component_Base();
	Component_Base(const Component_Base& copyObject);
	virtual ~Component_Base() = 0;

public:
	friend class GameObject;
};

JEONG_END