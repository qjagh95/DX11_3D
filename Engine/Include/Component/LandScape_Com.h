#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class NavigationMesh;
class JEONG_DLL LandScape_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	LandScape_Com* Clone() override;

	void CreateGrid(const string& KeyName, int XCount, int ZCount, const string& TexKey = "", const TCHAR* TexName = NULLPTR, const TCHAR* TexNormal = NULLPTR, const TCHAR* TexSpecular = NULLPTR, const char* FileName = NULLPTR, const string& PathKey = TEXTURE_PATH);

private:
	void ComputeTangent();
	void ComputeNormal();

private:
	unsigned int m_XCount;
	unsigned int m_ZCount;
	vector<Vertex3D> m_vecVertex;
	vector<int>	m_vecIndex;
	vector<Vector3>	m_vecFaceNormal;
	LandScapeCBuffer m_CBuffer;
	NavigationMesh*	m_NaviMesh;

protected:
	LandScape_Com();
	LandScape_Com(const LandScape_Com& CopyData);
	~LandScape_Com();

public:
	friend class GameObject;
};

JEONG_END