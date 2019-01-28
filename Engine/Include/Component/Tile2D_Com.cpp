#include "stdafx.h"
#include "Tile2D_Com.h"
#include "TileImage_Com.h"

#include "../Resource/Mesh.h"
#include "../Render/Shader.h"

JEONG_USING

Tile2D_Com::Tile2D_Com()
	:m_Shader(NULLPTR), m_Mesh(NULLPTR), m_Layout(NULLPTR), m_TileImage(NULLPTR)
{
	m_ComType = CT_STAGE2D;
	SetTag("Stage2D");
	m_isLine = true;
}

Tile2D_Com::Tile2D_Com(const Tile2D_Com& CopyData)
	:Component_Base(CopyData)
{
	*this = CopyData;
}

Tile2D_Com::~Tile2D_Com()
{
	SAFE_RELEASE(m_Mesh);
	SAFE_RELEASE(m_Shader);
	SAFE_RELEASE(m_TileImage);

	Safe_Release_VecList(m_vecTileImage);
}

bool Tile2D_Com::Init()
{
	m_TileOption = T2D_NORMAL;

	m_Mesh = ResourceManager::Get()->FindMesh("ColliderRect");
	m_Shader = ShaderManager::Get()->FindShader(COLLIDER_SHADER);

	m_Transform->SetWorldPivot(0.5f, 0.5f, 0.0f);
	m_vecTileImage.resize(3);

	return true;
}

int Tile2D_Com::Input(float DeltaTime)
{
	//m_TileImage->Input(DeltaTime);

	//for (size_t i = 0; i < m_vecTileImage.size(); i++)
	//{
	//	if (m_vecTileImage[i] != NULLPTR)
	//		continue;
	//		
	//	m_vecTileImage[i]->Input(DeltaTime);
	//}

	return 0;
}

int Tile2D_Com::Update(float DeltaTime)
{
	//m_TileImage->Update(DeltaTime);

	//for (size_t i = 0; i < m_vecTileImage.size(); i++)
	//{
	//	if (m_vecTileImage[i] != NULLPTR)
	//		continue;

	//	m_vecTileImage[i]->Update(DeltaTime);
	//}

	return 0;
}

int Tile2D_Com::LateUpdate(float DeltaTime)
{
	//m_TileImage->LateUpdate(DeltaTime);

	//for (size_t i = 0; i < m_vecTileImage.size(); i++)
	//{
	//	if (m_vecTileImage[i] != NULLPTR)
	//		continue;

	//	m_vecTileImage[i]->LateUpdate(DeltaTime);
	//}
	return 0;
}

void Tile2D_Com::Collision(float DeltaTime)
{
	//m_TileImage->Collision(DeltaTime);

	//for (size_t i = 0; i < m_vecTileImage.size(); i++)
	//{
	//	if (m_vecTileImage[i] != NULLPTR)
	//		continue;

	//	m_vecTileImage[i]->Collision(DeltaTime);
	//}
}

void Tile2D_Com::CollisionLateUpdate(float DeltaTime)
{
	//m_TileImage->CollisionLateUpdate(DeltaTime);

	//for (size_t i = 0; i < m_vecTileImage.size(); i++)
	//{
	//	if (m_vecTileImage[i] != NULLPTR)
	//		continue;

	//	m_vecTileImage[i]->CollisionLateUpdate(DeltaTime);
	//}
}

void Tile2D_Com::Render(float DeltaTime)
{
	//m_TileImage->Render(DeltaTime);

	//for (size_t i = 0; i < m_vecTileImage.size(); i++)
	//{
	//	if (m_vecTileImage[i] != NULLPTR)
	//		continue;

	//	m_vecTileImage[i]->Render(DeltaTime);
	//}

	if (m_isLine == false)
		return;

	TransformCBuffer tCBuffer = {};
	Camera_Com*	pMainCamera = m_Scene->GetMainCamera();

	tCBuffer.World = m_Transform->GetWorldMatrix();
	tCBuffer.View = pMainCamera->GetViewMatrix();
	tCBuffer.Projection = pMainCamera->GetProjection();
	tCBuffer.WV = tCBuffer.World * tCBuffer.View;
	tCBuffer.WVP = tCBuffer.WV * tCBuffer.Projection;
	tCBuffer.Pivot = Vector3::Zero;
	tCBuffer.Lenth = m_Mesh->GetLenth();

	tCBuffer.World.Transpose();
	tCBuffer.View.Transpose();
	tCBuffer.Projection.Transpose();
	tCBuffer.WV.Transpose();
	tCBuffer.WVP.Transpose();

	ShaderManager::Get()->UpdateCBuffer("Transform", &tCBuffer);

	Vector4	LineColor;
	switch (m_TileOption)
	{
		case T2D_NORMAL:
			LineColor = Vector4::Green;
			break;
		case T2D_NOMOVE:
			LineColor = Vector4::Red;
			break;
	}

	m_Shader->SetShader();
	{
		Device::Get()->GetContext()->IASetInputLayout(m_Layout);
		ShaderManager::Get()->UpdateCBuffer("Collider", &LineColor);
	}
	m_Mesh->Render();
}

Tile2D_Com * Tile2D_Com::Clone()
{
	return new Tile2D_Com(*this);
}

void Tile2D_Com::AfterClone()
{
}

void Tile2D_Com::Save(BineryWrite & Writer)
{
	Component_Base::Save(Writer);

	Writer.WriteData((int)m_TileType);
	Writer.WriteData((int)m_TileOption);
}

void Tile2D_Com::Load(BineryRead & Reader)
{
	Component_Base::Load(Reader);

	Reader.ReadData((int&)m_TileType);
	Reader.ReadData((int&)m_TileOption);

	SetTileType(m_TileType);

	switch (m_TileType)
	{
		case STT_TILE:
		{
			switch (m_TileOption)
			{
				case T2D_NORMAL:
					m_Mesh = ResourceManager::Get()->FindMesh("ColliderRect");
					break;
				case T2D_NOMOVE:
					m_Mesh = ResourceManager::Get()->FindMesh("TileNoMove");
					break;
			}
		}
			break;
		case STT_ISO:
		{
			switch (m_TileOption)
			{
				case T2D_NORMAL:
					m_Mesh = ResourceManager::Get()->FindMesh("IsoTileNomal");
					break;
				case T2D_NOMOVE:
					m_Mesh = ResourceManager::Get()->FindMesh("IsoTileNomove");
					break;
			}
		}
			break;
	}
}

void Tile2D_Com::SetTileType(STAGE2D_TILE_TYPE type)
{
	m_TileType = type;
	
	SAFE_RELEASE(m_Mesh);
	SAFE_RELEASE(m_Shader);

	switch (type)
	{
		case STT_TILE:
			m_Mesh = ResourceManager::Get()->FindMesh("ColliderRect");
			break;
		case STT_ISO:
			m_Mesh = ResourceManager::Get()->FindMesh("IsoTileNomal");
			break;
	}

	m_Shader = ShaderManager::Get()->FindShader(TILE_SHADER);
	m_Layout = ShaderManager::Get()->FindInputLayOut(POS_LAYOUT);
}

void Tile2D_Com::SetMesh(const string & KeyName)
{
	SAFE_RELEASE(m_Mesh);

	m_Mesh = ResourceManager::Get()->FindMesh(KeyName);
}

void Tile2D_Com::SetWorldPos(const Vector3 & Pos)
{
	m_Transform->SetWorldPos(Pos);
	m_TileImage->GetTransform()->SetWorldPos(Pos);

	for (size_t i = 0; i < m_vecTileImage.size(); i++)
		m_vecTileImage[i]->GetTransform()->SetWorldPos(Pos);
}

void Tile2D_Com::AddMainTileImage(const string& FileName)
{
	GameObject* newTileImageObject = GameObject::CreateObject("TileImageObject");
	m_TileImage = newTileImageObject->AddComponent<TileImage_Com>("TileImage");
	m_TileImage->SetTexture(FileName, CA2W(FileName.c_str()));

	newTileImageObject->GetTransform()->SetWorldPos(m_Transform->GetWorldPos());

	SAFE_RELEASE(newTileImageObject);
}

void Tile2D_Com::AddSubTileImage(const string& FileName, size_t ImageCount)
{
	GameObject* newTileImageObject = GameObject::CreateObject("TileImageObject");
	TileImage_Com* newImage = newTileImageObject->AddComponent<TileImage_Com>("TileImage");

	for (size_t i = 0; i < ImageCount; i++)
	{
		//float RandomAngle = RandomRange(-45, 45);

		switch (i)
		{
			case 0:
			{
				newImage->SetDistance(300.0f);
				newImage->SetTexture(FileName , CA2W(FileName.c_str()));
			}
				break;
			case 1:
			{
				newImage->SetDistance(200.0f);
				newImage->SetTexture(FileName, CA2W(FileName.c_str()));
			}
				break;
			case 2:
			{
				newImage->SetDistance(100.0f);
				newImage->SetTexture(FileName, CA2W(FileName.c_str()));
			}
				break;
		}
	}


	m_vecTileImage.push_back(newImage);

	SAFE_RELEASE(newTileImageObject);
}