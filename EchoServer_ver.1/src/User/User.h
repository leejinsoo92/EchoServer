/*
 * User.h
 *
 *  Created on: 2019. 8. 22.
 *      Author: leejinsoo
 */

#ifndef MNG_USERMNG_USER_USER_H_
#define MNG_USERMNG_USER_USER_H_

#include "../CircularBuffer/CircularBuffer.h"
#include "../Defines/Packet_Define.h"
#include <list>

class CUser {
public:
	CUser();
	virtual ~CUser();

public:
	void Set_Connect(bool _isConn){
		m_isConn = _isConn;
	}
	bool Get_Connect(){
		return m_isConn;
	}

	void Set_ConnectSock(int _Sock){
		m_iConnectSock = _Sock;
		//cout << "<< connect sock id : >>	" << m_iConnectSock;
	}
	int Get_ConnectSock(){
		return m_iConnectSock;
	}

	PACKET Get_Packet(){
		return m_Packet;
	}

	bool Get_isRecv(){
		return m_isRecv;
	}

public:
	void Send(int* _fd);
	void Send(int* _fd, list<PACKET>* _List);
	int Recv(int* _fd);
	bool PacketCheck(PACKET data);

private:
	int m_iConnectSock = 0;

	PACKET m_Packet;
	//PACKET m_PopPack;

	bool m_isConn = false;
	bool isprint = false;
	bool isDelFind = false;
	bool isSaveFind = false;
	bool m_isRecv = false;
private:
	CCircularBuffer<PACKET>* CirBuf;
	list<PACKET>	m_List;
};

#endif /* MNG_USERMNG_USER_USER_H_ */
