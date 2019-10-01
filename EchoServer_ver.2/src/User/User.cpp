/*
 * User.cpp
 *
 *  Created on: 2019. 8. 22.
 *      Author: leejinsoo
 */

#include "User.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include "../Server/Server.h"

CUser::CUser() {
	// TODO Auto-generated constructor stub
	m_iCirbufSize = 1024 * 32;
	m_pCirBuf = new CCircularBuf();
	m_isConn = false;
}

CUser::~CUser() {
	// TODO Auto-generated destructor stub
	delete m_pCirBuf;
	m_pCirBuf = nullptr;
}

void CUser::ProcMsg(int _fd)
{
	while(1)
	{
		if( m_pCirBuf->isEmpty() )
			return;

		int iDataSize = m_pCirBuf->GetDataSize();
		if( (int)sizeof( PACKET_HEAD ) >= iDataSize )
			return;

		PACKET_HEAD* head = (PACKET_HEAD*)m_pCirBuf->GetPacket(sizeof(PACKET_HEAD));
		if( head-> m_iPacketSize > iDataSize )
			return;

		//int iTailSize = head->m_iPacketSize - sizeof(PACKET_TAIL);
		//PACKET_TAIL* tail = (PACKET_TAIL*)m_pCirBuf->GetPacket(iTailSize);
//		PACKET_TAIL* tail = (PACKET_TAIL*)(head + head->m_iPacketSize - sizeof(PACKET_TAIL));
//		PACKET* packet = (PACKET*)head;
//		cout << "head : " << head->m_szHead << endl;
//		cout << "packet size : " << head->m_iPacketSize << endl;
//		cout << "ID : " << packet->m_szID << endl;
//		cout << "data : " << packet->m_szData << endl;
//		cout << "tail : " << tail->m_szTail << endl;


		//strncmp( tail->m_szTail, gpTail, sizeof(gpTail)) != 0
		if( strncmp( head->m_szHead, gpHead, sizeof(gpHead)) != 0
				)
		{
			m_pCirBuf->Pop( iDataSize );
			return;
		}

		switch(head->m_nCMD)
		{
		case CMD_USER_LOGIN_REQ:
		{
			PACKET_CS_LOGIN* packet = (PACKET_CS_LOGIN*)(head);

					cout << "head : " << packet->m_Head.m_szHead << endl;
					cout << "packet size : " << packet->m_Head.m_iPacketSize << endl;
					cout << "ID : " << packet->m_szID << endl;
					cout << "tail : " << packet->m_Tail.m_szTail << endl << endl;


			strncpy(m_szID, packet->m_szID, sizeof(m_szID));
			cout << "user id : " << m_szID << endl;
			PACKET_SC_LOGIN Answer;

			send(_fd, (char*)&Answer, sizeof(PACKET_SC_LOGIN), 0);
			cout << "send complete" << endl;
		}
			break;
		case CMD_USER_DATA_REQ:
		{
			PACKET_CS_ECHO* packet = (PACKET_CS_ECHO*)(head);

			cout << "recv echo data : " << packet->m_szData << endl;

			PACKET_SC_ECHO Answer;

			strncpy(Answer.m_szData, packet->m_szData, sizeof(Answer.m_szData));

			send(_fd, (char*)&Answer, sizeof(PACKET_SC_ECHO), 0);
		}
			break;
		case CMD_USER_SAVE_REQ:
		{

		}
			break;
		case CMD_USER_DELETE_REQ:
		{

		}
			break;
		case CMD_USER_PRINT_REQ:
		{

		}
			break;
		case CMD_USER_ERR:
		{

		}
			break;
		}

		m_pCirBuf->Pop( head->m_iPacketSize );
		m_isRecv = false;
	}
}

int CUser::Recv(int _fd)
{
	int iRecvLen = 0;

	while(1)
	{
		iRecvLen = recv(_fd, szRecvBuf, sizeof(szRecvBuf), 0);

		if(iRecvLen == 0)
			return -1;
		else if(iRecvLen < 0)
		{
			if( errno == EAGAIN)
			{
				m_isRecv = true;
				break;
			}
		}
		else
		{
			m_pCirBuf->Push( szRecvBuf, iRecvLen );
		}
	}
	return 0;
}












