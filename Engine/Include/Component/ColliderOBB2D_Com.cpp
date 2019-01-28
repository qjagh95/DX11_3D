#include "stdafx.h"
#include "ColliderOBB2D_Com.h"
#include "ColliderRect_Com.h"
#include "ColliderPoint_Com.h"
#include "ColliderCircle_Com.h"
#include "ColliderPixel_Com.h"
#include "Transform_Com.h"
#include "Camera_Com.h"

#include "../Resource/Mesh.h"
#include "../Scene/Scene.h"
#include "../Render/ShaderManager.h"

JEONG_USING

ColliderOBB2D_Com::ColliderOBB2D_Com()
{
	m_CollType = CT_OBB2D;

#ifdef _DEBUG
	m_DepthDisable = RenderManager::Get()->FindRenderState(DEPTH_DISABLE);
	m_Mesh = ResourceManager::Get()->FindMesh("ColliderRect");
#endif
}

ColliderOBB2D_Com::ColliderOBB2D_Com(const ColliderOBB2D_Com & CopyCollider)
	:Collider_Com(CopyCollider)
{
	*this = CopyCollider;
}

ColliderOBB2D_Com::~ColliderOBB2D_Com()
{
}

bool ColliderOBB2D_Com::Init()
{
	return true;
}

int ColliderOBB2D_Com::Input(float DeltaTime)
{
	return 0;
}

int ColliderOBB2D_Com::Update(float DeltaTime)
{

	return 0;
}

int ColliderOBB2D_Com::LateUpdate(float DeltaTime)
{
	//내 오브젝트의 회전행렬을 가져온다.
	Matrix RotMat = m_Transform->GetRotDelta();

	Vector3 T = Vector3(m_Transform->GetWorldMatrix()._41, m_Transform->GetWorldMatrix()._42, m_Transform->GetWorldMatrix()._43);
	//내 오브젝트의 WorldPos를 삽입한다.
	memcpy(&RotMat[3][0], &T, sizeof(Vector3));

	//벡터 행렬곱(Pos를 중심으로 + Virtual Pos가 된 위치) 따라와야되니 Coord (w값1))
	m_WorldInfo.CenterPos = m_Virtual.CenterPos.TransformCoord(RotMat);

	Vector3 TempAxis[2];
	for (int i = 0; i < 2; i++)
	{
		//Axis는 방향만 알면되니 Nomal(w값 0)
		m_WorldInfo.Axis[i] = m_Virtual.Axis[i].TransformNormal(RotMat);
		//m_WorldInfo.Axis[i].Normalize();
		//해당 방향에 해당 길이만큼 늘린다.
		TempAxis[i] = m_WorldInfo.Axis[i] * m_WorldInfo.Lenth[i];
	}
	
	//4개의 사각형 꼭지점을 구해야한다. (공간할당을 위함)
	Vector3 LB = m_WorldInfo.CenterPos - TempAxis[0] - TempAxis[1];
	Vector3 LT = m_WorldInfo.CenterPos - TempAxis[0] + TempAxis[1];
	Vector3 RT = m_WorldInfo.CenterPos + TempAxis[0] + TempAxis[1];
	Vector3 RB = m_WorldInfo.CenterPos + TempAxis[0] - TempAxis[1];

	pair<float, float> XMinMax = minmax({ LB.x, LT.x ,RT.x, RB.x }, [](float const& _first, float const& _second) {return _first < _second;});
	pair<float, float> YMinMax = minmax({ LB.y, LT.y, RT.y, RB.y }, [](float const& _first, float const& _second) {return _first < _second;});

	m_SectionMin = Vector3{ XMinMax.first, YMinMax.first, 0.0f };
	m_SectionMax = Vector3{ XMinMax.second, YMinMax.second, 0.0f };

	return 0;
}

void ColliderOBB2D_Com::Collision(float DeltaTime)
{
}

void ColliderOBB2D_Com::CollisionLateUpdate(float DeltaTime)
{
}

void ColliderOBB2D_Com::Render(float DeltaTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale, matRot;

	//크기를 구한다.(한쪽 반의 길이만 넣어줬으니 *2)
	Vector3 Lenth = Vector3(m_WorldInfo.Lenth[0] * 2.0f, m_WorldInfo.Lenth[1] * 2.0f, 1.0f);

	//회전 각도를 구한다. (x = 1 y = 0 축과 내적하여 각도를 구한다.)
	float Angle = m_WorldInfo.Axis[0].GetAngle(Vector3(1.0f, 0.0f, 0.0f));

	//X방향의 y축은 ->방향이 0이고 <-방향이 1이다. 1부터(180도) y축이줄어들기 시작해서 0이된다.
	if (m_WorldInfo.Axis[0].y < 0.0f)
		Angle = 360.0f - Angle;

	matRot.RotationZ(Angle);
	matPos.Translation(m_WorldInfo.CenterPos);
	matScale.Scaling(Lenth);

	Matrix matView;

	if (m_CollisionGroupName != "UI")
		matView = m_Scene->GetMainCamera()->GetViewMatrix();

	JEONG::Camera_Com* getCamera = m_Scene->GetMainCamera();
	TransformCBuffer TransCBuffer = {};

	TransCBuffer.World = matScale * matRot * matPos;
	TransCBuffer.View = matView;
	TransCBuffer.Projection = getCamera->GetProjection();
	TransCBuffer.Pivot = Vector3(0.5f, 0.5f, 0.0f);
	TransCBuffer.Lenth = m_Mesh->GetLenth();

	TransCBuffer.WV = TransCBuffer.World * TransCBuffer.View;
	TransCBuffer.WVP = TransCBuffer.WV * TransCBuffer.Projection;
	TransCBuffer.InvProjection = TransCBuffer.Projection;
	TransCBuffer.InvProjection.Inverse();

	TransCBuffer.World.Transpose();
	TransCBuffer.View.Transpose();
	TransCBuffer.Projection.Transpose();
	TransCBuffer.WV.Transpose();
	TransCBuffer.WVP.Transpose();
	TransCBuffer.InvProjection.Transpose();

	JEONG::ShaderManager::Get()->UpdateCBuffer("Transform", &TransCBuffer);

	Collider_Com::Render(DeltaTime);
#endif // _DEBUG
}

ColliderOBB2D_Com * ColliderOBB2D_Com::Clone()
{
	return new ColliderOBB2D_Com(*this);
}

void ColliderOBB2D_Com::SetInfo(const Vector3 & Center, Vector3 Axis[2], float Lenth[2])
{
	m_Virtual.CenterPos = Center;

	for (int i = 0; i < 2; i++)
	{
		m_Virtual.Axis[i] = Axis[i];
		m_Virtual.Lenth[i] = Lenth[i];
		m_WorldInfo.Lenth[i] = Lenth[i];
	}
}

bool ColliderOBB2D_Com::Collsion(Collider_Com * Dest, float DeltaTime)
{
	switch (Dest->GetCollType())
	{
		case CT_RECT:
			return CollsionOBB2DToRect(m_WorldInfo, ((ColliderRect_Com*)Dest)->GetInfo());
			break;
		case CT_POINT:
			return CollsionOBB2DToPoint(m_WorldInfo, ((ColliderPoint_Com*)Dest)->GetInfo());
			break;
		case CT_CIRCLE:
			return CollsionOBB2DToCircle(m_WorldInfo, ((ColliderCircle_Com*)Dest)->GetInfo());
			break;
		case CT_OBB2D:
			return CollsionOBB2DToOBB2D(m_WorldInfo, ((ColliderOBB2D_Com*)Dest)->GetInfo());
			break;
		case CT_PIXEL:
			return false;
			break;
	}

	return false;
}