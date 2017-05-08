#include "Server.h"
#include <exception>
#include <iostream>
#include <string>

using namespace std;

Server::Server()
{
	// notice that we step out to the global namespace
	// for the resolution of the function socket
	
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	_serverSocket = ::socket(AF_INET,  SOCK_STREAM,  IPPROTO_TCP); 

	if (_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

Server::~Server()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		::closesocket(_serverSocket);
	}
	catch (...) {}
}

void Server::serve(int port)
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

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		cout << "Waiting for client connection request" << endl;
		accept();
	}
}


void Server::accept()
{
	// this accepts the client and create a specific socket from server to this client
	SOCKET client_socket = ::accept(_serverSocket, NULL, NULL);

	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	cout << "Client accepted. Server and client can speak" << endl;

	// the function that handle the conversation with the client
	clientHandler(client_socket);
}


void Server::clientHandler(SOCKET clientSocket)
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

void Server::Register(SOCKET clientSocket)
{
	char m[21], p[21];
	bool nameFlag = 1;
	do
	{
		string s = "Enter Username (up to 20 characters): ";
		send(clientSocket, s.c_str(), s.size(), 0);  // last parameter: flag. for us will be 0.

		recv(clientSocket, m, 4, 0);
		m[20] = 0;
		nameFlag = 0;
		for (map<string, string>::iterator i = _userDataBase.begin(); i != _userDataBase.end(); i++)
		{
			if (i->first == m)
			{
				nameFlag = 1;
				cout << "Username already taken, please try again. " << endl;
				break;
			}
		}
	} while (nameFlag);

	cout << "Client name is: " << m << endl;

	string s = "Enter Password (up to 20 characters): ";
	send(clientSocket, s.c_str(), s.size(), 0);  // last parameter: flag. for us will be 0.
	recv(clientSocket, p, 4, 0);
	p[20] = 0;
	cout << "Client pass is: " << p << endl;

	_userDataBase.insert(pair<string, string>(m, p));
	send(clientSocket, s.c_str(), s.size(), 0);

}

void Server::signIn(SOCKET clientSocket)
{

}

