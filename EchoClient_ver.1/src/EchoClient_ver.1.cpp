//============================================================================
// Name        : 1.cpp
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
#define DEFAULT_PORT	9190
#define MAX_CONN		16
#define MAX_EVENTS	32
#define BUF_SIZE		16
#define MAX_LINE		256
//#define SERVER_IP		"192.168.8.37"
#define SERVER_IP		"127.0.0.1"

void client_run();

int main()
{
	client_run();

	return 0;
}

static void set_sockaddr(struct sockaddr_in *addr)
{
	bzero((char*)addr, sizeof(struct sockaddr_in));
	addr->sin_family	= AF_INET;
	addr->sin_addr.s_addr = inet_addr(SERVER_IP);//INADDR_ANY;inet_addr(SERVER_IP);
	addr->sin_port = htons(DEFAULT_PORT);
}


void client_run()
{
	int iRecv_Len = 0;
	int iCnt = 0;;
	char input[MAX_LINE];
	int iBuf_Len = 0;
	int sockfd = 0;
	int iData_Len = 0;
	char buf[MAX_LINE];
	char recv_buf[256];
	struct sockaddr_in srv_addr;

	unsigned short nMemoCmd = 0;

	PACKET Packet;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	set_sockaddr(&srv_addr);

	if(connect(sockfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) < 0)
	{
		perror("connect()");
		exit(1);
	}

	cout << "ID : ";
	fgets(Packet.m_szID, sizeof(Packet.m_szID), stdin);

	iData_Len = strlen(Packet.m_szID) - 1;
	Packet.m_szID[iData_Len] = '\0';

	Packet.m_nCMD ^= Packet.m_nCMD;
	Packet.m_nCMD |= CMD_USER_LOGIN_REQ;

	send(sockfd, (char*)&Packet, sizeof(Packet), 0);

	while(errno != EAGAIN && (iRecv_Len = recv(sockfd, (char*)&Packet, sizeof(Packet), 0)) > 0)
	{
		iBuf_Len -= iRecv_Len;
		if(iBuf_Len <= 0)
		{
			break;
		}

	}

	bool isSend = false;
	int cnt = 0;

	while(1)
	{
		int iMenu = 0;
		//char iInput[256];
		string iInput;
		//system("clear");
		memset(&Packet.m_szData, 0, sizeof(Packet.m_szData));

		if(isSend == false )
		{
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
			cin >> iMenu;
			isSend = true;

			if( iMenu > 5 || iMenu < 1)
			{
				cin.clear();
				cin.ignore(256,'\n');
				cout << "Please Enter Again!!" << endl;
				goto RETURN;
			}

		}
		else
		{
			while(errno != EAGAIN && (iRecv_Len = recv(sockfd, (char*)&Packet, sizeof(Packet), 0)) > 0)
			{
				iBuf_Len -= iRecv_Len;
				if(iBuf_Len <= 0)
				{

					break;
				}

			}
		}

		switch(iMenu)
		{
		case 1:
			cout << "Echo input : ";
			cin.ignore(256, '\n');
			getline(cin, iInput);

			memcpy(Packet.m_szData, iInput.c_str(), iInput.length());

			iBuf_Len = sizeof(Packet);

			Packet.m_nCMD ^= Packet.m_nCMD;
			Packet.m_nCMD |= CMD_USER_DATA_REQ;
			nMemoCmd = CMD_USER_DATA_REQ;

			send(sockfd, (char*) &Packet, sizeof(Packet), 0);

			break;
		case 2:
			cout << "Save input : ";
			cin.ignore(256, '\n');
			getline(cin, iInput);
			memcpy(Packet.m_szData, iInput.c_str(), iInput.length());

			cout << "packet data test : " << Packet.m_szData << endl;

			iBuf_Len = sizeof(Packet);

			Packet.m_nCMD ^= Packet.m_nCMD;
			Packet.m_nCMD |= CMD_USER_SAVE_REQ;
			nMemoCmd = CMD_USER_SAVE_REQ;

			send(sockfd, (char*) &Packet, sizeof(Packet), 0);

			break;
		case 3:
			cout << "Delete input : ";
			cin.ignore(256, '\n');
			getline(cin, iInput);
			memcpy(Packet.m_szData, iInput.c_str(), iInput.length());
			iBuf_Len = sizeof(Packet);

			Packet.m_nCMD ^= Packet.m_nCMD;
			Packet.m_nCMD |= CMD_USER_DELETE_REQ;
			nMemoCmd = CMD_USER_DELETE_REQ;

			send(sockfd, (char*) &Packet, sizeof(Packet), 0);
			break;
		case 4:
			iBuf_Len = sizeof(Packet);
			Packet.m_nCMD ^= Packet.m_nCMD;
			Packet.m_nCMD |= CMD_USER_PRINT_REQ;
			nMemoCmd = CMD_USER_PRINT_REQ;

			send(sockfd, (char*) &Packet, sizeof(Packet), 0);

			break;
		case 5:
			close(sockfd);
			exit(1);
			break;
		}


		//( (Packet.m_nCMD == CMD_USER_LOGIN_RESULT) ||
		if(isSend == true)
		{
			if( (Packet.m_nCMD == CMD_USER_DATA_RESULT) )
			{

				cout << "[" << Packet.m_nSock_ID << "]" << "echo  : " << Packet.m_szData << endl;
				isSend = false;
			}
			else if((Packet.m_nCMD == CMD_USER_SAVE_RESULT))
			{
				cout << "Save Compelete" << endl;
				isSend = false;
			}
			else if((Packet.m_nCMD == CMD_USER_DELETE_RESULT))
			{
				cout << "Delete Compelete" << endl;
				isSend = false;
			}
			else if( (Packet.m_nCMD == CMD_USER_PRINT_RESULT))
			{
				if(strcmp(Packet.m_szData, "END") == 0)
					isSend = false;
				else
					cout << "[" << Packet.m_nSock_ID << "]" << "struct  : " << Packet.m_szData << endl;

			}
			else if( (Packet.m_nCMD == CMD_USER_ERR))
			{
				if(nMemoCmd == CMD_USER_DATA_REQ)
				{
					cout << "Echo Error" << endl;
				}
				else if(nMemoCmd == CMD_USER_SAVE_REQ)
				{
					cout << "Save Error" << endl;
				}
				else if(nMemoCmd == CMD_USER_DELETE_REQ)
				{
					cout << "Delete Error" << endl;
				}
				else if(nMemoCmd == CMD_USER_PRINT_REQ)
				{
					cout << "Print Error" << endl;
				}
				nMemoCmd = 0;
				isSend = false;

			}
		}

	}
	close(sockfd);
}
