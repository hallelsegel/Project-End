#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "Protocol.h"
#include "TriviaServer.h"
#include "User.h"
#include "Helper.h"
#include "Question.h"

using namespace std;
class DataBase;
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

	bool insertGameToDB();
	void initQuestionsFromDB();
	void sendQuestionToAllUsers();
};