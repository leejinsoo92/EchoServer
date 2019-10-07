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

#define MAX_PACKET_SIZE 8096

const char gpHead[5] = {'A', 'A', '1', '1'};
const char gpTail[5] = {'1', '1', 'A', 'A'};


#pragma pack(push, 1)
struct PACKET_HEAD
{
	int m_iPacketSize;
	char m_szHead[5] = {'A', 'A', '1', '1'};
	unsigned short	m_nCMD;
};

struct PACKET_TAIL
{
	char m_szTail[5] = {'1', '1', 'A', 'A'};
};

struct PACKET_CS_LOGIN
{
	PACKET_HEAD		m_Head;

	char				m_szID[16];

	PACKET_TAIL		m_Tail;

	PACKET_CS_LOGIN()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_CS_LOGIN);
		m_Head.m_nCMD = CMD_USER_LOGIN_REQ;
	}
};

struct PACKET_SC_LOGIN
{
	PACKET_HEAD		m_Head;

	unsigned short 	m_nSock_ID = 0;

	PACKET_TAIL		m_Tail;

	PACKET_SC_LOGIN()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_SC_LOGIN);
		m_Head.m_nCMD = CMD_USER_LOGIN_RESULT;
	}
};

struct PACKET_CS_ECHO
{
	PACKET_HEAD		m_Head;

	char				m_szData[MAX_PACKET_SIZE];
	int 				m_iSize = 0;

	PACKET_TAIL		m_Tail;

	PACKET_CS_ECHO()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_CS_ECHO);
		m_Head.m_nCMD = CMD_USER_DATA_REQ;
	}
};

struct PACKET_SC_ECHO
{
	PACKET_HEAD		m_Head;

	char				m_szData[MAX_PACKET_SIZE];
	int 				m_iSize = 0;

	PACKET_TAIL		m_Tail;

	PACKET_SC_ECHO()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_SC_ECHO);
		m_Head.m_nCMD = CMD_USER_DATA_RESULT;
	}
};

struct PACKET_CS_SAVE
{
	PACKET_HEAD		m_Head;

	char				m_szData[MAX_PACKET_SIZE];
	int 				m_iSize = 0;

	PACKET_TAIL		m_Tail;

	PACKET_CS_SAVE()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_CS_SAVE);
		m_Head.m_nCMD = CMD_USER_SAVE_REQ;
	}
};

struct PACKET_SC_SAVE
{
	PACKET_HEAD		m_Head;

	bool				m_isComplete = false;

	PACKET_TAIL		m_Tail;

	PACKET_SC_SAVE()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_SC_SAVE);
		m_Head.m_nCMD = CMD_USER_SAVE_RESULT;
	}
};

struct PACKET_CS_DEL
{
	PACKET_HEAD		m_Head;

	char				m_szData[MAX_PACKET_SIZE];
	int 				m_iSize = 0;

	PACKET_TAIL		m_Tail;

	PACKET_CS_DEL()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_CS_DEL);
		m_Head.m_nCMD = CMD_USER_DELETE_REQ;
	}
};

struct PACKET_SC_DEL
{
	PACKET_HEAD		m_Head;

	bool				m_isComplete = false;

	PACKET_TAIL		m_Tail;

	PACKET_SC_DEL()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_SC_DEL);
		m_Head.m_nCMD = CMD_USER_DELETE_RESULT;
	}
};

struct PACKET_CS_PRINT
{
	PACKET_HEAD		m_Head;

	PACKET_TAIL		m_Tail;

	PACKET_CS_PRINT()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_CS_PRINT);
		m_Head.m_nCMD = CMD_USER_PRINT_REQ;
	}
};

struct PACKET_SC_PRINT
{
	PACKET_HEAD		m_Head;

	char				m_szData[MAX_PACKET_SIZE];
	bool				m_isComplete = false;

	PACKET_TAIL		m_Tail;

	PACKET_SC_PRINT()
	{
		m_Head.m_iPacketSize = sizeof(PACKET_SC_PRINT);
		m_Head.m_nCMD = CMD_USER_PRINT_RESULT;
	}
};

//struct PACKET
//{
//	PACKET_HEAD		m_Head;
//
//	unsigned short 	m_nSock_ID = 0;
//	char				m_szID[16];
//	char				m_szData[MAX_PACKET_SIZE];
//	int 				m_iSize = 0;
//
//	PACKET_TAIL		m_Tail;
//
//	PACKET()
//	{
//		m_Head.m_iPacketSize = sizeof(PACKET);
//	}
//};
#pragma pack(pop)

