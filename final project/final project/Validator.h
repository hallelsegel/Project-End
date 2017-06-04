#pragma once
#include <string>

using namespace std;

class Validator
{
public:
	static bool isPasswordValid(string password);
	static bool isUsernameValid(string username);

};

