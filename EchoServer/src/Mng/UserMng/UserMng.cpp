/*
 * UserMng.cpp
 *
 *  Created on: 2019. 8. 12.
 *      Author: leejinsoo
 */

#include "UserMng.h"
#include <iostream>

using namespace std;
pthread_mutex_t mutex_lock;
pthread_mutex_t mutex_dellock;

CUserMng::CUserMng() {
	// TODO Auto-generated constructor stub
	Ready_Sock();
	//m_iState = pthread_mutex_init(&mutex, NULL);
}

CUserMng::~CUserMng() {
	// TODO Auto-generated destructor stub
}

void CUserMng::Ready_Sock()
{
	Epoll_Mng = new CEpoll_Manager();
	m_iListen_Scok = socket(AF_INET, SOCK_STREAM, 0);
	Epoll_Mng->Set_Sockaddr(&m_Server_Addr);
	bind(m_iListen_Scok, (struct sockaddr*)&m_Server_Addr, sizeof(m_Server_Addr));

	Epoll_Mng->Set_NonBlocking(&m_iListen_Scok);
}

void* CUserMng::Thread_Routine(void* arg)
{
	//CCircularBuffer<PACKET> CirBuf(10);

	int fd_Num = (int)((CUserMng*)arg)->curFd;

	std::mutex MUTEX;
	list<PACKET>::iterator iter_begin = ((CUserMng*)arg)->m_Datalist.begin();
	list<PACKET>::iterator iter_end = ((CUserMng*)arg)->m_Datalist.end();

	string data = "END";
	bool isprint = false;
	bool isDelFind = false;
	bool isSaveFind = false;

	epoll_event clnt_sock = ((CUserMng*)arg)->m_Events[fd_Num];

	int cnt = 0;

	while(1)
	{
		//pthread_mutex_lock(&mutex_lock);
		switch(((CUserMng*)arg)->m_Packet.m_nCMD)
		{
		case CMD_USER_LOGIN_REQ:

			cout << "< " << ((CUserMng*)arg)->m_Packet.m_szID << " > Login Complete!" << endl;
			((CUserMng*)arg)->m_Packet.m_nCMD ^= CMD_USER_LOGIN_REQ;
			((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_LOGIN_RESULT;
			send(((CUserMng*)arg)->m_Events[fd_Num].data.fd, (char*)&(((CUserMng*)arg)->m_Packet), sizeof(((CUserMng*)arg)->m_Packet), 0);

			break;
		case CMD_USER_DATA_REQ:

			if( ((CUserMng*)arg)->m_iString_Len <= 0 )
			{
				break;
			}
			else
			{
				//cout << cnt++ << endl;
				cout << "[" << ((CUserMng*)arg)->m_Packet.m_szID << "] message : " << ((CUserMng*)arg)->m_Packet.m_szData << endl;

				((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;
				((CUserMng*)arg)->m_Packet.m_nCMD ^= CMD_USER_DATA_REQ;
				((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_DATA_RESULT;

				((CUserMng*)arg)->CirBuf.Push(&(((CUserMng*)arg)->m_Packet));
				((CUserMng*)arg)->CirBuf.Print();
				if(((CUserMng*)arg)->CirBuf.isFull())
					((CUserMng*)arg)->CirBuf.Pop();

				send(clnt_sock.data.fd, (char*)&(((CUserMng*)arg)->m_Packet), sizeof(((CUserMng*)arg)->m_Packet), 0);

			}

			break;
		case CMD_USER_SAVE_REQ:
			pthread_mutex_lock(&mutex_lock);
			//MUTEX.lock();

			if( ((CUserMng*)arg)->m_iString_Len <= 0 )
			{
				break;
			}
			else
			{
				//cout << ((CUserMng*)arg)->m_Packet.m_iTestCnt	 << endl;
				cout << "[" << ((CUserMng*)arg)->m_Packet.m_szID << "] message : " << ((CUserMng*)arg)->m_Packet.m_szData << endl;

				if( ((CUserMng*)arg)->m_Datalist.empty())
				{
					((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;
					((CUserMng*)arg)->m_Packet.m_nCMD ^= CMD_USER_SAVE_REQ;
					((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_SAVE_RESULT;
					((CUserMng*)arg)->m_Datalist.push_back(((CUserMng*)arg)->m_Packet);
				}
				else
				{
					for(list<PACKET>::iterator iter = ((CUserMng*)arg)->m_Datalist.begin(); iter != ((CUserMng*)arg)->m_Datalist.end(); ++iter)
					{
						//&& !((CUserMng*)arg)->m_Datalist.empty()
						if(strcmp(iter->m_szData, ((CUserMng*)arg)->m_Packet.m_szData) == 0 )
						{
							isSaveFind = true;
							break;
						}
					}

					if(isSaveFind == true)
					{
						((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;
						((CUserMng*)arg)->m_Packet.m_nCMD ^= ((CUserMng*)arg)->m_Packet.m_nCMD;
						((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_ERR;
						cout << "Data is already saved! (" << ((CUserMng*)arg)->m_Packet.m_szData << ")" << endl;
					}
					else
					{
						((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;
						((CUserMng*)arg)->m_Packet.m_nCMD ^= CMD_USER_SAVE_REQ;
						((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_SAVE_RESULT;
						((CUserMng*)arg)->m_Datalist.push_back(((CUserMng*)arg)->m_Packet);
					}
				}




//				((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;
//				((CUserMng*)arg)->m_Packet.m_nCMD ^= CMD_USER_SAVE_REQ;
//				((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_SAVE_RESULT;
//
//				((CUserMng*)arg)->m_Datalist.push_back(((CUserMng*)arg)->m_Packet);

				send(clnt_sock.data.fd, (char*)&(((CUserMng*)arg)->m_Packet), sizeof(((CUserMng*)arg)->m_Packet), 0);
				isSaveFind = false;
				if( !((CUserMng*)arg)->m_Datalist.empty())
				{
					for(list<PACKET>::iterator iter = ((CUserMng*)arg)->m_Datalist.begin(); iter != ((CUserMng*)arg)->m_Datalist.end(); ++iter)
					{
						cout << "save data : " << iter->m_szData << endl;

					}
				}
			}
			pthread_mutex_unlock(&mutex_lock);
			//MUTEX.unlock();
			break;
		case CMD_USER_DELETE_REQ:
			pthread_mutex_lock(&mutex_dellock);
			//MUTEX.lock();

			if( ((CUserMng*)arg)->m_iString_Len <= 0 )
			{
				break;
			}
			else
			{


				cout << "[" << ((CUserMng*)arg)->m_Packet.m_szID << "] message : " << ((CUserMng*)arg)->m_Packet.m_szData << endl;

				if( ((CUserMng*)arg)->m_Datalist.empty())
				{
					((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;
					((CUserMng*)arg)->m_Packet.m_nCMD ^= ((CUserMng*)arg)->m_Packet.m_nCMD;
					((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_ERR;
					cout << "This Stored is Empty" << endl;
				}
				else
				{

					for(list<PACKET>::iterator iter = iter_begin; iter != iter_end;)
					{
						if(strcmp(iter->m_szData, ((CUserMng*)arg)->m_Packet.m_szData) == 0)
						{
							iter = ((CUserMng*)arg)->m_Datalist.erase(iter);
							isDelFind = true;
						}
						else
							++iter;
					}

					if(isDelFind == true)
					{
						((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;
						((CUserMng*)arg)->m_Packet.m_nCMD ^= CMD_USER_DELETE_REQ;
						((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_DELETE_RESULT;
					}
					else
					{
						((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;
						((CUserMng*)arg)->m_Packet.m_nCMD ^= ((CUserMng*)arg)->m_Packet.m_nCMD;
						((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_ERR;
						cout << "We don't have data!" << endl;
					}
				}

				send(clnt_sock.data.fd, (char*)&(((CUserMng*)arg)->m_Packet), sizeof(((CUserMng*)arg)->m_Packet), 0);
				isDelFind = false;
				if( !((CUserMng*)arg)->m_Datalist.empty())
				{
					for(list<PACKET>::iterator iter = ((CUserMng*)arg)->m_Datalist.begin(); iter != ((CUserMng*)arg)->m_Datalist.end(); ++iter)
					{
						cout << "Struct Data : " << iter->m_szData << endl;
					}
				}
			}
			pthread_mutex_unlock(&mutex_dellock);
			//MUTEX.unlock();
			break;
		case CMD_USER_PRINT_REQ:
			if(isprint == true )
			{
				break;
			}
			else
			{
				memcpy(((CUserMng*)arg)->m_Packet.m_szData, data.c_str(), data.length());
				((CUserMng*)arg)->m_Datalist.push_back(((CUserMng*)arg)->m_Packet);
				for(list<PACKET>::iterator iter = ((CUserMng*)arg)->m_Datalist.begin(); iter != ((CUserMng*)arg)->m_Datalist.end(); ++iter)
				{

					((CUserMng*)arg)->m_Packet.m_nSock_ID = clnt_sock.data.fd;

					//cout << "before cmd : " << ((CUserMng*)arg)->m_Packet.m_nCMD << endl;
					((CUserMng*)arg)->m_Packet.m_nCMD ^= ((CUserMng*)arg)->m_Packet.m_nCMD ;
					((CUserMng*)arg)->m_Packet.m_nCMD |= CMD_USER_PRINT_RESULT;
					memcpy(((CUserMng*)arg)->m_Packet.m_szData, iter->m_szData, sizeof(iter->m_szData));

					send(clnt_sock.data.fd, (char*)&((CUserMng*)arg)->m_Packet, sizeof(((CUserMng*)arg)->m_Packet), 0);
					cout << "packet size : " << sizeof(((CUserMng*)arg)->m_Packet) << endl;
					cout << "struct data send complete !" << endl;

					if( strcmp(iter->m_szData, "END") == 0 )
					{
						((CUserMng*)arg)->m_Datalist.erase(iter);
						isprint = true;
						break;
					}
				}
			}

			break;
		default:
			((CUserMng*)arg)->m_iString_Len = recv(((CUserMng*)arg)->m_Events[fd_Num].data.fd,
					(char*)&(((CUserMng*)arg)->m_Packet), sizeof(PACKET), 0);

			break;
		}

		if( ((CUserMng*)arg)->m_iString_Len <= 0 )
		{
			break;
		}
		//pthread_mutex_unlock(&mutex_lock);
	}

	return NULL;
}

void CUserMng::Update()
{
	//void* iResult = nullptr;
	CirBuf = CCircularBuffer<PACKET>(10);

	listen(m_iListen_Scok, MAX_CONNECT);

	Epoll_Mng->Init_Epoll(&m_iEpfd);
	Epoll_Mng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_ADD, &m_iListen_Scok, EPOLLIN | EPOLLOUT | EPOLLET);

	m_iSock_Len = sizeof(m_Client_Addr);
	m_iState = pthread_mutex_init(&mutex_lock, NULL);
	m_iDelState = pthread_mutex_init(&mutex_dellock, NULL);

	if(m_iState != 0 || m_iDelState != 0)
	{
		perror("Mutex Create Error");
	}

	while(1)
	{
		m_iNfds = epoll_wait(m_iEpfd, m_Events, MAX_EVENTS, -1);

		for(int i = 0; i < m_iNfds; ++i)
		{
			if(m_Events[i].data.fd == m_iListen_Scok)
			{
				m_iConnect_Sock = accept(m_iListen_Scok, (struct sockaddr*)&m_Client_Addr, (socklen_t*)&m_iSock_Len);
				cout << "[+] Connect with" << " : " << ntohs(m_Client_Addr.sin_port) << endl;

//				cout << "------------------------------------" << endl;
//				cout << "nfds : " << i << endl;
//				cout << "fd : " << m_Events[i].data.fd << endl;
//				cout << "ls : " << m_iListen_Scok << endl;
//				cout << "cs : " << m_iConnect_Sock << endl;
//				cout << "------------------------------------" << endl;

				Epoll_Mng->Set_NonBlocking(&m_iConnect_Sock);

				Epoll_Mng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_ADD, &m_iConnect_Sock, EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
			}
			else if(m_Events[i].events & EPOLLIN)
			{
				curFd = i;
//				cout << "------------------------------------" << endl;
//				cout << "nfds : " << i << endl;
//				cout << "fd : " << m_Events[i].data.fd << endl;
//				cout << "ls : " << m_iListen_Scok << endl;
//				cout << "cs : " << m_iConnect_Sock << endl;
//				cout << "------------------------------------" << endl;

				pthread_create(&Thread_ID, NULL, Thread_Routine, (void*)this);
				pthread_detach(Thread_ID);

			}
			else
				cout << "[+] Unexpected" << endl;

			if(m_Events[i].events &(EPOLLRDHUP | EPOLLHUP))
			{
				cout << "[+] Connection Closed < " << m_Packet.m_szID << " > FD Num ( " << m_Events[i].data.fd << " )" << endl;
				Epoll_Mng->Epoll_Ctl(&m_iEpfd, EPOLL_CTL_DEL, &m_Events[i].data.fd, 0);
				close(m_Events[i].data.fd);

				break;
			}
		}
	}

	//pthread_join(Thread_ID, &iResult);
	//cout << (char*)iResult << endl;
	//free(iResult);

}
