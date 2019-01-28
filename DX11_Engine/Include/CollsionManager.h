#pragma once
#include "Component\Collider_Com.h"

JEONG_BEGIN

enum JEONG_DLL COLLSION_GROUP_TYPE
{
	CGT_2D,
	CGT_3D,
};

class GameObject;
class JEONG_DLL CollsionManager
{
private:
	//하나의 공간 그 자체를 의미하는 변수. Collider의 포인터를 받아야하기때문에 2차포인터 (vector대신 쓴다)
	struct CollsionSection
	{
		Collider_Com** ColliderList;
		int Size;					 //충돌체 카운트
		int Capacity;

		CollsionSection() : Size(0), Capacity(5) { ColliderList = new Collider_Com*[Capacity]; }
		~CollsionSection() { SAFE_DELETE_ARRARY(ColliderList); }
	};
	//전체공간이다.
	struct CollsionGroup
	{
		COLLSION_GROUP_TYPE Type;
		CollsionSection* SectionList;
		int SpaceCountX;
		int SpaceCountY;
		int SpaceCountZ;
		int SpaceCount;
		Vector3 Min;		//전체공간의 최소값
		Vector3 Max;	    //전체공간의 최대값
		Vector3 Lenth;		//Max - Min = 길이
		Vector3 SpaceLenth; //공간 하나당 크기

		CollsionGroup() {}
		~CollsionGroup() {	SAFE_DELETE_ARRARY(SectionList); }
	};

public:
	bool Init();
	bool CreateGroup(const string& KeyName, const Vector3& Min, const Vector3& Max, int SpaceCountX, int SpaceCountY, int SpaceCountZ, COLLSION_GROUP_TYPE eType = CGT_3D);
	void ChangeGroupType(const string& KeyName, COLLSION_GROUP_TYPE eType);
	void AddCollsion(GameObject* object);
	void ClearCollsionGroup() { Safe_Delete_Map(m_GroupMap); }
	void Collsion(float DeltaTime);

private:
	//그룹하나하나당 똑같은 직육면체 하나씩 존재한다고 생각하면 편하다.
	unordered_map<string, CollsionGroup*> m_GroupMap;

private:
	CollsionGroup* FindGroup(const string& KeyName);

public:
	CLASS_IN_SINGLE(CollsionManager)
};

JEONG_END