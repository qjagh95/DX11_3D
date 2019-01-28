#pragma once

JEONG_BEGIN

class JEONG_DLL Thread
{
public:
	bool Init();
	void Wait();
	void Awake();
	void Join();
	virtual void Run() = 0;

	static unsigned int __stdcall ThreadFunc(void* Arg);

private:
	HANDLE m_Thread;
	HANDLE m_Start;
	thread m_thread;

protected:
	Thread();

public:
	virtual ~Thread() = 0;

public:
	friend class ThreadManager;
};

JEONG_END

