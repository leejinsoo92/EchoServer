/*
 * UserMng.h
 *
 *  Created on: 2019. 8. 12.
 *      Author: leejinsoo
 */

#ifndef USERMNG_USERMNG_H_
#define USERMNG_USERMNG_H_

#include "../EpollMng/EpollManager.h"
#include "../../Defines/Packet_Define.h"
#include "../../CircularBuffer/CircularBuffer.h"
//#include "../UserMng/User/User.h"
#include <pthread.h>
#include <iostream>
#include <list>
#include <mutex>

using namespace std;

class CUserMng {
public:
	CUserMng();
	virtual ~CUserMng();

public:
	void Ready_Sock();
	void Update();

public:
	static void* Thread_Routine(void* arg);

private:
	CEpoll_Manager* Epoll_Mng;
	//CUser* m_CUser;

private:
	int m_iEpfd = 0;
	int m_iNfds = 0;
	int m_iListen_Scok = 0;
	int m_iConnect_Sock = 0;
	int m_iSock_Len = 0;
	int m_iString_Len = 0;
	int m_iData_Len = 0;

	int curFd = 0;

	char m_szRecvBuf[256];
	char m_szSendBuf[256];

	PACKET m_Packet;
	struct sockaddr_in m_Server_Addr;
	struct sockaddr_in m_Client_Addr;
	struct epoll_event m_Events[MAX_EVENTS];

	CCircularBuffer<PACKET> CirBuf;

	pthread_t Thread_ID;
	//pthread_t Thread_ID[4];

	int m_iState = 0;
	int m_iDelState = 0;
	//pthread_mutex_t mutex;


private:
	list<PACKET> m_Datalist;
};

#endif /* USERMNG_USERMNG_H_ */
