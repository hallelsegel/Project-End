#include "user.h"
#include "Helper.h"
#include "RecievedMessage.h"

RecievedMessage::RecievedMessage(SOCKET socket, int messageCode)
{
	this->_sock = socket;
	this->_messageCode = messageCode;
}

RecievedMessage::RecievedMessage(SOCKET socket, int messageCode, vector<string> values)
{
	this->_sock = socket;
	this->_messageCode = messageCode;
	this->_values = values;
}

// GETS
SOCKET RecievedMessage::getSock()
{
	return this->_sock;
}

User* RecievedMessage::getUser()
{
	return this->_user;
}

int RecievedMessage::getMessageCode()
{
	return this->_messageCode;
}

vector<string>& RecievedMessage::getValues()
{
	return this->_values;
}

// SETS
void RecievedMessage::setUser(User* user)
{
	this->_user = user;
}