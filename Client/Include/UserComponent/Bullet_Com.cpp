#include "ClientHeader.h"

#include "Bullet_Com.h"
#include "GameObject.h"

#include "Component/Transform_Com.h"
#include "Component/Renderer_Com.h"
#include "Component/Material_Com.h"
#include "Component/ColliderRect_Com.h"

Bullet_Com::Bullet_Com()
	:MoveSpeed(500.0f)
{
	m_ComType = (COMPONENT_TYPE)UT_BULLET;
}

Bullet_Com::Bullet_Com(const Bullet_Com & userCom)
	:UserComponent_Base(userCom)
{
	*this = userCom;
}

Bullet_Com::~Bullet_Com()
{
}

bool Bullet_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("BulletRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "JeongEn", TEXT("JeongEn.png"));
	SAFE_RELEASE(MaterialComponent);

	ColliderRect_Com* RectColl = m_Object->AddComponent<ColliderRect_Com>("BulletBody");
	RectColl->SetInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(150.0f, 150.0f, 0.0f));
	RectColl->SetPivot(Vector3(0.5f, 0.0f, 0.0f));
	RectColl->SetMyTypeName("Bullet");
	RectColl->PushContinueTypeName("Bullet");
	RectColl->PushContinueTypeName("Player");
	RectColl->PushContinueTypeName("MouseWorld");

	SAFE_RELEASE(RectColl);

	m_Transform->SetWorldScale(150.0f, 150.0f, 0.0f);
	m_Transform->SetWorldPivot(0.5f, 0.0f, 0.0f);

	return true;
}

int Bullet_Com::Input(float DeltaTime)
{
	return 0;
}

int Bullet_Com::Update(float DeltaTime)
{
	//생성할때 로테이션 함수에서 이미 WorldAxis변수를 갱신한다.
	m_Transform->Move(AXIS_Y, MoveSpeed, DeltaTime);

	if (m_Transform->GetWorldPos().x <= 0.0f)
		m_Object->SetIsActive(false);
	else if(m_Transform->GetWorldPos().x >= 1000.0f)
		m_Object->SetIsActive(false);

	if(m_Transform->GetWorldPos().y <= 0.0f)
		m_Object->SetIsActive(false);
	else if(m_Transform->GetWorldPos().y >= 700.0f)
		m_Object->SetIsActive(false);

	return 0;
}

int Bullet_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Bullet_Com::Collision(float DeltaTime)
{
}

void Bullet_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Bullet_Com::Render(float DeltaTime)
{
}

Bullet_Com * Bullet_Com::Clone()
{
	return new Bullet_Com(*this);
}
