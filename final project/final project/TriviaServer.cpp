#include "TriviaServer.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include "Helper.h"
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
} //fsghg

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

bool TriviaServer::handleSignup(RecievedMessage* msg)
{
	char m[21], p[21];
	bool nameFlag = 1;
	map<string, string>::iterator it;

}

User* TriviaServer::handleSignin(RecievedMessage* msg)
{
	char m[21], p[21];
	map<string, string>::iterator it;
	bool nameFlag = 1;
	string s;
	do
	{// get username
		s = "Enter Username (up to 20 characters): ";
		send(clientSocket, s.c_str(), s.size(), 0);
		recv(clientSocket, m, 4, 0);
		m[20] = 0;
		it = _connectedUsers.find(m);
		if (it != _connectedUsers.end()) cout << "Username found. " << endl;
		else {
			s = "Username not found, please try again\n";
			send(clientSocket, s.c_str(), s.size(), 0);
			continue;
		}
	// get username

		s = "Enter Password (up to 20 characters): ";
		send(clientSocket, s.c_str(), s.size(), 0);
		recv(clientSocket, p, 4, 0);
		it = _connectedUsers.find(s);
		if (it != _connectedUsers.end()) cout << "Password found. " << endl;
		else {
			s = "Username not found, please try again\n";
			send(clientSocket, s.c_str(), s.size(), 0);
			continue;
		}
		p[20] = 0;
		cout << "Client pass is: " << p << endl;


	} while (it == _connectedUsers.end());
	_connectedUsers.insert(pair<string, string>(m, p));
	s = "Login succesful \n";
	send(clientSocket, s.c_str(), s.size(), 0);
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
}

User* TriviaServer::getUserBySocket(SOCKET client_socket)
{

}

void TriviaServer::handleRecievedMessages()
{

}