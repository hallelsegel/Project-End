#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>

using namespace std;

class Game
{
public:
	Game(string, SOCKET);
	~Game();
	void sendFIrstQuestion();
	void handleFinishQuestion();
	bool handleNextTurn();
	bool handleAnswerFromUser(User*,int,int);
	bool leaveGame(User*);
	bool insertGameToDB();
	void initQuestionsFromDB();
	void sendQuestionTAllUsers();
	
	// GETS
	int getId();

	// SETS
	
private:

	vector<Questions*> _questions;
	vector<User*> __Players;
	int _question_no;
	int currQuestionIndex;
	DataBase& _db;
	map<string, int> _results;
	int currentTurnAnswers;
};