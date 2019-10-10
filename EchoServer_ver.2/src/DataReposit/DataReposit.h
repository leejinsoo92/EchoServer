/*
 * DataReposit.h
 *
 *  Created on: 2019. 10. 2.
 *      Author: leejinsoo
 */

#ifndef DATAREPOSIT_DATAREPOSIT_H_
#define DATAREPOSIT_DATAREPOSIT_H_

#include <iostream>
#include <list>
#include <vector>

#define MAX_PACKET_SIZE 10240

using namespace std;

class CDataReposit {
private:
	CDataReposit();
	virtual ~CDataReposit();

	static CDataReposit* instance;

public:
	static CDataReposit* getInstance();
	static void DeleteInstance();

public:
	bool isEmpty();
	bool SaveData(char* _data);
	bool DeleteData(char* _data);
	char* PrintSendData(int num);
	char* PrintSend();
	void PrintData();

	bool Get_isEnd(){
		return m_isEnd;
	}
	void Set_isEnd(bool _isEnd){
		m_isEnd = _isEnd;
	}

	int Get_PrintCnt(){
		return m_iPrintCnt;
	}

	void Set_PrintCnt(int _cnt){
		m_iPrintCnt = _cnt;
	}

	int Get_CurrentCnt(){
		return m_iCurrentCnt;
	}

private:
	int test = 0;
	char m_szPrintBuf[MAX_PACKET_SIZE]; // 리턴하는 버퍼
	char m_szTempBuf[MAX_PACKET_SIZE]; // 임시로 저장해두는 버퍼
	int m_iPrintCnt = 0;
	int m_iCurrentCnt = 0;
	int m_iRear = 0;
	bool m_isEnd = false;
	vector<string> m_listData;

};

#endif /* DATAREPOSIT_DATAREPOSIT_H_ */
