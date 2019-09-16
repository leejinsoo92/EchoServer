/*
 * CircularBuffer.h
 *
 *  Created on: 2019. 8. 27.
 *      Author: leejinsoo
 */

#ifndef MNG_CIRCULARBUFFER_CIRCULARBUFFER_H_
#define MNG_CIRCULARBUFFER_CIRCULARBUFFER_H_

#include <iostream>
//#include "../Defines/Packet_Define.h"
using namespace std;

template <typename T>
class CCircularBuffer {
public:
	CCircularBuffer();
	CCircularBuffer(int _bufsize) ;
	virtual ~CCircularBuffer();

public:
	bool isEmpty();
	bool isFull();
	void Push(T* _data);
	bool Pop();
	void Print();

private:
	int m_iBufSize 	= 0;
	T* m_tData 		= nullptr;
	int m_iFront 		= 0;
	int m_iRear 		= 0;
	int m_iSize 	= 0;
};
#include "CircularBuffer.hpp"
#endif /* MNG_CIRCULARBUFFER_CIRCULARBUFFER_H_ */
