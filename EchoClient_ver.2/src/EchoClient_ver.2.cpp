//============================================================================
// Name        : Echo_Client_1.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include "CircularBuf.h"

#include <iostream>
using namespace std;

#define DEFAULT_PORT	8080
#define MAX_CONN		16
#define MAX_EVENTS	32
#define BUF_SIZE		16
#define MAX_LINE		256
#define SERVER_IP		"192.168.8.39"

void client_run();
static void set_sockaddr(struct sockaddr_in *addr);
int Set_NonBlocking(int iSockfd);

int main()
{
	client_run();

	return 0;
}

void client_run()
{
	int sockfd = 0;
	int iData_Len = 0;
	struct sockaddr_in srv_addr;
	int iOpenMenu = 0;
	bool isAuto = true;
	bool isPrintFinish = false;

	int iRepositCnt = 0;
	int iCntCheck = 0;

	int iTotalRecvSize = 0;

	string loginTemp = "login request";
	string printTemp = "print request";
	string sID;
	unsigned short nMemoCmd = 0;

	int randSize = 0;

	int iOptval = 0;
	//int iOptlen = sizeof(iOptval);

	CCircularBuf* m_pCirBuf = new CCircularBuf();

	char szRecvBuf[MAX_PACKET_SIZE] = { } ;

	srand(time(NULL));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	set_sockaddr(&srv_addr);

	iOptval = 1024000;
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char*)&iOptval, sizeof(iOptval));
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char*)&iOptval, sizeof(iOptval));

	if(connect(sockfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) < 0)
	{
		perror("connect()");
		exit(1);
	}

	if(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
		cout << "error" << endl;


	cout << "===========================" << endl;
	cout << "	Echo Program" << endl;
	cout << endl;
	cout << "	1. 직접 입력" << endl;
	cout << "	2. 자동 입력" << endl;
	cout << "===========================" << endl;
	cout << "select : ";
	cin >> iOpenMenu;

	if(iOpenMenu == 1)
		isAuto = false;
	else if(iOpenMenu == 2)
		isAuto = true;

	cin.ignore();

	PACKET_CS_LOGIN Login_Packet;
	cout << "ID : ";
	fgets(Login_Packet.m_szID, sizeof(Login_Packet.m_szID), stdin);
	iData_Len = strlen(Login_Packet.m_szID) - 1;

	Login_Packet.m_szID[iData_Len] = '\0';

	send(sockfd, (char*)&Login_Packet, sizeof(PACKET_CS_LOGIN), 0);

	bool isSend = true;

	while(1)
	{
		int iMenu = 0;
		string sInput;
		int iDataSize;
		PACKET_HEAD *head;
		if(isSend == true)
		{
			RECV_BACK:
			usleep(100000);
			int iRecvLen = 0;
			while (1) {
				iRecvLen = recv(sockfd, szRecvBuf, sizeof(szRecvBuf), 0);

				if (iRecvLen == 0)
				{
					puts("ERROR  : 서버로 부터 연결이 끊어졌습니다.");
					puts("SYSTEM : Client를 종료합니다.");
					close(sockfd);
					exit(1);
					continue;
				}
				else if (iRecvLen < 0)
				{
					if ( errno == EAGAIN)
						break;
				}
				else
				{
					m_pCirBuf->Push( szRecvBuf, iRecvLen );

				}
			}

			head = (PACKET_HEAD*)m_pCirBuf->GetPacket(sizeof(PACKET_HEAD));
			PACKET_TAIL* tail = (PACKET_TAIL*)(head + head->m_iPacketSize - sizeof(PACKET_TAIL));

			if( m_pCirBuf->isEmpty() )
			{
				goto RECV_BACK;
			}

			iDataSize = m_pCirBuf->GetDataSize();
			if( (int)sizeof( PACKET_HEAD ) >= iDataSize )
			{
				goto RECV_BACK;
			}

			if( strncmp(head->m_szHead, gpHead, sizeof(gpHead)) != 0 )
			{
				m_pCirBuf->Pop( iDataSize );
				goto RECV_BACK;
			}


			switch( head->m_nCMD )
			{
			case CMD_USER_LOGIN_RESULT:
			{
				PACKET_SC_LOGIN* packet = (PACKET_SC_LOGIN*)(head);
				cout << "Login Complete!" << endl << endl;
				isSend = false;

			}
			break;
			case CMD_USER_DATA_RESULT:
			{
				PACKET_SC_ECHO* packet = (PACKET_SC_ECHO*)(head);
				cout << "echo  : " << packet->m_szData << endl<< endl;
				isSend = false;

			}
			break;
			case CMD_USER_SAVE_RESULT:
			{
				PACKET_SC_SAVE* packet = (PACKET_SC_SAVE*)(head);

				if( true == packet->m_isComplete )
					cout << "Save Compelete" << endl ;
				else
					cout << "Save Error" << endl ;
				cout << endl;
				isSend = false;

			}
			break;
			case CMD_USER_DELETE_RESULT:
			{
				PACKET_SC_DEL* packet = (PACKET_SC_DEL*)(head);

				if( true == packet->m_isComplete )
					cout << "Delete Compelete" << endl;
				else
					cout << "Delete Error" << endl ;
				cout << endl;
				isSend = false;


			}
			break;
			case CMD_USER_PRINT_RESULT:
			{
				PACKET_SC_PRINT* packet = (PACKET_SC_PRINT*)(head);
				if( true == packet->m_isComplete )
				{
					int iCnt = 0;
					int iStringCnt = 0;
					iTotalRecvSize += sizeof(packet->m_szData);
					while (1) {
						if (packet->m_szData[iCnt] == '\0') {
							++iStringCnt;
							++iCntCheck;
							cout << endl;

						} else
						{
							if(iCntCheck == iRepositCnt)
							{
								cout << "[ " << iRepositCnt + 1 << " ] ";
								++iRepositCnt;
							}
							cout << packet->m_szData[iCnt];
						}

						if (iStringCnt >= packet->m_iPrintCnt)
						{
							break;
						}
						++iCnt;
					}

					iTotalRecvSize = 0;
					iCntCheck = 0;
					iRepositCnt = 0;
					cout << endl;
					cout << "<< Print Finish! >>" << endl;
					cout << endl;
					isSend = false;
				}
				else
				{
					int iCnt = 0;
					int iStringCnt = 0;

					iTotalRecvSize += sizeof(packet->m_szData);
					while(1)
					{
						if( packet->m_szData[iCnt] == '\0')
						{
							++iStringCnt;
							++iCntCheck;
							cout << endl;
						}
						else
						{
							if (iCntCheck == iRepositCnt) {
								cout << "[ " << iRepositCnt + 1 << " ] ";
								++iRepositCnt;
							}
							cout << packet->m_szData[iCnt];
						}

						if(iStringCnt >= packet->m_iPrintCnt)
							break;
						++iCnt;
					}


					m_pCirBuf->Pop( head->m_iPacketSize );
					goto RE_PRINT;
				}

			}
			break;
			case CMD_USER_ERR:
			{
				PACKET_SC_LOGINERR* packet = (PACKET_SC_LOGINERR*)(head);
				if( false == packet->m_isComplete )
				{
					cout << "SYSTEM : 서버 인원이 꽉차서 접속할 수 없습니다." << endl;
					close(sockfd);
					cout << "SYSTEM : Client를 종료합니다." << endl;
					usleep(10000);
					exit(1);
				}
			}
			break;
			}

			m_pCirBuf->Pop( head->m_iPacketSize );

		}

		//system("clear");
		if(isSend == false )
		{
			if(isAuto == true)
				usleep(100000);

			randSize = rand() % 7 + 3;
			RETURN:

			cout << "--------------" << endl;
			cout << "     Menu     " << endl;
			cout << "1. Echo Data  " << endl;
			cout << "2. Save Data  " << endl;
			cout << "3. Delete Data" << endl;
			cout << "4. Print      " << endl;
			cout << "5. Quit       " << endl;
			cout << "--------------" << endl;
			cout << "select : ";

			if(true == isAuto)
			{
				iMenu = rand() % 4 + 1;
			}
			else
				cin >> iMenu;

			cout << endl;
			if( iMenu > 5 || iMenu < 1)
			{
				cin.clear();
				cin.ignore(256,'\n');
				cout << "Please Enter Again!!" << endl;
				goto RETURN;
			}

			switch(iMenu)
			{
			case 1:
			{
				cout << "Echo input : ";

				if(true == isAuto)
				{
					for(int i = 0; i < randSize; ++i)
					{
						sInput += rand() % 25 + 65;
					}
					cout << sInput << endl;
				}
				else
				{
					cin.ignore(256, '\n');
					getline(cin, sInput);
				}

				PACKET_CS_ECHO packet;

				strncpy(packet.m_szData, sInput.c_str(), sizeof(packet.m_szData));
				packet.m_iSize = sInput.length();
				packet.m_szData[packet.m_iSize] = '\0';

				send(sockfd, (char*)&packet, sizeof(PACKET_CS_ECHO), 0);
				isSend = true;

			}
				break;
			case 2:
			{
				cout << "Save input : ";
				if (true == isAuto) {
					for (int i = 0; i < randSize; ++i) {
						sInput += rand() % 25 + 65;
					}
					cout << sInput << endl;
				} else {
					cin.ignore(256, '\n');
					getline(cin, sInput);
				}
				cout << endl;

				PACKET_CS_SAVE packet;

				strncpy(packet.m_szData, sInput.c_str(), sizeof(packet.m_szData));
				packet.m_iSize = sInput.length();
				packet.m_szData[packet.m_iSize] = '\0';

				send(sockfd, (char*) &packet, sizeof(PACKET_CS_SAVE), 0);
				isSend = true;
			}
				break;
			case 3:
			{
				cout << "Delete input : ";
				if (true == isAuto) {
					for (int i = 0; i < randSize; ++i) {
						sInput += rand() % 25 + 65;
					}
					cout << sInput << endl;
				} else {
					cin.ignore(256, '\n');
					getline(cin, sInput);
				}
				cout << endl;
				PACKET_CS_DEL packet;

				strncpy(packet.m_szData, sInput.c_str(), sizeof(packet.m_szData));
				packet.m_iSize = sInput.length();
				packet.m_szData[packet.m_iSize] = '\0';

				send(sockfd, (char*) &packet, sizeof(PACKET_CS_DEL), 0);
				isSend = true;
			}
				break;
			case 4:
			{
				RE_PRINT:
				PACKET_CS_PRINT packet;

				send(sockfd, (char*) &packet, sizeof(PACKET_CS_PRINT), 0);
				isSend = true;
			}
				break;
			case 5:
				close(sockfd);
				exit(1);
				break;
			}

		}


	}
	close(sockfd);
}

static void set_sockaddr(struct sockaddr_in *addr)
{
	bzero((char*)addr, sizeof(struct sockaddr_in));
	addr->sin_family	= AF_INET;
	addr->sin_addr.s_addr = inet_addr(SERVER_IP);
	addr->sin_port = htons(DEFAULT_PORT);
}

int Set_NonBlocking(int iSockfd)
{
	if(fcntl(iSockfd, F_SETFL, fcntl(iSockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
		return -1;
	return 0;
}

