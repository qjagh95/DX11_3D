#pragma once
JEONG_BEGIN

class Thread;
class JEONG_DLL ThreadManager
{
public:
	bool Init();

	Thread* FindThread(const string& ThreadName);
	CRITICAL_SECTION* FindCritcal(const string& CritcalName);

	bool DeleteThread(const string& ThreadName);
	bool CritcalCreate(const string& CritcalName);

	template<typename T>
	T* ThreadCreate(const string& ThreadName)
	{
		T* newThread = (T*)FindThread(ThreadName);

		if (newThread != NULLPTR)
			return newThread;

		newThread = new T();

		if (newThread->Init() == false)
		{
			SAFE_DELETE(newThread);
			return NULLPTR;
		}

		m_ThreadMap.insert(make_pair(ThreadName, newThread));
		return newThread;
	}

private:
	unordered_map<string, Thread*> m_ThreadMap;
	unordered_map<string, CRITICAL_SECTION> m_CriMap;

public:
	CLASS_IN_SINGLE(ThreadManager)
};

JEONG_END