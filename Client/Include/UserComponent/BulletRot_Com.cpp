#include "ClientHeader.h"
#include "BulletRot_Com.h"
#include "GameObject.h"

#include "Component/Transform_Com.h"
#include "Component/Renderer_Com.h"
#include "Component/Material_Com.h"
#include "Component/ColliderRect_Com.h"

JEONG_USING

BulletRot_Com::BulletRot_Com()
	:Target(NULLPTR), MoveSpeed(700.0f), RotAngle(0.0f), Time(0.0f), isGap(false)
{
	m_ComType = (COMPONENT_TYPE)UT_BULLET;
}

BulletRot_Com::BulletRot_Com(const BulletRot_Com & userCom)
	: UserComponent_Base(userCom)
{
	*this = userCom;
	MoveSpeed = userCom.MoveSpeed;
	RotAngle = 0.0f;
	Time = 0.0f;
	isGap = false;
	Target = NULLPTR;
}

BulletRot_Com::~BulletRot_Com()
{
	SAFE_RELEASE(Target);
}

bool BulletRot_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("BulletRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "JeongEn", TEXT("JeongEn.png"));
	SAFE_RELEASE(MaterialComponent);

	ColliderRect_Com* RectColl = m_Object->AddComponent<ColliderRect_Com>("BulletRotBody");
	RectColl->SetInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(150.0f, 150.0f, 0.0f));
	SAFE_RELEASE(RectColl);

	m_Transform->SetWorldScale(150.0f, 150.0f, 0.0f);
	m_Transform->SetWorldPivot(0.5f, 0.0f, 0.0f);

	return true;
}

int BulletRot_Com::Input(float DeltaTime)
{
	return 0;
}

int BulletRot_Com::Update(float DeltaTime)
{
	Vector3 Look = Target->GetTransform()->GetWorldPos() - m_Transform->GetWorldPos();
	Vector3 ShotDir = m_Transform->GetWorldAxis(AXIS_Y);
	float Angle = ShotDir.GetAngle(Look);
	float Distance = m_Transform->GetWorldPos().GetDistance(Target->GetTransform()->GetWorldPos());

	//외적 z값이 0보다 크거나 작거나에따라서 앞뒤판단
	Vector3 Cross = ShotDir.Cross(Look);
	Cross.Nomallize();

	Time += DeltaTime;

	//스피드와 거리간의 비율을 곱해줘서 거리에따라서 더 빠르게돈다.
	m_Transform->RotationZ(Angle * Cross.z, DeltaTime * (MoveSpeed / Distance));
	m_Transform->Move(AXIS_Y, MoveSpeed, DeltaTime);

	if (Time >= 5.0f)
		m_Object->SetIsActive(false);

	return 0;
}

int BulletRot_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void BulletRot_Com::Collision(float DeltaTime)
{
}

void BulletRot_Com::CollisionLateUpdate(float DeltaTime)
{
}

void BulletRot_Com::Render(float DeltaTime)
{
}

BulletRot_Com * BulletRot_Com::Clone()
{
	return new BulletRot_Com(*this);
}

void BulletRot_Com::SetTarget(GameObject * target)
{
	Target = target;
}
