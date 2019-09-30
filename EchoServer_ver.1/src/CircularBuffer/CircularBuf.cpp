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

using namespace std;

CCircularBuf::CCircularBuf() {
	// TODO Auto-generated constructor stub

}
CCircularBuf::CCircularBuf(int _iBufSize)
	: m_iBufSize(_iBufSize)
{
	// TODO Auto-generated constructor stub
	m_CirQueue = new CIR_QUEUE[m_iBufSize];
}

CCircularBuf::~CCircularBuf() {
	// TODO Auto-generated destructor stub
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

void CCircularBuf::Push(char* _data)
{
	PACKET_HEAD Head;
	PACKET Body;
	PACKET_TAIL Tail;
	memcpy(&m_CirQueue[m_iRear].m_szBufData, _data, sizeof(CIR_QUEUE));
	DisAssemblePacket(m_CirQueue[m_iRear].m_szBufData, Head, Body, Tail);

	if(true == isFull())
	{
		m_iFront = (m_iFront + 1) % m_iBufSize;
	}

	if(strcmp(Head.m_szHead, "AA11") == 0 && strcmp(Tail.m_szTail, "11AA") == 0 && Head.m_iPacketSize > 0)
		m_iRear = (m_iRear + 1) % m_iBufSize;

}

void CCircularBuf::ChangeFront()
{
	m_iFront = (m_iFront + 1) % m_iBufSize;
	//memset(&m_tData[m_iFront], 0, sizeof(T));
}

char* CCircularBuf::Pop()
{
	return m_CirQueue[m_iFront].m_szBufData;
}


void CCircularBuf::Print()
{
	//cout << "Front : " << m_iFront << ", Rear : " << m_iRear << endl;
//	cout << "--- Circular Buffer Data ---" << endl;
//
//	for(int i = m_iFront; i != (m_iRear) % m_iBufSize; i = (i + 1) % m_iBufSize )
//	{
//		cout << "[ "<< m_tData[i].m_nSock_ID << " : " << m_tData[i].m_szID << " ]--";
//		cout << "(" << i << ")번째 " << m_tData[i].m_szData << endl;
//	}
}
