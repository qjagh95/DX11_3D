#include "stdafx.h"
#include "ColliderPoint_Com.h"
#include "ColliderRect_Com.h"
#include "ColliderCircle_Com.h"
#include "ColliderOBB2D_Com.h"
#include "ColliderPixel_Com.h"
#include "Camera_Com.h"
#include "Transform_Com.h"

#include "../Scene/Scene.h"

#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"

#include "../Render/Shader.h"
#include "../Render/ShaderManager.h"
#include "../Render/RenderManager.h"
#include "../Scene/SceneManager.h"

JEONG_USING

ColliderPoint_Com::ColliderPoint_Com()
{
	m_CollType = CT_POINT;

#ifdef _DEBUG
	m_DepthDisable = RenderManager::Get()->FindRenderState(DEPTH_DISABLE);
	m_Mesh = ResourceManager::Get()->FindMesh("ColliderRect");
#endif
}

ColliderPoint_Com::ColliderPoint_Com(const ColliderPoint_Com & CopyCollider)
	:Collider_Com(CopyCollider)
{
	*this = CopyCollider;
}

ColliderPoint_Com::~ColliderPoint_Com()
{
}

bool ColliderPoint_Com::Init()
{
	return true;
}

int ColliderPoint_Com::Input(float DeltaTime)
{
	return 0;
}

int ColliderPoint_Com::Update(float DeltaTime)
{
	return 0;
}

int ColliderPoint_Com::LateUpdate(float DeltaTime)
{
	JEONG::Scene* curScene = SceneManager::Get()->GetCurScene();
	Vector3 tempPos = m_Transform->GetWorldPos();
	m_WorldInfo = tempPos + m_Virtual;

	m_SectionMin = m_WorldInfo;
	m_SectionMax = m_WorldInfo;

	SAFE_RELEASE(curScene);

	return 0;
}

void ColliderPoint_Com::Collision(float DeltaTime)
{
}

void ColliderPoint_Com::CollisionLateUpdate(float DeltaTime)
{
}

void ColliderPoint_Com::Render(float DeltaTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale, matView;
	Camera_Com*	getCamera = m_Scene->GetMainCamera();

	matPos.Translation(m_WorldInfo - Vector3(1.5f, 1.5f, 0.0f));
	matScale.Scaling(3.0f, 3.0f, 1.0f);

	if (m_CollisionGroupName != "UI")
		matView = getCamera->GetViewMatrix();

	TransformCBuffer TransCBuffer = {};
	TransCBuffer.World = matScale * matPos;
	TransCBuffer.View = matView;
	TransCBuffer.Projection = getCamera->GetProjection();
	TransCBuffer.Pivot = Vector3{ 0.0f, 0.0f, 0.0f };
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

bool ColliderPoint_Com::Collsion(Collider_Com * Dest, float DeltaTime)
{
	switch (Dest->GetCollType())
	{
		case CT_RECT:
			return CollsionRectToPoint(((ColliderRect_Com*)Dest)->GetInfo(), m_WorldInfo);
			break;
		case CT_CIRCLE:
			return CollsionCircleToPoint(((ColliderCircle_Com*)Dest)->GetInfo(), m_WorldInfo);
			break;
		case CT_POINT:
			return m_WorldInfo == ((ColliderPoint_Com*)Dest)->GetInfo();
			break;
		case CT_OBB2D:
			return CollsionOBB2DToPoint(((ColliderOBB2D_Com*)Dest)->GetInfo(), m_WorldInfo);
			break;
	}
	return false;
}

ColliderPoint_Com * ColliderPoint_Com::Clone()
{
	return new ColliderPoint_Com(*this);
}
