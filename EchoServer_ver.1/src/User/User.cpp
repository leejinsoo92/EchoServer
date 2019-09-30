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
	//CirBuf = new CCircularBuffer<PACKET>(m_iCirbufSize);
	//CirBuf = new CCircularBuf[m_iCirbufSize];
	m_isConn = false;

	//m_iPackTemp = new char[4000000];
}

CUser::~CUser() {
	// TODO Auto-generated destructor stub
//	delete[] CirBuf;
//	CirBuf = nullptr;
}

void CUser::Send(int* _fd, list<PACKET>* _List)
{
	isDelFind = false;
	isSaveFind = false;

	char szPacket_Buf[MAX_PACKET_SIZE];
	PACKET_HEAD Head;
	PACKET Packet;
	PACKET_TAIL Tail;

//	if( CirBuf->isEmpty() == false )
//	{
//		memcpy(&szPacket_Buf, CirBuf->Pop(), sizeof(PACKET));
//		//CirBuf->Print();
//	}

	//if(m_iPackTemp != nullptr)
	//	memcpy(&szPacket_Buf, m_iPackTemp, sizeof(m_iPackTemp));

	if(m_isRecv == true)
	{
		DisAssemblePacket(m_iPackTemp, Head, Packet, Tail);
		usleep(10000);
			cout << "Head : " << Head.m_szHead << endl;
			cout << "Packet Size : " << Head.m_iPacketSize << endl << endl;
			cout << "ID : " << Packet.m_szID << endl;
			cout << "CMD : " << Packet.m_nCMD << endl;
			cout << "DATA : " << Packet.m_szData << endl << endl;
			cout << "Tail : " << Tail.m_szTail << endl;
	}

//	cout << "Head : " << Head.m_szHead << endl;
//	cout << "Packet Size : " << Head.m_iPacketSize << endl << endl;
//	cout << "ID : " << Packet.m_szID << endl;
//	cout << "CMD : " << Packet.m_nCMD << endl;
//	cout << "DATA : " << Packet.m_szData << endl << endl;
//	cout << "Tail : " << Tail.m_szTail << endl;

	if(true == PacketCheck(Head, Tail))
	{
		memset(&m_Packet, 0, sizeof(PACKET));
		memcpy(&m_Packet, &Packet, sizeof(PACKET));
		//CirBuf->ChangeFront();

		switch (Packet.m_nCMD)
		{
		case CMD_USER_LOGIN_REQ:

			//cout << "< " << m_Packet.m_nSock_ID << " : " << m_Packet.m_szID << " > Login Complete!" << endl;
			Packet.m_nSock_ID = m_iConnectSock;
			Packet.m_nCMD ^= Packet.m_nCMD;
			Packet.m_nCMD |= CMD_USER_LOGIN_RESULT;

			Head.m_iPacketSize = sizeof(Packet);

			AssemblePacket(szPacket_Buf, Head, Packet, Tail);

			send(*_fd,(char*) &szPacket_Buf, sizeof(szPacket_Buf), 0);
			break;
		case CMD_USER_DATA_REQ:

			//cout << "[" << m_iConnectSock << "] ID '" << m_Packet.m_szID << "' message : " << m_Packet.m_szData << endl;
			Packet.m_nSock_ID = m_iConnectSock;
			Packet.m_nCMD ^= Packet.m_nCMD;
			Packet.m_nCMD |= CMD_USER_DATA_RESULT;

			Head.m_iPacketSize = sizeof(Packet);

			AssemblePacket(szPacket_Buf, Head, Packet, Tail);

			send(*_fd,(char*) &szPacket_Buf, sizeof(szPacket_Buf), 0);

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
//	char szRecvBuf[MAX_PACKET_SIZE] = {};
//	//MSG_PEEK | MSG_DONTWAIT
//	while(MAX_PACKET_SIZE - offset > 0 && (n = recv(*_fd, (char*)&szRecvBuf + offset, MAX_PACKET_SIZE - offset,  0)) > 0)
//	{
//		memcpy(m_iPackTemp, szRecvBuf, offset);
//		cout << "recv buf : " << m_iPackTemp << endl;
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
//
////		CirBuf->Print();
////		if (CirBuf->isFull())
////			CirBuf->Pop();
//		return offset;
//	}
//}

bool CUser::PacketCheck(PACKET_HEAD head, PACKET_TAIL tail)
{
	if( strcmp( head.m_szHead, "AA11" ) != 0 )
		return false;
	if( head.m_iPacketSize <= 0 )
		return false;
//	if( strcmp( tail.m_szTail, "11AA" ) != 0 )
//		return false;
	//if( data.m_nSock_ID <= 0)
	//	return false;
	//if(data.m_szID == NULL)
	//	return false;
//	if( data.m_nCMD <= 0 || data.m_nCMD >= 11)
//		return false;
//	if( data.m_iSize <= 0 )
//		return false;

	return true;
}

void CUser::ClearBuf()
{
//	for(int i = 0; i < m_iCirbufSize; ++i)
//	{
//		memset(&CirBuf[i], 0, sizeof(PACKET));
//	}
}

int CUser::Recv(int* _fd)
{
	int iRecvLen = 0;
	//memset(&m_Packet, 0, sizeof(PACKET));
	char szRecvBuf[MAX_PACKET_SIZE] = {};
	PACKET_HEAD Head;
	PACKET Packet;
	PACKET_TAIL Tail;

	m_isRecv = false;
	while(1)
	{
		iRecvLen = recv( *_fd, (char*)&szRecvBuf, sizeof(szRecvBuf), 0);
		cout << iRecvLen << endl;
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
			memcpy(m_iPackTemp, szRecvBuf, sizeof(szRecvBuf));

			//cout << iRecvLen << endl;
//			if(MAX_PACKET_SIZE == sizeof(szRecvBuf))
//				m_isRecv = true;
			//CirBuf->Push(szRecvBuf);
			//if(CirBuf->isFull())
			//	CirBuf->ChangeFront();
		}
	}
	return 0;
}












