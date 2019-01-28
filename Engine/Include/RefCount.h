#pragma once
JEONG_BEGIN

class JEONG_DLL RefCount
{
public:
	RefCount();
	virtual ~RefCount() = 0;

	string GetTag() const { return m_TagName;}
	void SetTag(const string& TagName) { m_TagName = TagName; }
	int AddRefCount() { ReferanceCount++; return ReferanceCount; }
	int Release();
	int GetRefCount() const { return ReferanceCount; }

	bool GetIsActive() const { return isActive; }
	bool GetIsShow() const { return isShow; }
	
	void SetIsActive(bool Val) { isActive = Val; };
	void SetIsShow(bool Val) { isShow = Val; }

protected:
	string m_TagName;
	int ReferanceCount;
	bool isActive;
	bool isShow;
};

JEONG_END
