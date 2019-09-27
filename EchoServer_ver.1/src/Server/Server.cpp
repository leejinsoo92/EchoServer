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
//	for(int i = 0; i < USER_NUM; ++i)
//	{
//		m_User[i] = new CUser();
//	}

//	m_User = new CUser[USER_NUM];
}

CServer::~CServer() {
	// TODO Auto-generated destructor stub
	delete m_UserConnMng;
	m_UserConnMng = nullptr;

//	for(int i = 0; i < USER_NUM; ++i)
//	{
//		delete m_User[i];
//		m_User[i] = nullptr;
//	}
//	delete[] m_User;
//	m_User = nullptr;


	pthread_mutex_destroy(&mutex_lock);
}

void* CServer::Thread_Worker(void* arg)
{
	while(1)
	{
		for(int i = 0; i < USER_NUM; ++i)
		{
			//if( ((CServer*) arg)->m_iClient_Sock == (((CServer*) arg)->m_User[i])->Get_ConnectSock() )
			if( true == (((CServer*) arg)->m_User[i]).Get_Connect() )
			{
				//pthread_mutex_lock(&mutex_lock);
				(((CServer*) arg)->m_User[i]).Send(&(((CServer*) arg)->m_iClient_Sock), &(((CServer*) arg)->m_PacketList));
				//pthread_mutex_unlock(&mutex_lock);
			}
		}
	}
	return NULL;
}

void* CServer::Thread_UserCheck(void* arg)
{
	int iUserCnt = 0;
	while(1)
	{

		usleep(2000000);
		iUserCnt = 0;
		//int iFd = ((CServer*)arg)->m_iFd;
//		int m_iEvent_Num = ((CServer*)arg)->m_iEvent_Num;
//		int Events_Fd = (((CServer*) arg)->m_UserConnMng)->m_Events[m_iEvent_Num].data.fd;
//
//		if (((CServer*)arg)->m_UserConnMng->m_Events[m_iEvent_Num].events & (EPOLLRDHUP | EPOLLHUP))
//		{
//			for(int i = 0; i < USER_NUM; ++i)
//			{
//				((CServer*)arg)->m_User[i]->Set_Connect(false);
//				((CServer*)arg)->m_User[i]->Set_ConnectSock(0);
//				//((CServer*)arg)->m_User[i]->ClearBuf();
//				epoll_ctl(((CServer*)arg)->m_iEpfd, EPOLL_CTL_DEL, Events_Fd, NULL);
//				//((CServer*)arg)->m_UserConnMng->Epoll_Ctl(&(((CServer*)arg)->m_iEpfd), EPOLL_CTL_DEL, &Events_Fd, 0);
//				close(Events_Fd);
//				cout << "[+] Connection Closed < " << ((CServer*)arg)->m_User[i]->Get_Packet().m_szID << " > FD Num ( " << Events_Fd << " )" << endl;
//			}
//		}

		cout << "===========================================" << endl;
		cout << "                User Check" << endl;
		cout << "-------------------------------------------" << endl;

		for(int i = 0; i < USER_NUM; ++i)
		{
			//if(((CServer*) arg)->m_iClient_Sock == (((CServer*) arg)->m_User[i])->Get_ConnectSock())
			if( true == (((CServer*) arg)->m_User[i]).Get_Connect() )
			{
				iUserCnt++;
				cout << "ID :" << ((CServer*) arg)->m_User[i].Get_Packet().m_szID << endl;
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

	pthread_mutex_init(&mutex_lock, NULL);


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
			m_iClient_Sock = m_UserConnMng->m_Events[i].data.fd;
			if (m_UserConnMng->m_Events[i].data.fd == m_iListen_Scok)
			{

				m_iAddrsize = sizeof(&m_iClient_Sock);
				m_iClient_Sock = accept(m_iListen_Scok, (struct sockaddr*) &m_iClient_Sock, (socklen_t*) &m_iAddrsize);
				m_UserConnMng->Set_NonBlocking(&m_iClient_Sock);
				m_UserConnMng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_ADD, &m_iClient_Sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
				cout << "[+] Connect with" << " : " << m_iClient_Sock << endl;
				for(int j = 0; j < USER_NUM; ++j)
				{
					if(m_iClient_Socks[j] == 0)
					{
						m_iClient_Socks[j] = m_iClient_Sock;
						m_User[j].Set_Connect(true);
						m_User[j].Set_ConnectSock(m_iClient_Sock);
						break;
					}

//					if(m_User[j]->Get_Connect() == false)
//					{
//						m_User[j]->Set_Connect(true);
//						m_User[j]->Set_ConnectSock(m_iConnect_Sock);
//						break;
//					}
				}
			}

			else if(m_UserConnMng->m_Events[i].events & EPOLLIN)
			{
				for(int j = 0; j < USER_NUM; ++j)
				{
					//if( m_UserConnMng->m_Events[i].data.fd == m_User[j]->Get_ConnectSock() )
					if( m_UserConnMng->m_Events[i].data.fd == m_iClient_Socks[j] )
					{
						if( -1  == m_User[j].Recv(&(m_UserConnMng->m_Events[i].data.fd)) )
						{
							cout << "Recv Error!!!" << endl;
							m_User[j].Set_Connect(false);
							m_User[j].Set_ConnectSock(0);
							m_iClient_Socks[j] = 0;
							//m_User[m_iFd]->ClearBuf();
							epoll_ctl(m_iEpfd, EPOLL_CTL_DEL, m_UserConnMng->m_Events[i].data.fd, NULL);
							close(m_UserConnMng->m_Events[i].data.fd);
							cout << "[+] Connection Closed < " << m_User[j].Get_Packet().m_szID << " > FD Num ( " << m_UserConnMng->m_Events[i].data.fd << " )" << endl;
						}
						break;
					}
				}

			}
			else
				cout << "[+] Eerror : Unknown epoll evnet" << endl;


			if (m_UserConnMng->m_Events[i].events & (EPOLLRDHUP | EPOLLHUP))
			{
				for(int j = 0; j < USER_NUM; ++j)
				{
					if( m_UserConnMng->m_Events[i].data.fd == m_User[j].Get_ConnectSock() )
					{
						m_User[j].Set_Connect(false);
						m_User[j].Set_ConnectSock(0);
						//m_User[m_iFd]->ClearBuf();
						epoll_ctl(m_iEpfd, EPOLL_CTL_DEL, m_UserConnMng->m_Events[i].data.fd, NULL);
						//m_UserConnMng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_DEL, &m_UserConnMng->m_Events[i].data.fd, 0);
						close(m_UserConnMng->m_Events[i].data.fd);
						cout << "[+] Connection Closed < " << m_User[j].Get_Packet().m_szID << " > FD Num ( " << m_UserConnMng->m_Events[i].data.fd << " )" << endl;
						break;
					}
				}
			}

		}
	}

	//for(int i = 0; i < 4; ++i)
	//pthread_join(Thread_Work, (void**)&m_iThrStat);
	//pthread_join(Thread_User, (void**)&m_iThrStat);
}
