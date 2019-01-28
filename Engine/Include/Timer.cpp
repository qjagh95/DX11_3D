#include "stdafx.h"
#include "Timer.h"
#include "Core.h"

JEONG_USING

Timer::Timer()
	:DeltaTime(0.0f), Fps(0.0f), TimeScale(1.0f), TimeVar(0.0f)
	,CurTime(), PrevTime(), Count()
{
}

Timer::~Timer()
{
}

bool Timer::Init()
{
	//너는 1초에 얼마나 셀수있니?
	QueryPerformanceFrequency(&Count);

	//얼마나 누적시켰어?
	QueryPerformanceCounter(&CurTime);

	PrevTime = CurTime;

	return true;
}

void Timer::Update()
{
	QueryPerformanceCounter(&CurTime);

	//현재누적시간 - 이전누적시간을 초당카운트로 나눠주면 프레임간의 간격이 나온다
	DeltaTime = (CurTime.QuadPart - PrevTime.QuadPart) / (float)Count.QuadPart;
	PrevTime = CurTime;

	Fps++;
	TimeVar += DeltaTime;
	
	if (TimeVar >= 1.0f)
	{
		ReturnFrame = Fps;
		Fps = 0.0f;
		TimeVar = 0.0f;
	}

#ifdef _DEBUG
	static char Buffer[255] = {};
	sprintf_s(Buffer, "Frame : %f", ReturnFrame);

	SetWindowTextA(Core::Get()->GetHwnd(), Buffer);
#endif
}
