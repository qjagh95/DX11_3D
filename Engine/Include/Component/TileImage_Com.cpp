#include "stdafx.h"
#include "TileImage_Com.h"

JEONG_USING

TileImage_Com::TileImage_Com()
{
	m_ComType = CT_TILEIMAGE;
}

TileImage_Com::TileImage_Com(const TileImage_Com & CopyData)
	:Component_Base(CopyData)
{
}

TileImage_Com::~TileImage_Com()
{
}

bool TileImage_Com::Init()
{
	Renderer_Com* TileImageRender = m_Object->AddComponent<Renderer_Com>("ChildImageRender");
	TileImageRender->SetMesh("TextureRect");
	TileImageRender->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(TileImageRender);

	m_Distance = 400;
	m_Transform->SetWorldScale(Vector3(128.0f, 128.0f, 0.0f));
	return true;
}

int TileImage_Com::Input(float DeltaTime)
{
	return 0;
}

int TileImage_Com::Update(float DeltaTime)
{
	//오브젝트 리스트 돌려서 Distance비교 확대, 축소 로테이션...

	return 0;
}

int TileImage_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void TileImage_Com::Collision(float DeltaTime)
{
}

void TileImage_Com::CollisionLateUpdate(float DeltaTime)
{
}

void TileImage_Com::Render(float DeltaTime)
{
}

TileImage_Com * TileImage_Com::Clone()
{
	return new TileImage_Com(*this);
}

void TileImage_Com::AfterClone()
{
}

void TileImage_Com::Save(BineryWrite & Writer)
{
	Component_Base::Save(Writer);

	Writer.WriteData(m_Distance);
}

void TileImage_Com::Load(BineryRead & Reader)
{
	Component_Base::Load(Reader);

	Reader.ReadData(m_Distance);
}

void TileImage_Com::SetTexture(const string& KeyName, wchar_t* FileName, const string & PathKey)
{
	Material_Com* getMaterial = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	getMaterial->SetDiffuseTexture(0, KeyName, FileName);
	SAFE_RELEASE(getMaterial);
}

void TileImage_Com::SetDiffuseColor(const Vector4 & Color)
{
	Material_Com* getMaterial = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	getMaterial->SetMaterial(Color);
	SAFE_RELEASE(getMaterial);
}
