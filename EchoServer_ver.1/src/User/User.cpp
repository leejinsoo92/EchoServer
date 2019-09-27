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
	m_iCirbufSize = 50;
	CirBuf = new CCircularBuffer<PACKET>(m_iCirbufSize);
	m_isConn = false;
}

CUser::~CUser() {
	// TODO Auto-generated destructor stub
//	delete[] CirBuf;
//	CirBuf = nullptr;
}

void CUser::Send(int* _fd)
{
	isprint = false;

	//m_Packet = CirBuf->Pop();

	switch (m_Packet.m_nCMD)
	{
	case CMD_USER_LOGIN_REQ:

		cout << "< " << m_Packet.m_szID << " > Login Complete!" << endl;
		m_Packet.m_nCMD ^= m_Packet.m_nCMD;
		m_Packet.m_nCMD |= CMD_USER_LOGIN_RESULT;
		send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);
		break;
	case CMD_USER_DATA_REQ:

		cout << "[" << m_Packet.m_szID << "] message : " << m_Packet.m_szData << endl;
		m_Packet.m_nCMD ^= m_Packet.m_nCMD;
		m_Packet.m_nCMD |= CMD_USER_DATA_RESULT;
		send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);

		break;
	}
}

void CUser::Send(int* _fd, list<PACKET>* _List)
{
	isDelFind = false;
	isSaveFind = false;

	PACKET PacketCpy;
	if( CirBuf->isEmpty() == false )
	{
		memcpy(&PacketCpy, CirBuf->Pop(), sizeof(PACKET));
		CirBuf->Print();
	}

	if(true == PacketCheck(PacketCpy))
	{
		memset(&m_Packet, 0, sizeof(PACKET));
		memcpy(&m_Packet, &PacketCpy, sizeof(PACKET));
		CirBuf->ChangeFront();

		switch (PacketCpy.m_nCMD)
		{
		case CMD_USER_LOGIN_REQ:

			//cout << "< " << m_Packet.m_nSock_ID << " : " << m_Packet.m_szID << " > Login Complete!" << endl;
			PacketCpy.m_nSock_ID = m_iConnectSock;
			PacketCpy.m_nCMD ^= PacketCpy.m_nCMD;
			PacketCpy.m_nCMD |= CMD_USER_LOGIN_RESULT;
			send(*_fd,(char*) &PacketCpy, sizeof(PACKET), 0);
			break;
		case CMD_USER_DATA_REQ:

			//cout << "[" << m_iConnectSock << "] ID '" << m_Packet.m_szID << "' message : " << m_Packet.m_szData << endl;
			PacketCpy.m_nSock_ID = m_iConnectSock;
			PacketCpy.m_nCMD ^= PacketCpy.m_nCMD;
			PacketCpy.m_nCMD |= CMD_USER_DATA_RESULT;
			send(*_fd,(char*) &PacketCpy, sizeof(PACKET), 0);

			break;
		case CMD_USER_SAVE_REQ:
			cout << "[" << m_Packet.m_szID << "] message : " << m_Packet.m_szData << endl;

			if ( (*_List).empty())
			{
				m_Packet.m_nSock_ID = *_fd;
				m_Packet.m_nCMD ^= m_Packet.m_nCMD;
				m_Packet.m_nCMD |= CMD_USER_SAVE_RESULT;
				(*_List).push_back(m_Packet);
			}
			else
			{
				for (list<PACKET>::iterator iter = (*_List).begin();iter != (*_List).end(); ++iter) {

					if (strcmp(iter->m_szData, m_Packet.m_szData) == 0) {
						isSaveFind = true;
						break;
					}
				}

				if (isSaveFind == true) {
					m_Packet.m_nSock_ID = *_fd;
					m_Packet.m_nCMD ^= m_Packet.m_nCMD;
					m_Packet.m_nCMD |= CMD_USER_ERR;
					cout << "Data is already saved! (" << m_Packet.m_szData << ")" << endl;
				} else {
					m_Packet.m_nSock_ID = *_fd;
					m_Packet.m_nCMD ^= m_Packet.m_nCMD;
					m_Packet.m_nCMD |= CMD_USER_SAVE_RESULT;
					(*_List).push_back(m_Packet);
				}
			}
			isSaveFind = false;

			send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);
			break;
		case CMD_USER_DELETE_REQ:

			cout << "[" << m_Packet.m_szID << "] message : " << m_Packet.m_szData<< endl;

			if ((*_List).empty())
			{
				m_Packet.m_nSock_ID = *_fd;
				m_Packet.m_nCMD ^= m_Packet.m_nCMD;
				m_Packet.m_nCMD |= CMD_USER_ERR;
				cout << "This Stored is Empty" << endl;
			}
			else
			{

				for (list<PACKET>::iterator iter = (*_List).begin(); iter != (*_List).end(); )
				{
					if (strcmp(iter->m_szData, m_Packet.m_szData) == 0)
					{
						iter = (*_List).erase(iter);
						isDelFind = true;
					}
					else
						++iter;
				}

				if (isDelFind == true)
				{
					m_Packet.m_nSock_ID = *_fd;
					m_Packet.m_nCMD ^= m_Packet.m_nCMD;
					m_Packet.m_nCMD |= CMD_USER_DELETE_RESULT;
				}
				else
				{
					m_Packet.m_nSock_ID = *_fd;
					m_Packet.m_nCMD ^= m_Packet.m_nCMD;
					m_Packet.m_nCMD |= CMD_USER_ERR;
					cout << "We don't have data!" << endl;
				}
			}
			isDelFind = false;
			send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);
			break;
		case CMD_USER_PRINT_REQ:
			if ((*_List).empty())
			{
				memset(&m_Packet.m_szData, 0, sizeof(MAX_PACKET_SIZE));
				string data = "END";
				strcpy(m_Packet.m_szData,  data.c_str());
				(*_List).push_back(m_Packet);

				m_Packet.m_nSock_ID = *_fd;
				m_Packet.m_nCMD ^= m_Packet.m_nCMD;
				m_Packet.m_nCMD |= CMD_USER_ERR;
				send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);
				(*_List).pop_back();
				break;
			}

			//memset(&m_Packet.m_szData, 0, sizeof(MAX_PACKET_SIZE));
			string data = "END";
			strcpy(m_Packet.m_szData,  data.c_str());
			(*_List).push_back(m_Packet);
			for (list<PACKET>::iterator iter = (*_List).begin(); iter != (*_List).end(); ++iter)
			{

				m_Packet.m_nSock_ID = *_fd;
				m_Packet.m_nCMD ^= m_Packet.m_nCMD;
				m_Packet.m_nCMD |= CMD_USER_PRINT_RESULT;
				strcpy(m_Packet.m_szData, iter->m_szData);

				send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);
				cout << "save in struct, data info : " << m_Packet.m_szData << endl;
//				//cout << "packet size : " << sizeof(m_Packet) << endl;
//				cout << "struct data send complete !" << endl;

				if (strcmp(iter->m_szData, "END") == 0)
				{
					(*_List).erase(iter);
					break;
				}

			}
			break;
		}//switch

		m_isRecv = false;

	}//if
}

//int CUser::Recv(int* _fd)
//{
//	int n, offset = 0;
//	errno = 0;
//	PACKET RecvPack;
//
//	//MSG_PEEK | MSG_DONTWAIT
//	while(sizeof(PACKET) - offset > 0 && (n = recv(*_fd, (char*)&RecvPack + offset, sizeof(PACKET) - offset,  0)) > 0)
//	{
//		offset += n;
//	}
//
//	if( offset == 0 && errno == EAGAIN )
//	{
//		//cout << "[Server] no message recived." << endl;
//		return -1;
//	}
//	else
//	{
////		cout << "Recv Func() working Push Data" << endl;
////		cout << RecvPack.m_nSock_ID << endl;
////		cout << RecvPack.m_szID << endl;
////		cout << RecvPack.m_szData << endl;
////		cout << RecvPack.m_iSize << endl;
//
//		CirBuf->Push(&RecvPack);
////		CirBuf->Print();
////		if (CirBuf->isFull())
////			CirBuf->Pop();
//		return offset;
//	}
//}

bool CUser::PacketCheck(PACKET data)
{
//	if( strcmp( data.m_szHead, "AA11" ) != 0 )
//		return false;
//	if( strcmp( data.m_szTail, "11AA" ) != 0 )
//		return false;
	//if( data.m_nSock_ID <= 0)
	//	return false;
	//if(data.m_szID == NULL)
	//	return false;
	if( data.m_nCMD <= 0 || data.m_nCMD >= 11)
		return false;
	if( data.m_iSize <= 0 )
		return false;

	return true;
}

void CUser::ClearBuf()
{
	for(int i = 0; i < m_iCirbufSize; ++i)
	{
		memset(&CirBuf[i], 0, sizeof(PACKET));
	}
}

int CUser::Recv(int* _fd)
{
	int iRecvLen = 0;
	//memset(&m_Packet, 0, sizeof(PACKET));
	PACKET RecvPack;
	while(1)
	{
		iRecvLen = recv( *_fd, (char*)&RecvPack, sizeof(PACKET), 0);

		if(iRecvLen == 0)
			return -1;
		else if(iRecvLen < 0)
		{
			if( errno == EAGAIN)
				break;
		}
		else
		{
			CirBuf->Push(&RecvPack);
			//if(CirBuf->isFull())
			//	CirBuf->ChangeFront();
		}
	}
	return 0;
}












