#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>

using namespace std;

class Server
{
public:
	Server();
	~Server();
	void serve(int port);

private:

	void accept();
	void clientHandler(SOCKET clientSocket);
	void Register(SOCKET clientSocket);
	void signIn(SOCKET clientSocket);
	SOCKET _serverSocket;
	map<string, string> _userDataBase;
};

