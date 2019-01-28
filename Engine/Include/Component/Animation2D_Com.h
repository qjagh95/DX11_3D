#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class Renderer_Com;
class JEONG_DLL Animation2D_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Animation2D_Com* Clone() override;
	void AfterClone() override {}

	void AddClip(const string& AnimationName, ANIMATION2D_TYPE Type, ANIMATION_OPTION Option, float PlayLimitTime, const vector<Clip2DFrame>& vecFrame, const string& TexKey, const TCHAR* FileName, const string& PathKey = TEXTURE_PATH);
	void SetDefaultClip(const string& ClipName);
	void ChangeClip(const string& AnimationName);
	bool GetIsEnd() const { return m_isEnd; }

private:
	unordered_map<string, AnimationClip2D*> m_ClipMap;
	AnimationClip2D* m_CurClip;
	string m_DefaultClipName;
	bool m_isEnd;

private:
	AnimationClip2D* FindClip(const string& KeyName);

private:
	Animation2D_Com();
	Animation2D_Com(const Animation2D_Com& copyData);
	~Animation2D_Com();

public:
	friend class GameObject;
};

JEONG_END

