#include "stdafx.h"
#include "Animation2D_Com.h"
#include "Renderer_Com.h"
#include "Material_Com.h"

#include "../GameObject.h"
#include "../PathManager.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"

JEONG_USING

Animation2D_Com::Animation2D_Com()
	:m_CurClip(NULLPTR)
{
	m_ComType = CT_ANIMATION2D;
	m_isEnd = false;
}

Animation2D_Com::Animation2D_Com(const Animation2D_Com& copyData)
{
	m_DefaultClipName = copyData.m_DefaultClipName;
	m_ClipMap.clear();

	unordered_map<string, AnimationClip2D*>::const_iterator	StartIter = copyData.m_ClipMap.begin();
	unordered_map<string, AnimationClip2D*>::const_iterator	EndIter = copyData.m_ClipMap.end();

	for (;StartIter != EndIter ; StartIter++ )
	{
		AnimationClip2D* CopyClip = new AnimationClip2D();

		*CopyClip = *(StartIter->second);
		CopyClip->CurTexture->AddRefCount();

		m_ClipMap.insert(make_pair(StartIter->first, CopyClip));

		if (copyData.m_CurClip->AnimationName == StartIter->first)
			m_CurClip = CopyClip;
	}
}

Animation2D_Com::~Animation2D_Com()
{
	unordered_map<string, AnimationClip2D*>::iterator StartIter = m_ClipMap.begin();
	unordered_map<string, AnimationClip2D*>::iterator EndIter = m_ClipMap.end();

	for (;StartIter != EndIter; StartIter++)
	{
		SAFE_RELEASE(StartIter->second->CurTexture);
		SAFE_DELETE(StartIter->second);
	}

	m_ClipMap.clear();
}

bool Animation2D_Com::Init()
{
	return true;
}

int Animation2D_Com::Input(float DeltaTime)
{
	return 0;
}

int Animation2D_Com::Update(float DeltaTime)
{
	return 0;
}

int Animation2D_Com::LateUpdate(float DeltaTime)
{
	//LimitTime = 1.0 / 10장 = 0.1초
	float FrameTime = m_CurClip->PlayLimitTime / m_CurClip->vecFrame.size();

	m_CurClip->PlayTime += DeltaTime;

	while (m_CurClip->PlayTime >= FrameTime)
	{
		m_CurClip->PlayTime = 0.0f;
		m_CurClip->Frame++;

		if (m_CurClip->Frame >= m_CurClip->vecFrame.size())
		{
			m_isEnd = true;
			m_CurClip->Frame = 0;

			if (m_CurClip->AnimationOption == AO_ONCE_DESTROY)
			{
				m_Object->SetIsActive(false);
				break;
			}
		}
		else
		{
			m_isEnd = false;
		}
	}
	
	//프레임 UV를 계산한다.
	Renderer_Com* getRender = FindComponentFromType<Renderer_Com>(CT_RENDER);

	if (getRender != NULLPTR)
	{
		Animation2DCBuffer aCBuffer = {};
		aCBuffer.Frame = m_CurClip->Frame;

		if (m_CurClip->AnimationType == A2D_ATLS)
		{
			//프레임추출 (이미지 한장한장 사이즈를 전부 벡터에 넣었다)
			Clip2DFrame& FrameRect = m_CurClip->vecFrame[m_CurClip->Frame];
			//UV를 값을 추출한다. (이미지좌표 / 전체사이즈)
			aCBuffer.LeftTopUV.x = FrameRect.LeftTop.x / m_CurClip->TextureWidth;
			aCBuffer.LeftTopUV.y = FrameRect.LeftTop.y / m_CurClip->TextureHeight;
			aCBuffer.RightBottomUV.x = FrameRect.RightBottom.x / m_CurClip->TextureWidth;
			aCBuffer.RightBottomUV.y = FrameRect.RightBottom.y / m_CurClip->TextureHeight;
		}
		else
		{
			//프레임 이미지의 경우 UV는 0부터 1까지 
			aCBuffer.LeftTopUV = Vector2(0.0f, 0.0f);
			aCBuffer.RightBottomUV = Vector2(1.0f, 1.0f);
		}
		//쉐이더로 넘겨준다.(RendererCBuffer에 memcpy만한다)
		getRender->UpdateRendererCBuffer("Animation2D", &aCBuffer, sizeof(Animation2DCBuffer));

		SAFE_RELEASE(getRender);
	}

	return 0;
}

void Animation2D_Com::Collision(float DeltaTime)
{
}

void Animation2D_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Animation2D_Com::Render(float DeltaTime)
{
}

Animation2D_Com * Animation2D_Com::Clone()
{
	return new Animation2D_Com(*this);
}

void Animation2D_Com::AddClip(const string & AnimationName, ANIMATION2D_TYPE Type, ANIMATION_OPTION Option, float PlayLimitTime, const vector<Clip2DFrame>& vecFrame, const string & TexKey, const TCHAR * FileName, const string & PathKey)
{
	AnimationClip2D* newClip = FindClip(AnimationName);

	if (newClip != NULLPTR)
		return;

	ResourceManager::Get()->CreateTexture(TexKey, FileName, PathKey);

	newClip = new AnimationClip2D();
	newClip->AnimationName = AnimationName;
	newClip->AnimationOption = Option;
	newClip->AnimationType = Type;
	newClip->PlayLimitTime = PlayLimitTime;
	newClip->vecFrame = vecFrame;
	newClip->PlayTime = 0.0f;
	newClip->Frame = 0;
	newClip->CurTexture = ResourceManager::Get()->FindTexture(TexKey);
	newClip->TextureHeight = (float)newClip->CurTexture->GetHeight();
	newClip->TextureWidth = (float)newClip->CurTexture->GetWidth();

	m_ClipMap.insert(make_pair(AnimationName, newClip));

	if (m_DefaultClipName == "")
		m_DefaultClipName = AnimationName;

	if (m_CurClip == NULLPTR)
		m_CurClip = newClip;
}

void Animation2D_Com::SetDefaultClip(const string & ClipName)
{
	m_DefaultClipName = ClipName; 
}

void Animation2D_Com::ChangeClip(const string & AnimationName)
{
	if (m_CurClip->AnimationName == AnimationName)
		return;

	m_CurClip = FindClip(AnimationName);

	m_CurClip->Frame = 0;
	m_CurClip->PlayTime = 0.0f;

	Material_Com* getMeterial = FindComponentFromType<Material_Com>(CT_MATERIAL);

	if (getMeterial != NULLPTR)
	{
		//체인지 텍스쳐가 다를 수 있으니까 바꿔준다.
		getMeterial->SetDiffuseTexture(0, m_CurClip->CurTexture);
		SAFE_RELEASE(getMeterial);
	}
}

AnimationClip2D * Animation2D_Com::FindClip(const string & KeyName)
{
	unordered_map<string, AnimationClip2D*>::iterator FindIter = m_ClipMap.find(KeyName);

	if (FindIter == m_ClipMap.end())
		return NULLPTR;

	return FindIter->second;
}
