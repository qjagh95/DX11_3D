#include "stdafx.h"
#include "ColliderRect_Com.h"
#include "ColliderPoint_Com.h"
#include "ColliderCircle_Com.h"
#include "ColliderOBB2D_Com.h"
#include "ColliderPixel_Com.h"
#include "Transform_Com.h"
#include "Camera_Com.h"

#include "../Render/RenderManager.h"
#include "../Render/ShaderManager.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh.h"

#include "../Scene/Scene.h"
JEONG_USING

ColliderRect_Com::ColliderRect_Com()
{
	m_CollType = CT_RECT;
	
#ifdef _DEBUG
	m_DepthDisable = RenderManager::Get()->FindRenderState(DEPTH_DISABLE);
	m_Mesh = ResourceManager::Get()->FindMesh("ColliderRect");
#endif
}

ColliderRect_Com::ColliderRect_Com(const ColliderRect_Com & CopyCollider)
	:Collider_Com(CopyCollider)
{
	m_Virtual = CopyCollider.m_Virtual;
}

ColliderRect_Com::~ColliderRect_Com()
{
}

bool ColliderRect_Com::Init()
{
	return true;
}

int ColliderRect_Com::Input(float DeltaTime)
{
	return 0;
}

int ColliderRect_Com::Update(float DeltaTime)
{
	return 0;
}

int ColliderRect_Com::LateUpdate(float DeltaTime)
{
	//중심점공식
	Vector3 TempPos = m_Transform->GetWorldPos() - m_Transform->GetPivot() * m_Transform->GetWorldScale();

	m_WorldInfo.Min = TempPos + m_Virtual.Min + m_Pivot * m_Virtual.Lenth;
	m_WorldInfo.Max = TempPos + m_Virtual.Max + m_Pivot * m_Virtual.Lenth;
	m_WorldInfo.Lenth = m_Virtual.Lenth;

	m_SectionMin = m_WorldInfo.Min;
	m_SectionMax = m_WorldInfo.Max;

	return 0;
}

void ColliderRect_Com::Collision(float DeltaTime)
{
}

void ColliderRect_Com::CollisionLateUpdate(float DeltaTime)
{
}

void ColliderRect_Com::Render(float DeltaTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale, matView;
	matPos.Translation(m_WorldInfo.Min);
	//사이즈만큼 커져랏
	matScale.Scaling(m_WorldInfo.Lenth);

	Camera_Com*	getCamera = m_Scene->GetMainCamera();
	TransformCBuffer TransCBuffer = {};

	if (m_CollisionGroupName != "UI")
		matView = getCamera->GetViewMatrix();

	TransCBuffer.World = matScale * matPos;
	TransCBuffer.View = matView;
	TransCBuffer.Projection = getCamera->GetProjection();
	TransCBuffer.Pivot = m_Pivot;
	TransCBuffer.Lenth = m_Mesh->GetLenth();

	TransCBuffer.WV = TransCBuffer.World * TransCBuffer.View;
	TransCBuffer.WVP = TransCBuffer.WV * TransCBuffer.Projection;

	TransCBuffer.World.Transpose();
	TransCBuffer.View.Transpose();
	TransCBuffer.Projection.Transpose();
	TransCBuffer.WV.Transpose();
	TransCBuffer.WVP.Transpose();

	ShaderManager::Get()->UpdateCBuffer("Transform", &TransCBuffer);

	Collider_Com::Render(DeltaTime);
#endif // _DEBUG
}

ColliderRect_Com * ColliderRect_Com::Clone()
{
	return new ColliderRect_Com(*this);
}

bool ColliderRect_Com::Collsion(Collider_Com * Dest, float DeltaTime)
{
	switch (Dest->GetCollType())
	{
		case CT_RECT:
			return CollsionRectToRect(m_WorldInfo, ((ColliderRect_Com*)Dest)->m_WorldInfo);
			break;
		case CT_POINT:
			return CollsionRectToPoint(m_WorldInfo, ((ColliderPoint_Com*)Dest)->GetInfo());
			break;
		case CT_CIRCLE:
			return CollsionRectToCircle(m_WorldInfo, ((ColliderCircle_Com*)Dest)->GetInfo());
			break;
		case CT_OBB2D:
			return CollsionOBB2DToRect(((ColliderOBB2D_Com*)Dest)->GetInfo(), m_WorldInfo);
			break;
		case CT_PIXEL:
			return CollsionRectToPixel(m_WorldInfo, ((ColliderPixel_Com*)Dest)->GetInfo());
			break;
	}
	return false;
}

void ColliderRect_Com::SetInfo(const Vector3& Min, const Vector3& Max)
{
	m_Virtual.Min = Min;
	m_Virtual.Max = Max;
	m_Virtual.Lenth = Max - Min;

	m_Virtual.Min.z = 0.0f;
	m_Virtual.Max.z = 0.0f;
	m_Virtual.Lenth.z = 1.0f;
}