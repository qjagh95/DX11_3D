#pragma once

JEONG_BEGIN

class JEONG_DLL Timer
{
public:
	Timer();
	~Timer();
	
	bool Init();
	void Update();

	float GetDeltaTime() const { return DeltaTime * TimeScale; }
	float GetFps() const { return ReturnFrame; }
	void SetTimeScale(float Time) { TimeScale = Time; }

private:
	LARGE_INTEGER CurTime;
	LARGE_INTEGER PrevTime;
	LARGE_INTEGER Count;	
	float DeltaTime;
	float TimeVar;
	float Fps;
	float ReturnFrame;
	float TimeScale;
	string TimerName;

public:
	friend class TimeManager;
};

JEONG_END

