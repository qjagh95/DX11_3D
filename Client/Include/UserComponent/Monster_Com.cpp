#include "ClientHeader.h"
#include "Monster_Com.h"
#include "GameObject.h"
#include "BulletBoom_Com.h"

#include "Component/Transform_Com.h"
#include "Component/Renderer_Com.h"
#include "Component/Material_Com.h"
#include "Component/ColliderCircle_Com.h"
#include "Component/ColliderOBB2D_Com.h"

Monster_Com::Monster_Com()
	:Target(NULLPTR), TimeVar(0.0f)
{
}

Monster_Com::Monster_Com(const Monster_Com & userCom)
	:UserComponent_Base(userCom)
{
	Target = NULLPTR;
}

Monster_Com::~Monster_Com()
{
	SAFE_RELEASE(Target);
}

bool Monster_Com::Init()
{
	Renderer_Com* RenderComponent = m_Object->AddComponent<Renderer_Com>("MonsterRender");
	RenderComponent->SetMesh("TextureRect");
	RenderComponent->SetRenderState(ALPHA_BLEND);
	SAFE_RELEASE(RenderComponent);

	Material_Com* MaterialComponent = m_Object->FindComponentFromType<Material_Com>(CT_MATERIAL);
	MaterialComponent->SetDiffuseTexture(0, "Minion", TEXT("yso.jpg"));

	SAFE_RELEASE(MaterialComponent);

	m_Transform->SetWorldPos(500.0f, 500.0f, 0.0f);
	m_Transform->SetWorldScale(300.0f, 300.0f, 1.0f);
	m_Transform->SetWorldPivot(0.5f, 0.0f, 0.0f);

	//ColliderRect_Com* RectColl = m_Object->AddComponent<ColliderRect_Com>("MonsterBody");
	//RectColl->SetInfo(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 100.0f, 0.0f));
	//RectColl->SetMyTypeName("Monster");
	//RectColl->SetCollsionCallback<Monster_Com>(CCT_FIRST, this, &Monster_Com::BulletHit);
	//RectColl->SetCollsionCallback<Monster_Com>(CCT_FIRST, this, &Monster_Com::BulletRotHit);
	//SAFE_RELEASE(RectColl);

	ColliderCircle_Com* CircleColl = m_Object->AddComponent<ColliderCircle_Com>("MCircleBody");
	CircleColl->SetInfo(150.0f);
	CircleColl->SetMyTypeName("M");
	CircleColl->SetCollsionCallback<Monster_Com>(CCT_FIRST, this, &Monster_Com::BulletHit);
	CircleColl->SetCollsionCallback<Monster_Com>(CCT_FIRST, this, &Monster_Com::BulletRotHit);
	SAFE_RELEASE(CircleColl);

	//ColliderOBB2D_Com* OBBColl = m_Object->AddComponent<ColliderOBB2D_Com>("MonsterOBBBody");
	//Vector3 Axis[2] = { Vector3::Axis[0], Vector3::Axis[1] };
	//float Lenth[2] = { 150.0f, 150.0f };
	//OBBColl->SetInfo(Vector3(0.0f, 150.0f, 0.0f), Axis, Lenth);
	//SAFE_RELEASE(OBBColl);

	Target = GameObject::FindObject("Player");
	return true;
}

int Monster_Com::Input(float DeltaTime)
{
	return 0;
}

int Monster_Com::Update(float DeltaTime)
{
	//if (m_Transform->GetWorldPos().GetDistance(Target->GetTransform()->GetWorldPos()) < 300.0f)
	//	m_Transform->LookAt(Target, AXIS_Y);

	//TimeVar += DeltaTime;

	//if (TimeVar >= 1.0f)
	//{
	//	GameObject* newBullet = GameObject::CreateClone("Bullet", "Bullet_Clone", m_Layer);
	//	newBullet->SetMoveDir(MD_DOWN);
	//	newBullet->GetTransform()->SetWorldPos(m_Transform->GetWorldPos());

	//	Vector3 Look = Target->GetTransform()->GetWorldPos() - m_Transform->GetWorldPos();
	//	Vector3 ShotDir = newBullet->GetTransform()->GetWorldAxis(AXIS_Y);
	//	float Angle = ShotDir.GetAngle(Look);

	//	//외적으로 앞뒤판단.
	//	Vector3 Cross = ShotDir.Cross(Look);
	//	Cross.Nomallize();

	//	newBullet->GetTransform()->RotationZ(Angle * Cross.z);

	//	SAFE_RELEASE(newBullet);
	//	TimeVar = 0.0f; 
	//}

	return 0;
}

int Monster_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Monster_Com::Collision(float DeltaTime)
{
}

void Monster_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Monster_Com::Render(float DeltaTime)
{
}

Monster_Com * Monster_Com::Clone()
{
	return new Monster_Com(*this);
}

void Monster_Com::BulletHit(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "BulletBody")
	{
		GameObject* newClone = GameObject::CreateClone("Boom", "Boom", m_Layer);
		newClone->GetTransform()->SetWorldPos(m_Transform->GetWorldPos());
		SAFE_RELEASE(newClone);

		Dest->GetGameObject()->SetIsActive(false);
	}
}

void Monster_Com::BulletRotHit(Collider_Com * Src, Collider_Com * Dest, float DeltaTime)
{
	if (Dest->GetTag() == "BulletRotBody")
	{
		GameObject* newClone = GameObject::CreateClone("Boom", "Boom", m_Layer);
		newClone->GetTransform()->SetWorldPos(m_Transform->GetWorldPos());
		SAFE_RELEASE(newClone);

		Dest->GetGameObject()->SetIsActive(false);
	}


}
