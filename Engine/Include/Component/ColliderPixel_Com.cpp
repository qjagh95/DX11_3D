#include "stdafx.h"
#include "ColliderPixel_Com.h"
#include "ColliderRect_Com.h"
#include "ColliderPoint_Com.h"
#include "ColliderCircle_Com.h"
#include "ColliderOBB2D_Com.h"
#include "Transform_Com.h"
#include "Camera_Com.h"

#include "../PathManager.h"

#include "../Render/RenderManager.h"
#include "../Render/ShaderManager.h"

#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh.h"

#include "../Scene/Scene.h"
JEONG_USING

ColliderPixel_Com::ColliderPixel_Com()
{
	m_CollType = CT_PIXEL;
#ifdef _DEBUG
	m_DepthDisable = RenderManager::Get()->FindRenderState(DEPTH_DISABLE);
	m_Mesh = ResourceManager::Get()->FindMesh("ColliderRect");
#endif
}

ColliderPixel_Com::ColliderPixel_Com(const ColliderPixel_Com & CopyCollider)
	:Collider_Com(CopyCollider)
{
	m_Virtual = CopyCollider.m_Virtual;

	m_Virtual.Color = new Pixel24[m_Virtual.Width * m_Virtual.Height];
	memcpy(m_Virtual.Color, CopyCollider.m_Virtual.Color, sizeof(Pixel24) * m_Virtual.Width * m_Virtual.Height);
}

ColliderPixel_Com::~ColliderPixel_Com()
{
	SAFE_DELETE_ARRARY(m_Virtual.Color);
}

bool ColliderPixel_Com::Init()
{
	return true;
}

int ColliderPixel_Com::Input(float DeltaTime)
{
	return 0;
}

int ColliderPixel_Com::Update(float DeltaTime)
{
	return 0;
}

int ColliderPixel_Com::LateUpdate(float DeltaTime)
{
	Vector3 TempPos = m_Transform->GetWorldPos() - m_Transform->GetPivot() * m_Transform->GetWorldScale();

	m_WorldInfo.ImageRect.Min = TempPos + m_Virtual.ImageRect.Min + m_Pivot * m_Virtual.ImageRect.Lenth;
	m_WorldInfo.ImageRect.Max = TempPos + m_Virtual.ImageRect.Max + m_Pivot * m_Virtual.ImageRect.Lenth;
	m_WorldInfo.ImageRect.Lenth = m_Virtual.ImageRect.Lenth;

	m_SectionMin = m_WorldInfo.ImageRect.Min;
	m_SectionMax = m_WorldInfo.ImageRect.Max;

	return 0;
}

void ColliderPixel_Com::Collision(float DeltaTime)
{
}

void ColliderPixel_Com::CollisionLateUpdate(float DeltaTime)
{
}

void ColliderPixel_Com::Render(float DeltaTime)
{
#ifdef _DEBUG
	Matrix	matPos, matScale, matView;
	matPos.Translation(m_WorldInfo.ImageRect.Min);
	//사이즈만큼 커져랏
	matScale.Scaling(m_WorldInfo.ImageRect.Lenth);

	if (m_CollisionGroupName != "UI")
		matView = m_Scene->GetMainCamera()->GetViewMatrix();

	Camera_Com*	getCamera = m_Scene->GetMainCamera();
	TransformCBuffer TransCBuffer = {};

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

ColliderPixel_Com * ColliderPixel_Com::Clone()
{
	return new ColliderPixel_Com(*this);
}

void ColliderPixel_Com::SetInfo(const Vector3 & Min, const Pixel24 & OutColor, const char * FileName, const string & PathKey)
{
	const char* Path = PathManager::Get()->FindPathMultiByte(PathKey);
	string FullPath;

	if (Path != NULLPTR)
		FullPath = Path;

	FullPath += FileName;

	FILE* pFile = NULLPTR;
	fopen_s(&pFile, FullPath.c_str(), "rb");

	if (pFile == NULLPTR)
		return;

	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER InfoHeader;

	fread(&FileHeader, sizeof(FileHeader), 1, pFile);
	fread(&InfoHeader, sizeof(FileHeader), 1, pFile);

	int Width = InfoHeader.biWidth;
	int Height = InfoHeader.biHeight;

	m_Virtual.OutColor = OutColor;
	m_Virtual.Color = new Pixel24[Width * Height];
	fread(m_Virtual.Color, sizeof(Pixel24), Width * Height, pFile);

	fclose(pFile);

	m_Virtual.ImageRect.Min = Min;
	m_Virtual.ImageRect.Max = Min + Vector3{ (float)Width, (float)Height, 0.0f };
	m_Virtual.ImageRect.Lenth = m_Virtual.ImageRect.Max - m_Virtual.ImageRect.Min;

	m_Virtual.ImageRect.Min.z = 0.0f;
	m_Virtual.ImageRect.Max.z = 0.0f;
	m_Virtual.ImageRect.Lenth.z = 1.0f;

	m_Virtual.Width = Width;
	m_Virtual.Height = Height;

	m_WorldInfo = m_Virtual;
}

bool ColliderPixel_Com::Collsion(Collider_Com * Dest, float DeltaTime)
{
	switch (m_CollType)
	{
		case CT_RECT:
			return CollsionRectToPixel(((ColliderRect_Com*)Dest)->GetInfo(), m_WorldInfo);
			break;
	}
	return false;
}
