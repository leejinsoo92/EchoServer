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
		pthread_mutex_init(&listlock, NULL);
		instance = new CDataReposit();
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

void CDataReposit::SaveData(char* _data)
{
	pthread_mutex_lock(&listlock);
	m_listData.push_back(_data);
	pthread_mutex_unlock(&listlock);
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
	pthread_mutex_lock(&listlock);
	vector<string>::iterator iter_begin = m_listData.begin();
	vector<string>::iterator iter_end = m_listData.end();
	vector<string>::iterator iter = iter_begin + num;

	if( iter != iter_end )
	{
		char* szRedata = new char[ (*iter).size() + 1];
		std::copy((*iter).begin(), (*iter).end(), szRedata);
		szRedata[(*iter).size()] = '\0';
		pthread_mutex_unlock(&listlock);
		return szRedata;
	}
	pthread_mutex_unlock(&listlock);
	return nullptr;
}

void CDataReposit::PrintData()
{
	cout << endl;
	cout << "[DataReposit] Data list " << endl;
	int iNum = 0;
	pthread_mutex_lock(&listlock);
	for(vector<string>::iterator iter = m_listData.begin(); iter != m_listData.end(); ++iter)
	{
		cout << "[ " << iNum++ << " ] " << *iter << endl;
	}
	pthread_mutex_unlock(&listlock);
	cout << endl;
}




