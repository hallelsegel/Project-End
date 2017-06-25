#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include "RecievedMessage.h"
#include "Game.h"
#include "Room.h"
#include "User.h"
#include "DataBase.h"
#include "Validator.h"

using namespace std;

class TriviaServer
{
public:
	TriviaServer();
	~TriviaServer();
	void serve(int port);

private:
	SOCKET _serverSocket;
	map<int, Room*> _roomsList;
	map<SOCKET, User*> _connectedUsers;
	queue<RecievedMessage*> _queRcvMessages;
	int _roomIdSequence;
	DataBase _db;
	mutex _mtxRecievedMessages;
	
	void bindAndListen(int port);
	void accept();
	void clientHandler(SOCKET clientSocket);
	Room* getRoomById(int roomId);
	User* getUserByName(string username);
	User* getUserBySocket(SOCKET client_socket);
	void handleRecievedMessages();
	void safeDeleteUser(RecievedMessage* msg);

	bool handleSignup(RecievedMessage* msg); //203
	User* handleSignin(RecievedMessage* msg); //200
	void handleSignout(RecievedMessage* msg); //201
	void handleLeaveGame(RecievedMessage* msg); //222
	void handleStartGame(RecievedMessage* msg); //217
	void handlePlayerAnswer(RecievedMessage* msg); //219
	bool handleCreateRoom(RecievedMessage* msg); //213
	bool handleCloseRoom(RecievedMessage* msg); //215
	bool handleJoinRoom(RecievedMessage* msg); //209
	bool handleLeaveRoom(RecievedMessage* msg); //211
	void handleGetUsersInRoom(RecievedMessage* msg); //207
	void handleGetRooms(RecievedMessage* msg); //205
	void handleGetBestScores(RecievedMessage* msg); //223
	void handleGetPersonalStatus(RecievedMessage* msg); //225
	void handleQuestionCount(RecievedMessage* msg); //240 (our idea)
	void handleAddQuestion(RecievedMessage* msg); //230 (our idea)
	void addRecievedMessage(RecievedMessage* msg); //225
	RecievedMessage* buildRecieveMessage(SOCKET client_socket, int msgCode); //225

	
};

