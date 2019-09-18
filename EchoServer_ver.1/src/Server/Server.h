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
//#include "../Defines/Packet_Define.h"
#include <list>
#include <pthread.h>

#define USER_NUM 100

class CServer {
public:
	CServer();
	virtual ~CServer();

public:
	//static void* Thread_Echo_Send(void* arg);
//	static void* Thread_Send(void* arg);
//	static void* Thread_Recv(void* arg);
	static void* Thread_Routine(void* arg);

public:
	void Set_List(PACKET _SetPacket);
	list<PACKET>* Get_List();
	void Update();

private:
	CUserMng 		m_UserConnMng;
	CUser			m_User[USER_NUM];
	//CUser*			m_User;
	struct sockaddr_in m_Client_Addr;

private:
	int m_iEpfd = 0;
	int m_iEvent_Cnt = 0;
	int m_iListen_Scok = 0;
	int m_iConnect_Sock = 0;
	int m_iSock_Len = 0;
	int m_iString_Len = 0;
	int m_iData_Len = 0;

	int m_iEvent_Num = 0;

private:
	int m_iClient_Cnt = 0;

private:
	bool isprint = false;
	bool isDelFind = false;
	bool isSaveFind = false;

	bool isThrOn = false;

	int iFd = 0;

private:
	//list<CUser*>	m_UserList;
	list<PACKET>	m_PacketList;
	pthread_t Thread_ID;
	pthread_attr_t attr;

	int m_iThrStatus = 0;
};

#endif /* SERVER_SERVER_H_ */
