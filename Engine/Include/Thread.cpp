#include "stdafx.h"
#include "Thread.h"

JEONG_USING

Thread::Thread()
{
}

Thread::~Thread()
{
	if (m_Thread)
	{
		WaitForSingleObject(m_Thread, INFINITE);
		CloseHandle(m_Thread);
	}

	//m_thread.join();
}

bool Thread::Init()
{
	m_Start = CreateEvent(NULLPTR, FALSE, FALSE, NULLPTR);
	m_Thread = (HANDLE)_beginthreadex(nullptr, 0, Thread::ThreadFunc, this, 0, NULLPTR);
	//m_thread = thread(&Thread::ThreadFunc, this);

	return true;
}

void Thread::Wait()
{
	//동기화 함수. 완료될때까지 무한대기.
	WaitForSingleObject(m_Start, INFINITE);
}

void Thread::Awake()
{
	SetEvent(m_Start);
}

void Thread::Join()
{
	m_thread.join();
}

unsigned int Thread::ThreadFunc(void * Arg)
{
	Thread* getThread = (Thread*)Arg;
	getThread->Wait();
	getThread->Run();
	
	return 0;
}
