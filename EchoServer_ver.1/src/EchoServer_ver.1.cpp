//============================================================================
// Name        : 1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Server/Server.h"

using namespace std;

int main() {
	cout << "!!!Hello Server!!!" << endl; // prints !!!Hello World!!!

	CServer EchoServer = CServer();

	EchoServer.Update();

	return 0;
}
