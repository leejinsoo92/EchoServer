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

#define USER_NUM 100

class CServer {
public:
	CServer();
	virtual ~CServer();

public:
	void Set_List(PACKET _SetPacket);
	list<PACKET>* Get_List();
	void Update();

private:
	CUserMng 		m_UserConnMng;
	CUser			m_User[USER_NUM];

	PACKET m_Packet;

	struct sockaddr_in m_Client_Addr;

private:
	int m_iEpfd = 0;
	int m_iEvent_Cnt = 0;
	int m_iListen_Scok = 0;
	int m_iConnect_Sock = 0;
	int m_iSock_Len = 0;
	int m_iString_Len = 0;
	int m_iData_Len = 0;

private:
	int m_iClient_Cnt = 0;

private:
	bool isprint = false;
	bool isDelFind = false;
	bool isSaveFind = false;

	int iFd = 0;

private:
	//list<CUser*>	m_UserList;
	list<PACKET>	m_PacketList;
};

#endif /* SERVER_SERVER_H_ */
