/*
 * EpollManager.cpp
 *
 *  Created on: 2019. 8. 12.
 *      Author: leejinsoo
 */

#include "EpollManager.h"
#include <iostream>
using namespace std;

CEpoll_Manager::CEpoll_Manager() {
	// TODO Auto-generated constructor stub

}

CEpoll_Manager::~CEpoll_Manager() {
	// TODO Auto-generated destructor stub
}

void CEpoll_Manager::Init_Epoll(int* iEpfd)
{
	*iEpfd = epoll_create1(0);
	if( *iEpfd == -1)
	{
		cout << "epoll_create() error" << endl;
		exit(1);
	}
}

void CEpoll_Manager::Epoll_Ctl(int* iEpfd, int iOption, int* iFd, uint32_t iEvents)
{
	struct epoll_event Ev;
	Ev.data.fd = *iFd;
	Ev.events = iEvents;

	if(epoll_ctl(*iEpfd, iOption, *iFd, &Ev) == -1)
	{
		cout << "epoll_ctl() error" << endl;
		exit(1);
	}
}


void CEpoll_Manager::Set_Sockaddr(struct sockaddr_in *addr)
{
	memset(addr, 0, sizeof(sockaddr_in));
	addr->sin_family 			= AF_INET;
	addr->sin_addr.s_addr 	= htonl(INADDR_ANY);
	addr->sin_port 			= ntohs(DEFAULT_PORT);
}

int CEpoll_Manager::Set_NonBlocking(int* iSockfd)
{
	if(fcntl(*iSockfd, F_SETFL, fcntl(*iSockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
		return -1;
	return 0;
}
