#include "user.h"
#include "Helper.h"
#include "RecievedMeassage.h"

RecievedMeassage::RecievedMeassage(SOCKET socket, int messageCode)
{
	this->_sock = socket;
	this->_messageCode = messageCode;
}

RecievedMeassage::RecievedMeassage(SOCKET socket, int messageCode, vector<string> values)
{
	this->_sock = socket;
	this->_messageCode = messageCode;
	this->_values = values;
}

// GETS
SOCKET RecievedMeassage::getSock()
{
	return this->_sock;
}

User* RecievedMeassage::getUSer()
{
	return this->_user;
}

int RecievedMeassage::getMessageCode()
{
	return this->_messageCode;
}

vector<string>& RecievedMeassage::getValues()
{
	return this->_values;
}

// SETS
void RecievedMeassage::setUser(User* user)
{
	this->_user = user;
}