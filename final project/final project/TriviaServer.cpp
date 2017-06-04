#include "TriviaServer.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include "Helper.h"
#include "Validator.h"
#include "DataBase.h"

using namespace std;

TriviaServer::TriviaServer()
{
	// notice that we step out to the global namespace
	// for the resolution of the function socket
	
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	_serverSocket = ::socket(AF_INET,  SOCK_STREAM,  IPPROTO_TCP); 

	if (_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
	_db = DataBase();
	if (!_db.open())
		throw exception("Unable to open database");
}

TriviaServer::~TriviaServer()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		::closesocket(_serverSocket);
	}
	catch (...) {}
}

void TriviaServer::serve(int port)
{
	
	bindAndListen(port);
	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		cout << "Waiting for client connection request" << endl;
		accept();
	}
}

void TriviaServer::bindAndListen(int port)
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (::bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (::listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	cout << "Listening on port " << port << endl;

}


void TriviaServer::accept()
{
	// this accepts the client and create a specific socket from server to this client
	SOCKET client_socket = ::accept(_serverSocket, NULL, NULL);

	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	cout << "Client accepted. Server and client can speak" << endl;

	// the function that handle the conversation with the client
	thread t(&TriviaServer::clientHandler, this, client_socket);
	t.detach();
}


void TriviaServer::clientHandler(SOCKET clientSocket)
{

	try
	{		
		// Closing the socket (in the level of the TCP protocol)
		closesocket(clientSocket); 
	}
	catch (const std::exception& e)
	{
		closesocket(clientSocket);
	}
}

bool TriviaServer::handleSignup(RecievedMessage* msg) //203
{
	string username = msg->getValues()[0];
	string password = msg->getValues()[1];
	string email = msg->getValues()[2];
	if (Validator::isPasswordValid(password) == false)			msg->getUser()->send(SERVER_SIGN_UP_PASS_ILEGAL);
	else if (Validator::isUsernameValid(username) == false)		msg->getUser()->send(SERVER_SIGN_UP_USERNAME_ILEGAL);
	else if (_db.isUserExists(username))						msg->getUser()->send(SERVER_SIGN_UP_USERNAME_ALLREDY_EXIST);
	else if (_db.addNewUser(username, password, email))			msg->getUser()->send(SERVER_SIGN_UP_SUCCESS);
	else														msg->getUser()->send(SERVER_SIGN_UP_OTHER);
}

User* TriviaServer::handleSignin(RecievedMessage* msg) //200
{
	string username = msg->getValues()[0];
	string password = msg->getValues()[1];
	SOCKET socket = msg->getSock();
	if (!_db.isUserAndPassMatch(username, password))		{
		msg->getUser()->send(SERVER_SIGN_IN_WRONG_DETAILS);
		return nullptr;
	}
	if (getUserByName(username) != nullptr)					{
		msg->getUser()->send(SERVER_SIGN_IN_ALLREADY_CONNECTAED);
		return nullptr;
	}
	User* u = new User(username, socket);
	_connectedUsers.insert(pair<SOCKET, User*>(socket, u));
	return u;
}

void TriviaServer::handleSignout(RecievedMessage* msg) //201
{
	SOCKET sock = msg->getSock();
	map<SOCKET, User*>::iterator it;
	for (it = _connectedUsers.begin(); it->first != sock; it++){}
	if (it != _connectedUsers.end())
	{
		_connectedUsers.erase(it);
		handleLeaveRoom(msg);
		handleCloseRoom(msg);
		handleLeaveGame(msg);
	}
	else cout << "No such user." << endl; //error info
}

bool TriviaServer::handleLeaveRoom(RecievedMessage* msg)
{
	SOCKET sock = msg->getSock();
	map<SOCKET, User*>::iterator it;
	for (it = _connectedUsers.begin(); it->first != sock; it++){}
	if (it != _connectedUsers.end())
	{
		Room* room = it->second->getRoom();
		map<int, Room*>::iterator it2;
		for (it2 = _roomsList.begin(); it2->second != room; it2++){}
		if (it2 != _roomsList.end())
		{
			it->second->leaveRoom();
		}
		else return false;

	}
	else return false;
}

bool TriviaServer::handleCloseRoom(RecievedMessage* msg)
{
	SOCKET sock = msg->getSock();
	map<SOCKET, User*>::iterator it;
	for (it = _connectedUsers.begin(); it->first != sock; it++){}
	if (it != _connectedUsers.end())
	{
		Room* room = it->second->getRoom();
		map<int, Room*>::iterator it2;
		for (it2 = _roomsList.begin(); it2->second != room; it2++){}
		if (it2 != _roomsList.end())
		{
			if (it->second->closeRoom() != -1)
				_roomsList.erase(it2);
		}
		else return false;

	}
	else return false;
}

Room* TriviaServer::getRoomById(int roomId)	
{
	return _roomsList[roomId];
}

User* TriviaServer::getUserByName(string username)
{
	for (map<SOCKET, User*>::iterator i = _connectedUsers.begin(); i != _connectedUsers.end(); i++)
	{
		if (i->second->getUsername() == username) return i->second;
	}
	return nullptr;
}

User* TriviaServer::getUserBySocket(SOCKET client_socket)
{
	//map<SOCKET, User*> _connectedUsers;
	for (map<SOCKET, User*>::iterator i = _connectedUsers.begin(); i != _connectedUsers.end(); i++)
	{
		if (i->first == client_socket) return i->second;
	}
}

void TriviaServer::handleRecievedMessages()
{

}