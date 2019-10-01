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
#include "Packet_Define.h"

#include <iostream>
using namespace std;

//8080
//9190
#define DEFAULT_PORT	8080
#define MAX_CONN		16
#define MAX_EVENTS	32
#define BUF_SIZE		16
#define MAX_LINE		256
#define SERVER_IP		"192.168.8.39"
//#define SERVER_IP		"127.0.0.1"

struct CIR_BUF
{
	char szTempBuf[1024 * 32];
	int iFront = 0;
	int iRear = 0;
	int iMaxSize = 0;

	CIR_BUF()
	{
		iMaxSize = 1024 * 32;
	}
};

void client_run();
static void set_sockaddr(struct sockaddr_in *addr);
int Set_NonBlocking(int iSockfd);
int RecvFromServer(int sockfd, char* buf, int buf_size, CIR_BUF* pCirBuf);

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
	int numbytes = 0;

	string loginTemp = "login request";
	string printTemp = "print request";
	string sID;
	unsigned short nMemoCmd = 0;

	int randSize = 0;

	char szRecvBuf[MAX_PACKET_SIZE] = { } ;
	CIR_BUF pCirBuf;

	srand(time(NULL));

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	set_sockaddr(&srv_addr);

	if(connect(sockfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) < 0)
	{
		perror("connect()");
		exit(1);
	}

	if(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
			cout << "error" << endl;


	PACKET_CS_LOGIN Login_Packet;
	cout << "ID : ";
	fgets(Login_Packet.m_szID, sizeof(Login_Packet.m_szID), stdin);
	iData_Len = strlen(Login_Packet.m_szID) - 1;

//	sID = "TEST";
//	strcpy(Login_Packet.m_szID, sID.c_str());
	Login_Packet.m_szID[iData_Len] = '\0';

	send(sockfd, (char*)&Login_Packet, sizeof(PACKET_CS_LOGIN), 0);

	bool isSend = true;

	while(1)
	{
		int iMenu = 0;
		string sInput;

		if(isSend == true)
		{
			int iRecvLen = 0;
			while (1) {
				iRecvLen = recv(sockfd, szRecvBuf, sizeof(szRecvBuf), 0);

				if (iRecvLen == 0)
					continue;
				else if (iRecvLen < 0) {
					if ( errno == EAGAIN)
						break;
				} else {
					if (pCirBuf.iRear + iRecvLen > pCirBuf.iMaxSize)
						pCirBuf.iRear = 0;

					memcpy(&pCirBuf.szTempBuf[pCirBuf.iRear], szRecvBuf,
							iRecvLen);
					pCirBuf.iRear += iRecvLen;
				}
			}

		/*if ((numbytes = RecvFromServer(sockfd, (char*)&szRecvBuf, sizeof(szRecvBuf), &pCirBuf)) == 0)
		{
			cout << "[Client] Server Terminated." << endl;
			return;
		}*/


		PACKET_HEAD* head = (PACKET_HEAD*)(&pCirBuf.szTempBuf[pCirBuf.iFront]);
		PACKET_TAIL* tail = (PACKET_TAIL*)(head + head->m_iPacketSize - sizeof(PACKET_TAIL));
		//PACKET* packet = (PACKET*)(head);

		//strncmp(tail->m_szTail, gpTail, sizeof(gpTail)) != 0
		if( strncmp(head->m_szHead, gpHead, sizeof(gpHead)) != 0
				)
		{
			if(pCirBuf.iFront + head->m_iPacketSize > pCirBuf.iMaxSize)
				pCirBuf.iFront = 0;
			else
				pCirBuf.iFront += head->m_iPacketSize;
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
			cout << "Save Compelete" << endl ;
		}
		break;
		case CMD_USER_DELETE_RESULT:
		{
			cout << "Delete Compelete" << endl;
		}
		break;
		case CMD_USER_PRINT_RESULT:
		{
//			if (strncmp(packet->m_szData, "END", sizeof(packet->m_szData)) == 0)
//				isSend = false;
//			else
//				cout << "[" << packet->m_nSock_ID << "]" << "	 struct  : " << packet->m_szData << endl;
		}
		break;
		case CMD_USER_ERR:
		{
			if (nMemoCmd == CMD_USER_DATA_REQ) {
				cout << "Echo Error" << endl;
			} else if (nMemoCmd == CMD_USER_SAVE_REQ) {
				cout << "Save Error" << endl;
			} else if (nMemoCmd == CMD_USER_DELETE_REQ) {
				cout << "Delete Error" << endl;
			} else if (nMemoCmd == CMD_USER_PRINT_REQ) {
				cout << "Print Error" << endl;
				isSend = false;
			}
			nMemoCmd = 0;
		}
		break;
		}

		if(pCirBuf.iFront + head->m_iPacketSize > pCirBuf.iMaxSize)
			pCirBuf.iFront = 0;
		else
			pCirBuf.iFront += head->m_iPacketSize;
		}
		usleep(100000);

		//system("clear");
		if(isSend == false )
		{
			//memset(&Packet.m_szData, 0, sizeof(MAX_PACKET_SIZE));
			randSize = rand() % 5 + 3;
//			RETURN:
//
//			cout << "--------------" << endl;
//			cout << "     Menu     " << endl;
//			cout << "1. Echo Data  " << endl;
//			cout << "2. Save Data  " << endl;
//			cout << "3. Delete Data" << endl;
//			cout << "4. Print      " << endl;
//			cout << "5. Quit       " << endl;
//			cout << "--------------" << endl;
//			cout << "select : ";
//
//			iMenu = 1;//rand() % 4 + 1;
//			cout << iMenu << endl;
//
//			if( iMenu > 5 || iMenu < 1)
//			{
//				cin.clear();
//				cin.ignore(256,'\n');
//				cout << "Please Enter Again!!" << endl;
//				goto RETURN;
//			}

			switch(1)
			{
			case 1:
			{
				cout << "Echo input : ";
				for(int i = 0; i < randSize; ++i)
				{
					sInput += rand() % 25 + 65;
				}
				//sInput = rand() % 25 + 65;
				cout << sInput << endl;

				PACKET_CS_ECHO packet;

				nMemoCmd = CMD_USER_DATA_REQ;

				strncpy(packet.m_szData, sInput.c_str(), sizeof(packet.m_szData));
				packet.m_iSize = sInput.length();
				packet.m_szData[packet.m_iSize] = '\0';

				send(sockfd, (char*)&packet, sizeof(PACKET_CS_ECHO), 0);
				isSend = true;

			}
				break;
//			case 2:
//				cout << "Save input : ";
//				for(int i = 0; i < randSize; ++i)
//				{
//					sInput += rand() % 25 + 65;
//				}
//				//sInput = rand() % 25 + 65;
//				cout << sInput << endl;
//
//				Packet.m_nCMD ^= Packet.m_nCMD;
//				Packet.m_nCMD |= CMD_USER_SAVE_REQ;
//				nMemoCmd = CMD_USER_SAVE_REQ;
//				strcpy(Packet.m_szData, sInput.c_str());
//				Packet.m_iSize = sInput.length();
//				//cout << "packet data size : " << Packet.m_iSize << endl;
//
//				send(sockfd, (char*) &Packet, sizeof(PACKET), 0);
//
//				break;
//			case 3:
//				cout << "Delete input : ";
//				for (int i = 0; i < randSize; ++i) {
//					sInput += rand() % 25 + 65;
//				}
//				//sInput = rand() % 25 + 65;
//				cout << sInput << endl;
//
//				Packet.m_nCMD ^= Packet.m_nCMD;
//				Packet.m_nCMD |= CMD_USER_DELETE_REQ;
//				nMemoCmd = CMD_USER_DELETE_REQ;
//				strcpy(Packet.m_szData, sInput.c_str());
//				Packet.m_iSize = sInput.length();
//				//cout << "packet data size : " << Packet.m_iSize << endl;
//
//				send(sockfd, (char*) &Packet, sizeof(PACKET), 0);
//				break;
//			case 4:
//
//				Packet.m_nCMD ^= Packet.m_nCMD;
//				Packet.m_nCMD |= CMD_USER_PRINT_REQ;
//				nMemoCmd = CMD_USER_PRINT_REQ;
//
//				strcpy(Packet.m_szData, printTemp.c_str());
//				Packet.m_iSize = printTemp.length();
//				//cout << "packet data size : " << Packet.m_iSize << endl;
//				cout << "Ask Struct Data to Client~" << endl;
//				send(sockfd, (char*) &Packet, sizeof(PACKET), 0);
//				isSend = true;
//				break;
//			case 5:
//				close(sockfd);
//				exit(1);
//				break;
			}

		}


	}
	close(sockfd);
}

static void set_sockaddr(struct sockaddr_in *addr)
{
	bzero((char*)addr, sizeof(struct sockaddr_in));
	addr->sin_family	= AF_INET;
	addr->sin_addr.s_addr = inet_addr(SERVER_IP);//INADDR_ANY;inet_addr(SERVER_IP);
	addr->sin_port = htons(DEFAULT_PORT);
}

int Set_NonBlocking(int iSockfd)
{
	if(fcntl(iSockfd, F_SETFL, fcntl(iSockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
		return -1;
	return 0;
}

int RecvFromServer(int sockfd, char* buf, int buf_size, CIR_BUF* pCirBuf)
{
	int iRecvLen = 0;
	char recvBuf[MAX_PACKET_SIZE];
	while (1) {
		iRecvLen = recv(sockfd, recvBuf, sizeof(recvBuf), 0);

		if (iRecvLen == 0)
			return -1;
		else if (iRecvLen <= 0)
		{
			if ( errno == EAGAIN)
				break;
		}
		else
		{
			if(pCirBuf->iRear + iRecvLen > pCirBuf->iMaxSize)
				pCirBuf->iRear = 0;

			memcpy( &pCirBuf->szTempBuf[pCirBuf->iRear], recvBuf, iRecvLen);
			pCirBuf->iRear += iRecvLen;
		}
	}
	return 0;
}

