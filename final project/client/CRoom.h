#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <thread>
#include <condition_variable>
using namespace std;

class CRoom
{
public:
	CRoom(SOCKET, bool);
	~CRoom();

	bool getRooms();				//205
	void getUsersByRoom(int roomId);//207
	bool joinRoom();				//209
	bool leaveRoom();				//211
	bool createRoom();				//213
	bool closeRoom();				//215
	bool handleRoom();
	void roomDisplay();

private:
	SOCKET _clientSock;
	vector<string> _users;
	int _roomNum;
	int _questionTime;
	int _questionsNo;
	bool _isAdmin;
	bool _inRoom;
};