/*
 * User.h
 *
 *  Created on: 2019. 8. 22.
 *      Author: leejinsoo
 */

#ifndef MNG_USERMNG_USER_USER_H_
#define MNG_USERMNG_USER_USER_H_

#include "../CircularBuffer/CircularBuf.h"
//#include "../Defines/Packet_Define.h"
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
	void Send(int* _fd, list<PACKET>* _List);
	int Recv(int* _fd);
	bool PacketCheck(PACKET_HEAD head, PACKET_TAIL tail);
	void ClearBuf();

private:
	int m_iConnectSock = 0;

	PACKET m_Packet;
	//PACKET m_PopPack;

	bool m_isConn = false;
	bool isprint = false;
	bool isDelFind = false;
	bool isSaveFind = false;
	bool m_isRecv = false;
	int m_iCirbufSize = 0;

	char m_iPackTemp[10000];

private:
	//CCircularBuffer<PACKET>* CirBuf;
	//CCircularBuf* CirBuf;
	list<PACKET>	m_List;
};

#endif /* MNG_USERMNG_USER_USER_H_ */
