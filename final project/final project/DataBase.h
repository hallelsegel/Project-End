#pragma once
#include <list>
#include <vector>
#include <string>
#include "Question.h"
#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "Protocol.h"

using namespace std;
struct sqlite3;
class TriviaServer;
class DataBase
{
private:;
	sqlite3* _sqldb;
	int _lastId;
	char* _lastUsername;
	static int sqlExecCallbackID(void* param, int argc, char** argv, char** azColName);
	static int sqlExecCallbackUM(void* param, int argc, char** argv, char** azColName);
	static int callbackUsers(void* param, int argc, char** argv, char** azColName);
	static int callbackQuestions(void* param, int argc, char** argv, char** azColName);
	static int callbackCount(void* params, int argc, char** argv, char** azColName);
	bool fileExistsOnDisk(const string& filename);
	bool initDatabase();

public:
	DataBase();
	~DataBase();
	void setLastId(char* lastId);
	void resetLastId();
	void setLastUM(char* lastUM);
	void resetLastUM();

	bool open();
	void close();
	void clear();

	bool isUserExists(string username);
	bool addNewUser(string username, string password, string email);
	bool isUserAndPassMatch(string username, string password);
	vector<Question*> initQuestion(int questionNo);
	map<int, string> getBestScores();
	vector<string> getPersonalStatus(string username);
	int insertNewGame();
	bool updateGameStatus(int gameId);
	bool addAnswerToPlayer(int gameId, string username, int questionId, string answer, bool isCorrect, int answerTime);
};
