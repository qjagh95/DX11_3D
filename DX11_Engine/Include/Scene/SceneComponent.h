#pragma once
#include "../RefCount.h"
JEONG_BEGIN

class Scene;
class JEONG_DLL SceneComponent : public RefCount
{
public:
	virtual bool Init() = 0;
	virtual int Input(float DeltaTime);
	virtual int Update(float DeltaTime);
	virtual int LateUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void CollisionLateUpdate(float DeltaTime);
	virtual void Render(float DeltaTime);

protected:
	Scene* m_Scene;

protected:
	SceneComponent();
	~SceneComponent();

public:
	friend class Scene;
};

JEONG_END

