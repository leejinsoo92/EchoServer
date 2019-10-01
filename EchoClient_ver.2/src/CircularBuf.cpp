/*
 * CircularBuf.cpp
 *
 *  Created on: 2019. 9. 30.
 *      Author: leejinsoo
 */

#include "CircularBuf.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

using namespace std;


pthread_mutex_t cir_sync;

CCircularBuf::CCircularBuf() {
	// TODO Auto-generated constructor stub
	m_iBufSize = 1024 * 32; // 32kbytes
	m_CirQueue = new char[m_iBufSize];
	pthread_mutex_init(&cir_sync, NULL);
}
CCircularBuf::CCircularBuf(int _iBufSize)
	: m_iBufSize(_iBufSize)
{
	// TODO Auto-generated constructor stub
	m_CirQueue = new char[m_iBufSize];
	pthread_mutex_init(&cir_sync, NULL);
}

CCircularBuf::~CCircularBuf() {
	// TODO Auto-generated destructor stub

	delete m_CirQueue;
	m_CirQueue = nullptr;

}

bool CCircularBuf::isEmpty()
{
	if(m_iFront == m_iRear)
		return true;
	return false;
}

bool CCircularBuf::isFull()
{
	if(((m_iRear + 1) % m_iBufSize) == m_iFront)
		return true;
	return false;
}

int CCircularBuf::GetDataSize()
{
	if( m_iRear > m_iFront )
		return (m_iRear - m_iFront);

	if( m_iFront > m_iRear )
		return ( m_iBufSize - (m_iFront - m_iRear) );

	return 0;
}

int CCircularBuf::GetAvailableSpace()
{
	if( m_iRear > m_iFront )
		return ( m_iBufSize - (m_iRear - m_iFront) );

	if( m_iFront > m_iRear )
		return ( m_iFront - m_iRear );

	return m_iBufSize;
}

char* CCircularBuf::GetPacket( int _size )
{
	pthread_mutex_lock(&cir_sync);
	if( m_iRear > m_iFront )
	{
		pthread_mutex_unlock(&cir_sync);
		return &m_CirQueue[m_iFront];
	}

	if( m_iBufSize <= m_iFront + _size )
	{
		pthread_mutex_unlock(&cir_sync);
		return &m_CirQueue[m_iFront];
	}

	int iDivide = m_iBufSize - m_iFront;
	memcpy( m_szBufPacket, &m_CirQueue[m_iFront], iDivide);
	memcpy( &m_szBufPacket[iDivide], m_CirQueue, _size - iDivide);

	pthread_mutex_unlock(&cir_sync);
	return m_szBufPacket;
}

bool CCircularBuf::Push(char* _data,  int _size)
{
	pthread_mutex_lock(&cir_sync);
	if(true == isFull())
	{
		pthread_mutex_unlock(&cir_sync);
		return false;
	}

	if( _size > GetAvailableSpace() )
	{
		pthread_mutex_unlock(&cir_sync);
		return false;
	}

	if( m_iBufSize < m_iRear + _size )
	{
		int iDivide = m_iBufSize - m_iRear;
		memcpy(&m_CirQueue[m_iRear], _data, iDivide);

		m_iRear = _size - iDivide;
		memcpy(m_CirQueue, _data + iDivide, m_iRear);
	}
	else
	{
		memcpy(&m_CirQueue[m_iRear], _data, _size);
		m_iRear += _size;
	}
	pthread_mutex_unlock(&cir_sync);
	return true;
}


void CCircularBuf::Pop( int _size )
{
	if( m_iBufSize < m_iFront + _size )
		m_iFront = _size - ( m_iBufSize - m_iFront );
	else
		m_iFront += _size;

	if(m_iFront == m_iRear)
		m_iFront = m_iRear = 0;
}


void CCircularBuf::Print()
{

}
