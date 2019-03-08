#pragma once
#include "stdafx.h"

template<typename T>
class Heap
{
	Heap()
	{
		m_Size = 0;
		m_Capacity = 100;
		m_Heap = new T[m_Capacity];

		SetSortFunc(Heap<T>::Sort);
	}

	~Heap()
	{
		SAFE_DELETE_ARRAY(m_Heap);
	}

private:
	T*	m_Heap;
	int	m_Size;
	int	m_Capacity;
	function<bool(const T&, const T&)>	m_CmpFunc;

public:
	void SetSortFunc(void(*pFunc)(const T&, const T&))
	{
		m_CmpFunc = bind(pFunc, placeholders::_1, placeholders::_2);
	}

	template <typename ClassType>
	void SetSortFunc(ClassType* pObj, void(ClassType::*pFunc)(const T&, const T&))
	{
		m_CmpFunc = bind(pFunc, pObj, placeholders::_1, placeholders::_2);
	}

public:
	void Resize()
	{
		if (m_Capacity == m_Size)
		{
			m_Capacity *= 2;
			T*	pList = new T[m_Capacity];
			memset(pList, 0, sizeof(T) * m_Capacity);

			memcpy(pList, m_pHeap, sizeof(T) * m_Size);

			SAFE_DELETE_ARRAY(m_Heap);

			m_Heap = pList;
		}
	}

	void Resize(int iCapacity)
	{
		m_Capacity = iCapacity;

		T*	pList = new T[m_Capacity];
		memset(pList, 0, sizeof(T) * m_Capacity);

		memcpy(pList, m_Heap, sizeof(T) * m_Size);

		SAFE_DELETE_ARRAY(m_Heap);

		m_Heap = pList;
	}

	void Insert(const T& data)
	{
		// 배열의 가장 마지막에 데이터를 넣어준 후에 부모와 비교하며
		// 교체한다. 재귀함수를 이용해서 처리한다.
	}

private:
	static bool Sort(const T& src, const T& dest)
	{
		return src < dest;
	}
};
