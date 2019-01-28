#include "ClientHeader.h"

#include "BulletBoom_Com.h"
#include "GameObject.h"

#include "Component/Transform_Com.h"
#include "Component/Renderer_Com.h"
#include "Component/Material_Com.h"
#include "Component/ColliderRect_Com.h"

BulletBoom_Com::BulletBoom_Com()
{
	SetTag("BulletBoom");
}

BulletBoom_Com::BulletBoom_Com(const BulletBoom_Com & userCom)
	:UserComponent_Base(userCom)
{
	*this = userCom;

	myAnimation = NULLPTR;
}

BulletBoom_Com::~BulletBoom_Com()
{
	SAFE_RELEASE(myAnimation);
}

bool BulletBoom_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("BoomRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "Boom", TEXT("Boom.png"));
	SAFE_RELEASE(MaterialComponent);

	myAnimation = m_Object->AddComponent<Animation2D_Com>("BoomAnimation");

	vector<Clip2DFrame> vecClipFrame;
	Clip2DFrame	tFrame = {};

	for (int i = 0; i < 25; ++i)
	{
		tFrame.LeftTop = Vector2(0.0f + i * 255.0f, 0.0f);
		tFrame.RightBottom = Vector2(255.0f + (i * 255.0f), 185.0f);
		vecClipFrame.push_back(tFrame);
	}

	myAnimation->AddClip("Boom", A2D_ATLS, AO_ONCE_DESTROY, 2.0f, vecClipFrame, "Boom", L"Boom.png");

	m_Transform->SetWorldPivot(0.5f, 0.0f, 0.0f);
	m_Transform->SetWorldScale(500.0f, 500.0f, 1.0f);

	return true;
}

int BulletBoom_Com::Input(float DeltaTime)
{
	return 0;
}

int BulletBoom_Com::Update(float DeltaTime)
{
	return 0;
}

int BulletBoom_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void BulletBoom_Com::Collision(float DeltaTime)
{
}

void BulletBoom_Com::CollisionLateUpdate(float DeltaTime)
{
}

void BulletBoom_Com::Render(float DeltaTime)
{
}

BulletBoom_Com * BulletBoom_Com::Clone()
{
	return new BulletBoom_Com(*this);
}

