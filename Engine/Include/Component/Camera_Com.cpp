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
	//카메라 뷰 변환은 사실 모든 오브젝트를 원점기준으로 
	//오브젝트를 옮겨주는 역할이다.
	//카메라가 움직이는게 아닌 오브젝트가 움직임.
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

	//카메라 축을 이용하여 행렬은 직교행렬
	//직교행렬은 전치행렬과 역행렬이 서로 같다.
	//카메라 축을 가져온다.
	for (unsigned int i = 0; i < AXIS_MAX; i++)
		memcpy(&m_View[i][0], &m_Transform->GetWorldAxis((AXIS)i), sizeof(Vector3));
	
	/*
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1
	*/

	//역행렬 = 전치행렬
	m_View.Transpose();

	//원점으로 되돌린다.
	TempPos *= -1.0f;

	//내적하는 이유 : 카메라 축 * 행렬 = 내적값
	//(41, 42, 43) 값의 결과.
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
		Perspective 투영 행렬 공식

		거리에따라 Scale조절 = tan
		z는 0 ~ 1

		(1 / tan(Fov / 2) / 종횡비, 0, 0, 0)
		(0 , 1/tan(Fov / 2) , 0, 0)
		(0, 0, zf / (zf - zn), 1)
		(0 , 0, -zn * zf / (zf - zn), 0)

		*/
		case CT_PERSPECTIVE:
			m_Projection = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle), m_Width / m_Height, m_Near, m_Far);
			break;

		/*
		Ortho 투영 행렬 공식

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