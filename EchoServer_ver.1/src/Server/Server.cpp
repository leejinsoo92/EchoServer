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
}

void* CServer::Thread_Routine(void* arg)
{
	while(1)
	{
		int iFd = ((CServer*)arg)->iFd;
		int m_iEvent_Num = ((CServer*)arg)->m_iEvent_Num;
		int Events_Fd = (((CServer*) arg)->m_UserConnMng)->m_Events[m_iEvent_Num].data.fd;

	if((((CServer*) arg)->m_UserConnMng)->m_Events[m_iEvent_Num].events & EPOLLIN)
	{
		//for(int j = 0; j < USER_NUM; ++j)
		//{
		//	if( Events_Fd == (((CServer*) arg)->m_User[j])->Get_ConnectSock() )
		//	{
				if( true == (((CServer*) arg)->m_User[iFd])->Get_isRecv() )
					(((CServer*) arg)->m_User[iFd])->Send(&(Events_Fd), &(((CServer*) arg)->m_PacketList));
//				iFd = j;
//				break;
		//	}
	//	}
	}
		//while (1)
		//{

////			((CServer*) arg)->m_iString_Len = (((CServer*) arg)->m_User[iFd])->Recv(&(Events_Fd));
////
//			if (((CServer*) arg)->m_iString_Len < 0)
//			{
//				//pthread_mutex_unlock(&mutex_lock);
//				break;
//			}
//
//			((CServer*) arg)->isprint = false;

			//pthread_mutex_lock(&mutex_lock);



			//(((CServer*) arg)->m_UserConnMng)->Epoll_Ctl(&(((CServer*) arg)->m_iEpfd), EPOLL_CTL_MOD, &Events_Fd, EPOLLIN | EPOLLET);
			//pthread_mutex_unlock(&mutex_lock);


//			switch ( (((CServer*) arg)->m_User[iFd])->Get_Packet().m_nCMD)
//			{
//			case CMD_USER_LOGIN_REQ:
//
//				(((CServer*) arg)->m_User[iFd])->Send(&(Events_Fd));
//
//				break;
//			case CMD_USER_DATA_REQ:
//
//				if (((CServer*) arg)->m_iString_Len <= 0) {
//					break;
//				} else {
//
//					(((CServer*) arg)->m_User[iFd])->Send(&(Events_Fd));
//				}
//
//				break;
//			case CMD_USER_SAVE_REQ:
//
//				if (((CServer*) arg)->m_iString_Len <= 0) {
//					break;
//				} else {
//					(((CServer*) arg)->m_User[iFd])->Send(&(Events_Fd),
//							&(((CServer*) arg)->m_PacketList));
//
//					if (!(((CServer*) arg)->m_PacketList.empty())) {
//						for (list<PACKET>::iterator iter =
//								((CServer*) arg)->m_PacketList.begin();
//								iter != ((CServer*) arg)->m_PacketList.end();
//								++iter) {
//							cout << "save data : " << iter->m_szData << endl;
//
//						}
//					}
//				}
//
//				break;
//			case CMD_USER_DELETE_REQ:
//
//				if (((CServer*) arg)->m_iString_Len <= 0) {
//					break;
//				} else {
//					(((CServer*) arg)->m_User[iFd])->Send(&(Events_Fd),
//							&(((CServer*) arg)->m_PacketList));
//
//					if (!(((CServer*) arg)->m_PacketList.empty())) {
//						for (list<PACKET>::iterator iter =
//								((CServer*) arg)->m_PacketList.begin();
//								iter != ((CServer*) arg)->m_PacketList.end();
//								++iter) {
//							cout << "Struct Data : " << iter->m_szData << endl;
//						}
//					}
//				}
//
//				break;
//			case CMD_USER_PRINT_REQ:
//				if (((CServer*) arg)->isprint == true) {
//					break;
//				} else {
//
//					(((CServer*) arg)->m_User[iFd])->Send(&(Events_Fd),	&(((CServer*) arg)->m_PacketList));
//					((CServer*) arg)->isprint = true;
//				}
//				break;
//			}

		//}
	}
	//}
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
	if( m_UserConnMng->Client_Connect(&m_iEpfd, &m_iListen_Scok) == false )
	{
		perror("Connect Failed!");
		exit(1);
	}

	m_iSock_Len = sizeof(m_Client_Addr);


	pthread_mutex_init(&mutex_lock, NULL);
//	pthread_mutex_init(&mutex_dellock, NULL);
//
//	pthread_mutex_init(&sync_mutex, NULL);
//	pthread_mutex_init(&thread_mutex, NULL);
//	pthread_cond_init(&sync_cond, NULL);
//	pthread_cond_init(&thread_cond, NULL);

	//while(1){
	//pthread_mutex_lock(&sync_mutex);
//	m_iThrStatus = pthread_create(&Thread_Conn, NULL, Thread_Connect, (void*)this);
//	if (m_iThrStatus < 0) {
//		perror("Thread Create Error!!");
//		exit(0);
//	}
//
//	//pthread_cond_wait(&sync_cond, &sync_mutex);
//
//	m_iThrStatus = pthread_create(&Thread_Rout, NULL, Thread_Routine, (void*)this);
//	if (m_iThrStatus < 0) {
//		perror("Thread Create Error!!");
//		exit(0);
//	}
//
//	//pthread_cond_wait(&sync_cond, &sync_mutex);
//
//	m_iThrStatus = pthread_create(&Thread_Diconn, NULL, Thread_DisConnect, (void*)this);
//	if (m_iThrStatus < 0) {
//		perror("Thread Create Error!!");
//		exit(0);
//	}
//
//	pthread_join(Thread_Conn, (void**)&m_iThrStat);
//	pthread_join(Thread_Rout, (void**)&m_iThrStat);
//	pthread_join(Thread_Diconn, (void**)&m_iThrStat);

	//for(int i = 0; i < 4; ++i)
	//{
	m_iThrStatus = pthread_create(&Thread_Rout, NULL, Thread_Routine, (void*)this);
	if (m_iThrStatus < 0) {
		perror("Thread Create Error!!");
		exit(0);
	}
	//}
	pthread_detach(Thread_Rout);

	while (1)
	{
		//cout << "!!!!!!!!!!!!! EPOLL WAIT !!!!!!!!!!!!!" << endl;
		m_iEvent_Cnt = epoll_wait(m_iEpfd, m_UserConnMng->m_Events, MAX_EVENTS, -1);
		isThrOn = false;
		for (int i = 0; i < m_iEvent_Cnt; ++i)
		{

			if (m_UserConnMng->m_Events[i].data.fd == m_iListen_Scok)
			{
				for(int j = 0; j < 100; ++j)
				{
					if(m_User[j]->Get_Connect() == false)
					{
						m_iConnect_Sock = accept(m_iListen_Scok, (struct sockaddr*) &m_Client_Addr, (socklen_t*) &m_iSock_Len);
						m_User[j]->Set_Connect(true);
						m_User[j]->Set_ConnectSock(m_iConnect_Sock);
						cout << "[+] Connect with" << " : " << ntohs(m_Client_Addr.sin_port) << endl;

						m_UserConnMng->Set_NonBlocking(&m_iConnect_Sock);

						m_UserConnMng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_ADD, &m_iConnect_Sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
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
				m_iEvent_Num = i;
				for(int j = 0; j < USER_NUM; ++j)
				{
					if( m_UserConnMng->m_Events[i].data.fd == m_User[j]->Get_ConnectSock() )
					{
						iFd = j;
						break;
					}
				}

				if( -1  == m_User[iFd]->Recv(&(m_UserConnMng->m_Events[i].data.fd)) )
				{
					cout << "Recv Error!!!" << endl;
				}

//				for(int j = 0; j < USER_NUM; ++j)
//				{
//					if( m_UserConnMng->m_Events[i].data.fd == (m_User[j])->Get_ConnectSock() )
//					{
//						if( m_User[j]->Recv(&(m_UserConnMng->m_Events[i].data.fd)) == -1 )
//						{
//							cout << "Recv Error!!!" << endl;
//						}
//					}
//				}

//				while(1)
//				{
//					m_iString_Len = m_User[iFd]->Recv(&(m_UserConnMng->m_Events[i].data.fd));
//
//					if (m_iString_Len <= 0)
//					{
//						break;
//					}
//				}




			}
			else
				cout << "[+] Eerror : Unknown epoll evnet" << endl;

//			else if(m_UserConnMng.m_Events[i].events & EPOLLIN)
//			{
////				isThrOn = true;
////				m_iEvent_Num = i;
////				for(int j = 0; j < USER_NUM; ++j)
////				{
////					if( m_UserConnMng.m_Events[i].data.fd == m_User[j].Get_ConnectSock() )
////					{
////						iFd = j;
////						break;
////					}
////				}
//
//
//				m_iThrStatus = pthread_create(&Thread_Rout, NULL, Thread_Routine, (void*)this);
//				cout << "Create Thread_Routine [ " << Thread_Rout << " ] " << endl;
//				if (m_iThrStatus < 0) {
//					perror("Thread Create Error!!");
//					exit(0);
//				}
//
//				//pthread_detach(Thread_ID);
//			}

			/*
			else if (m_UserConnMng.m_Events[i].events & EPOLLIN)
			{

				m_iEvent_Num = i;
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
				cout << "[+] Unexpected" << endl;*/



			if (m_UserConnMng->m_Events[i].events & (EPOLLRDHUP | EPOLLHUP))
			{
				cout << "[+] Connection Closed < " << m_User[iFd]->Get_Packet().m_szID << " > FD Num ( " << m_UserConnMng->m_Events[i].data.fd << " )" << endl;
				m_User[iFd]->Set_Connect(false);
				m_User[iFd]->Set_ConnectSock(0);
				m_UserConnMng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_DEL, &m_UserConnMng->m_Events[i].data.fd, 0);
				close(m_UserConnMng->m_Events[i].data.fd);

				break;
			}
		}
	}

	//for(int i = 0; i < 4; ++i)
		//pthread_join(Thread_Rout, (void**)&m_iThrStat);
}
