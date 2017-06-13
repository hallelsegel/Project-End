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
		cout << "Choose an action:\n1. Sign out\n2. Join room" << endl;
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
			if (answer == 1) if (signIn()) _isConnected = 1;
			else if (answer == 2) signUp();
		}
		else {
			if (answer == 1)
			{
				send(_clientSocket, "201", 3, 0);
				_isConnected = 0;
			}
			else if (answer == 2) if (joinRoom());
		}
		system("PAUSE");
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

bool Client::joinRoom() //209
{
	if (!getRooms()) return false;
	int roomId;
	cin >> roomId;
	string msg = "209" + Helper::getPaddedNumber(roomId, 4);
	string rcvMsg = Helper::getPartFromSocket(_clientSocket, 3, 0);
	if (rcvMsg == SERVER_JOIN_ROOM_NOT_EXIST)
	{
		cout << "This room doesn't exist or an unknown problem" << endl;
		return false;
	}
	else if (rcvMsg == SERVER_JOIN_ROOM_FULL)
	{
		cout << "The room was filled before you could join" << endl;
		return false;
	}
	else if (rcvMsg == SERVER_JOIN_ROOM_SUCCESS)
	{
		cout << "Joining succesful!\nIn this room: " << endl;
		cout << "Number of questions: " << Helper::getIntPartFromSocket(_clientSocket, 2) << " | ";
		cout << "Time to answer per question: " << Helper::getIntPartFromSocket(_clientSocket, 2) << " seconds." << endl;
	}
	return true;
}

bool Client::getRooms() //205
{
	send(_clientSocket, "205", 3, 0);
	string rcvMsg = Helper::getPartFromSocket(_clientSocket, 3, 0);
	if (rcvMsg != SERVER_GET_EXIST_ROOMS){
		cout << "Error on getting room's list" << endl;
		return false;
	}

	int roomNum = Helper::getIntPartFromSocket(_clientSocket, 4);
	if (roomNum == 0){
		cout << "There are no rooms open at this moment." << endl;
		return false;
	}
	cout << "Choose of these existing rooms: " << endl;
	for (int i = 0; i < roomNum; i++)
	{
		cout << "Room Id: " << Helper::getIntPartFromSocket(_clientSocket, 4) << " | ";
		cout << "Room Name: " << Helper::getPartFromSocket(_clientSocket, Helper::getIntPartFromSocket(_clientSocket, 2), 0);
		getUsersByRoom(i);
	}
	return true;
}

void Client::getUsersByRoom(int roomId) //207
{
	string msg = "207" + Helper::getPaddedNumber(roomId, 4);
	send(_clientSocket, msg.c_str(), msg.size(), 0);
	string rcvMsg = Helper::getPartFromSocket(_clientSocket, 3, 0);
	if (rcvMsg != SERVER_USER_IN_ROOM);

	int userNum = Helper::getIntPartFromSocket(_clientSocket, 1);
	if (userNum == 0) cout << "There are no users, the room probably closed" << endl;
	cout << " | Users: ";
	for (int i = 0; i < userNum; i++)
	{
		cout << Helper::getPartFromSocket(_clientSocket, Helper::getIntPartFromSocket(_clientSocket, 2), 0) << " | ";
	}
	cout << endl;
}
