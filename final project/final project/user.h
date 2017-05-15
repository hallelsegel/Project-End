#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>

using namespace std;

class User
{
public:
	User(string,SOCKET);
	void send(string);
	void clearGame();
	bool createRoom(int, string, int, int, int);
	bool joinRoom(Room*);
	void leaveRoom();
	int closeRoom();
	bool leaveGame();

	// GETS
	string getUsername();
	SOCKET getSocket();
	Room* getRoom();
	Game* getGame();
	
	// SETS
	void setGame(Game*);
	
private:

	string _username;
	Room* _currRoom;
	Game* _currGame;
	SOCKET _sock;
};