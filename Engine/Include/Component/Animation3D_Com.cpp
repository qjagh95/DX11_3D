#include "stdafx.h"
#include "Animation3D_Com.h"
#include "../Resource/FBXLoader.h"

JEONG_USING

Animation3D_Com::Animation3D_Com()
{
	m_ComType = CT_ANIMATION3D;
	m_pBoneTex = NULLPTR;
	m_pBoneRV = NULLPTR;
	m_pCurClip = NULLPTR;
	m_pNextClip = NULLPTR;
	m_bEnd = false;
	m_fAnimationGlobalTime = 0.0f;
	m_fClipProgress = 0.0f;
	m_fChangeTime = 0.0f;
	m_fChangeLimitTime = 0.25f;
	m_fFrameTime = 1.0f / 30.0f;
	m_iFrameMode = 0;
}

Animation3D_Com::Animation3D_Com(const Animation3D_Com& CopyData)
	:Component_Base(CopyData)
{
	m_pNextClip = nullptr;
	m_fAnimationGlobalTime = 0.0f;
	m_fClipProgress = 0.0f;
	m_fChangeTime = 0.0f;
	m_fChangeLimitTime = CopyData.m_fChangeLimitTime;
	m_fFrameTime = CopyData.m_fFrameTime;
	m_iFrameMode = CopyData.m_iFrameMode;

	m_vecBones = CopyData.m_vecBones;

	for (size_t i = 0; i < CopyData.m_vecBones.size(); ++i)
		++m_vecBones[i]->iRefCount;

	m_pBoneTex = nullptr;

	CreateBoneTexture();

	unordered_map<string, AnimationClip*>::const_iterator	iter;
	unordered_map<string, AnimationClip*>::const_iterator	iterEnd = CopyData.m_mapClip.end();

	m_mapClip.clear();

	for (iter = CopyData.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		AnimationClip*	pClip = new AnimationClip();
		*pClip = *iter->second;
		
		pClip->vecCallback.clear();

		if (CopyData.m_pCurClip->strName == iter->first)
			m_pCurClip = pClip;

		if (CopyData.m_pDefaultClip->strName == iter->first)
			m_pDefaultClip = pClip;

		m_mapClip.insert(make_pair(iter->first, pClip));
	}

	m_vecBoneMatrix.clear();
	m_vecBoneMatrix.resize(CopyData.m_vecBoneMatrix.size());

	for (size_t i = 0; i < m_vecBoneMatrix.size(); ++i)
		m_vecBoneMatrix[i] = new Matrix;
}	

Animation3D_Com::~Animation3D_Com()
{

	Safe_Delete_Map(m_mapClip);
	Safe_Delete_VecList(m_vecBoneMatrix);

	SAFE_RELEASE(m_pBoneRV);
	SAFE_RELEASE(m_pBoneTex);

	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		--m_vecBones[i]->iRefCount;

		if (m_vecBones[i]->iRefCount == 0)
		{
			SAFE_DELETE(m_vecBones[i]->matBone);
			SAFE_DELETE(m_vecBones[i]->matOffset);
			SAFE_DELETE(m_vecBones[i]);
		}
	}

	m_vecBones.clear();
}

bool Animation3D_Com::Init()
{
	return true;
}

int Animation3D_Com::Input(float DeltaTime)
{
	return 0;
}

int Animation3D_Com::Update(float DeltaTime)
{
	return 0;
}

int Animation3D_Com::LateUpdate(float DeltaTime)
{
	if (m_mapClip.empty())
		return 0;

	if (m_vecBoneMatrix.empty())
	{
		m_vecBoneMatrix.resize(m_vecBones.size());

		for (size_t i = 0; i < m_vecBoneMatrix.size(); ++i)
			m_vecBoneMatrix[i] = new Matrix;
	}

	m_bEnd = false;

	// 모션이 변할때
	if (m_pNextClip != NULLPTR)
	{
		m_fChangeTime += DeltaTime;

		bool bChange = false;
		if (m_fChangeTime >= m_fChangeLimitTime)
		{
			m_fChangeTime = m_fChangeLimitTime;
			bChange = true;
		}

		float	fAnimationTime = m_fAnimationGlobalTime + m_pCurClip->fStartTime;

		// 본 수만큼 반복한다.
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			// 키프레임이 없을 경우
			if (m_pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
			{
				*m_vecBoneMatrix[i] = *m_vecBones[i]->matBone;
				continue;
			}

			int	iFrameIndex = m_pCurClip->iChangeFrame;
			int	iNextFrameIndex = m_pNextClip->iStartFrame;

			const KeyFrame* pCurKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[iFrameIndex];
			const KeyFrame* pNextKey = m_pNextClip->vecKeyFrame[i]->vecKeyFrame[iNextFrameIndex];

			float	fPercent = m_fChangeTime / m_fChangeLimitTime;

			Vector3 cScale = pCurKey->vScale;
			Vector3 nScale = pNextKey->vScale;
			Vector3 cPos = pCurKey->vPos;
			Vector3 nPos = pNextKey->vPos;
			Vector4 cRot = pCurKey->vRot;
			Vector4 nRot = pNextKey->vRot;

			XMVECTOR vS = XMVectorLerp(cScale.Convert(), nScale.Convert(), fPercent);
			XMVECTOR vT = XMVectorLerp(cPos.Convert(), nPos.Convert(), fPercent);
			XMVECTOR vR = XMQuaternionSlerp(cRot.Convert(), nRot.Convert(), fPercent);

			XMVECTOR vZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);

			//선형변환된 행렬에 이동벡터를 더한 행렬. (아핀행렬변환)
			Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);

			*m_vecBones[i]->matBone = matBone;
			matBone = *m_vecBones[i]->matOffset * matBone;
			*m_vecBoneMatrix[i] = matBone;
		}

		if (bChange)
		{
			m_pCurClip = m_pNextClip;
			m_pNextClip = nullptr;
			m_fAnimationGlobalTime = 0.f;
			m_fChangeTime = 0.f;
		}
	}

	// 기존 모션이 계속 동작될때
	else
	{
		m_fAnimationGlobalTime += DeltaTime;
		m_fClipProgress = m_fAnimationGlobalTime / m_pCurClip->fTimeLength;

		while (m_fAnimationGlobalTime >= m_pCurClip->fTimeLength)
		{
			m_fAnimationGlobalTime -= m_pCurClip->fTimeLength;
			m_bEnd = true;
		}

		float fAnimationTime = m_fAnimationGlobalTime + m_pCurClip->fStartTime;

		int	iStartFrame = m_pCurClip->iStartFrame;
		int	iEndFrame = m_pCurClip->iEndFrame;

		int	iFrameIndex = (int)(fAnimationTime / m_pCurClip->fFrameTime);

		if (m_bEnd)
		{
			switch (m_pCurClip->eOption)
			{
			case AO_LOOP:
				iFrameIndex = iStartFrame;
				break;
			case AO_ONCE_DESTROY:
				m_Object->SetIsActive(false);
				break;
			}
		}

		else
		{
			int	iNextFrameIndex = iFrameIndex + 1;

			m_pCurClip->iChangeFrame = iFrameIndex;

			if (iNextFrameIndex > iEndFrame)
				iNextFrameIndex = iStartFrame;

			// 본 수만큼 반복한다.
			for (size_t i = 0; i < m_vecBones.size(); ++i)
			{
				// 키프레임이 없을 경우
				if (m_pCurClip->vecKeyFrame[i]->vecKeyFrame.empty())
				{
					*m_vecBoneMatrix[i] = *m_vecBones[i]->matBone;
					continue;
				}

				const KeyFrame* pCurKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[iFrameIndex];
				const KeyFrame* pNextKey = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[iNextFrameIndex];

				m_vBlendPos = pCurKey->vPos;
				m_vBlendScale = pCurKey->vScale;
				m_vBlendRot = pCurKey->vRot;

				// 현재 프레임의 시간을 얻어온다.
				double	 dFrameTime = pCurKey->dTime;
				float	fPercent = (float)((fAnimationTime - dFrameTime) / m_pCurClip->fFrameTime);

				Vector3 cScale = pCurKey->vScale;
				Vector3 nScale = pNextKey->vScale;
				Vector3 cPos = pCurKey->vPos;
				Vector3 nPos = pNextKey->vPos;
				Vector4 cRot = pCurKey->vRot;
				Vector4 nRot = pNextKey->vRot;

				XMVECTOR vS = XMVectorLerp(cScale.Convert(), nScale.Convert(), fPercent);
				XMVECTOR vT = XMVectorLerp(cPos.Convert(), nPos.Convert(), fPercent);
				XMVECTOR vR = XMQuaternionSlerp(cRot.Convert(), nRot.Convert(), fPercent);

				XMVECTOR vZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);
				Matrix	matBone = XMMatrixAffineTransformation(vS, vZero, vR, vT);

				*m_vecBones[i]->matBone = matBone;
				matBone = *m_vecBones[i]->matOffset * matBone;

				*m_vecBoneMatrix[i] = matBone;
			}
		}
	}

	if (!m_bEnd)
	{
		//행렬정보를 텍스쳐에다 저장하겠다.
		
		D3D11_MAPPED_SUBRESOURCE	tMap = {};
		Device::Get()->GetContext()->Map(m_pBoneTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

		Matrix*	pMatrix = (Matrix*)tMap.pData;

		for (size_t i = 0; i < m_vecBoneMatrix.size(); ++i)
			pMatrix[i] = *m_vecBoneMatrix[i];

		Device::Get()->GetContext()->Unmap(m_pBoneTex, 0);
	}

	return 0;
}

void Animation3D_Com::Collision(float DeltaTime)
{
}

void Animation3D_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Animation3D_Com::Render(float DeltaTime)
{
}

Animation3D_Com * Animation3D_Com::Clone()
{
	return new Animation3D_Com(*this);
}

void Animation3D_Com::AfterClone()
{
}

bool Animation3D_Com::CreateBoneTexture()
{
	SAFE_RELEASE(m_pBoneTex);

	D3D11_TEXTURE2D_DESC	tDesc = {};
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tDesc.Height = 1;
	tDesc.Width = (UINT)m_vecBones.size() * 4;
	tDesc.MipLevels = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.SampleDesc.Count = 1;

	if (FAILED(Device::Get()->GetDevice()->CreateTexture2D(&tDesc, NULL, &m_pBoneTex)))
		return false;

	if (FAILED(Device::Get()->GetDevice()->CreateShaderResourceView(m_pBoneTex, NULL, &m_pBoneRV)))
		return false;

	return true;
}

void Animation3D_Com::AddClip(ANIMATION_OPTION eOption, FbxAnimationClip * pClip)
{
	AnimationClip*	pAnimClip = FindClip(pClip->strName);

	if (pAnimClip)
		return;

	pAnimClip = new AnimationClip();

	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	pAnimClip->eOption = eOption;
	pAnimClip->strName = pClip->strName;
	pAnimClip->iChangeFrame = 0;

	switch (pClip->eTimeMode)
	{
	case FbxTime::eFrames24:
		pAnimClip->iFrameMode = 24;
		break;
	case FbxTime::eFrames30:
		pAnimClip->iFrameMode = 30;
		break;
	case FbxTime::eFrames60:
		pAnimClip->iFrameMode = 60;
		break;
	}

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	pAnimClip->iStartFrame = 0; //영혁 수정 : Anm에서 매 클립 iStartFrame은 0
	pAnimClip->iEndFrame = (int)(pClip->tEnd.GetFrameCount(pClip->eTimeMode) - pClip->tStart.GetFrameCount(pClip->eTimeMode));
	pAnimClip->iFrameLength = (pAnimClip->iEndFrame) + 1; 

	pAnimClip->fFrameTime = 1.0f / (float)(pAnimClip->iFrameMode); 
	pAnimClip->fStartTime = 0.0f;
	pAnimClip->fEndTime = (pAnimClip->iFrameLength) * (pAnimClip->fFrameTime);
	pAnimClip->fTimeLength = pAnimClip->fEndTime;

	//키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로 뽑아온다.
	for (size_t i = 0; i < pClip->vecBoneKeyFrame.size(); ++i)
	{
		BoneKeyFrame*	pBoneKeyFrame = new BoneKeyFrame;

		pBoneKeyFrame->iBoneIndex = pClip->vecBoneKeyFrame[i].iBoneIndex;
		pAnimClip->vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(pClip->vecBoneKeyFrame[i].vecKeyFrame.size());

		for (size_t j = 0; j < pClip->vecBoneKeyFrame[i].vecKeyFrame.size(); ++j)
		{
			KeyFrame* pKeyFrame = new KeyFrame;
			pKeyFrame->dTime = j * pAnimClip->fFrameTime;

			// 현재 본의 키 프레임에 해당하는 행렬 정보를 얻어온다.
			FbxAMatrix	mat = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;

			FbxVector4 vPos, vScale;
			FbxQuaternion qRot;

			// 행렬로부터 위치, 크기, 회전 정보를 얻어온다.
			vPos = mat.GetT();
			vScale = mat.GetS();
			qRot = mat.GetQ();

			pKeyFrame->vScale = Vector3((float)vScale.mData[0], (float)vScale.mData[1], (float)vScale.mData[2]);
			pKeyFrame->vPos = Vector3((float)vPos.mData[0], (float)vPos.mData[1],(float)vPos.mData[2]);
			pKeyFrame->vRot = Vector4((float)qRot.mData[0], (float)qRot.mData[1], (float)qRot.mData[2], (float)qRot.mData[3]);

			pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);
		}
	}

	if (m_iFrameMode == 0)
	{
		m_iFrameMode = pAnimClip->iFrameMode;
		m_fFrameTime = pAnimClip->fFrameTime;
	}

	if (m_mapClip.empty())
	{
		m_pDefaultClip = pAnimClip;
		m_pCurClip = pAnimClip;
	}

	m_strAddClipName.clear();
	m_strAddClipName.push_back(pAnimClip->strName);
	m_mapClip.insert(make_pair(pAnimClip->strName, pAnimClip));
}


void Animation3D_Com::AddClip(const string & strName, ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame, float fPlayTime, const vector<BoneKeyFrame*>& vecFrame)
{
	AnimationClip*	pAnimClip = FindClip(strName);

	if (pAnimClip)
		return;

	pAnimClip = new AnimationClip();

	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	pAnimClip->eOption = eOption;
	pAnimClip->strName = strName;

	pAnimClip->iChangeFrame = 0;

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	pAnimClip->iStartFrame = 0;
	pAnimClip->iEndFrame = iEndFrame - iStartFrame;
	pAnimClip->iFrameLength = pAnimClip->iEndFrame - pAnimClip->iStartFrame;

	pAnimClip->fPlayTime = fPlayTime;
	pAnimClip->fFrameTime = fPlayTime / pAnimClip->iFrameLength;

	// 시간 정보를 저장해준다.
	pAnimClip->fStartTime = 0.f;
	pAnimClip->fEndTime = pAnimClip->fPlayTime;
	pAnimClip->fTimeLength = pAnimClip->fPlayTime;


	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < vecFrame.size(); ++i)
	{
		BoneKeyFrame*	pBoneKeyFrame = new BoneKeyFrame();

		pBoneKeyFrame->iBoneIndex = vecFrame[i]->iBoneIndex;

		pAnimClip->vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(pAnimClip->iFrameLength);

		if (!vecFrame[i]->vecKeyFrame.empty())
		{
			for (size_t j = iStartFrame; j <= iEndFrame; ++j)
			{
				KeyFrame*	pKeyFrame = new KeyFrame();

				pKeyFrame->dTime = (j - iStartFrame) * pAnimClip->fFrameTime;
				pKeyFrame->vScale = vecFrame[i]->vecKeyFrame[j]->vScale;
				pKeyFrame->vRot = vecFrame[i]->vecKeyFrame[j]->vRot;
				pKeyFrame->vPos = vecFrame[i]->vecKeyFrame[j]->vPos;

				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);
			}
		}
	}

	if (m_mapClip.empty())
	{
		m_pDefaultClip = pAnimClip;
		m_pCurClip = pAnimClip;
	}

	m_strAddClipName.clear();

	m_strAddClipName.push_back(pAnimClip->strName);

	m_mapClip.insert(make_pair(pAnimClip->strName, pAnimClip));
}

void Animation3D_Com::AddClip(const TCHAR * pFullPath)
{
	char	strFullPath[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_UTF8, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath),
		0, 0);
#else
	strcpy_s(strFullPath, pFullPath);
#endif // UNICODE

	AddClipFromMultibyte(strFullPath);
}

void Animation3D_Com::AddClipFromMultibyte(const char * pFullPath)
{
	char	strExt[_MAX_EXT] = {};

	_splitpath_s(pFullPath, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT);
	_strupr_s(strExt);

	if (strcmp(strExt, ".FBX") == 0)
		LoadFbxAnimation(pFullPath);

	else
		Load(pFullPath);
}

AnimationClip * Animation3D_Com::FindClip(const string & strName)
{
	unordered_map<string, AnimationClip*>::iterator	iter = m_mapClip.find(strName);

	if (iter == m_mapClip.end())
		return nullptr;

	return iter->second;
}

void Animation3D_Com::GetCurrentKeyFrame(vector<BoneKeyFrame*>& vecFrame)
{
	for (size_t i = 0; i < m_pCurClip->vecKeyFrame.size(); ++i)
	{
		BoneKeyFrame*	pBoneFrame = new BoneKeyFrame();
		vecFrame.push_back(pBoneFrame);

		pBoneFrame->iBoneIndex = m_pCurClip->vecKeyFrame[i]->iBoneIndex;

		for (size_t j = 0; j < m_pCurClip->vecKeyFrame[i]->vecKeyFrame.size(); ++j)
		{
			KeyFrame*	pFrame = new KeyFrame;

			pFrame->dTime = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[j]->dTime;
			pFrame->vScale = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[j]->vScale;
			pFrame->vPos = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[j]->vPos;
			pFrame->vRot = m_pCurClip->vecKeyFrame[i]->vecKeyFrame[j]->vRot;

			pBoneFrame->vecKeyFrame.push_back(pFrame);
		}
	}
}

void Animation3D_Com::ChangeClipKey(const string & strOrigin, const string & strChange)
{
	unordered_map<string, AnimationClip*>::iterator	iter = m_mapClip.find(strOrigin);

	if (iter == m_mapClip.end())
		return;

	AnimationClip*	pClip = iter->second;

	m_mapClip.erase(iter);
	m_mapClip.insert(make_pair(strChange, pClip));

	if (strOrigin == m_pCurClip->strName)
		m_pCurClip->strName = pClip->strName;

	if (strOrigin == m_pDefaultClip->strName)
		m_pDefaultClip->strName = pClip->strName;
}

Bone * Animation3D_Com::FindBone(const string & strBoneName)
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strBoneName)
			return m_vecBones[i];
	}

	return NULLPTR;
}

int Animation3D_Com::FindBoneIndex(const string & strBoneName)
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		if (m_vecBones[i]->strName == strBoneName)
			return (int)i;
	}

	return -1;
}

Matrix Animation3D_Com::GetBoneMatrix(const string & strBoneName)
{
	int	iBone = FindBoneIndex(strBoneName);

	if (iBone == -1)
		return Matrix();

	return *m_vecBoneMatrix[iBone];
}

bool Animation3D_Com::ChangeClip(const string & strClip)
{
	if (m_pCurClip->strName == strClip)
		return false;

	m_pNextClip = FindClip(strClip);

	if (m_pNextClip == NULLPTR)
		return false;

	return true;
}

bool Animation3D_Com::Save(const string& FileName, const string & PathKey)
{
	BineryWrite Writer(FileName);

	Writer.WriteData(m_iFrameMode);
	Writer.WriteData(m_fChangeLimitTime);

	// 애니메이션 클립정보를 저장한다.
	Writer.WriteData(m_mapClip.size());
	Writer.WriteData(m_pDefaultClip->strName);

	Writer.WriteData(m_pCurClip->strName);

	unordered_map<string, AnimationClip*>::iterator	iter;
	unordered_map<string, AnimationClip*>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		AnimationClip*	pClip = iter->second;

		Writer.WriteData(pClip->strName);
		Writer.WriteData(pClip->eOption);
		Writer.WriteData(pClip->fStartTime);
		Writer.WriteData(pClip->fEndTime);
		Writer.WriteData(pClip->fTimeLength);
		Writer.WriteData(pClip->fFrameTime);
		Writer.WriteData(pClip->fPlayTime);
		Writer.WriteData(pClip->iFrameMode);
		Writer.WriteData(pClip->iStartFrame);
		Writer.WriteData(pClip->iEndFrame);
		Writer.WriteData(pClip->iFrameLength);

		Writer.WriteData(pClip->vecKeyFrame.size());

		size_t	iCount = pClip->vecKeyFrame.size();

		for (size_t i = 0; i < iCount; ++i)
		{
			Writer.WriteData(pClip->vecKeyFrame[i]->iBoneIndex);
			Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame.size());

			size_t	iFrameCount = pClip->vecKeyFrame[i]->vecKeyFrame.size();

			for (size_t j = 0; j < iFrameCount; ++j)
			{
				Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame[j]->dTime);
				Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame[j]->vPos);
				Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame[j]->vScale);
				Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame[j]->vRot);
			}
		}
	}

	return true;
}

bool Animation3D_Com::SaveFullPath(const TCHAR * FullPath, const string & strPathKey)
{
	BineryWrite Writer(FullPath);

	Writer.WriteData(m_iFrameMode);
	Writer.WriteData(m_fChangeLimitTime);

	// 애니메이션 클립정보를 저장한다.
	Writer.WriteData(m_mapClip.size());
	Writer.WriteData(m_pDefaultClip->strName);

	Writer.WriteData(m_pCurClip->strName);

	unordered_map<string, AnimationClip*>::iterator	iter;
	unordered_map<string, AnimationClip*>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		AnimationClip*	pClip = iter->second;

		Writer.WriteData(pClip->strName);
		Writer.WriteData(pClip->eOption);
		Writer.WriteData(pClip->fStartTime);
		Writer.WriteData(pClip->fEndTime);
		Writer.WriteData(pClip->fTimeLength);
		Writer.WriteData(pClip->fFrameTime);
		Writer.WriteData(pClip->fPlayTime);
		Writer.WriteData(pClip->iFrameMode);
		Writer.WriteData(pClip->iStartFrame);
		Writer.WriteData(pClip->iEndFrame);
		Writer.WriteData(pClip->iFrameLength);

		Writer.WriteData(pClip->vecKeyFrame.size());

		size_t	iCount = pClip->vecKeyFrame.size();

		for (size_t i = 0; i < iCount; ++i)
		{
			Writer.WriteData(pClip->vecKeyFrame[i]->iBoneIndex);
			Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame.size());

			size_t	iFrameCount = pClip->vecKeyFrame[i]->vecKeyFrame.size();

			for (size_t j = 0; j < iFrameCount; ++j)
			{
				Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame[j]->dTime);
				Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame[j]->vPos);
				Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame[j]->vScale);
				Writer.WriteData(pClip->vecKeyFrame[i]->vecKeyFrame[j]->vRot);
			}
		}
	}

	return true;
}

bool Animation3D_Com::Load(const string& FileName, const string & strPathKey)
{
	BineryRead Reader(FileName);

	Reader.ReadData(m_iFrameMode);
	Reader.ReadData(m_fChangeLimitTime);

	size_t iCount = 0, iLength = 0;
	Reader.ReadData(iCount);

	string strDefaultClip;
	Reader.ReadData(strDefaultClip);

	string	strCurClip;
	Reader.ReadData(strCurClip);

	m_strAddClipName.clear();

	for (int l = 0; l < iCount; ++l)
	{
		AnimationClip*	pClip = new AnimationClip();

		// 애니메이션 클립 키를 저장한다.
		string strClipName = {};
		Reader.ReadData(strClipName);

		m_strAddClipName.push_back(strClipName);
		m_mapClip.insert(make_pair(strClipName, pClip));

		pClip->strName = strClipName;
		pClip->iChangeFrame = 0;

		Reader.ReadData((int&)pClip->eOption);

		Reader.ReadData(pClip->fStartTime);
		Reader.ReadData(pClip->fEndTime);
		Reader.ReadData(pClip->fTimeLength);
		Reader.ReadData(pClip->fFrameTime);
		Reader.ReadData(pClip->fPlayTime);
		Reader.ReadData(pClip->iFrameMode);
		Reader.ReadData(pClip->iStartFrame);
		Reader.ReadData(pClip->iEndFrame);
		Reader.ReadData(pClip->iFrameLength);

		size_t	iFrameCount = 0;
		Reader.ReadData(iFrameCount);

		for (size_t i = 0; i < iFrameCount; ++i)
		{
			BoneKeyFrame*	pBoneKeyFrame = new BoneKeyFrame();
			pClip->vecKeyFrame.push_back(pBoneKeyFrame);

			Reader.ReadData(pBoneKeyFrame->iBoneIndex);
			
			size_t	iBoneFrameCount = 0;
			Reader.ReadData(iBoneFrameCount);

			for (size_t j = 0; j < iBoneFrameCount; ++j)
			{
				KeyFrame*	pKeyFrame = new KeyFrame();
				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

				Reader.ReadData(pKeyFrame->dTime);
				Reader.ReadData(pKeyFrame->vPos);
				Reader.ReadData(pKeyFrame->vScale);
				Reader.ReadData(pKeyFrame->vRot);
			}
		}
	}

	m_pCurClip = FindClip(strCurClip);
	m_pDefaultClip = FindClip(strDefaultClip);

	CreateBoneTexture();

	return true;
}

bool Animation3D_Com::LoadFullPath(const TCHAR * FullPath, const string & strPathKey)
{
	BineryRead Reader(FullPath);

	Reader.ReadData(m_iFrameMode);
	Reader.ReadData(m_fChangeLimitTime);

	size_t iCount = 0, iLength = 0;
	Reader.ReadData(iCount);

	string strDefaultClip;
	Reader.ReadData(strDefaultClip);

	string	strCurClip;
	Reader.ReadData(strCurClip);

	m_strAddClipName.clear();

	for (int l = 0; l < iCount; ++l)
	{
		AnimationClip*	pClip = new AnimationClip();

		// 애니메이션 클립 키를 저장한다.
		string strClipName = {};
		Reader.ReadData(strClipName);

		m_strAddClipName.push_back(strClipName);
		m_mapClip.insert(make_pair(strClipName, pClip));

		pClip->strName = strClipName;
		pClip->iChangeFrame = 0;

		Reader.ReadData((int&)pClip->eOption);

		Reader.ReadData(pClip->fStartTime);
		Reader.ReadData(pClip->fEndTime);
		Reader.ReadData(pClip->fTimeLength);
		Reader.ReadData(pClip->fFrameTime);
		Reader.ReadData(pClip->fPlayTime);
		Reader.ReadData(pClip->iFrameMode);
		Reader.ReadData(pClip->iStartFrame);
		Reader.ReadData(pClip->iEndFrame);
		Reader.ReadData(pClip->iFrameLength);

		size_t	iFrameCount = 0;
		Reader.ReadData(iFrameCount);

		for (size_t i = 0; i < iFrameCount; ++i)
		{
			BoneKeyFrame*	pBoneKeyFrame = new BoneKeyFrame();
			pClip->vecKeyFrame.push_back(pBoneKeyFrame);

			Reader.ReadData(pBoneKeyFrame->iBoneIndex);

			size_t	iBoneFrameCount = 0;
			Reader.ReadData(iBoneFrameCount);

			for (size_t j = 0; j < iBoneFrameCount; ++j)
			{
				KeyFrame*	pKeyFrame = new KeyFrame();
				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);

				Reader.ReadData(pKeyFrame->dTime);
				Reader.ReadData(pKeyFrame->vPos);
				Reader.ReadData(pKeyFrame->vScale);
				Reader.ReadData(pKeyFrame->vRot);
			}
		}
	}

	m_pCurClip = FindClip(strCurClip);
	m_pDefaultClip = FindClip(strDefaultClip);

	CreateBoneTexture();

	return true;
}

bool Animation3D_Com::SaveBone(const string& FileName, const string & strPathKey)
{
	BineryWrite Writer(FileName);
	
	// ===================== 본 정보 저장 =====================
	size_t	iCount = m_vecBones.size();
	Writer.WriteData(iCount);

	size_t	iLength = 0;

	for (size_t i = 0; i < iCount; ++i)
	{
		Writer.WriteData(m_vecBones[i]->strName);
		Writer.WriteData(m_vecBones[i]->iDepth);
		Writer.WriteData(m_vecBones[i]->iParentIndex);
		Writer.WriteData(m_vecBones[i]->matOffset->matrix);
		Writer.WriteData(m_vecBones[i]->matBone->matrix);
	}

	return true;
}

bool Animation3D_Com::SaveBoneFullPath(const TCHAR * FullPath, const string & strPathKey)
{
	BineryWrite Writer(FullPath);

	// ===================== 본 정보 저장 =====================
	size_t	iCount = m_vecBones.size();
	Writer.WriteData(iCount);

	size_t	iLength = 0;

	for (size_t i = 0; i < iCount; ++i)
	{
		Writer.WriteData(m_vecBones[i]->strName);
		Writer.WriteData(m_vecBones[i]->iDepth);
		Writer.WriteData(m_vecBones[i]->iParentIndex);
		Writer.WriteData(m_vecBones[i]->matOffset->matrix);
		Writer.WriteData(m_vecBones[i]->matBone->matrix);
	}

	return true;
}

bool Animation3D_Com::LoadBone(const string& FileName, const string & strPathKey)
{
	BineryRead Reader(FileName);

	// ===================== 본 정보 읽기 =====================
	size_t	iCount = 0;
	Reader.ReadData(iCount);

	size_t	iLength = 0;

	for (size_t i = 0; i < iCount; ++i)
	{
		Bone*	pBone = new Bone();
		m_vecBones.push_back(pBone);

		pBone->matBone = new Matrix;
		pBone->matOffset = new Matrix;

		string strBoneName;
		Reader.ReadData(strBoneName);
		pBone->strName = strBoneName;

		Reader.ReadData(pBone->iDepth);
		Reader.ReadData(pBone->iParentIndex);
		Reader.ReadData((Matrix)pBone->matOffset->matrix);
		Reader.ReadData((Matrix)pBone->matBone->matrix);
	}

	return true;
}

bool Animation3D_Com::LoadBoneFullPath(const TCHAR * FullPath, const string & strPathKey)
{
	BineryRead Reader(FullPath);

	// ===================== 본 정보 읽기 =====================
	size_t	iCount = 0;
	Reader.ReadData(iCount);

	size_t	iLength = 0;

	for (size_t i = 0; i < iCount; ++i)
	{
		Bone*	pBone = new Bone();
		m_vecBones.push_back(pBone);

		pBone->matBone = new Matrix;
		pBone->matOffset = new Matrix;

		string strBoneName;
		Reader.ReadData(strBoneName);
		pBone->strName = strBoneName;

		Reader.ReadData(pBone->iDepth);
		Reader.ReadData(pBone->iParentIndex);
		Reader.ReadData((Matrix)pBone->matOffset->matrix);
		Reader.ReadData((Matrix)pBone->matBone->matrix);
	}

	return true;
}

bool Animation3D_Com::LoadBoneAndAnimationFullPath(const TCHAR * pFullPath)
{
	// 본 파일로 이름을 변환한다.
	TCHAR	strBone[MAX_PATH] = {};
	lstrcpy(strBone, pFullPath);

	int	iLength = lstrlen(strBone);

	strBone[iLength - 3] = 'b';
	strBone[iLength - 2] = 'n';
	strBone[iLength - 1] = 'e';

	if (!LoadBoneFullPath(strBone))
		return false;

	return LoadFullPath(pFullPath);
}

bool Animation3D_Com::ModifyClip(const string & strKey, const string & strChangeKey, ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame, float fPlayTime, const vector<BoneKeyFrame*>& vecFrame)
{
	AnimationClip*	pClip = FindClip(strKey);

	if (!pClip)
		return false;

	m_mapClip.erase(strKey);

	int	iLength = iEndFrame - iStartFrame;

	pClip->fPlayTime = fPlayTime;
	pClip->fFrameTime = pClip->fPlayTime / iLength;
	pClip->eOption = eOption;
	pClip->strName = strChangeKey;
	pClip->iStartFrame = 0;
	pClip->iEndFrame = iLength;
	pClip->iFrameLength = iLength;
	pClip->fStartTime = 0.f;
	pClip->fEndTime = fPlayTime;
	pClip->fTimeLength = fPlayTime;

	Safe_Delete_VecList(pClip->vecKeyFrame);

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < vecFrame.size(); ++i)
	{
		BoneKeyFrame*	pBoneKeyFrame = new BoneKeyFrame();
		pBoneKeyFrame->iBoneIndex = vecFrame[i]->iBoneIndex;
		pClip->vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(vecFrame[i]->vecKeyFrame.size());

		if (!vecFrame[i]->vecKeyFrame.empty())
		{
			for (size_t j = iStartFrame; j <= iEndFrame; ++j)
			{
				KeyFrame*	pKeyFrame = new KeyFrame();
				pKeyFrame->dTime = (j - iStartFrame) * pClip->fFrameTime;
				pKeyFrame->vScale = vecFrame[i]->vecKeyFrame[j]->vScale;
				pKeyFrame->vRot = vecFrame[i]->vecKeyFrame[j]->vRot;
				pKeyFrame->vPos = vecFrame[i]->vecKeyFrame[j]->vPos;

				pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);
			}
		}
	}

	m_mapClip.insert(make_pair(strChangeKey, pClip));

	return true;
}

bool Animation3D_Com::DeleteClip(const string & strKey)
{
	unordered_map<string, AnimationClip*>::iterator	iter = m_mapClip.find(strKey);

	if (iter == m_mapClip.end())
		return false;

	m_mapClip.erase(iter);
	SAFE_DELETE(iter->second);

	if (strKey == m_pDefaultClip->strName)
	{
		if (!m_mapClip.empty())
			m_pDefaultClip = FindClip(m_mapClip.begin()->first);
	}

	if (strKey == m_pCurClip->strName)
		m_pCurClip = m_pDefaultClip;

	return true;
}

void Animation3D_Com::GetClipTagList(vector<string>* _vecstrList)
{
	unordered_map<string, AnimationClip*>::const_iterator	iter;
	unordered_map<string, AnimationClip*>::const_iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
		_vecstrList->push_back(iter->second->strName);
}

void Animation3D_Com::LoadFbxAnimation(const char * pFullPath)
{
	FBXLoader loader;

	loader.LoadFbx(pFullPath);

	// 애니메이션 클립을 추가한다.
	const vector<FbxAnimationClip*>* pvecClip = loader.GetClips();

	// 클립을 읽어온다.
	vector<FbxAnimationClip*>::const_iterator	iterC;
	vector<FbxAnimationClip*>::const_iterator	iterCEnd = pvecClip->end();

	m_strAddClipName.clear();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		AddClip(AO_LOOP, *iterC);
		m_strAddClipName.push_back((*iterC)->strName);
	}
}
