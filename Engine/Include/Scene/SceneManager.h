#pragma once
JEONG_BEGIN

class Scene;
class Layer;
class GameObject;
class JEONG_DLL SceneManager
{
public:
	bool Init();
	int Input(float DeltaTime);
	int Update(float DeltaTime);
	int LateUpdate(float DeltaTime);
	int Collision(float DeltaTime);
	int CollsionLateUpdate(float DeltaTime);
	void Render(float DeltaTime);

	Scene* GetCurScene() const;
	Scene* GetNextScene() const;
	void CreateNextScene(bool isChange = true);
	void SetIsChange(bool isChange);
	void AddLayer(const string& TagName, int ZOrder, bool isCurrent = true);
	void ChangeLayerZOrder(const string& TagName, int ZOrder, bool isCurrent = true);
	Layer* FindLayer(const string& TagName, bool isCurrent = true);
	GameObject* FindObject(const string& TagName);

	template<typename T>
	bool AddSceneComponent(const string& TagName, bool isCurrent = true)
	{
		if (isCurrent == true)
			return m_CurScene->AddSceneComponent<T>(TagName);
		
		return m_NextScene->AddSceneComponent<T>(TagName);
	}

private:
	int ChangeScene();

private:
	Scene* m_CurScene;
	Scene* m_NextScene;
	bool m_isChange;

public:
	CLASS_IN_SINGLE(SceneManager)
};

JEONG_END