/*
 * Server.cpp
 *
 *  Created on: 2019. 9. 9.
 *      Author: leejinsoo
 */

#include "Server.h"

CServer::CServer() {
	// TODO Auto-generated constructor stub
	m_UserConnMng = CUserMng();
	for(int i = 0; i < USER_NUM; ++i)
	{
		m_User[i] = CUser();
	}
}

CServer::~CServer() {
	// TODO Auto-generated destructor stub
}

void CServer::Set_List(PACKET _SetPacket)
{
	m_PacketList.push_back(_SetPacket);
}
list<PACKET>* CServer::Get_List()
{
	return &m_PacketList;
}

void CServer::Update()
{
	if( m_UserConnMng.Client_Connect(&m_iEpfd, &m_iListen_Scok) == false )
	{
		perror("Connect Failed!");
		exit(1);
	}


	m_iSock_Len = sizeof(m_Client_Addr);

	while (1)
	{
		m_iEvent_Cnt = epoll_wait(m_iEpfd, m_UserConnMng.m_Events, MAX_EVENTS, -1);

		for (int i = 0; i < m_iEvent_Cnt; ++i)
		{
			if (m_UserConnMng.m_Events[i].data.fd == m_iListen_Scok)
			{
				m_iConnect_Sock = accept(m_iListen_Scok, (struct sockaddr*) &m_Client_Addr, (socklen_t*) &m_iSock_Len);

				m_User[m_iClient_Cnt].Set_Connect(true);
				m_User[m_iClient_Cnt].Set_ConnectSock(m_iConnect_Sock);
				m_iClient_Cnt++;

				cout << "[+] Connect with" << " : " << ntohs(m_Client_Addr.sin_port) << endl;

				m_UserConnMng.Set_NonBlocking(&m_iConnect_Sock);

				m_UserConnMng.Epoll_Ctl(&m_iEpfd, EPOLL_CTL_ADD, &m_iConnect_Sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
			}
			else if (m_UserConnMng.m_Events[i].events & EPOLLIN)
			{

				for(int j = 0; j < USER_NUM; ++j)
				{
					if( m_UserConnMng.m_Events[i].data.fd == m_User[j].Get_ConnectSock() )
					{
						iFd = j;
						break;
					}
				}

				while (1)
				{
					isprint = false;
					isDelFind = false;
					isSaveFind = false;
					switch (m_User[iFd].Get_Packet().m_nCMD)
					{
					case CMD_USER_LOGIN_REQ:

						m_User[iFd].Send(&(m_UserConnMng.m_Events[i].data.fd));

						break;
					case CMD_USER_DATA_REQ:

						if (m_iString_Len <= 0) {
							break;
						} else {
							m_User[iFd].Send(&(m_UserConnMng.m_Events[i].data.fd));
						}

						break;
					case CMD_USER_SAVE_REQ:

						if (m_iString_Len <= 0) {
							break;
						} else {

							m_User[iFd].Send(&(m_UserConnMng.m_Events[i].data.fd), &m_PacketList);
							isSaveFind = false;
							if (!m_PacketList.empty())
							{
								for (list<PACKET>::iterator iter = m_PacketList.begin(); iter != m_PacketList.end(); ++iter)
								{
									cout << "save data : " << iter->m_szData << endl;

								}
							}
						}

						break;
					case CMD_USER_DELETE_REQ:
						if (m_iString_Len <= 0) {
							break;
						}
						else
						{
							m_User[iFd].Send(&(m_UserConnMng.m_Events[i].data.fd), &m_PacketList);
							isDelFind = false;

							if (!m_PacketList.empty())
							{
								for (list<PACKET>::iterator iter = m_PacketList.begin(); iter != m_PacketList.end(); ++iter)
								{
									cout << "Struct Data : " << iter->m_szData << endl;
								}
							}
						}

						break;
					case CMD_USER_PRINT_REQ:
						if (isprint == true) {
							break;
						} else {
							m_User[iFd].Send(&(m_UserConnMng.m_Events[i].data.fd), &m_PacketList);
							isprint = true;
						}

						break;
					default:
						m_iString_Len = m_User[iFd].Recv(&(m_UserConnMng.m_Events[i].data.fd));

						break;
					}

					if (m_iString_Len <= 0) {
						break;
					}

				}

			}
			else
				cout << "[+] Unexpected" << endl;

			if (m_UserConnMng. m_Events[i].events & (EPOLLRDHUP | EPOLLHUP))
			{
				cout << "[+] Connection Closed < " << m_User[iFd].Get_Packet().m_szID << " > FD Num ( " << m_UserConnMng.m_Events[i].data.fd << " )" << endl;
				m_UserConnMng.Epoll_Ctl(&m_iEpfd, EPOLL_CTL_DEL, &m_UserConnMng.m_Events[i].data.fd, 0);
				close(m_UserConnMng.m_Events[i].data.fd);

				break;
			}
		}
	}
}
