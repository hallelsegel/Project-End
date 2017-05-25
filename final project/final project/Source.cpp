#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "TriviaServer.h
#include <iostream>
#include <exception>

using namespace std;

int main()
{
	try
	{
		WSAInitializer wsaInit;
		Server myServer;

		myServer.serve(8876);
	}
	catch (exception& e)
	{
		cout << "Error occured: " << e.what() << endl;
	}
	system("PAUSE");
	return 0;
}