/*
 * User.h
 *
 *  Created on: 2019. 8. 22.
 *      Author: leejinsoo
 */

#ifndef MNG_USERMNG_USER_USER_H_
#define MNG_USERMNG_USER_USER_H_

#include "../CircularBuffer/CircularBuf.h"
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

//	PACKET Get_Packet(){
//		return m_Packet;
//	}
	char* Get_ID(){
		return m_szID;
	}


	bool Get_isRecv(){
		return m_isRecv;
	}

public:
	void ProcMsg(int _fd);
	int Recv(int _fd);


private:
	int m_iConnectSock = 0;
	bool m_isConn = false;
	bool isprint = false;
	bool isDelFind = false;
	bool isSaveFind = false;
	bool m_isRecv = false;
	int m_iCirbufSize = 0;
	char				m_szID[16];
private:
	CCircularBuf* m_pCirBuf;
	char szRecvBuf[MAX_PACKET_SIZE];

};

#endif /* MNG_USERMNG_USER_USER_H_ */
