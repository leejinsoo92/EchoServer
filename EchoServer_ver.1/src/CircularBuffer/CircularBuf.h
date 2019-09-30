/*
 * CircularBuf.h
 *
 *  Created on: 2019. 9. 30.
 *      Author: leejinsoo
 */

#ifndef CIRCULARBUFFER_CIRCULARBUF_H_
#define CIRCULARBUFFER_CIRCULARBUF_H_

#include "../Defines/Packet_Define.h"
#include <iostream>
using namespace std;

typedef struct Cirbuf_Data_Queue
{
	char m_szBufData[MAX_PACKET_SIZE];
}CIR_QUEUE;

class CCircularBuf {
public:
	CCircularBuf();
	CCircularBuf(int _iBufSize);
	virtual ~CCircularBuf();

public:
	bool isEmpty();
	bool isFull();
	void Push(char* _data);
	void ChangeFront();
	char*  Pop();
	void Print();

private:
	int m_iBufSize 	= 0;
	//char m_szBufData[MAX_PACKET_SIZE];
	CIR_QUEUE* m_CirQueue = nullptr;
	int m_iFront 		= 0;
	int m_iRear 		= 0;
};

#endif /* CIRCULARBUFFER_CIRCULARBUF_H_ */
