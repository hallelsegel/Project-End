#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "Protocol.h"

using namespace std;

class RecievedMessage
{
public:
	RecievedMessage(SOCKET,int);
	RecievedMessage(SOCKET, int, vector<string>);

	// GETS
	SOCKET getSock();
	User* getUser();
	int getMessageCode();
	vector<string>& getValues();

	// SETS
	void setUser(User*);

private:

	SOCKET _sock;
	User* _user;
	int _messageCode;
	vector<string> _values;
};