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
	void printOptions();
	void handleOption();

									//Message Code:
	bool signIn();					//200
	bool signUp();					//203

	/* room fucntions */
	bool getRooms();				//205
	void getUsersByRoom(int roomId);//207
	bool joinRoom();				//209
	bool leaveRoom();				//211
	bool createRoom();				//213
	bool closeRoom();				//215
	
	/* game functions*/
	bool startGame();				//217
	bool sendAnswer();				//219
	bool leaveGame();				//222
		
	/* feature functions */
	bool getBestScores();			//223
	bool getPersonalStatus();		//225

private:
	SOCKET _clientSocket;
	bool _isConnected;
};