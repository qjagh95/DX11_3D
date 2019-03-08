#pragma once
#include "Component_Base.h"

JEONG_BEGIN

struct JEONG_DLL Bone
{
	string		strName;
	int			iDepth;
	int			iParentIndex;
	Matrix*		matOffset;
	Matrix*		matBone;
	list<class BoneSocket*>	SocketList;
	int			iRefCount;

	Bone() : iRefCount(1) {}
};

struct JEONG_DLL KeyFrame
{
	double	dTime;
	Vector3	vPos;
	Vector3	vScale;
	Vector4	vRot;
};

struct JEONG_DLL BoneKeyFrame
{
	int	iBoneIndex;
	vector<KeyFrame*> vecKeyFrame;
	int	iRefCount;

	BoneKeyFrame() : iRefCount(1) { }
	~BoneKeyFrame() {	Safe_Delete_VecList(vecKeyFrame); }
};

struct JEONG_DLL AnimationCallback
{
	int		iAnimationProgress;
	float	fAnimationProgress;
	function<void(float)> func;
	bool	bCall;
};

struct JEONG_DLL AnimationClip
{
	ANIMATION_OPTION	eOption;
	string strName;
	float fStartTime;
	float fEndTime;
	float fTimeLength;
	float fFrameTime;
	float fPlayTime;
	int iStartFrame;
	int iEndFrame;
	int iFrameLength;
	int iFrameMode;
	int iChangeFrame;
	vector<BoneKeyFrame*>		vecKeyFrame;
	vector<AnimationCallback*>	vecCallback;

	AnimationClip() : 
		eOption(AO_LOOP),
		strName(""),
		iFrameMode(0),
		fStartTime(0),
		fEndTime(0),
		fTimeLength(0),
		iStartFrame(0),
		iEndFrame(0),
		iFrameLength(0),
		fPlayTime(1.0f)
	{
	}

	~AnimationClip()
	{
		for (size_t i = 0; i < vecKeyFrame.size(); ++i)
		{
			--vecKeyFrame[i]->iRefCount;

			if (vecKeyFrame[i]->iRefCount == 0)
				SAFE_DELETE(vecKeyFrame[i]);
		}

		vecKeyFrame.clear();
	}
};

class JEONG_DLL Animation3D_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Animation3D_Com* Clone() override;
	void AfterClone() override;

	bool CreateBoneTexture();
	void AddClip(ANIMATION_OPTION eOption, struct FbxAnimationClip* pClip);
	void AddClip(const string& strName, ANIMATION_OPTION eOption,int iStartFrame, int iEndFrame, float fPlayTime, const vector<BoneKeyFrame*>& vecFrame);
	void AddClip(const TCHAR* pFullPath);
	void AddClipFromMultibyte(const char* pFullPath);
	AnimationClip* FindClip(const string& strName);

	void GetCurrentKeyFrame(vector<BoneKeyFrame*>& vecFrame);

	void ChangeClipKey(const string& strOrigin, const string& strChange);
	Bone* FindBone(const string& strBoneName);
	int FindBoneIndex(const string& strBoneName);
	Matrix GetBoneMatrix(const string& strBoneName);
	bool ChangeClip(const string& strClip);

	bool Save(const string& FileName, const string& strPathKey = MESH_PATH);
	bool SaveFullPath(const TCHAR* FullPath, const string& strPathKey = MESH_PATH);
	bool Load(const string& FileName, const string& strPathKey = MESH_PATH);
	bool LoadFullPath(const TCHAR* FullPath, const string& strPathKey = MESH_PATH);
	bool SaveBone(const string& FileName, const string& strPathKey = MESH_PATH);
	bool SaveBoneFullPath(const TCHAR* FullPath, const string& strPathKey = MESH_PATH);
	bool LoadBone(const string& FileName, const string& strPathKey = MESH_PATH);
	bool LoadBoneFullPath(const TCHAR* FullPath, const string& strPathKey = MESH_PATH);
	bool LoadBoneAndAnimationFullPath(const TCHAR* pFullPath);
	bool ModifyClip(const string& strKey, const string& strChangeKey, ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame, float fPlayTime, const vector<BoneKeyFrame*>& vecFrame);
	bool DeleteClip(const string& strKey);
	void GetClipTagList(vector<string>* _vecstrList);

	void ReturnDefaultClip() { ChangeClip(m_pDefaultClip->strName); }
	const list<string>* GetAddClipName() const { return &m_strAddClipName; }
	ID3D11ShaderResourceView** GetBoneTexture() { return &m_pBoneRV; }
	void AddBone(Bone* pBone) { m_vecBones.push_back(pBone); }
	bool IsAnimationEnd()	const { return m_bEnd; }
	AnimationClip* GetCurrentClip()	const { return m_pCurClip; }

private:
	vector<Bone*> m_vecBones;
	ID3D11Texture2D* m_pBoneTex;
	ID3D11ShaderResourceView* m_pBoneRV;
	unordered_map<string, AnimationClip*>	m_mapClip;
	list<string> m_strAddClipName;
	AnimationClip* m_pDefaultClip;
	AnimationClip* m_pCurClip;
	AnimationClip* m_pNextClip;
	bool m_bEnd;
	float m_fAnimationGlobalTime;
	float m_fClipProgress;
	float m_fChangeTime;
	float m_fChangeLimitTime;
	float m_fFrameTime;
	int	m_iFrameMode;
	vector<Matrix*> m_vecBoneMatrix;
	Vector3 m_vBlendScale;
	Vector4 m_vBlendRot;
	Vector3 m_vBlendPos;

private:
	void LoadFbxAnimation(const char* pFullPath);

protected:
	Animation3D_Com();
	Animation3D_Com(const Animation3D_Com& CopyData);
	~Animation3D_Com();

public:
	friend class GameObject;
	friend class Mesh;
};

JEONG_END
