#include "stdafx.h"
#include "ColliderCircle_Com.h"
#include "ColliderPoint_Com.h"
#include "ColliderRect_Com.h"
#include "ColliderOBB2D_Com.h"
#include "Transform_Com.h"
#include "Camera_Com.h"

#include "../GameObject.h"

#include "../Scene/Scene.h"

#include "../Render/RenderManager.h"
#include "../Render/ShaderManager.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh.h"

JEONG_USING
ColliderCircle_Com::ColliderCircle_Com()
{
	m_CollType = CT_CIRCLE;
#ifdef _DEBUG
	m_DepthDisable = RenderManager::Get()->FindRenderState(DEPTH_DISABLE);
	m_Mesh = ResourceManager::Get()->FindMesh("ColliderCircle");
#endif
}

ColliderCircle_Com::ColliderCircle_Com(const ColliderCircle_Com& CopyData)
	:Collider_Com(CopyData)
{
	*this = CopyData;
}

ColliderCircle_Com::~ColliderCircle_Com()
{
}

bool ColliderCircle_Com::Init()
{
	return true;
}

int ColliderCircle_Com::Input(float DeltaTime)
{
	return 0;
}

int ColliderCircle_Com::Update(float DeltaTime)
{
	Vector3 objectPos = m_Transform->GetWorldPos() + m_Pivot * m_VirtualInfo.Radius;

	m_WorldInfo.Radius = m_VirtualInfo.Radius;

	m_WorldInfo.CenterPos.x = objectPos.x;
	m_WorldInfo.CenterPos.y = objectPos.y + m_WorldInfo.Radius;

	m_SectionMin.x = m_WorldInfo.CenterPos.x - m_WorldInfo.Radius;
	m_SectionMin.y = m_WorldInfo.CenterPos.y - m_WorldInfo.Radius;

	m_SectionMax.x = m_WorldInfo.CenterPos.x + m_WorldInfo.Radius;
	m_SectionMax.y = m_WorldInfo.CenterPos.y + m_WorldInfo.Radius;

	return 0;
}

int ColliderCircle_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void ColliderCircle_Com::Collision(float DeltaTime)
{
}

void ColliderCircle_Com::CollisionLateUpdate(float DeltaTime)
{
}

void ColliderCircle_Com::Render(float DeltaTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale, matView;
	matPos.Translation(Vector3(m_WorldInfo.CenterPos.x + m_WorldInfo.Radius, m_WorldInfo.CenterPos.y, 0.0f));
	matScale.Scaling(m_WorldInfo.Radius);

	if (m_CollisionGroupName != "UI")
		matView = m_Scene->GetMainCamera()->GetViewMatrix();

	Camera_Com*	getCamera = m_Scene->GetMainCamera();
	TransformCBuffer TransCBuffer = {};

	TransCBuffer.World = matScale * matPos;
	TransCBuffer.View = matView;
	TransCBuffer.Projection = getCamera->GetProjection();
	TransCBuffer.Pivot = m_Transform->GetPivot();
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

ColliderCircle_Com * ColliderCircle_Com::Clone()
{
	return new ColliderCircle_Com(*this);
}

bool ColliderCircle_Com::Collsion(Collider_Com * Dest, float DeltaTime)
{
	switch (Dest->GetCollType())
	{
		case CT_RECT:
			return CollsionRectToCircle(((ColliderRect_Com*)Dest)->GetInfo(), m_WorldInfo);
			break;
		case CT_CIRCLE:
			return CollsionCircleToCircle(((ColliderCircle_Com*)Dest)->GetInfo(), m_WorldInfo);
			break;
		case CT_POINT:
			return CollsionCircleToPoint(m_WorldInfo, ((ColliderPoint_Com*)Dest)->GetInfo());
			break;
		case CT_OBB2D:
			return  CollsionOBB2DToCircle(((ColliderOBB2D_Com*)Dest)->GetInfo(), m_WorldInfo);
			break;
		case CT_PIXEL:
			return false;
			break;
	}
	return false;
}

