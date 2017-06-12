#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "TriviaServer.h"
#include <iostream>
#include <exception>
#include <regex>
using namespace std;

int main()
{
	try
	{
		WSAInitializer wsaInit;
		TriviaServer myServer;

		myServer.serve(8820);
	}
	catch (exception& e)
	{
		cout << "Error occured: " << e.what() << endl;
	}
	system("PAUSE");
	return 0;
}