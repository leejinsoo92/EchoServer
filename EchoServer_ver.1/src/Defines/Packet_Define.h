#define CMD_USER_LOGIN_REQ		0x01
#define CMD_USER_LOGIN_RESULT	0x02
#define CMD_USER_DATA_REQ		0x03
#define CMD_USER_DATA_RESULT	0x04
#define CMD_USER_SAVE_REQ		0x05
#define CMD_USER_SAVE_RESULT	0x06
#define CMD_USER_DELETE_REQ		0x07
#define CMD_USER_DELETE_RESULT	0x08
#define CMD_USER_PRINT_REQ		0x09
#define CMD_USER_PRINT_RESULT	0x0A
#define CMD_USER_ERR				0x0B

#define MAX_PACKET_SIZE 256

#pragma pack(push, 1)
typedef struct Packet_Head_Info
{
	char m_szHead[5] = {'A', 'A', '1', '1'};
	int m_iPacketSize;
}PACKET_HEAD;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct Packet_Tail_Info
{
	char m_szTail[5] = {'1', '1', 'A', 'A'};
}PACKET_TAIL;
#pragma pack(pop)

#pragma pack(push, 1) // 1바이트 크기로 정렬
typedef struct Milestone_Packet
{
	unsigned short 	m_nSock_ID = 0;
	char				m_szID[16];
	unsigned short	m_nCMD = 0;
	char				m_szData[256];
	int 				m_iSize = 0;
}PACKET;
#pragma pack(pop) // 정렬 설정을 이전 상태(기본값)로 되돌림

template<class A, class B, class C>
void AssemblePacket(char* buf, A& head, B& body, C& tail)
{
	memcpy(buf, &head, sizeof(A));
	memcpy(buf + sizeof(A), &body, sizeof(B));
	memcpy(buf + sizeof(A) + sizeof(B), &tail, sizeof(C));
}

template<class A, class B, class C>
void DisAssemblePacket(char* buf, A& head, B& body, C& tail)
{
	memcpy(&head, buf, sizeof(A));
	memcpy(&body, buf + sizeof(A), sizeof(B));
	memcpy(&tail, buf + sizeof(A) + sizeof(B), sizeof(C));
}
