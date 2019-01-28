#include "stdafx.h"
#include "BackColor_Com.h"

JEONG_USING

BackColor_Com::BackColor_Com()
{
	m_ComType = CT_BACKCOLOR;
}

BackColor_Com::BackColor_Com(const BackColor_Com & CopyData)
	:UIBase_Com(CopyData)
{
	*this = CopyData;
}

BackColor_Com::~BackColor_Com()
{
	SAFE_RELEASE(m_Material);
}

bool BackColor_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("BackColorObject");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	RenderComponent->SetScreenRender(true);
	SAFE_RELEASE(RenderComponent);

	m_Color = Vector4::Blue;

	m_Material = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	m_Material->SetDiffuseTexture(0, "White", TEXT("White.png"));
	m_Material->SetMaterial(m_Color);

	m_Transform->SetWorldScale(Vector3((float)Device::Get()->GetWinSize().Width, (float)Device::Get()->GetWinSize().Height, 1.0f));

	return true;
}

int BackColor_Com::Input(float DeltaTime)
{
	m_Material->SetMaterial(m_Color);
	return 0;
}

int BackColor_Com::Update(float DeltaTime)
{
	return 0;
}

int BackColor_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void BackColor_Com::Collision(float DeltaTime)
{
}

void BackColor_Com::CollisionLateUpdate(float DeltaTime)
{
}

void BackColor_Com::Render(float DeltaTime)
{
}

BackColor_Com * BackColor_Com::Clone()
{
	return new BackColor_Com(*this);
}

void BackColor_Com::AfterClone()
{
}
