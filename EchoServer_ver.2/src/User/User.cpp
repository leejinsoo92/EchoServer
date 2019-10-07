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
#include "../DataReposit/DataReposit.h"

#include <mutex>
pthread_mutex_t data_lock;

CUser::CUser() {
	// TODO Auto-generated constructor stub
	m_iCirbufSize = 1024 * 32;
	m_pCirBuf = new CCircularBuf();
	m_isConn = false;

	pthread_mutex_init(&data_lock, NULL);
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
		{
			m_isRecv = false;
			return;
		}

		int iDataSize = m_pCirBuf->GetDataSize();
		if( (int)sizeof( PACKET_HEAD ) >= iDataSize )
		{
			m_isRecv = false;
			return;
		}

		PACKET_HEAD* head = (PACKET_HEAD*)m_pCirBuf->GetPacket(sizeof(PACKET_HEAD));
		if( head-> m_iPacketSize > iDataSize )
		{
			m_isRecv = false;
			return;
		}

		//int iTailSize = head->m_iPacketSize - sizeof(PACKET_TAIL);
		//PACKET_TAIL* tail = (PACKET_TAIL*)m_pCirBuf->GetPacket(iTailSize);
//		PACKET_TAIL* tail = (PACKET_TAIL*)(head + head->m_iPacketSize - sizeof(PACKET_TAIL));

		//strncmp( tail->m_szTail, gpTail, sizeof(gpTail)) != 0
		if( strncmp( head->m_szHead, gpHead, sizeof(gpHead)) != 0 )
		{
			m_pCirBuf->Pop( iDataSize );
			m_isRecv = false;
			return;
		}

		switch(head->m_nCMD)
		{
		case CMD_USER_LOGIN_REQ:
		{
			PACKET_CS_LOGIN* packet = (PACKET_CS_LOGIN*)(head);

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

			//cout << "recv echo data : " << packet->m_szData << endl;

			PACKET_SC_ECHO Answer;

			strncpy(Answer.m_szData, packet->m_szData, sizeof(Answer.m_szData));

			send(_fd, (char*)&Answer, sizeof(PACKET_SC_ECHO), 0);
		}
			break;
		case CMD_USER_SAVE_REQ:
		{
			//pthread_mutex_lock(&data_lock);
			PACKET_CS_SAVE* packet = (PACKET_CS_SAVE*)(head);

			//cout << "recv save data : " << packet->m_szData << endl;

			CDataReposit* DataList = CDataReposit::getInstance();

			DataList->SaveData(packet->m_szData);
			//DataList->PrintData();

			PACKET_SC_SAVE Answer;

			Answer.m_isComplete = true;

			send(_fd, (char*)&Answer, sizeof(PACKET_SC_SAVE), 0);
			//pthread_mutex_unlock(&data_lock);
		}
			break;
		case CMD_USER_DELETE_REQ:
		{
			//pthread_mutex_lock(&data_lock);
			PACKET_CS_DEL *packet = (PACKET_CS_DEL*) (head);

			//cout << "recv delete data : " << packet->m_szData << endl;

			CDataReposit *DataList = CDataReposit::getInstance();

			PACKET_SC_DEL Answer;
			if( true == DataList->DeleteData(packet->m_szData) )
				Answer.m_isComplete = true;
			else
				Answer.m_isComplete = false;

			//DataList->PrintData();

			send(_fd, (char*) &Answer, sizeof(PACKET_SC_DEL), 0);
			//pthread_mutex_unlock(&data_lock);
		}
			break;
		case CMD_USER_PRINT_REQ:
		{
			//pthread_mutex_lock(&data_lock);
			PACKET_CS_PRINT *packet = (PACKET_CS_PRINT*) (head);

			CDataReposit *DataList = CDataReposit::getInstance();
			int iPrintCnt = 0;
			while(1)
			{
				PACKET_SC_PRINT Answer;

				if(DataList->PrintSendData(iPrintCnt) == nullptr)
				{
					Answer.m_isComplete = true;
					const char* temp = "finish";
					strncpy(Answer.m_szData, temp, sizeof(Answer.m_szData));
					Answer.m_szData[6] = '\0';
					send(_fd, (char*) &Answer, sizeof(PACKET_SC_PRINT), 0);
					break;
				}

				Answer.m_isComplete = false;
				strncpy(Answer.m_szData, DataList->PrintSendData(iPrintCnt), sizeof(Answer.m_szData));
				send(_fd, (char*) &Answer, sizeof(PACKET_SC_PRINT), 0);

				iPrintCnt++;
				usleep(100000);
			}
			//pthread_mutex_unlock(&data_lock);
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












