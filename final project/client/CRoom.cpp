#include "CRoom.h"
#include "Helper.h"
#include "Protocol.h"
#include "CGame.h"

using namespace std;

CRoom::CRoom(SOCKET clientSock, bool admin)
{
	this->_clientSock = clientSock;
	this->_isAdmin = admin;
}

CRoom::~CRoom()
{
}

bool CRoom::joinRoom() //209
{
	if (!getRooms()) return false;
	int roomId;
	cin >> roomId;
	if (roomId < 1 || roomId > _roomNum){
		cout << "Invalid room id" << endl;
		return false;
	}
	string msg = "209" + Helper::getPaddedNumber(roomId, 4);
	send(_clientSock, msg.c_str(), msg.size(), 0);
	string rcvMsg = Helper::getPartFromSocket(_clientSock, 4, 0);
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
	else if (rcvMsg == SERVER_JOIN_ROOM_SUCCESS){
		_questionsNo = Helper::getIntPartFromSocket(_clientSock, 2);
		_questionTime = Helper::getIntPartFromSocket(_clientSock, 2);
		return handleRoom();
	}
	return true;
}

bool CRoom::getRooms() //205
{
	send(_clientSock, "205", 3, 0);
	string rcvMsg = Helper::getPartFromSocket(_clientSock, 3, 0);
	if (rcvMsg != SERVER_GET_EXIST_ROOMS){
		cout << "Error on getting room's list" << endl;
		return false;
	}

	_roomNum = Helper::getIntPartFromSocket(_clientSock, 4);
	if (_roomNum == 0){
		cout << "There are no rooms open at this moment." << endl;
		return false;
	}
	cout << "Choose of these existing rooms: " << endl;
	for (int i = 0; i < _roomNum; i++)
	{
		cout << "Room Id: " << Helper::getIntPartFromSocket(_clientSock, 4) << " | ";
		cout << "Room Name: " << Helper::getPartFromSocket(_clientSock, Helper::getIntPartFromSocket(_clientSock, 2), 0);
		getUsersByRoom(i+1);
	}
	return true;
}

void CRoom::getUsersByRoom(int roomId) //207
{
	string msg = "207" + Helper::getPaddedNumber(roomId, 4);
	send(_clientSock, msg.c_str(), msg.size(), 0);
	string rcvMsg = Helper::getPartFromSocket(_clientSock, 3, 0);
	if (rcvMsg != SERVER_USER_IN_ROOM) cout << "Error in getting users from room" << endl;
	else{
		int userNum = Helper::getIntPartFromSocket(_clientSock, 1);
		if (userNum == 0) cout << "There are no users, the room probably closed" << endl;
		else {
			cout << " | Users: ";
			for (int i = 0; i < userNum; i++)
			{
				_users.push_back(Helper::getPartFromSocket(_clientSock, Helper::getIntPartFromSocket(_clientSock, 2), 0));
				cout << _users.back() << " | " << endl;
			}
			cout << endl;
		}
	}
}

bool CRoom::handleRoom()
{
	//Wait for the next thing to happen with the room, inside a thread so that we can still get input to leave the room
	_inRoom = 1;
	thread t(&CRoom::roomDisplay, this);
	t.detach();
	string choice;
	cin >> choice;
	if (choice == "L" && !_isAdmin){//leave room
		send(_clientSock, "211", 3, 0);
		//here roomDisplay should catch the answer
	}
	else if (choice == "C" && _isAdmin){ //close room (by admin)
		send(_clientSock, "215", 3, 0);
		//here roomDisplay should catch the answer
	}
	else if (choice == "S" && _isAdmin){
		send(_clientSock, "217", 3, 0);
		//tell the server to start the game, roomDisplay should catch the answer (first question)
	}
	else cout << "Invalid input." << endl;
	return true;
}

void CRoom::roomDisplay()
{
	while (_inRoom){
		//show room loading screen
		system("cls");
		if (_isAdmin) cout << "Room opening successful!\nIn this room:";
		else cout << "Joining succesful!\nIn this room: " << endl;
		cout << "Number of questions: " << _questionsNo << " | ";
		cout << "Time to answer per question: " << _questionTime << " seconds." << endl;
		cout << "Current users in room";
		for (int i = 0; i < (int)_users.size(); i++) cout << " | " << _users[i];
		cout << "\nPlease wait for the game to start";
		if (_isAdmin == 0) cout << ", or leave this room by entering 'L'" << endl;
		else cout << ", close this room by entering 'C' or start the game by entering 'S'" << endl;

		string rcvMsg = Helper::getPartFromSocket(_clientSock, 3, 0);
		cout << "Recieved: " << rcvMsg << endl;
		if (rcvMsg == SERVER_SEND_QUESTION && !_isAdmin)
		{
			CGame newGame(_clientSock, _questionsNo, _questionTime);
			newGame.handleQuestion();
		}
		else if (rcvMsg == SERVER_CLOSE_ROOM)
		{
			cout << "Room closed" << endl;
			_inRoom = 0;
		}
		else if (rcvMsg == SERVER_USER_IN_ROOM) {
			_users.clear();
			int userNum = Helper::getIntPartFromSocket(_clientSock, 1);
			if (userNum == 0) cout << "There are no users, the room probably closed" << endl;
			else {
				cout << " | Users: ";
				for (int i = 0; i < userNum; i++)
				{
					_users.push_back(Helper::getPartFromSocket(_clientSock, Helper::getIntPartFromSocket(_clientSock, 2), 0));
					cout << _users.back() << " | " << endl;
				}
				cout << endl;
			}
			roomDisplay(); //reset the loading screen
		}
		else if (rcvMsg == "112") //cought from the message handleRoom sent (leave room)
		{
			if (Helper::getIntPartFromSocket(_clientSock, 1) == 0)	_inRoom = 0;
		}
	}
}

bool CRoom::createRoom()				//213
{
	int userNum;
	string roomName;
	system("cls");

	cout << "Enter the room's name (no spaces): ";
	cin >> roomName;
	if (roomName.size() < 1) { cout << "You must have a name" << endl; return createRoom(); }
	if (roomName.size() > 99) { cout << "You can't have a name longer than 99 characters" << endl; return createRoom(); }

	cout << "Enter maximum number of users to play in your room: ";
	cin >> userNum;
	if (userNum < 1) { cout << "You can't have less than 1 player" << endl; return createRoom(); }
	if (userNum > 9) { cout << "You can't have more than 9 players" << endl; return createRoom(); }

	cout << "Enter the number of questions: ";
	cin >> _questionsNo;
	if (_questionsNo < 1) { cout << "You can't have less than 1 question" << endl; return createRoom(); }
	if (_questionsNo > 99) { cout << "You can't have more than 99 questions" << endl; return createRoom(); }

	cout << "Enter the number of seconds to answer each question: ";
	cin >> _questionTime;
	if (_questionTime < 1)	{ cout << "You can't have less than 1 second" << endl; return createRoom(); }
	if (_questionTime > 99) { cout << "You can't have more than 99 seconds" << endl; return createRoom(); }

	string msg = "213" + Helper::getPaddedNumber(roomName.size(), 2) + roomName + to_string(userNum);
	msg += Helper::getPaddedNumber(_questionsNo, 2) + Helper::getPaddedNumber(_questionTime, 2);
	send(_clientSock, msg.c_str(), msg.size(), 0);

	string rcvMsg = Helper::getPartFromSocket(_clientSock, 4, 0);
	if (rcvMsg == SERVER_CREATE_ROOM_FAIL) { cout << "Error opening room"; return false; }
	if (rcvMsg == SERVER_CREATE_ROOM_SUCCESS) handleRoom();
	return true;
}