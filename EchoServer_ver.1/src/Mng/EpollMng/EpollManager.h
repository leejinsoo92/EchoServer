/*
 * EpollManager.h
 *
 *  Created on: 2019. 8. 12.
 *      Author: leejinsoo
 */

#ifndef EPOLLMNG_EPOLLMANAGER_H_
#define EPOLLMNG_EPOLLMANAGER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_EVENTS 		32
#define DEFAULT_PORT		9190
#define MAX_CONNECT		16

class CEpoll_Manager
{
public:
	CEpoll_Manager();
	virtual ~CEpoll_Manager();

public:
	void Init_Epoll(int* iEpfd);
	void Epoll_Ctl(int* iEpfd, int iOption, int* iFd, uint32_t iEvents);
	void Set_Sockaddr(struct sockaddr_in *addr);
	int Set_NonBlocking(int* iSockfd);
};

#endif /* EPOLLMNG_EPOLLMANAGER_H_ */
