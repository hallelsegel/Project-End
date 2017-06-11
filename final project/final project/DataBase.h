﻿#pragma once
#include <list>
#include <vector>
#include <string>
#include "Question.h"
#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "Protocol.h"
#include "TriviaServer.h"

using namespace std;
struct sqlite3;

class DataBase
{
private:;
	sqlite3* _sqldb;
	int _lastId;

	int callbackCount(void* param, int argc, char** argv, char** azColName);
	int callbackQuestions(void* param, int argc, char** argv, char** azColName);
	int callbackBestScores(void* param, int argc, char** argv, char** azColName);
	int callbackPersonalStatus(void* param, int argc, char** argv, char** azColName);
	bool fileExistsOnDisk(const string& filename);
	bool initDatabase();

public:
	DataBase();
	~DataBase();
	void setLastId(char* lastId);
	void resetLastId();
	bool open();
	void close();
	void clear();

	bool isUserExists(string username);
	bool addNewUser(string username, string password, string email);
	bool isUserAndPassMatch(string username, string password);
	vector<Question*> initQuestion(int questionNo);
	int insertNewGame();
	bool updateGameStatus(int gameId);
	bool addAnswerToPlayer(int gameId, string username, int questionId, string answer, bool isCorrect, int answerTime);
};
