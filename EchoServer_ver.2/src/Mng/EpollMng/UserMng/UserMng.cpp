/*
 * UserMng.cpp
 *
 *  Created on: 2019. 8. 12.
 *      Author: leejinsoo
 */

#include "UserMng.h"
#include <iostream>

using namespace std;

CUserMng::CUserMng() {
	// TODO Auto-generated constructor stub

}

CUserMng::~CUserMng() {
	// TODO Auto-generated destructor stub
}



bool CUserMng::Client_Connect(int* _iEpfd, int* _iListen_Scok)
{


	if( ( *_iListen_Scok = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("[+] - ERROR : listen sock Error");
		return false;
	}

	CEpoll_Manager::Set_Sockaddr(&m_Server_Addr);

	m_iOptval = 1024000;
	setsockopt(*_iListen_Scok, SOL_SOCKET, SO_SNDBUF, (char*)&m_iOptval, sizeof(m_iOptval));
	setsockopt(*_iListen_Scok, SOL_SOCKET, SO_RCVBUF, (char*)&m_iOptval, sizeof(m_iOptval));

	m_iOptval = 1;
	setsockopt(*_iListen_Scok, SOL_SOCKET, SO_REUSEADDR, (char*)&m_iOptval, sizeof(m_iOptval));

	if( bind(*_iListen_Scok, (struct sockaddr*)&m_Server_Addr, sizeof(m_Server_Addr)) == -1 )
	{
		perror("[+] - ERROR : bind Error");
		return false;
	}
	CEpoll_Manager::Set_NonBlocking(_iListen_Scok);

	if( listen(*_iListen_Scok, MAX_CONNECT) == -1 )
	{
		perror("[+] - ERROR : listen Error");
		return false;
	}

	CEpoll_Manager::Init_Epoll(_iEpfd);
	m_Events = (struct epoll_event *)malloc(sizeof MAX_EVENTS);
	CEpoll_Manager::Epoll_Ctl(_iEpfd, EPOLL_CTL_ADD, _iListen_Scok, EPOLLIN);
	return true;
}

