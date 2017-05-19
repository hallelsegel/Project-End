#include "User.h"
#include "Helper.h"
#include "Server.h"

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
	if (this->_currRoom != nullptr)
	{
		send(this->_sock, "1141", 5, 0); //////*******************************************************//////
		return false;
	}
	this->_currRoom = new Room(roomId, roomName, maxUsers, questionsNo, questionTime);
	send(this->_sock, "1140", 5, 0); //////*******************************************************//////
	return true;
}

bool User::joinRoom(Room*)
{
	if (this->_currRoom != nullptr)
	{
		return false;
	}

}

void User::leaveRoom()
{
	if (this->_currRoom != nullptr) this->_currRoom = nullptr;
}

int User::closeRoom()
{
	if (this->_currRoom = nullptr) return -1;
	int roomNum = this->_currRoom;
	this->_currRoom = nullptr;
	return roomNum;
}

bool User::leaveGame()
{
	if (this->_currGame = nullptr) return false;
	
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
