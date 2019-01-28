#include "stdafx.h"
#include "Collider_Com.h"
#include "Transform_Com.h"

#include "../Device.h"

#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"

#include "../Render/Shader.h"
#include "../Render/ShaderManager.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderState.h"

JEONG_USING

JEONG::Collider_Com::Collider_Com()
{
	m_ComType = CT_COLLIDER;
	m_CollType = CT_RECT;
	m_CollisionGroupName = "Default";
	m_UpdateCollision = true;

#ifdef _DEBUG
	m_Shader = ShaderManager::Get()->FindShader(COLLIDER_SHADER);
	m_LayOut = ShaderManager::Get()->FindInputLayOut(POS_LAYOUT);
	m_Mesh = NULLPTR;
	m_DepthDisable = NULLPTR;
	m_Color = Vector4::Green;
#endif // _DEBUG
}

JEONG::Collider_Com::Collider_Com(const Collider_Com & CopyCollider)
	:Component_Base(CopyCollider)
{
	*this = CopyCollider;

#ifdef _DEBUG
	m_LayOut = CopyCollider.m_LayOut;
	m_Mesh = CopyCollider.m_Mesh;
	m_Shader = CopyCollider.m_Shader;
	m_DepthDisable = CopyCollider.m_DepthDisable;
	m_Color = Vector4::Green;

	if (m_Shader)
		m_Shader->AddRefCount();

	if (m_Mesh)
		m_Mesh->AddRefCount();

	if (m_DepthDisable)
		m_DepthDisable->AddRefCount();
#endif // _DEBUG

}

JEONG::Collider_Com::~Collider_Com()
{
#ifdef _DEBUG
	SAFE_RELEASE(m_DepthDisable);
	SAFE_RELEASE(m_Shader);
	SAFE_RELEASE(m_Mesh);
#endif

	list<Collider_Com*>::iterator StartIter = m_PrevCollision.begin();
	list<Collider_Com*>::iterator EndIter = m_PrevCollision.end();

	for (; StartIter != EndIter; StartIter++)
	{
		OnCollsionEnd(*StartIter, 0.0f);
		(*StartIter)->OnCollsionEnd(this, 0.0f);
		(*StartIter)->ErasePrevCollision(this);
	}
}

bool JEONG::Collider_Com::Init()
{
	return true;
}

int JEONG::Collider_Com::Input(float DeltaTime)
{  
	return 0;
}

int JEONG::Collider_Com::Update(float DeltaTime)
{
	return 0;
}

int JEONG::Collider_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void JEONG::Collider_Com::Collision(float DeltaTime)
{
}

void JEONG::Collider_Com::CollisionLateUpdate(float DeltaTime)
{
}

void JEONG::Collider_Com::Render(float DeltaTime)
{
#ifdef _DEBUG
	if (m_PrevCollision.empty() == true)
		m_Color = Vector4::Green;
	else
		m_Color = Vector4::Red;

	m_Shader->SetShader();
	Device::Get()->GetContext()->IASetInputLayout(m_LayOut);

	ShaderManager::Get()->UpdateCBuffer("Collider", &m_Color);

	if (m_DepthDisable != NULLPTR)
		m_DepthDisable->SetState();

	m_Mesh->Render();

	if (m_DepthDisable != NULLPTR)
		m_DepthDisable->ResetState();
#endif
}

bool JEONG::Collider_Com::CheckPrevCollision(Collider_Com * Dest)
{
	list<Collider_Com*>::iterator StartIter = m_PrevCollision.begin();
	list<Collider_Com*>::iterator EndIter = m_PrevCollision.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter) == Dest)
			return true;
	}

	return false;
}

void JEONG::Collider_Com::ErasePrevCollision(Collider_Com * Dest)
{
	list<JEONG::Collider_Com*>::iterator StartIter = m_PrevCollision.begin();
	list<JEONG::Collider_Com*>::iterator EndIter = m_PrevCollision.end();

	for (; StartIter != EndIter; StartIter++)
	{
		if ((*StartIter) == Dest)
		{
			m_PrevCollision.erase(StartIter);
			return;
		}
	}
}

void JEONG::Collider_Com::CheckPrevCollisionInSection(float DeltaTime)
{
	list<JEONG::Collider_Com*>::iterator StartIter = m_PrevCollision.begin();
	list<JEONG::Collider_Com*>::iterator EndIter = m_PrevCollision.end();

	for (; StartIter != EndIter; )
	{
		//속해있는 그룹이 같지않으면 End함수호출 후 이전컬라이더를 지운다.
		//그룹이 같다면 해당사항 없다.
		if (m_CollisionGroupName != (*StartIter)->m_CollisionGroupName)
		{
			OnCollsionEnd((*StartIter), DeltaTime);
			(*StartIter)->OnCollsionEnd(this, DeltaTime);

			(*StartIter)->ErasePrevCollision(this);
			StartIter = m_PrevCollision.erase(StartIter);

			EndIter = m_PrevCollision.end();
			continue;
		}

		//나와 충돌한 놈의 충돌체 공간인덱스
		const list<int>* getSectionIndex = (*StartIter)->GetSectionIndexList();
		list<int>::const_iterator StartIter1 = getSectionIndex->begin();
		list<int>::const_iterator EndIter1 = getSectionIndex->end();

		//나의 충돌체 공간인덱스
		list<int>::iterator	StartIter2 = m_SelectionIndexList.begin();
		list<int>::iterator	EndIter2 = m_SelectionIndexList.end();

		bool bPair = false;
		for (; StartIter1 != EndIter1; ++StartIter1)
		{
			//속한 공간의 인덱스가 같다면 넘어감.
			for (; StartIter2 != EndIter2; ++StartIter2)
			{
				if (*StartIter1 == *StartIter2)
				{
					bPair = true;
					break;
				}
			}

			if (bPair == true)
				break;
		}

		//충돌하고 공간이 같지않는데 이전컬라이더가 있다면 End함수호출.
		if (bPair == false)
		{
			OnCollsionEnd((*StartIter), DeltaTime);
			(*StartIter)->OnCollsionEnd(this, DeltaTime);

			(*StartIter)->ErasePrevCollision(this);
			StartIter = m_PrevCollision.erase(StartIter);

			EndIter = m_PrevCollision.end();
			continue;
		}

		++StartIter;
	}
}

void JEONG::Collider_Com::OnCollsionFirst(Collider_Com * Dest, float DeltaTime)
{
	list<function<void(Collider_Com*, Collider_Com*, float)>>::iterator	StartIter = m_CollisionFunc[CCT_FIRST].begin();
	list<function<void(Collider_Com*, Collider_Com*, float)>>::iterator	EndIter = m_CollisionFunc[CCT_FIRST].end();

	//함수포인터 실행
	for (; StartIter != EndIter; ++StartIter)
		(*StartIter)(this, Dest, DeltaTime);
}

void JEONG::Collider_Com::OnCollsionDoing(Collider_Com * Dest, float DeltaTime)
{
	list<function<void(Collider_Com*, Collider_Com*, float)>>::iterator	StartIter = m_CollisionFunc[CCT_DOING].begin();
	list<function<void(Collider_Com*, Collider_Com*, float)>>::iterator	EndIter = m_CollisionFunc[CCT_DOING].end();

	//함수포인터 실행
	for (; StartIter != EndIter; ++StartIter)
		(*StartIter)(this, Dest, DeltaTime);
}

void JEONG::Collider_Com::OnCollsionEnd(Collider_Com * Dest, float DeltaTime)
{
	list<function<void(Collider_Com*, Collider_Com*, float)>>::iterator	StartIter = m_CollisionFunc[CCT_END].begin();
	list<function<void(Collider_Com*, Collider_Com*, float)>>::iterator	EndIter = m_CollisionFunc[CCT_END].end();

	//함수포인터 실행
	for (; StartIter != EndIter; ++StartIter)
		(*StartIter)(this, Dest, DeltaTime);
}

void JEONG::Collider_Com::SetCallback(function<void(Collider_Com*, Collider_Com*, float)> const & _function, COLLSION_CALLBACK_TYPE _type)
{
	m_CollisionFunc[_type].push_back(_function);
}

void JEONG::Collider_Com::SetCollsionCallback(COLLSION_CALLBACK_TYPE eType, void(*pFunc)(Collider_Com *, Collider_Com *, float))
{
	function<void(Collider_Com *, Collider_Com *, float)> newFunc;

	newFunc = bind(pFunc, placeholders::_1, placeholders::_2, placeholders::_3);
	m_CollisionFunc[eType].push_back(newFunc);
}

bool JEONG::Collider_Com::CollsionRectToRect(const BoxInfo & Src, const BoxInfo & Dest)
{
	if (Src.Min.x > Dest.Max.x)
		return false;

	else if (Src.Max.x < Dest.Min.x)
		return false;

	else if (Src.Min.y > Dest.Max.y)
		return false;

	else if (Src.Max.y < Dest.Min.y)
		return false;

	return true;
}

bool JEONG::Collider_Com::CollsionRectToPoint(const BoxInfo & Src, const Vector3 & Dest)
{
	if (Src.Min.x > Dest.x)
		return false;

	else if (Src.Max.x < Dest.x)
		return false;

	if (Src.Min.y > Dest.y)
		return false;

	else if (Src.Max.y < Dest.y)
		return false;

	return true;
}

bool JEONG::Collider_Com::CollsionRectToCircle(const BoxInfo & Src, const CircleInfo & Dest)
{
	BoxInfo CircleRect;
	Vector3 CenterPos = Dest.CenterPos;

	if ((Src.Min.x <= CenterPos.x && Src.Max.x >= CenterPos.x) && (Src.Max.y >= CenterPos.y && Src.Min.y <= CenterPos.y))
	{
		CircleRect.Min.x = Src.Min.x - Dest.Radius;
		CircleRect.Min.y = Src.Min.y - Dest.Radius;

		CircleRect.Max.x = Src.Max.x + Dest.Radius;
		CircleRect.Max.y = Src.Max.y + Dest.Radius;

		if ((CircleRect.Min.x <= CenterPos.x && CircleRect.Max.x >= CenterPos.x) && (CircleRect.Max.y >= CenterPos.y && CircleRect.Min.y <= CenterPos.y))
			return true;
		else
			return false;
	}
	else
	{
		Vector3 LeftBottom = Src.Min;
		Vector3 RightTop = Src.Max;
		Vector3 LeftTop = Vector3(Src.Min.x, Src.Max.y, 0.0f);
		Vector3 RightBottom = Vector3(Src.Max.x, Src.Min.y, 0.0f);

		bool l = LeftBottom.GetDistance(CenterPos) <= Dest.Radius;
		bool r = RightTop.GetDistance(CenterPos) <= Dest.Radius;
		bool b = LeftTop.GetDistance(CenterPos) <= Dest.Radius;
		bool t = RightBottom.GetDistance(CenterPos) <= Dest.Radius;

		if (l == true || r == true || b == true || t == true)
			return true;
	}

	return false;
}

bool JEONG::Collider_Com::CollsionCircleToCircle(CircleInfo & Src, const CircleInfo & Dest)
{
	return Src.CenterPos.GetDistance(Dest.CenterPos) <= Src.Radius + Dest.Radius;
}

bool JEONG::Collider_Com::CollsionCircleToPoint(CircleInfo & Src, Vector3 & Dest)
{
	return Src.CenterPos.GetDistance(Dest) <= Src.Radius;
}

bool JEONG::Collider_Com::CollsionOBB2DToRect(const OBB2DInfo & Src, const BoxInfo & Dest)
{
	OBB2DInfo BoxToObb;
	BoxToObb.CenterPos = (Dest.Min + Dest.Max) * 0.5f;
	BoxToObb.Lenth[0] = Dest.Lenth.x * 0.5f;
	BoxToObb.Lenth[1] = Dest.Lenth.y * 0.5f;
	BoxToObb.Axis[0] = Vector3{ 1.0f , 0.0f, 0.0f };
	BoxToObb.Axis[1] = Vector3{ 0.0f , 1.0f, 0.0f };

	return CollsionOBB2DToOBB2D(Src, BoxToObb);
}

bool JEONG::Collider_Com::CollsionOBB2DToPoint(const OBB2DInfo & Src, const Vector3 & Dest)
{
	// OBB 상자가 Z축으로 회전한 만큼 반대로 점을 OBB상자의 중점을 기준으로
	// 회전시킨 좌표를 구한다. 이렇게 할 경우 이 점을 OBB상자를 월드축에 정렬된
	// 일반 RECT로 만들었을 경우의 점 위치가 나오게 되기 때문에
	// Rect를 이용해서 처리가 가능하다.

	// 회전 각도를 구한다. 회전은 2D 이기 때문에 Z축 회전을 한다.
	float Angle = Src.Axis[0].GetAngle(Vector3(1.0f, 0.0f, 0.0f));

	if (Src.Axis[0].y < 0.0f)
		Angle = 360.0f - Angle;

	Matrix matRot;
	matRot.RotationZ(Angle);
	memcpy(&matRot._41, &Src.CenterPos, sizeof(Vector3));

	Vector3 tempPos = Dest - Src.CenterPos;
	tempPos = tempPos.TransformCoord(matRot);

	BoxInfo Info;
	Info.Min = Src.CenterPos - Vector3(Src.Lenth[0], Src.Lenth[1], 0.0f);
	Info.Max = Src.CenterPos + Vector3(Src.Lenth[0], Src.Lenth[1], 0.0f);

	return CollsionRectToPoint(Info, tempPos);
}

bool JEONG::Collider_Com::CollsionOBB2DToCircle(const OBB2DInfo & Src, const CircleInfo & Dest)
{
	//문제있음
	
	//1. 회전된 각도를 구한다.
	//2. 사각형변수를 선언한다.
	//3. 회전된 사각형을 역 으로 회전시킨다.
	//4. CollsionToRectToCircle();

	Vector3 Distance = Dest.CenterPos - Src.CenterPos;
	Vector3 DistanceNormal = Vector3::Nomallize(Distance);

	float A = abs(Src.Axis[0].Dot(DistanceNormal));
	float B = abs(Src.Axis[1].Dot(DistanceNormal));

	float Lenth = abs(Distance.Dot(Src.Axis[0]));
	float Temp1 = Dest.Radius;
	float Temp2 = A * Src.Lenth[0];

	if (Lenth >= Temp1 + Temp2)
		return false;

	Lenth = abs(Distance.Dot(Src.Axis[1]));
	Temp1 = Dest.Radius;
	Temp2 = B * Src.Lenth[1];

	if (Lenth >= Temp1 + Temp2)
		return false;

	return true;
}

bool JEONG::Collider_Com::CollsionOBB2DToOBB2D(const OBB2DInfo & Src, const OBB2DInfo & Dest)
{
	//두 점 사이의 거리를 구한다.
	Vector3 Distance = Dest.CenterPos - Src.CenterPos;

	//미리 내적처리. (방향을 구한다)
	float SrcXToDestXDot = abs(Src.Axis[0].Dot(Dest.Axis[0]));
	float SrcXToDestYDot = abs(Src.Axis[0].Dot(Dest.Axis[1]));
	float SrcYToDestXDot = abs(Src.Axis[1].Dot(Dest.Axis[0]));
	float SrcYToDestYDot = abs(Src.Axis[1].Dot(Dest.Axis[1]));

	//Src의 X축에 대한 내적처리
	//Src X축에 대한 길이를 구한다.
	//Src.Axis X축을 Distance벡터에 투영하겠다.(X축에 대하여 Distance의 길이가나옴)
	float Lenth = abs(Distance.Dot(Src.Axis[0]));
	float Temp1 = Src.Lenth[0];
	float Temp2 = (SrcXToDestXDot * Dest.Lenth[0]) + (SrcXToDestYDot * Dest.Lenth[1]);

	if (Lenth >= Temp1 + Temp2)
		return false;

	//Src의 Y축에 대한 내적처리
	Lenth = abs(Distance.Dot(Src.Axis[1]));
	Temp1 = Src.Lenth[1];
	Temp2 = (SrcYToDestXDot * Dest.Lenth[0]) + (SrcYToDestYDot * Dest.Lenth[1]);

	if (Lenth >= Temp1 + Temp2)
		return false;

	//Dest의 X축에 대한 내적처리 
	Lenth = abs(Distance.Dot(Dest.Axis[0]));
	Temp1 = Dest.Lenth[0];
	Temp2 = (SrcXToDestXDot * Src.Lenth[0]) + (SrcXToDestYDot * Src.Lenth[1]);

	if (Lenth >= Temp1 + Temp2)
		return false;

	//Dest의 Y축에 대한 내적처리 
	Lenth = abs(Distance.Dot(Dest.Axis[1]));
	Temp1 = Dest.Lenth[1];
	Temp2 = (SrcYToDestXDot * Src.Lenth[0]) + (SrcYToDestYDot * Src.Lenth[1]);

	if (Lenth >= Temp1 + Temp2)
		return false;

	return true;
}

bool JEONG::Collider_Com::CollsionRectToPixel(BoxInfo Src, const PixelInfo& Dest)
{
	if (CollsionRectToRect(Dest.ImageRect, Src) == false)
		return false;

	//Box를 픽셀공간으로 변환한다.
	Src.Min -= Dest.ImageRect.Min;
	Src.Max -= Dest.ImageRect.Min;

	Src.Min.x = Src.Min.x <= 0.0f ? 0.0f : Src.Min.x;
	Src.Min.y = Src.Min.y <= 0.0f ? 0.0f : Src.Min.y;

	Src.Max.x = Src.Max.x >= Dest.Width ? Dest.Width : Src.Max.x;
	Src.Max.y = Src.Max.y >= Dest.Height ? Dest.Height : Src.Max.y;

	for (int y = (int)Src.Min.y; y < (int)Src.Max.y; y++)
	{
		for (int x = (int)Src.Min.x; x < (int)Src.Max.x; x++)
		{
			int Index = y * Dest.Width + x;
			Pixel24& TempPixel = Dest.Color[Index];

			if(TempPixel.r != 0 || TempPixel.g != 0 || TempPixel.b != 0)
				return true;
		}
	}
	return false;
}
