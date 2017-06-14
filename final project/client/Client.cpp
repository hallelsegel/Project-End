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
	_isConnected = 0;

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

void Client::printOptions()
{
	system("cls");
	if (_isConnected == 0) //need to sign in
	{
		cout << "Choose an action:\n1. Sign in\n2. Sign up" << endl;
	}
	else
	{
		cout << "Choose an action:\n1. Sign out\n2. Join room\n3. Create room\n4. View highscores\n5. View personal stats" << endl;
	}
}

void Client::handleOption()
{
	int answer = 0;
	while (answer != 15)
	{
		printOptions();
		cin >> answer;
		if (_isConnected == 0) {
			if (answer == 1){
				if (signIn()) _isConnected = 1;
			}
			else if (answer == 2) signUp();
		}
		else {
			if (answer == 1)
			{
				send(_clientSocket, "201", 3, 0);
				_isConnected = 0;
			}
			else if (answer == 2) {
				CRoom newRoom(_clientSocket, 0);
				if (newRoom.joinRoom() == 0)   cin.clear(); cin.ignore(INT_MAX, '\n'); //clean buffer
			}
			else if (answer == 3){
				CRoom newRoom(_clientSocket, 1);
				if (newRoom.createRoom() == 0) cin.clear(); cin.ignore(INT_MAX, '\n'); //clean buffer
			}
			else if (answer == 4){
				getBestScores();
			}
			else if (answer == 5){
				getPersonalStatus();
			}
		}
		if (answer != 15)  cin.clear(); cin.ignore(INT_MAX, '\n'); //clean buffer
	}
	string s = "299";
	send(_clientSocket, s.c_str(), s.size(), 0);
}

bool Client::signIn() //200
{
	string name, password, rcvMsg;
	cout << "Enter name:" << endl;
	cin >> name;
	cout << "Enter password:" << endl;
	cin >> password;
	string msg = "200" + Helper::getPaddedNumber(name.size(), 2) + name + Helper::getPaddedNumber(password.size(), 2) + password; //200 == sign in
	send(_clientSocket, msg.c_str(), msg.size(), 0);
	cout << "Message sent to server: " << msg << endl;
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

bool Client::signUp() //203
{
	string name, password, email, rcvMsg;
	cout << "Enter name:" << endl;
	cin >> name;
	cout << "Enter password:" << endl;
	cin >> password;
	cout << "Enter email address:" << endl;
	cin >> email;
	string msg = "203" + Helper::getPaddedNumber(name.size(), 2) + name + Helper::getPaddedNumber(password.size(), 2) + password; //203 == sign up
	msg = msg + Helper::getPaddedNumber(email.size(), 2) + email;
	send(_clientSocket, msg.c_str(), msg.size(), 0);
	cout << "Message sent to server: " << msg << endl;
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

bool Client::getBestScores()
{
	send(_clientSocket, "223", 3, 0);
	string rcvMsg = Helper::getPartFromSocket(_clientSocket, 3, 0);
	if (rcvMsg != SERVER_BEST_SCORES)
	{
		cout << "Error in getting highscores. please try again" << endl;
		return false;
	}
	else
	{
		cout << "Highest scores: " << endl;
		for (int i = 0; i < 3; i++)
		{
			int usernameLen = Helper::getIntPartFromSocket(_clientSocket, 2);
			if (usernameLen == 0) break;
			cout << "User " << Helper::getPartFromSocket(_clientSocket, usernameLen, 0);
			cout << " has answered correctly " << Helper::getIntPartFromSocket(_clientSocket, 6) << "% of the time" << endl;
		}
	}
	return true;
}

bool Client::getPersonalStatus()
{
	send(_clientSocket, "225", 3, 0);
	string rcvMsg = Helper::getPartFromSocket(_clientSocket, 3, 0);
	if (rcvMsg != SERVER_PERSONAL_STATUS)
	{
		cout << "Error in getting personal status. please try again" << endl;
		return false;
	}
	else
	{
		cout << "Your stats: " << endl;
		int numOfGames = Helper::getIntPartFromSocket(_clientSocket, 4);
		if (numOfGames == 0)
		{
			cout << "You have played no games, and thus have no personal stats... go play some now!" << endl;
			return false;
		}
		cout << "You have played " << numOfGames << " games" << endl;
		cout << "With " << Helper::getIntPartFromSocket(_clientSocket, 6) << " correct answers" << endl;
		cout << "And " << Helper::getIntPartFromSocket(_clientSocket, 6) << " wrong answers." << endl;
		cout << "Your average time to answer is " << (Helper::getIntPartFromSocket(_clientSocket, 4) / 100.0) << " seconds. " << endl;
	}
	return true;
}

