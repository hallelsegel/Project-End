#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include "Protocol.h"
#include "Helper.h"
#include "CRoom.h"

class Client
{
public:
	Client();
	~Client();
	void Connect(std::string serverIP, int port);
	void printOptions();
	void handleOption();
									//Message Code:
	bool signIn();					//200
	bool signUp();					//203
	
	/* feature functions */
	bool getBestScores();			//223
	bool getPersonalStatus();		//225

private:
	SOCKET _clientSocket;
	bool _isConnected;
};