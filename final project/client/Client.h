#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include "Protocol.h"
#include "Helper.h"

class Client
{
public:
	Client();
	~Client();
	void Connect(std::string serverIP, int port);
	void suggestMessage();
	bool sendSignIn();
	bool sendSignUp();

private:
	SOCKET _clientSocket;
};