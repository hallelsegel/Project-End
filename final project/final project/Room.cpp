#include "Room.h"
#include "Helper.h"
#include "User.h"
using namespace std;

Room::Room(int id, User* admin, string name, int maxUsers, int questionsNo, int questionTime)
{
	this->_id = id; 
	this->_name = name;
	this->_maxUsers = maxUsers;
	this->_questionTime = questionTime;
	this->_questionsNo = questionsNo;
	this->_users.push_back(admin); // inserts the admin user into the User* vector.
	this->_admin = admin;
}

Room::~Room()
{
}

bool Room::joinRoom(User* user)
{
	if (this->_maxUsers = this->_users.size())
	{
		user->send(SERVER_JOIN_ROOM_FULL);
		return(false);
	}
	else
	{
		this->_users.push_back(user);
		user->send(SERVER_JOIN_ROOM_SUCCESS);
		sendMessage(getUsersListMessage());
		return (true);
	}
}

void Room::leaveRoom(User* user)
{
	int i;
	for (i = 0; i < (int)this->_users.size() && this->_users[i]->getSocket() != user->getSocket(); i++);
	_users.erase(_users.begin() + i);
	user->send(SERVER_LEAVE_ROOM);
	sendMessage(user, getUsersListMessage());
}

int Room::closeRoom(User* user)
{
	if (user != this->_admin)
	{
		return -1;
	}
	else
	{
		sendMessage(SERVER_CLOSE_ROOM);
		for (int i = 0; i < (int)this->_users.size(); i++)
		{
			if (this->_users[i] != this->_admin)
			{
				this->_users[i]->clearRoom();
			}
		}
		return this->_id;
	}
}

void Room::sendMessage(string message)
{
	sendMessage(NULL, message);
}

void Room::sendMessage(User* excludeUser, string message)
{
	for (unsigned int i = 0; i < _users.size(); i++)
	{
		if (excludeUser != NULL)
		{
			if (excludeUser->getSocket() != _users[i]->getSocket())
				_users[i]->send(message);
		}
		else
			_users[i]->send(message);
	}
}

// GETS
vector<User*> Room::getUsers()
{
	return _users;
}

string Room::getUsersListMessage()
{
	string message = SERVER_USER_IN_ROOM;

	message += to_string(_users.size());
	for (unsigned int i = 0; i < _users.size(); i++)
	{
		message += Helper::getPaddedNumber(_users[i]->getUsername().length(), 2);
		message += _users[i]->getUsername();
	}
	return message;
}

int Room::getQuestionNo()
{
	return this->_questionsNo;
}

int Room::getId()
{
	return this->_id;
}

string Room::getName()
{
	return this->_name;
}

string Room::getUsersAsAsString(vector<User*> usersList, User* excludeUser)
{
	string users;
	for (unsigned int i = 0; i < usersList.size(); i++)
	{
		if (usersList[i]->getSocket() != excludeUser->getSocket())
		{
			users += ',' + usersList[i]->getUsername();
		}
	}
	return users;
}

// SETS
