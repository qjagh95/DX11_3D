#include "stdafx.h"
#include "Camera_Com.h"
#include "../GameObject.h"
#include "Transform_Com.h"
JEONG_USING

Camera_Com::Camera_Com()
{
	m_ComType = CT_CAMERA;
	m_CameraType = CT_PERSPECTIVE;
	m_Target = NULLPTR;
}

Camera_Com::~Camera_Com()
{
}

Camera_Com::Camera_Com(const Camera_Com & CopyData)
	:Component_Base(CopyData)
{
	m_CameraType = CopyData.m_CameraType;
	m_View = CopyData.m_View;
	m_Projection = CopyData.m_Projection;
	m_Target = NULLPTR;
}

bool Camera_Com::Init()
{

	return true;
}

int Camera_Com::Input(float DeltaTime)
{
	return 0;
}

int Camera_Com::Update(float DeltaTime)
{
	//ī�޶� �� ��ȯ�� ��� ��� ������Ʈ�� ������������ 
	//������Ʈ�� �Ű��ִ� �����̴�.
	//ī�޶� �����̴°� �ƴ� ������Ʈ�� ������.
	m_View.Identity();

	if (m_Target != NULLPTR)
	{
		Vector3	Move = m_Target->GetDeltaMove();

		if (Move != Vector3::Zero)
			m_Transform->Move(Move);
	}

	/*
	Xx Xy Xz 0
	Yx Yy Yz 0
	Zx Zy Zz 0
	0  0  0  1
	*/

	Vector3 TempPos = m_Transform->GetWorldPos();

	//ī�޶� ���� �̿��Ͽ� ����� �������
	//��������� ��ġ��İ� ������� ���� ����.
	//ī�޶� ���� �����´�.
	for (unsigned int i = 0; i < AXIS_MAX; i++)
		memcpy(&m_View[i][0], &m_Transform->GetWorldAxis((AXIS)i), sizeof(Vector3));
	
	/*
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1
	*/

	//����� = ��ġ���
	m_View.Transpose();

	//�������� �ǵ�����.
	TempPos *= -1.0f;

	//�����ϴ� ���� : ī�޶� �� * ��� = ������
	//(41, 42, 43) ���� ���.
	for (unsigned int i = 0; i < AXIS_MAX; i++)
		m_View[3][i] = TempPos.Dot(m_Transform->GetWorldAxis((AXIS)i));

	return 0;
}

int Camera_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void Camera_Com::Collision(float DeltaTime)
{
}

void Camera_Com::CollisionLateUpdate(float DeltaTime)
{
}

void Camera_Com::Render(float DeltaTime)
{
}

Camera_Com * Camera_Com::Clone()
{
	return new Camera_Com(*this);
}

void Camera_Com::SetCameraType(CAMERA_TYPE eType)
{
	m_CameraType = eType;

	switch (eType)
	{
		/*
		Perspective ���� ��� ����

		�Ÿ������� Scale���� = tan
		z�� 0 ~ 1

		(1 / tan(Fov / 2) / ��Ⱦ��, 0, 0, 0)
		(0 , 1/tan(Fov / 2) , 0, 0)
		(0, 0, zf / (zf - zn), 1)
		(0 , 0, -zn * zf / (zf - zn), 0)

		*/
		case CT_PERSPECTIVE:
			m_Projection = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle), m_Width / m_Height, m_Near, m_Far);
			break;

		/*
		Ortho ���� ��� ����

		r = winsize Min
		l = winsize max

		2/(r-l)      0            0           0
		0            2/(t-b)      0           0
		0            0            1/(zf-zn)   0
		(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
		*/
		case CT_ORTHO:
			m_Projection = XMMatrixOrthographicOffCenterLH(0.0f, m_Width, 0.0f, m_Height, m_Near, m_Far);
			break;
	}
}

void Camera_Com::SetCameraInfo(CAMERA_TYPE eType, float Width, float Height, float ViewAngle, float Near, float Far)
{
	m_Width = Width;
	m_Height = Height;
	m_Near = Near;
	m_Far = Far;
	m_ViewAngle = ViewAngle;

	SetCameraType(eType);
}

void Camera_Com::SetWidth(float Width)
{
	m_Width = Width;
	SetCameraType(m_CameraType);
}

void Camera_Com::SetHeight(float Height)
{
	m_Height = Height;
	SetCameraType(m_CameraType);
}

void Camera_Com::SetViewAngle(float Angle)
{
	m_ViewAngle = Angle;
	SetCameraType(m_CameraType);
}

void Camera_Com::SetNear(float Near)
{
	m_Near = Near;
	SetCameraType(m_CameraType);
}

void Camera_Com::SetFar(float Far)
{
	m_Far = Far;
	SetCameraType(m_CameraType);
}

Matrix Camera_Com::GetViewMatrix() const
{
	return m_View;
}

Matrix Camera_Com::GetProjection() const
{
	return m_Projection;
}

void Camera_Com::SetTarget(GameObject* pTarget)
{
	m_Target = pTarget->GetTransform();
}

void Camera_Com::SetTarget(Component_Base* pTarget)
{
	m_Target = pTarget->GetTransform();
}