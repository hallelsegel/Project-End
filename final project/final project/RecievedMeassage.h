#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>

using namespace std;

class RecievedMeassage
{
public:
	RecievedMeassage(SOCKET,int);
	RecievedMeassage(SOCKET, int, vector<string>);

	// GETS
	SOCKET getSock();
	User* getUSer();
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