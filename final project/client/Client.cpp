#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Client.h"
#include <exception>
#include <string>
#include <iostream>

using namespace std;

Client::Client()
{
	// notice that we step out to the global namespace
	// for the resolution of the function socket

	// we connect to server that use TCP. that why SOCK_STREAM & IPPROTO_TCP
	_clientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_clientSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");

}

Client::~Client()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		::closesocket(_clientSocket);
	}
	catch (...) {}
}


void Client::Connect(string serverIP, int port)
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr =  inet_addr(serverIP.c_str());    // the IP of the server

	int status = ::connect(_clientSocket, (struct sockaddr*)&sa, sizeof(sa));

	if (status == INVALID_SOCKET)
		throw std::exception("Cant connect to server");
}

void Client::suggestMessage()
{
	int answer = 0;
	while (answer != 15)
	{
		system("cls");
		cout << "Choose an action:\n1. Sign in\n2. Sign up" << endl;
		cin >> answer;
		if (answer == 1) sendSignIn();
		else if (answer == 2) sendSignUp();
	}
	string s = "299";
	send(_clientSocket, s.c_str(), s.size(), 0);
}

bool Client::sendSignIn() //200
{
	string name, password, rcvMsg;
	cout << "Enter name:" << endl;
	cin >> name;
	cout << "Enter password:" << endl;
	cin >> password;
	string s = "200" + Helper::getPaddedNumber(name.size(), 2) + name + Helper::getPaddedNumber(password.size(), 2) + password; //200 == sign in
	send(_clientSocket, s.c_str(), s.size(), 0);
	cout << "Message sent to server: " << s << endl;
	try{
		rcvMsg = Helper::getPartFromSocket(_clientSocket, 4, 0);
		if (rcvMsg == SERVER_SIGN_IN_SUCCESS)
		{
			cout << "Sign in succesful, you are connected, " + name + "!" << endl;
			return true;
		}
		else if (rcvMsg == SERVER_SIGN_IN_ALLREADY_CONNECTAED) cout << "Sign in unsuccesful, this user is already connected" << endl;
		else if (rcvMsg == SERVER_SIGN_IN_WRONG_DETAILS) cout << "Sign in unsuccesful, wrong name or password" << endl;
		return false;
	}
	catch (exception e) { cout << e.what() << endl; }
}

bool Client::sendSignUp() //203
{
	string name, password, email, rcvMsg;
	cout << "Enter name:" << endl;
	cin >> name;
	cout << "Enter password:" << endl;
	cin >> password;
	cout << "Enter email address:" << endl;
	cin >> email;
	string s = "203" + Helper::getPaddedNumber(name.size(), 2) + name + Helper::getPaddedNumber(password.size(), 2) + password; //203 == sign up
	s = s + Helper::getPaddedNumber(email.size(), 2) + email;
	send(_clientSocket, s.c_str(), s.size(), 0);
	cout << "Message sent to server: " << s << endl;
	try{
		rcvMsg = Helper::getPartFromSocket(_clientSocket, 4, 0);
		if (rcvMsg == SERVER_SIGN_UP_SUCCESS)
		{
			cout << "Sign up succesful, you are can now sign in, " + name + "!" << endl;
			return true;
		}
		else if (rcvMsg == SERVER_SIGN_UP_PASS_ILEGAL) cout << "Sign in unsuccesful, the password has to be longer than 4 characters"\
			 ", include at least: 1 lowercase letter, 1 uppercase letter, 1 number and no spaces" << endl;
		else if (rcvMsg == SERVER_SIGN_UP_USERNAME_ALLREDY_EXIST) cout << "Sign in unsuccesful, username is already taken" << endl;
		else if (rcvMsg == SERVER_SIGN_UP_USERNAME_ILEGAL) cout << "Sign in unsuccesful, the username has to be begin with a letter, "\
			"and include no spaces. " << endl;
		else if (rcvMsg == SERVER_SIGN_UP_OTHER) cout << "Sign in unsuccesful, for no known reason" << endl;
		return false;
	}
	catch (exception e) { cout << e.what() << endl; }
}
