/*
 * UserMng.cpp
 *
 *  Created on: 2019. 8. 12.
 *      Author: leejinsoo
 */

#include "UserMng.h"
#include <iostream>

using namespace std;
pthread_mutex_t mutex_lock;
pthread_mutex_t mutex_dellock;

CUserMng::CUserMng() {
	// TODO Auto-generated constructor stub

}

CUserMng::~CUserMng() {
	// TODO Auto-generated destructor stub
}



bool CUserMng::Client_Connect(int* _iEpfd, int* _iListen_Scok)
{
	*_iListen_Scok = socket(AF_INET, SOCK_STREAM, 0);
	CEpoll_Manager::Set_Sockaddr(&m_Server_Addr);
	bind(*_iListen_Scok, (struct sockaddr*)&m_Server_Addr, sizeof(m_Server_Addr));

	CEpoll_Manager::Set_NonBlocking(_iListen_Scok);

	if( listen(*_iListen_Scok, MAX_CONNECT) == -1 )
		return false;


	CEpoll_Manager::Init_Epoll(_iEpfd);
	CEpoll_Manager::Epoll_Ctl(_iEpfd, EPOLL_CTL_ADD, _iListen_Scok, EPOLLIN | EPOLLOUT | EPOLLET);

	return true;
}

