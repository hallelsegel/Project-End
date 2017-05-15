#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <queue>

using namespace std;

class Server
{
public:
	Server();
	~Server();
	void serve(int port);

private:

	void bindAndListen();
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
	void addRecievedMessage(RecievedMessage* msg); //225
	void buildRecievedMessage(RecievedMessage* msg); //225



	SOCKET _serverSocket;
	map<int, Room*> _roomsList;
	map<SOCKET, User*> _connectedUsers;
	queue<RecievedMessage*> _queRcvMessages;
	static int _roomIdSequence;
};

