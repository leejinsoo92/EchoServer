/*
 * Server.h
 *
 *  Created on: 2019. 9. 9.
 *      Author: leejinsoo
 */

#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_

#include "../Mng/EpollMng/UserMng/UserMng.h"
#include "../User/User.h"
#include <iostream>
#include <unordered_map>
#include <pthread.h>

#define USER_NUM 1000

using namespace std;

class CServer {
public:
	CServer();
	virtual ~CServer();

public:

	static void* Thread_Worker(void* arg);
	static void* Thread_UserCheck(void* arg);

public:

	void Run();

private:
	CUserMng* 		m_UserConnMng;
	CUser			m_User[USER_NUM];

	//unordered_map<int, CUser*> m_UserHash;

	struct sockaddr_in m_Client_Addr;

private:
	int m_iEpfd = 0;
	int m_iEvent_Cnt = 0;
	int m_iListen_Scok = 0;
	int m_iConnect_Sock = 0;

	int m_iSock_Len = 0;
	int m_iClient_Fd = 0;
	int m_iClient_Socks[USER_NUM] = { 0, };
	socklen_t m_iAddrsize = 0;
	struct sockaddr_in clientaddr;

	int m_iString_Len = 0;
	int m_iData_Len = 0;

	int m_iEvent_Num = 0;

private:
	bool isprint = false;
	bool isDelFind = false;
	bool isSaveFind = false;

	bool isBreak = false;


	int m_iFd = 0;

	int m_iThrStat = 0;
private:
	pthread_t Thread_Work = 0;
	pthread_t Thread_User = 0;
	pthread_attr_t attr;

	int m_iThrStatus = 0;
};

#endif /* SERVER_SERVER_H_ */
