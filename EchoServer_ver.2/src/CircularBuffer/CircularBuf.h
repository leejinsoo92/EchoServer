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

class CCircularBuf {
public:
	CCircularBuf();
	CCircularBuf(int _iBufSize);
	virtual ~CCircularBuf();

public:
	bool isEmpty();
	bool isFull();
	int GetDataSize();
	int GetAvailableSpace(); // 비어있는 공간
	char* GetPacket(int _size);
	bool Push(char* _data, int _size);
	void Pop(int _size);
	void Print();

private:
	int m_iBufSize 	= 0;
	int m_iFront 		= 0;
	int m_iRear 		= 0;

	char* m_CirQueue = nullptr;
	char m_szBufPacket[MAX_PACKET_SIZE];

};

#endif /* CIRCULARBUFFER_CIRCULARBUF_H_ */
