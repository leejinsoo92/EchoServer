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
	CirBuf = CCircularBuffer<PACKET>(10);
}

CUser::~CUser() {
	// TODO Auto-generated destructor stub
}

void CUser::Send(int* _fd)
{
	isprint = false;

	switch (m_Packet.m_nCMD)
	{
	case CMD_USER_LOGIN_REQ:

		cout << "< " << m_Packet.m_szID << " > Login Complete!" << endl;
		m_Packet.m_nCMD ^= m_Packet.m_nCMD;
		m_Packet.m_nCMD |= CMD_USER_LOGIN_RESULT;
		send(*_fd,(char*) &m_Packet, sizeof(m_Packet), 0);
		break;
	case CMD_USER_DATA_REQ:

		cout << "[" << m_Packet.m_szID << "] message : " << m_Packet.m_szData << endl;
		m_Packet.m_nCMD ^= m_Packet.m_nCMD;
		m_Packet.m_nCMD |= CMD_USER_DATA_RESULT;
		send(*_fd,(char*) &m_Packet, sizeof(m_Packet), 0);
		break;
	}
}

void CUser::Send(int* _fd, list<PACKET>* _List)
{

	isDelFind = false;
	isSaveFind = false;

	switch (m_Packet.m_nCMD)
	{
	case CMD_USER_SAVE_REQ:
		cout << "[" << m_Packet.m_szID << "] message : " << m_Packet.m_szData << endl;

		if ( (*_List).empty())
		{
			m_Packet.m_nSock_ID = *_fd;
			m_Packet.m_nCMD ^= CMD_USER_SAVE_REQ;
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
				cout << "Data is already saved! (" << m_Packet.m_szData << ")"
						<< endl;
			} else {
				m_Packet.m_nSock_ID = *_fd;
				m_Packet.m_nCMD ^= CMD_USER_SAVE_REQ;
				m_Packet.m_nCMD |= CMD_USER_SAVE_RESULT;
				(*_List).push_back(m_Packet);
			}
		}
		isSaveFind = false;
		send(*_fd,(char*) &m_Packet, sizeof(m_Packet), 0);
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
		send(*_fd,(char*) &m_Packet, sizeof(m_Packet), 0);
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

			send(*_fd,(char*) &m_Packet, sizeof(m_Packet), 0);

			cout << "packet size : " << sizeof(m_Packet) << endl;
			cout << "struct data send complete !" << endl;

			if (strcmp(iter->m_szData, "END") == 0)
			{
				(*_List).erase(iter);
				break;
			}
		}
		break;
	}

}

int CUser::Recv(int* _fd)
{
	int iRecvLen = 0;
	iRecvLen = recv( *_fd, (char*)&m_Packet, sizeof(PACKET), 0);

	if(iRecvLen > 0 && (m_Packet.m_nCMD != CMD_USER_PRINT_REQ ))
	{
		CirBuf.Push(&m_Packet);
		CirBuf.Print();
		if(CirBuf.isFull())
			CirBuf.Pop();
	}
	return iRecvLen;
}
void CUser::Update()
{

}
