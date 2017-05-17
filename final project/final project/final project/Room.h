#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>

using namespace std;

class Room
{
public:
	Room(int, User*, string, int, int, int);
	bool joinRoom(User*);
	void leaveRoom(User*);
	int closeRoom(User*);
	void sendMessage(string);
	void sendMessage(User*,string);

	// GETS
	vector<User*> getUsers();
	string getUsersListMessage();
	int getQuestionNo();	
	int getId();
	string getName();
	string getUsersAsAsString();

	// SETS

private:

	vector<User*> _users;
	User* admin;
	int _maxUsers;
	int _questionTime;
	int questionsNo;
	string _name;
	int _id;
};