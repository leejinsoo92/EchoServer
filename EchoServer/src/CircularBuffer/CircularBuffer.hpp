/*
 * CircularBuffer.cpp
 *
 *  Created on: 2019. 8. 27.
 *      Author: leejinsoo
 */

#include "CircularBuffer.h"
#include <iostream>
using namespace std;


template <typename T>
CCircularBuffer<T>::CCircularBuffer()
{
	// TODO Auto-generated constructor stub

}

template <typename T>
CCircularBuffer<T>::CCircularBuffer(int _bufsize)
	: m_iBufSize(_bufsize)
{
	// TODO Auto-generated constructor stub
	m_tData = new T[m_iBufSize];
}

template <typename T>
CCircularBuffer<T>::~CCircularBuffer()
{
	// TODO Auto-generated destructor stub
//	delete m_tData;
//	m_tData = nullptr;
}

template <typename T>
bool CCircularBuffer<T>::isEmpty()
{
	return m_iFront == m_iRear;
}

template <typename T>
bool CCircularBuffer<T>::isFull()
{
	return (m_iRear + 1) % m_iBufSize == m_iFront;
}

template <typename T>
void CCircularBuffer<T>::Push(T* _data)
{
	if(isFull())
	{
		perror("Circular Buffer is Full!!");
	}
	else
	{
//		m_iSize++;
//		m_iRear = (++m_iRear) % m_iBufSize;
//		m_tData[m_iRear] = *_data;
		m_iRear = (m_iRear) % m_iBufSize;
		m_tData[m_iRear] = *_data;
		++m_iRear;
	}
}
template <typename T>
bool CCircularBuffer<T>::Pop()
{
	//int iPreIndex = 0;

	if(isEmpty())
		return false;
	else
	{
//		m_iSize--;
//		iPreIndex = m_iFront;
//		m_iFront = (++m_iFront) % m_iBufSize;
		m_iFront = (m_iFront + 1) % m_iBufSize;
		return true;
	}

	//return m_tData[iPreIndex];
}

template <typename T>
void CCircularBuffer<T>::Print()
{
	cout << "Front : " << m_iFront << ", Rear : " << m_iRear << endl;
	cout << "--- Circular Buffer Data ---" << endl;
	for(int i = m_iFront; i != (m_iRear) % m_iBufSize; i = (i + 1) % m_iBufSize )
	{
		cout << "[" << i << "] " << m_tData[i].m_szData << endl;
	}
}











