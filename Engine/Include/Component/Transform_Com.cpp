#include "stdafx.h"
#include "Transform_Com.h"
#include "../GameObject.h"

JEONG::Transform_Com::Transform_Com()
{
	m_ComType = CT_TRANSFORM;
	m_isUpdate = true;
	m_isStatic = false;

	m_DeltaScale.Identity();
	m_DeltaRot.Identity();
	m_DeltaPos.Identity();
}

JEONG::Transform_Com::Transform_Com(const JEONG::Transform_Com& copyObject)
	:Component_Base(copyObject)
{
	*this = copyObject;
}

JEONG::Transform_Com::~Transform_Com()
{
}

bool JEONG::Transform_Com::Init()
{
	for (int i = 0; i < 3; i++)
	{
		m_LocalAxis[i] = Vector3::Axis[i];
		m_WorldAxis[i] = Vector3::Axis[i];
	}

	return true;
}

int JEONG::Transform_Com::Input(float DeltaTime)
{
	return 0;
}
//여기선 어차피 오브젝트가 계속 돌린다.
//오브젝트가 따로 Transform을 가지고있다.
int JEONG::Transform_Com::Update(float DeltaTime)
{
	if (m_isStatic == true)
		return 0;
	else if (m_isUpdate == false)
		return 0;

	m_MatLocal = m_MatLocalScale * m_MatLocalRotation * m_MatWorldPos;

	//최종World에 곱해질 Parent행렬 선언.
	Matrix Parent;
	Parent.Identity();

	//자기자신의 행렬정보를 변화량 변수에 넣어준 후 플래그에 따라서 곱한다.
	m_DeltaScale = m_MatWorldScale;
	m_DeltaRot = m_MatWorldRotation;
	m_DeltaPos = m_MatWorldPos;

	if (m_ParentFlag & TPF_SCALE)
	{
		m_DeltaScale *= m_ParentScale;
		Parent *= m_ParentScale;
	}

	if (m_ParentFlag & TPF_ROT)
	{
		m_DeltaRot *= m_ParentRot;
		Parent *= m_ParentRot;
	}

	if (m_ParentFlag & TPF_POS)
	{
		m_DeltaPos *= m_ParentPos;
		Parent *= m_ParentPos;
	}

	// WORLD = 자기꺼 * 부모꺼다.
	m_MatWorld = m_MatWorldScale * m_MatWorldRotation * m_MatWorldPos;
	m_MatWorld *= Parent;	//최종적으로 위 부모행렬과 곱한다. 만약 자식이 없거나 내가 부모, 플래그가 없다면
							//Identity 단위행렬로 들어가서 곱해봤자 자기자신이다.
	m_isUpdate = false;

	return 0;
}

int JEONG::Transform_Com::LateUpdate(float DeltaTime)
{
	if (m_isStatic == true)
		return 0;
	else if (m_isUpdate == false)
		return 0;

	m_MatLocal = m_MatLocalScale * m_MatLocalRotation * m_MatWorldPos;

	m_DeltaScale = m_MatWorldScale;
	m_DeltaRot = m_MatWorldRotation;
	m_DeltaPos = m_MatWorldPos;

	Matrix Parent;
	Parent.Identity();

	if (m_ParentFlag & TPF_SCALE)
	{
		m_DeltaScale *= m_ParentScale;
		Parent *= m_ParentScale;
	}

	if (m_ParentFlag & TPF_ROT)
	{
		m_DeltaRot *= m_ParentRot;
		Parent *= m_ParentRot;
	}

	if (m_ParentFlag & TPF_POS)
	{
		m_DeltaPos *= m_ParentPos;
		Parent *= m_ParentPos;
	}

	// WORLD = 자기꺼 * 부모꺼다.
	m_MatWorld = m_MatWorldScale * m_MatWorldRotation * m_MatWorldPos;
	m_MatWorld *= Parent;
	//World = S R T결합.

	return 0;
}

void JEONG::Transform_Com::Collision(float DeltaTime)
{
}

void JEONG::Transform_Com::CollisionLateUpdate(float DeltaTime)
{
}

void JEONG::Transform_Com::Render(float DeltaTime)
{
	m_DeltaMove = Vector3::Zero;
}

JEONG::Transform_Com * JEONG::Transform_Com::Clone()
{
	return new JEONG::Transform_Com(*this);
}

void JEONG::Transform_Com::SetLocalScale(const Vector3 & vScale)
{
	//크기값을 받는다
	m_LocalScale = vScale;
	//행렬에 크기값을 곱한다
	m_MatLocalScale.Scaling(m_LocalScale);

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetLocalScale(float x, float y, float z)
{
	//크기값을 받는다
	m_LocalScale = Vector3(x, y, z);
	//행렬에 크기값을 곱한다
	m_MatLocalScale.Scaling(m_LocalScale);

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetLocalRotation(const Vector3 & vRot)
{
	//회전값을 받는다
	m_LocalRotation = vRot;
	//회전행렬에 곱해준다
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetLocalRotation(float x, float y, float z)
{
	//회전값을 받는다
	m_LocalRotation = Vector3(x, y, z);
	//회전행렬에 곱해준다
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetLocalRotX(float x)
{
	//회전값을 받는다
	m_LocalRotation.x = x;
	//회전행렬에 곱해준다
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetLocalRotY(float y)
{
	//회전값을 받는다
	m_LocalRotation.y = y;
	//회전행렬에 곱해준다
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetLocalRotZ(float z)
{
	//회전값을 받는다
	m_LocalRotation.z = z;
	//회전행렬에 곱해준다
	m_MatLocalRotation.Rotation(m_LocalRotation);

	ComputeLocalAxis();

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetLocalPos(const Vector3 & vPos)
{
	m_LocalPos = vPos;

	m_MatLocalPos.Translation(m_LocalPos);

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetLocalPos(float x, float y, float z)
{
	m_LocalPos = Vector3(x, y, z);

	m_MatLocalPos.Translation(m_LocalPos);

	m_isUpdate = true;
}

//각 축의 방향을 알아온다.
void JEONG::Transform_Com::ComputeLocalAxis()
{
	for (int i = 0; i < 3; ++i)
	{
		//행렬 곱함수.
		m_LocalAxis[i] = Vector3::Axis[i].TransformNormal(m_MatLocalRotation);
		//크기1벡터로 만들어서 방향값을 얻어오기 위함.
		m_LocalAxis[i].Nomallize();
	}
}

void JEONG::Transform_Com::SetWorldScale(const Vector3 & vScale)
{
	//크기값을 받는다
	m_WorldScale = vScale;
	//행렬에 크기값을 곱한다
	m_MatWorldScale.Scaling(m_WorldScale);

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetWorldScale(float x, float y, float z)
{
	//크기값을 받는다
	m_WorldScale = Vector3(x, y, z);
	//행렬에 크기값을 곱한다
	m_MatWorldScale.Scaling(m_WorldScale);

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetWorldRotX(float x)
{
	//회전값을 받는다
	m_WorldRotation.x = x;
	//회전행렬에 곱해준다
	m_MatWorldRotation.Rotation(m_WorldRotation);

	ComputeWorldAxis();

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetWorldRotY(float y)
{
	//회전값을 받는다
	m_WorldRotation.y = y;
	//회전행렬에 곱해준다
	m_MatWorldRotation.Rotation(m_WorldRotation);

	ComputeWorldAxis();

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetWorldRotZ(float z)
{
	//회전값을 받는다
	m_WorldRotation.z = z;
	//회전행렬에 곱해준다
	m_MatWorldRotation.Rotation(m_WorldRotation);

	ComputeWorldAxis();

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetWorldPos(const Vector3 & vPos)
{
	m_DeltaMove = vPos - m_WorldPos;
	m_WorldPos = vPos;

	m_MatWorldPos.Translation(m_WorldPos);

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetWorldPos(float x, float y, float z)
{
	m_DeltaMove = Vector3(x, y, z) - m_WorldPos;
	m_WorldPos = Vector3(x, y, z);

	m_MatWorldPos.Translation(m_WorldPos);

	m_isUpdate = true;
}

void JEONG::Transform_Com::SetWorldPivot(const Vector3& vPos)
{
	m_Pivot = vPos;
}
void JEONG::Transform_Com::SetWorldPivot(float x, float y, float z)
{
	m_Pivot = Vector3(x, y, z);
}

void JEONG::Transform_Com::Move(AXIS eAxis, float Speed)
{
	Move(m_WorldAxis[eAxis] * Speed);
}

void JEONG::Transform_Com::Move(AXIS eAxis, float Speed, float DeltaTime)
{
	Move(m_WorldAxis[eAxis] * Speed * DeltaTime);
}

void JEONG::Transform_Com::Move(const Vector3 & vDir, float Speed)
{
	Move(vDir * Speed);
}

void JEONG::Transform_Com::Move(const Vector3 & vDir, float Speed, float DeltaTime)
{
	Move(vDir * Speed * DeltaTime);
}

void JEONG::Transform_Com::Move(const Vector3 & vMove)
{
	m_WorldPos += vMove;
	m_DeltaMove = vMove;

	m_MatWorldPos.Translation(m_WorldPos);

	m_isUpdate = true;
}

void JEONG::Transform_Com::RotationX(float x)
{
	Rotation(Vector3(x, 0.0f, 0.0f));
}

void JEONG::Transform_Com::RotationX(float x, float DeltaTime)
{
	Rotation(Vector3(x * DeltaTime, 0.0f, 0.0f));
}

void JEONG::Transform_Com::RotationY(float y)
{
	Rotation(Vector3(0.0f, y, 0.0f));
}

void JEONG::Transform_Com::RotationY(float y, float DeltaTime)
{
	Rotation(Vector3(0.0f, y * DeltaTime, 0.0f));
}

void JEONG::Transform_Com::RotationZ(float z)
{
	Rotation(Vector3(0.0f, 0.0f, z));
}

void JEONG::Transform_Com::RotationZ(float z, float DeltaTime)
{
	//로테이션함수에서 += 해준다.
	Rotation(Vector3(0.0f, 0.0f, z * DeltaTime));
}

void JEONG::Transform_Com::Rotation(const Vector3 & vRot, float DeltaTime)
{
	Rotation(vRot * DeltaTime);
}

void JEONG::Transform_Com::Rotation(const Vector3 & vRot)
{
	m_WorldRotation += vRot;
	m_MatWorldRotation.Rotation(m_WorldRotation);
	//회전행렬값에따라서 내 WorldAxis값을 변환한다.
	ComputeWorldAxis();
	m_isUpdate = true;
}

//각 축의 방향을 알아온다.
void JEONG::Transform_Com::ComputeWorldAxis()
{
	for (int i = 0; i < 3; ++i)
	{
		//지정된 매트릭스(회전행렬)에 따라 벡터를 반환한다.
		m_WorldAxis[i] = Vector3::Axis[i].TransformNormal(m_MatWorldRotation);
		//크기1벡터로 만들어서 방향값을 얻어오기 위함.
		m_WorldAxis[i].Nomallize();
	}
}

void JEONG::Transform_Com::LookAt(JEONG::GameObject * object, AXIS eAxis)
{
	LookAt(object->GetTransform()->GetWorldPos(), eAxis);
}

void JEONG::Transform_Com::LookAt(Component_Base * component, AXIS eAxis)
{
	LookAt(component->GetTransform()->GetWorldPos(), eAxis);
}

void JEONG::Transform_Com::LookAt(const Vector3 & Vec, AXIS eAxis)
{
	//바라보려는 방향을 구한다. (벡터뺄셈 = 바라보는방향) 
	Vector3 View = Vec - m_WorldPos;
	View.Nomallize();

	//가상축을 선언한다.
	Vector3 Axis = Vector3::Axis[eAxis];

	//가상축과 바라보려는 방향(View)의 각도를 구한다 (내적)
	float Angle = Axis.GetAngle(View);

	Vector3 vRotAxis = Axis.Cross(View);
	vRotAxis.Nomallize();

	//가상 축에 대한 회전행렬 생성.
	m_MatWorldRotation.RotationAxis(Angle, vRotAxis);
	ComputeWorldAxis();

	m_isUpdate = true;
}

float JEONG::Transform_Com::GetAngle(JEONG::GameObject * Target)
{
	return GetAngle(Target->GetTransform());
}

float JEONG::Transform_Com::GetAngle(Transform_Com * Target)
{
	return m_WorldPos.GetAngle(Target->GetWorldPos());
}

void JEONG::Transform_Com::SetParentFlag(int Flag)
{
	m_ParentFlag = Flag;
}

void JEONG::Transform_Com::AddParentFlag(TRANSFORM_PARENT_FLAG Flag)
{
	m_ParentFlag |= Flag;
}

void JEONG::Transform_Com::DeleteParentFlag(TRANSFORM_PARENT_FLAG Flag)
{
	if (m_ParentFlag & Flag)
		m_ParentFlag ^= Flag; //xor0일때 1 , 1일때 0
}

void JEONG::Transform_Com::DeleteParentFlag()
{
	m_ParentFlag = 0;
}

void JEONG::Transform_Com::SetParentPos(const Matrix& parentPos)
{
	m_ParentPos = parentPos;
}
void JEONG::Transform_Com::SetParentRot(const Matrix& parentRot)
{
	m_ParentRot = parentRot;
}

void JEONG::Transform_Com::SetParentScale(const Matrix& parentScale)
{
	m_ParentScale = parentScale;
}

Matrix JEONG::Transform_Com::GetParentPos() const
{
	return m_ParentPos;
}

Matrix JEONG::Transform_Com::GetParentRot() const
{
	return m_ParentRot;
}

Matrix JEONG::Transform_Com::GetParentScale() const
{
	return m_ParentScale;
}