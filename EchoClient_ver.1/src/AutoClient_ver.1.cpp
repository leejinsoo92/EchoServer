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

void client_run();
bool PacketCheck(PACKET data);
static void set_sockaddr(struct sockaddr_in *addr);
int RecvFromServer(int sockfd, char* buf, int buf_size);
bool PacketCheck(PACKET data);

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
	unsigned short nMemoCmd = 0;

	int randSize = 0;

	PACKET Packet;

	srand(time(NULL));

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

	//Packet.m_nCMD ^= Packet.m_nCMD;
	Packet.m_nCMD |= CMD_USER_LOGIN_REQ;
	strcpy(Packet.m_szData, loginTemp.c_str());

	Packet.m_iSize = loginTemp.length();
	//cout << "data size : " << Packet.m_iSize << endl;

	send(sockfd, (char*)&Packet, sizeof(Packet), 0);
	cout << "success send about login data!" << endl;
	//memset(&Packet.m_szData, 0, sizeof(MAX_PACKET_SIZE));

//	if((numbytes = RecvFromServer(sockfd, (char*)&Packet, sizeof(PACKET))) == 0)
//	{
//		cout << "[Client] Server Terminated." << endl;
//		return;
//	}

	bool isSend = false;

	while(1)
	{
		int iMenu = 0;
		string sInput;

		//usleep(1000000);
		memset(&Packet.m_szData, 0, sizeof(MAX_PACKET_SIZE));
		//RecvFromServer(sockfd, (char*) &Packet, sizeof(PACKET));
//		if ((numbytes = RecvFromServer(sockfd, (char*) &Packet, sizeof(PACKET)))
//				== 0) {
//			cout << "[Client] Server Terminated." << endl;
//			return;
//		}
		recv(sockfd, (char*)&Packet, sizeof(PACKET), 0);

		//if( true == PacketCheck(Packet))
		//{
			if ((Packet.m_nCMD == CMD_USER_LOGIN_RESULT)) {
				cout << "Login Complete!" << endl;
			}
			else if ((Packet.m_nCMD == CMD_USER_DATA_RESULT)) {
				cout << "[" << Packet.m_nSock_ID << "]" << "echo  : " << Packet.m_szData << endl;
			}
			else if ((Packet.m_nCMD == CMD_USER_SAVE_RESULT)) {
				cout << "Save Compelete" << endl;
				//isSend = false;
			}
			else if ((Packet.m_nCMD == CMD_USER_DELETE_RESULT)) {
				cout << "Delete Compelete" << endl;
				//isSend = false;
			}
			else if ((Packet.m_nCMD == CMD_USER_PRINT_RESULT)) {
				if (strcmp(Packet.m_szData, "END") == 0)
					isSend = false;
				else
					cout << "[" << Packet.m_nSock_ID << "]" << "	 struct  : " << Packet.m_szData << endl;

			}
			else if ((Packet.m_nCMD == CMD_USER_ERR)) {
				if (nMemoCmd == CMD_USER_DATA_REQ) {
					cout << "Echo Error" << endl;
				}
				else if (nMemoCmd == CMD_USER_SAVE_REQ) {
					cout << "Save Error" << endl;
				}
				else if (nMemoCmd == CMD_USER_DELETE_REQ) {
					cout << "Delete Error" << endl;
				}
				else if (nMemoCmd == CMD_USER_PRINT_REQ) {
					cout << "Print Error" << endl;
					isSend = false;
				}
				nMemoCmd = 0;
				//isSend = false;
			}
			//isSend = false;
		//}


		usleep(1000000);

		//if(isSend == false )
		//{
			memset(&Packet.m_szData, 0, sizeof(MAX_PACKET_SIZE));
			randSize = rand() % 5 + 3;
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

			iMenu = 1;//rand() % 4 + 1;
			cout << iMenu << endl;

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
				cout << "Echo input : ";
				for(int i = 0; i < randSize; ++i)
				{
					sInput += rand() % 25 + 65;
				}
				//sInput = rand() % 25 + 65;
				cout << sInput << endl;

				Packet.m_nCMD ^= Packet.m_nCMD;
				Packet.m_nCMD |= CMD_USER_DATA_REQ;
				nMemoCmd = CMD_USER_DATA_REQ;

				strcpy(Packet.m_szData, sInput.c_str());
				Packet.m_iSize = sInput.length();
				Packet.m_szData[Packet.m_iSize] = '\0';

				send(sockfd, (char*) &Packet, sizeof(PACKET), 0);

				break;
			case 2:
				cout << "Save input : ";
				for(int i = 0; i < randSize; ++i)
				{
					sInput += rand() % 25 + 65;
				}
				//sInput = rand() % 25 + 65;
				cout << sInput << endl;

				Packet.m_nCMD ^= Packet.m_nCMD;
				Packet.m_nCMD |= CMD_USER_SAVE_REQ;
				nMemoCmd = CMD_USER_SAVE_REQ;
				strcpy(Packet.m_szData, sInput.c_str());
				Packet.m_iSize = sInput.length();
				//cout << "packet data size : " << Packet.m_iSize << endl;

				send(sockfd, (char*) &Packet, sizeof(PACKET), 0);

				break;
			case 3:
				cout << "Delete input : ";
				for (int i = 0; i < randSize; ++i) {
					sInput += rand() % 25 + 65;
				}
				//sInput = rand() % 25 + 65;
				cout << sInput << endl;

				Packet.m_nCMD ^= Packet.m_nCMD;
				Packet.m_nCMD |= CMD_USER_DELETE_REQ;
				nMemoCmd = CMD_USER_DELETE_REQ;
				strcpy(Packet.m_szData, sInput.c_str());
				Packet.m_iSize = sInput.length();
				//cout << "packet data size : " << Packet.m_iSize << endl;

				send(sockfd, (char*) &Packet, sizeof(PACKET), 0);
				break;
			case 4:

				Packet.m_nCMD ^= Packet.m_nCMD;
				Packet.m_nCMD |= CMD_USER_PRINT_REQ;
				nMemoCmd = CMD_USER_PRINT_REQ;

				strcpy(Packet.m_szData, printTemp.c_str());
				Packet.m_iSize = printTemp.length();
				//cout << "packet data size : " << Packet.m_iSize << endl;
				cout << "Ask Struct Data to Client~" << endl;
				send(sockfd, (char*) &Packet, sizeof(PACKET), 0);
				isSend = true;
				break;
			case 5:
				close(sockfd);
				exit(1);
				break;
			}

			//isSend = true;

		//}


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

//int RecvFromServer(int sockfd, char* buf, int buf_size)
//{
//	int n, offset = 0;
//	errno = 0;
//
//	//MSG_DONTWAIT
//	while(buf_size - offset > 0 && (n = recv(sockfd, buf + offset , buf_size - offset, 0)) > 0)
//	{
//		offset += n;
//	}
//
//	//&& errno == EAGAIN)
//	if( offset == 0 && errno == EAGAIN )
//	{
//		//cout << "[Client] no message recived." << endl;
//		return -1;
//	}
//	else
//		return offset;
//}

//int RecvFromServer(int sockfd, char* buf, int buf_size)
//{
//	int iRecvLen = 0;
//	while (1) {
//		iRecvLen = recv(sockfd, buf, buf_size, 0);
//
//		if (iRecvLen == 0)
//			return -1;
//		else if (iRecvLen < 0) {
//			if ( errno == EAGAIN)
//				break;
//		}
//	}
//	return 0;
//}

bool PacketCheck(PACKET data)
{
	if( strcmp( data.m_szHead, "AA11" ) != 0 )
		return false;
	if( strcmp( data.m_szTail, "11AA" ) != 0 )
		return false;
	if( data.m_nCMD <= 0 || data.m_nCMD >= 11)
		return false;
	if( data.m_iSize <= 0 )
		return false;

	return true;
}

