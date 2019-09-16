//============================================================================
// Name        : EchoServer.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Mng/UserMng/UserMng.h"

using namespace std;

int main() {
	cout << "!!!Hello Server!!!" << endl; // prints !!!Hello World!!!

	CUserMng UserMng = CUserMng();

	UserMng.Update();

	cout << "finish" << endl;

	return 0;
}



















