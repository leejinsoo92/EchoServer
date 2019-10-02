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
	void DeleteData(char* _data);
	void PrintData();

private:
	int test = 0;
	list<string> m_listData;

};

#endif /* DATAREPOSIT_DATAREPOSIT_H_ */
