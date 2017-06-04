#include "Validator.h"
#include <regex>

static bool isPasswordValid(string password)
{
	regex num("[1-9]");
	regex upper("[A-Z]");
	regex lower("[a-z]");
	if (password.length() < 4 || \
		password.find(' ') || \
		!regex_match(password.c_str(), num) || \
		!regex_match(password.c_str(), upper) || \
		!regex_match(password.c_str(), lower))
		return false;
	return true;
}

static bool isUsernameValid(string username)
{
	regex alpha("[a-zA-Z]");
	if (!regex_match(&username[0], alpha) || \
		username.find(' ') || \
		username.length > 0) return false;
	return true;
}