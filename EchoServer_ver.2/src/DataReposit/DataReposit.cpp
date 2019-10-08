/*
 * DataReposit.cpp
 *
 *  Created on: 2019. 10. 2.
 *      Author: leejinsoo
 */

#include "DataReposit.h"
#include <string.h>
#include <mutex>

pthread_mutex_t listlock;

CDataReposit::CDataReposit() {
	// TODO Auto-generated constructor stub

}

CDataReposit::~CDataReposit() {
	// TODO Auto-generated destructor stub
	DeleteInstance();
	m_listData.clear();
}

CDataReposit* CDataReposit::instance = nullptr;
CDataReposit* CDataReposit::getInstance()
{
	if( nullptr == instance )
	{
		instance = new CDataReposit();
		pthread_mutex_init(&listlock, NULL);
	}
	return instance;
}

void CDataReposit::DeleteInstance()
{
	delete instance;
	instance = nullptr;

}

bool CDataReposit::isEmpty()
{
	return m_listData.empty();
}

bool CDataReposit::SaveData(char* _data)
{
	pthread_mutex_lock(&listlock);
	if(true == m_listData.empty())
	{
		m_listData.push_back(_data);
		pthread_mutex_unlock(&listlock);
		return true;
	}
	for(vector<string>::iterator iter = m_listData.begin(); iter != m_listData.end(); ++iter)
	{
		if( iter->compare(_data) == 0)
		{
			pthread_mutex_unlock(&listlock);
			return false;
		}
	}

	m_listData.push_back(_data);
	pthread_mutex_unlock(&listlock);
	return true;
}

bool CDataReposit::DeleteData(char* _data)
{
	pthread_mutex_lock(&listlock);
	if(true == m_listData.empty())
	{
		pthread_mutex_unlock(&listlock);
		return false;
	}
	for(vector<string>::iterator iter = m_listData.begin(); iter != m_listData.end(); )
	{
		if( iter->compare(_data) == 0)
		{
			iter = m_listData.erase(iter);
			pthread_mutex_unlock(&listlock);
			return true;
		}
		else
			++iter;
	}

	pthread_mutex_unlock(&listlock);
	return false;
}

char* CDataReposit::PrintSendData(int num)
{
	vector<string>::iterator iter_begin = m_listData.begin();
	vector<string>::iterator iter_end = m_listData.end();
	vector<string>::iterator iter = iter_begin + num;

	if( iter != iter_end )
	{
		char* szRedata = new char[ (*iter).size() + 1];
		std::copy((*iter).begin(), (*iter).end(), szRedata);
		szRedata[(*iter).size()] = '\0';
		return szRedata;
	}
	return nullptr;
}

char* CDataReposit::PrintSend()
{
	vector<string>::iterator iter_begin = m_listData.begin();
	vector<string>::iterator iter_end = m_listData.end();
	vector<string>::iterator iter = iter_begin + m_iPrintCnt;

	while(1)
	{
		if( iter != iter_end )
		{
			if(MAX_PACKET_SIZE < m_iRear + (*iter).size() + 1)
			{
				m_iRear = 0;
				m_isEnd = false;
				break;
			}

//			if( iter + 1 == iter_end )
//			{
//				m_iRear = 0;
//				m_isEnd = true;
//				break;
//			}

			std::copy((*iter).begin(), (*iter).end(), m_szTempBuf + m_iRear);
			m_szTempBuf[m_iRear + (*iter).size()] = '\0';
			++m_iPrintCnt;
			m_iRear += (*iter).size() + 1;
		}
		else
		{
			m_iRear = 0;
			m_isEnd = true;
			break;
		}
		++iter;
	}

	memset(m_szPrintBuf, 0, sizeof(MAX_PACKET_SIZE)); // 보내는 버퍼 초기화
	memcpy(m_szPrintBuf, m_szTempBuf, sizeof(m_szTempBuf)); // 임시 버퍼 -> 보내는 버퍼
	cout << "temp size : " << sizeof(m_szTempBuf) << endl;
	memset(m_szTempBuf, 0, sizeof(MAX_PACKET_SIZE)); // 임시 버퍼 초기화
	cout << "printbuf size : " << sizeof(m_szPrintBuf) << endl;
	return m_szPrintBuf;

	return nullptr;
}

void CDataReposit::PrintData()
{
	cout << endl;
	cout << "[DataReposit] Data list " << endl;
	int iNum = 0;
	for(vector<string>::iterator iter = m_listData.begin(); iter != m_listData.end(); ++iter)
	{
		cout << "[ " << iNum++ << " ] " << *iter << endl;
	}
	cout << endl;
}




