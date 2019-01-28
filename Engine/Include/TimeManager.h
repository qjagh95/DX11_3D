#pragma once

JEONG_BEGIN

class Timer;
class JEONG_DLL TimeManager
{
public:
	bool Init();
	bool CreateTimer(const string& TagName);
	Timer* FindTimer(const string& TagName);

private:
	//멀티쓰레드환경에서는 타이머가 따로돌아야한다.
	unordered_map<string, Timer*> m_TimerMap;

public:
	CLASS_IN_SINGLE(TimeManager)
};

JEONG_END