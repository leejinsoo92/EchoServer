/*
 * UserMng.h
 *
 *  Created on: 2019. 8. 12.
 *      Author: leejinsoo
 */

#ifndef USERMNG_USERMNG_H_
#define USERMNG_USERMNG_H_

#include "../../EpollMng/EpollManager.h"
#include <iostream>
#include <list>

using namespace std;

class CUserMng : public CEpoll_Manager{
public:
	CUserMng();
	virtual ~CUserMng();

public:
	bool Client_Connect(int* _iEpfd, int* _iListen_Scok);

private:
	struct sockaddr_in m_Server_Addr;

public:
	//struct epoll_event m_Events[MAX_EVENTS];
	struct epoll_event* m_Events;
};

#endif /* USERMNG_USERMNG_H_ */
