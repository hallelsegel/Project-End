#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "Protocol.h"

using namespace std;

class Game
{
public:
	Game(const vector<User*>&, int, DataBase&);
	~Game();
	void sendFirstQuestion();
	void handleFinishGame();
	bool handleNextTurn();
	bool handleAnswerFromUser(User*,int,int);
	bool leaveGame(User*);
	bool insertGameToDB();
	void initQuestionsFromDB();
	void sendQuestionToAllUsers();
	
	// GETS
	int getId();

	// SETS
	
private:

	vector<Question*> _questions;
	vector<User*> _players;
	int _question_no;
	int _currQuestionIndex;
	DataBase& _db;
	map<string, int> _results;
	int _currentTurnAnswers;
	int _ID;
};