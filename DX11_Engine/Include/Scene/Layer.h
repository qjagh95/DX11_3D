#pragma once
#include "../RefCount.h"
JEONG_BEGIN

class Scene;
class GameObject;
class JEONG_DLL Layer : public RefCount
{
public:
	bool Init();
	int Input(float DeltaTime);
	int Update(float DeltaTime);
	int LateUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void CollisionLateUpdate(float DeltaTime);
	void Render(float DeltaTime);
	
	int GetZOrder() const { return m_ZOrder; }
	void SetZOrder(int zorder);
	Scene* GetScene() const { return m_Scene; }

	void AddObject(GameObject* object);
	GameObject* FindObject(const string& TagName);

private:
	Scene* m_Scene;
	int m_ZOrder;

	list<GameObject*> m_ObjectList;

private:
	Layer();
	~Layer();

public: 
	friend class Scene;
};

JEONG_END

