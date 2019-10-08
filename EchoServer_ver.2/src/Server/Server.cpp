/*
 * Server.cpp
 *
 *  Created on: 2019. 9. 9.
 *      Author: leejinsoo
 */

#include "Server.h"
#include "../DataReposit/DataReposit.h"
#include <mutex>
pthread_mutex_t mutex_lock;
//use to create thread

pthread_mutex_t sync_mutex;// = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  sync_cond;// = PTHREAD_COND_INITIALIZER;


// use to thread sync
pthread_mutex_t thread_mutex;// = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  thread_cond;//  = PTHREAD_COND_INITIALIZER;

CServer::CServer() {
	// TODO Auto-generated constructor stub
	m_UserConnMng = new CUserMng();
}

CServer::~CServer() {
	// TODO Auto-generated destructor stub
	delete m_UserConnMng;
	m_UserConnMng = nullptr;

	pthread_mutex_destroy(&sync_mutex);
}

void* CServer::Thread_Worker(void* arg)
{
	while(1)
	{

		for(int i = 0; i < USER_NUM; ++i)
		{
			if( true == (((CServer*) arg)->m_User[i]).Get_Connect() )
			{
				if ( true == (((CServer*) arg)->m_User[i]).Get_isRecv() )
					(((CServer*) arg)->m_User[i]).ProcMsg((((CServer*) arg)->m_iClient_Socks[i]));
			}
		}

	}
	return NULL;
}

void* CServer::Thread_UserCheck(void* arg)
{

	while(1)
	{
		int iUserCnt = 0;
		usleep(2000000);

		cout << "===========================================" << endl;
		cout << "                User Check" << endl;
		cout << "-------------------------------------------" << endl;


		for(int i = 0; i < USER_NUM; ++i)
		{
			if( true == (((CServer*) arg)->m_User[i]).Get_Connect() )
			{
				iUserCnt++;
				//cout << "ID :" << ((CServer*) arg)->m_User[i].Get_ID() << endl;
			}
		}
		cout << endl;
		cout << " 현재 접속해 있는 유저 수 : " << iUserCnt << " 명" << endl;

		cout << endl;
		cout << "===========================================" << endl;
	}

	return NULL;
}

void CServer::Run()
{
	if( m_UserConnMng->Client_Connect(&m_iEpfd, &m_iListen_Scok) == false )
	{
		perror("[+] - ERROR : Connect Failed");
		exit(1);
	}

	pthread_mutex_init(&sync_mutex, NULL);


	m_iThrStatus = pthread_create(&Thread_Work, NULL, Thread_Worker, (void*)this);
	if (m_iThrStatus < 0) {
		perror("Thread Worker Create Error!!");
		exit(0);
	}
	pthread_detach(Thread_Work);

//	m_iThrStatus = pthread_create(&Thread_User, NULL, Thread_UserCheck, (void*)this);
//	if (m_iThrStatus < 0) {
//		perror("Thread UserCheck Create Error!!");
//		exit(0);
//	}
//	pthread_detach(Thread_User);

	while (1)
	{
		m_iEvent_Cnt = epoll_wait(m_iEpfd, m_UserConnMng->m_Events, MAX_EVENTS, -1);

		for (int i = 0; i < m_iEvent_Cnt; ++i)
		{
			m_iClient_Fd = m_UserConnMng->m_Events[i].data.fd;
			if (m_iClient_Fd == m_iListen_Scok)
			{
				m_iAddrsize = sizeof(&clientaddr);
				m_iClient_Fd = accept(m_iListen_Scok, (struct sockaddr*) &clientaddr, (socklen_t*) &m_iAddrsize);
				m_UserConnMng->Set_NonBlocking(&m_iClient_Fd);
				m_UserConnMng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_ADD, &m_iClient_Fd, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
				cout << "[+] Connect with" << " : " << inet_ntoa(clientaddr.sin_addr) << endl;

				for(int j = 0; j < USER_NUM; ++j)
				{
					if(m_iClient_Socks[j] == 0)
					{
						m_iClient_Socks[j] = m_iClient_Fd;
						m_User[j].Set_Connect(true);
						m_User[j].Set_ConnectSock(m_iClient_Fd);
						break;
					}
				}
			}

			else
			{

				for(int j = 0; j < USER_NUM; ++j)
				{
					if( m_iClient_Fd == m_iClient_Socks[j] )
					{
						if( -1  == m_User[j].Recv(m_iClient_Fd) )
						{
							cout << "Recv Error!!!" << endl;
							m_User[j].Set_Connect(false);
							m_User[j].Set_ConnectSock(0);
							m_iClient_Socks[j] = 0;

							epoll_ctl(m_iEpfd, EPOLL_CTL_DEL, m_UserConnMng->m_Events[i].data.fd, NULL);
							close(m_UserConnMng->m_Events[i].data.fd);
							cout << "[+] Connection Closed < " << m_User[j].Get_ID() << " > FD Num ( " << m_UserConnMng->m_Events[i].data.fd << " )" << endl;
						}
						break;
					}
				}

			}

			if (m_UserConnMng->m_Events[i].events & (EPOLLRDHUP | EPOLLHUP))
			{

				for(int j = 0; j < USER_NUM; ++j)
				{
					if( m_UserConnMng->m_Events[i].data.fd == m_User[j].Get_ConnectSock() )
					{
						m_User[j].Set_Connect(false);
						m_User[j].Set_ConnectSock(0);

						epoll_ctl(m_iEpfd, EPOLL_CTL_DEL, m_UserConnMng->m_Events[i].data.fd, NULL);
						close(m_UserConnMng->m_Events[i].data.fd);
						cout << "[+] Connection Closed < " << m_User[j].Get_ID() << " > FD Num ( " << m_UserConnMng->m_Events[i].data.fd << " )" << endl;
						break;
					}
				}
			}

		}
	}

}
