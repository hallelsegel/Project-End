#include "Validator.h"
#include <regex>

bool Validator::isPasswordValid(string password)
{
	if (password.length() < 4) 		return false;
	else if (password.find(' ') != string::npos) 	return false;
	else if (regex_search(password, regex("[A-Z]+")) == 0) 	return false;
	else if (regex_search(password, regex(".*[0-9]+")) == 0)		return false;
	else if (regex_search(password, regex("[a-z]+")) == 0)	return false;

	return true;
}

bool Validator::isUsernameValid(string username)
{
	regex alpha("^[a-zA-Z]");
	if (!regex_search(username, alpha)) 	return false;
	else if( username.find(' ') != string::npos) 	return false;
	else if (username.length() == 0) return false;
	return true;
}