
#pragma comment (lib, "ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include "WSAInitializer.h"
#include "Client.h"
#include <exception>
#include <iostream>

using namespace std;

int main()
{
	try
	{
		WSAInitializer wsaInit;
		Client c1;
		c1.Connect("127.0.0.1", 8820);
		c1.suggestMessage();
	}	
	catch (exception& e)
	{
		cout << "Error occured: " << e.what() << endl;
	}
	system("PAUSE");
	return 0;
}