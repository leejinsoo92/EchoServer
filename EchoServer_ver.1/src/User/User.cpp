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
	CirBuf = new CCircularBuffer<PACKET>(20);
}

CUser::~CUser() {
	// TODO Auto-generated destructor stub
}

void CUser::Send(int* _fd)
{
	isprint = false;

	m_Packet = CirBuf->Pop();

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

	m_Packet = CirBuf->Pop();

	if(true == PacketCheck(m_Packet))
	{
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
				m_Packet.m_nSock_ID = *_fd;
				m_Packet.m_nCMD ^= m_Packet.m_nCMD;
				m_Packet.m_nCMD |= CMD_USER_ERR;
				send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);
				break;
			}

			string data = "END";
			memcpy(m_Packet.m_szData, data.c_str(), data.length());

			(*_List).push_back(m_Packet);
			for (list<PACKET>::iterator iter = (*_List).begin(); iter != (*_List).end(); ++iter)
			{

				m_Packet.m_nSock_ID = *_fd;
				m_Packet.m_nCMD ^= m_Packet.m_nCMD;
				m_Packet.m_nCMD |= CMD_USER_PRINT_RESULT;
				memcpy(m_Packet.m_szData, iter->m_szData, sizeof(iter->m_szData));

				send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);

				cout << "packet size : " << sizeof(m_Packet) << endl;
				cout << "struct data send complete !" << endl;

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

int CUser::Recv(int* _fd)
{
	int n, offset = 0;
	errno = 0;

	while(sizeof(PACKET) - offset > 0 && (n = recv(*_fd, (char*)&m_Packet + offset, sizeof(PACKET) - offset, MSG_DONTWAIT)) > 0)
	{
		offset += n;
	}

//	if( sizeof(PACKET) - offset == 0)
//	{
//		CirBuf->Push(&m_Packet);
//				CirBuf->Print();
//				if(CirBuf->isFull())
//					CirBuf->Pop();
//				m_isRecv = true;
//
//		cout << "packet sizeof : " << sizeof(PACKET) << endl;
//		cout << "offset size : " << offset << endl;
//
//		cout << "cirbuf ID : " << CirBuf->m_tData->m_szID << endl;
//		cout << "cirbuf cmd : " << CirBuf->m_tData->m_nCMD << endl;
//		cout << "cirbuf size : " << CirBuf->m_tData->m_iSize << endl;
//		cout << "cirbuf data : " << CirBuf->m_tData->m_szData << endl;
//
//	}

	if( offset == 0 && errno == EAGAIN )
	{
		//cout << "[Server] no message recived." << endl;
		return -1;
	}
	else
	{
		CirBuf->Push(&m_Packet);
		CirBuf->Print();
		if (CirBuf->isFull())
			CirBuf->Pop();
		m_isRecv = true;
		return offset;
	}
}

bool CUser::PacketCheck(PACKET data)
{
	if( strcmp( data.m_szHead, "AA11" ) != 0 )
		return false;
	if( strcmp( data.m_szTail, "11AA" ) != 0 )
		return false;
	if( data.m_nCMD <= 0 || data.m_nCMD >= 11)
		return false;
	if( data.m_iSize <= 0 )
		return false;

	return true;
}

//int CUser::Recv(int* _fd)
//{
//	int iRecvLen = 0;
//	while(1)
//	{
//		iRecvLen = recv( *_fd, (char*)&m_Packet, sizeof(PACKET), 0);
//
//		if(iRecvLen == 0)
//			return -1;
//		else if(iRecvLen < 0 && errno == EAGAIN)
//			break;
//		else
//		{
//			CirBuf->Push(&m_Packet);
//			CirBuf->Print();
//			if(CirBuf->isFull())
//				CirBuf->Pop();
//		}
//	}
//	return 0;
//}







//bool CUser::isPackFull()
//{
//	PACKET pop_pack = CirBuf.Pop();
//
//	if(strcmp(pop_pack.m_szHead, "AA11") == 0 && strcmp(pop_pack.m_szTail, "11AA") == 0)
//		return true;
//
//	return false;
//}

bool CUser::Parsing(int* _fd, list<PACKET>* _List)
{
	m_Packet = CirBuf->Pop();

	if(strcmp(m_Packet.m_szHead, "AA11") == 0 && strcmp(m_Packet.m_szTail, "11AA") == 0)
	{
		switch (m_Packet.m_nCMD)
			{
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
				string data = "END";
				memcpy(m_Packet.m_szData, data.c_str(), data.length());

				(*_List).push_back(m_Packet);
				for (list<PACKET>::iterator iter = (*_List).begin(); iter != (*_List).end(); ++iter)
				{
					m_Packet.m_nSock_ID = *_fd;
					m_Packet.m_nCMD ^= m_Packet.m_nCMD;
					m_Packet.m_nCMD |= CMD_USER_PRINT_RESULT;
					memcpy(m_Packet.m_szData, iter->m_szData, sizeof(iter->m_szData));

					send(*_fd,(char*) &m_Packet, sizeof(PACKET), 0);

					cout << "packet size : " << sizeof(m_Packet) << endl;
					cout << "struct data send complete !" << endl;

					if (strcmp(iter->m_szData, "END") == 0)
					{
						(*_List).erase(iter);
						break;
					}
				}
				break;
			}//switch


		return true;
	}

	return false;
}













