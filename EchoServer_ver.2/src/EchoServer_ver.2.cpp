//============================================================================
// Name        : 2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Server/Server.h"
#include "DataReposit/DataReposit.h"

using namespace std;

int main() {
	cout << "!!!Hello Server!!!" << endl;

	CServer EchoServer = CServer();

	EchoServer.Run();

	CDataReposit *DataList = CDataReposit::getInstance();
	DataList->DeleteInstance();
	return 0;
}
