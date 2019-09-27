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
	//m_tData = new T;
//	for(int i = 0; i < 50; ++i)
//	{
//		m_tData[i] = new T;
//	}
	//m_tData = new T[50];
	//m_iBufSize = 50;
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
	delete[] m_tData;
	m_tData = nullptr;
//	for(int i = 0; i < 50; ++i)
//	{
//		delete m_tData[i];
//		m_tData[i] = nullptr;
//	}

}

template <typename T>
bool CCircularBuffer<T>::isEmpty()
{
	if(m_iFront == m_iRear)
		return true;
	return false;
}

template <typename T>
bool CCircularBuffer<T>::isFull()
{
	if(((m_iRear + 1) % m_iBufSize) == m_iFront)
		return true;
	return false;
}

template <typename T>
void CCircularBuffer<T>::Push(T* _data)
{
	//cout << "CirBuf Push func()" << endl;
//	cout << _data->m_szData << endl;

	//memset(&m_tData[m_iRear], 0, sizeof(T));
	memcpy(&m_tData[m_iRear], _data, sizeof(T));

	//cout << m_tData[m_iRear].m_szData << endl;

	if(true == isFull())
	{
		m_iFront = (m_iFront + 1) % m_iBufSize;
	}

	m_iRear = (m_iRear + 1) % m_iBufSize;

//	if(isFull())
//	{
//		perror("Circular Buffer is Full!!");
//	}
//	else
//	{
//		m_iRear = (m_iRear) % m_iBufSize;
//		memcpy(m_tData[m_iRear], _data, sizeof(T));
//		//m_tData[m_iRear] = _data;
//		++m_iRear;
//	}
}

template <typename T>
void CCircularBuffer<T>::ChangeFront()
{
	m_iFront = (m_iFront + 1) % m_iBufSize;
	//memset(&m_tData[m_iFront], 0, sizeof(T));
}

template <typename T>
T* CCircularBuffer<T>::Pop()
{
//	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
//	cout << "circular buf pop data : " << m_tData[iCur].m_szData << endl << endl;
//	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//	cout << "---- Circular Buffer Test Print ----" << endl;
//	cout << "Current : " << iCur << endl;
//	cout << "Front   : " << m_iFront << endl;

	return &m_tData[m_iFront];
}

//template <typename T>
//T CCircularBuffer<T>::Full_Pop()
//{
//	int iCur = m_iFront;
//
//	if(isEmpty())
//		return false;
//	else
//	{
//		m_iFront = (m_iFront + 1) % m_iBufSize;
//		return m_tData[iCur];
//	}
//}

template <typename T>
void CCircularBuffer<T>::Print()
{
	//cout << "Front : " << m_iFront << ", Rear : " << m_iRear << endl;
	cout << "--- Circular Buffer Data ---" << endl;

	for(int i = m_iFront; i != (m_iRear) % m_iBufSize; i = (i + 1) % m_iBufSize )
	{

		cout << "[ "<< m_tData[i].m_nSock_ID << " : " << m_tData[i].m_szID << " ]--";
		cout << "(" << i << ")번째 " << m_tData[i].m_szData << endl;
	}
}











