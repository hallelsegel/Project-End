#include "User.h"
#include "Helper.h"
#include "Room.h"
User::User(string username, SOCKET sock)
{
	this->_username = username;
	this->_sock = sock;
}

void User::send(string message)
{
	Helper::sendData(this->_sock, message);
}

void User::clearGame()
{
	this->_currGame = nullptr;
}

bool User::createRoom(int roomId, string roomName, int maxUsers, int questionsNo, int questionTime)
{
	bool re = _currRoom == nullptr;
	string message = SERVER_CREATE_ROOM_SUCCESS;
	if (re)
		_currRoom = new Room(roomId, this, roomName, maxUsers, questionsNo, questionTime);
	else
		message = SERVER_CREATE_ROOM_FAIL;
	send(message);
	return re;
}

bool User::joinRoom(Room* newRoom)
{
	if (_currRoom == nullptr && newRoom->joinRoom(this))
	{
		_currRoom = newRoom;
		return true;
	}
	return false;
}

void User::leaveRoom()
{
	if (this->_currRoom != nullptr) this->_currRoom->leaveRoom(this), this->_currRoom = nullptr;
}

int User::closeRoom()
{
	if (this->_currRoom == nullptr) return -1;
	int roomNum = this->_currRoom->closeRoom(this);
	this->_currRoom = nullptr;
	this->clearRoom();
	return roomNum;
}

bool User::leaveGame()
{
	if (this->_currGame == nullptr) return false;
	
	this->_currGame = nullptr;
	return true;
}

void User::clearRoom()
{
	this->_currRoom = NULL;
}

// GETS
string User::getUsername()
{
	return(this->_username);
}

SOCKET User::getSocket()
{
	return(this->_sock);
}

Room* User::getRoom()
{
	return(this->_currRoom);
}

Game* User::getGame()
{
	return(this->_currGame);
}

// SETS
void User::setGame(Game* gm)
{
	this->_currRoom = NULL;
	this->_currGame = gm;
}
