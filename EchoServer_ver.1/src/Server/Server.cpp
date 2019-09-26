/*
 * Server.cpp
 *
 *  Created on: 2019. 9. 9.
 *      Author: leejinsoo
 */

#include "Server.h"
#include <mutex>
pthread_mutex_t mutex_lock;
pthread_mutex_t mutex_dellock;

//use to create thread

pthread_mutex_t sync_mutex;// = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  sync_cond;// = PTHREAD_COND_INITIALIZER;


// use to thread sync
pthread_mutex_t thread_mutex;// = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  thread_cond;//  = PTHREAD_COND_INITIALIZER;

CServer::CServer() {
	// TODO Auto-generated constructor stub
	m_UserConnMng = new CUserMng();
	for(int i = 0; i < USER_NUM; ++i)
	{
		m_User[i] = new CUser();
	}
}

CServer::~CServer() {
	// TODO Auto-generated destructor stub
	free(m_UserConnMng);
	for(int i = 0; i < USER_NUM; ++i)
	{
		free(m_User[i]);
	}
}

void* CServer::Thread_Worker(void* arg)
{
	while(1)
	{
		//int iFd = ((CServer*)arg)->m_iFd;
		int m_iEvent_Num = ((CServer*)arg)->m_iEvent_Num;
		int Events_Fd = (((CServer*) arg)->m_UserConnMng)->m_Events[m_iEvent_Num].data.fd;

		//if((((CServer*) arg)->m_UserConnMng)->m_Events[m_iEvent_Num].events & EPOLLIN)
		//{
			for(int i = 0; i < USER_NUM; ++i)
			{
				if( true == (((CServer*) arg)->m_User[i])->Get_Connect() )
				{
					//pthread_mutex_lock(&mutex_lock);
					(((CServer*) arg)->m_User[i])->Send(&(Events_Fd), &(((CServer*) arg)->m_PacketList));
					//pthread_mutex_unlock(&mutex_lock);
				}
			}

		//}

//		for(int i = 0; i < USER_NUM; ++i)
//		{
//			if( true == (((CServer*) arg)->m_User[i])->Get_Connect() )
//			{
//				(((CServer*) arg)->m_User[i])->Send(&(Events_Fd), &(((CServer*) arg)->m_PacketList));
//			}
//		}


	}
	return NULL;
}

void* CServer::Thread_UserCheck(void* arg)
{
	int iUserCnt = 0;
	while(1)
	{

		usleep(1000000);
		iUserCnt = 0;
		int iFd = ((CServer*)arg)->m_iFd;
		int m_iEvent_Num = ((CServer*)arg)->m_iEvent_Num;
		int Events_Fd = (((CServer*) arg)->m_UserConnMng)->m_Events[m_iEvent_Num].data.fd;

//		if (((CServer*)arg)->m_UserConnMng->m_Events[m_iEvent_Num].events & (EPOLLRDHUP | EPOLLHUP))
//		{
//			((CServer*)arg)->m_User[iFd]->Set_Connect(false);
//			((CServer*)arg)->m_User[iFd]->Set_ConnectSock(0);
//			epoll_ctl(((CServer*)arg)->m_iEpfd, EPOLL_CTL_DEL, Events_Fd, NULL);
//			//m_UserConnMng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_DEL, &m_UserConnMng->m_Events[i].data.fd, 0);
//			close(Events_Fd);
//			cout << "[+] Connection Closed < " << ((CServer*)arg)->m_User[iFd]->Get_Packet().m_szID << " > FD Num ( " << Events_Fd << " )" << endl;
//
//		}

		cout << "===========================================" << endl;
		cout << "                User Check" << endl;
		cout << "-------------------------------------------" << endl;

		for(int i = 0; i < USER_NUM; ++i)
		{
			if( true == (((CServer*) arg)->m_User[i])->Get_Connect() )
			{
				iUserCnt++;
			}
		}
		cout << endl;
		cout << " 현재 접속해 있는 유저 수 : " << iUserCnt << " 명" << endl;
		cout << endl;
		cout << "===========================================" << endl;
	}

	return NULL;
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
	//m_iAddrsize = sizeof(&m_iClient_Sock);

	if( m_UserConnMng->Client_Connect(&m_iEpfd, &m_iListen_Scok) == false )
	{
		perror("[+] - ERROR : Connect Failed");
		exit(1);
	}

	//pthread_mutex_init(&mutex_lock, NULL);


	m_iThrStatus = pthread_create(&Thread_Work, NULL, Thread_Worker, (void*)this);
	if (m_iThrStatus < 0) {
		perror("Thread Worker Create Error!!");
		exit(0);
	}
	pthread_detach(Thread_Work);

	m_iThrStatus = pthread_create(&Thread_User, NULL, Thread_UserCheck, (void*)this);
	if (m_iThrStatus < 0) {
		perror("Thread UserCheck Create Error!!");
		exit(0);
	}
	pthread_detach(Thread_User);

	while (1)
	{
		m_iEvent_Cnt = epoll_wait(m_iEpfd, m_UserConnMng->m_Events, MAX_EVENTS, -1);

		if(m_iEvent_Cnt == -1)
		{
			perror("[+] - ERROR : Epoll_Wait Failed");
			break;
		}

		for (int i = 0; i < m_iEvent_Cnt; ++i)
		{
			m_iEvent_Num = i;
			if (m_UserConnMng->m_Events[i].data.fd == m_iListen_Scok)
			{
				//m_iClient_Sock = m_UserConnMng->m_Events[i].data.fd;
				m_iAddrsize = sizeof(&m_Client_Addr);
				m_iConnect_Sock = accept(m_iListen_Scok, (struct sockaddr*) &m_Client_Addr, (socklen_t*) &m_iAddrsize);
				m_UserConnMng->Set_NonBlocking(&m_iConnect_Sock);
				m_UserConnMng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_ADD, &m_iConnect_Sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
				cout << "[+] Connect with" << " : " << ntohs(m_Client_Addr.sin_port) << endl;
				for(int j = 0; j < 100; ++j)
				{
					if(m_User[j]->Get_Connect() == false)
					{
						m_User[j]->Set_Connect(true);
						m_User[j]->Set_ConnectSock(m_iConnect_Sock);
						break;
					}
				}

			}

			else if(m_UserConnMng->m_Events[i].events & EPOLLIN)
			{
//				cout << "[ Thread Self Check Point ]" << endl;
//							pthread_t id;
//							id = pthread_self();
//							cout <<"Thread Routine : " << id << endl;


				for(int j = 0; j < USER_NUM; ++j)
				{
					if( m_UserConnMng->m_Events[i].data.fd == m_User[j]->Get_ConnectSock() )
					{
						m_iFd = j;

						if( -1  == m_User[j]->Recv(&(m_UserConnMng->m_Events[i].data.fd)) )
						{
							cout << "Recv Error!!!" << endl;
							m_User[m_iFd]->Set_Connect(false);
							m_User[m_iFd]->Set_ConnectSock(0);
							epoll_ctl(m_iEpfd, EPOLL_CTL_DEL, m_UserConnMng->m_Events[i].data.fd, NULL);
							close(m_UserConnMng->m_Events[i].data.fd);
							cout << "[+] Connection Closed < " << m_User[m_iFd]->Get_Packet().m_szID << " > FD Num ( " << m_UserConnMng->m_Events[i].data.fd << " )" << endl;
						}
						break;
					}
				}

			}
			else
				cout << "[+] Eerror : Unknown epoll evnet" << endl;


			if (m_UserConnMng->m_Events[i].events & (EPOLLRDHUP | EPOLLHUP))
			{
				m_User[m_iFd]->Set_Connect(false);
				m_User[m_iFd]->Set_ConnectSock(0);
				epoll_ctl(m_iEpfd, EPOLL_CTL_DEL, m_UserConnMng->m_Events[i].data.fd, NULL);
				//m_UserConnMng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_DEL, &m_UserConnMng->m_Events[i].data.fd, 0);
				close(m_UserConnMng->m_Events[i].data.fd);
				cout << "[+] Connection Closed < " << m_User[m_iFd]->Get_Packet().m_szID << " > FD Num ( " << m_UserConnMng->m_Events[i].data.fd << " )" << endl;
				break;
			}
		}
	}

	//for(int i = 0; i < 4; ++i)
	//	pthread_join(Thread_Rout, (void**)&m_iThrStat);
}
