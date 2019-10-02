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

void CDataReposit::DeleteData(char* _data)
{
	pthread_mutex_lock(&listlock);
	for(list<string>::iterator iter = m_listData.begin(); iter != m_listData.end(); )
	{
		if( iter->compare(_data) == 0)
		{
			iter = m_listData.erase(iter);
		}
		else
			++iter;
	}
	pthread_mutex_unlock(&listlock);
}

void CDataReposit::PrintData()
{
	cout << endl;
	cout << "[DataReposit] Save Data list " << endl;
	int iNum = 0;
	pthread_mutex_lock(&listlock);
	for(list<string>::iterator iter = m_listData.begin(); iter != m_listData.end(); ++iter)
	{
		cout << "[ " << iNum++ << " ] " << *iter << endl;
	}
	pthread_mutex_unlock(&listlock);
	cout << endl;
}




