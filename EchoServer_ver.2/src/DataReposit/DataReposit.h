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
	void SaveData(char* _data);
	bool DeleteData(char* _data);
	char* PrintSendData(int num);
	void PrintData();

private:
	int test = 0;
	vector<string> m_listData;

};

#endif /* DATAREPOSIT_DATAREPOSIT_H_ */
