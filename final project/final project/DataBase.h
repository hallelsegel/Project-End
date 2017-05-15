#pragma once
#include <list>
#include <vector>
#include <string>

using namespace std;
struct sqlite3;

class DataBase
{
private:;
	sqlite3* _sqldb;
	int _lastId;

	static int callbackCount(void* param, int argc, char** argv, char** azColName);
	static int callbackQuestions(void* param, int argc, char** argv, char** azColName);
	static int callbackBestScores(void* param, int argc, char** argv, char** azColName);
	static int callbackPersonalStatus(void* param, int argc, char** argv, char** azColName);

	bool initDatabase();

public:
	DataBase();
	~DataBase();

	bool isUserExists(string username);
	bool addNewUser(string username, string password, string email);
	bool isUserAndPassMatch(string password, string email);
	vector<Question*> initQuestion(int questionNo);
	int insertNewGame();
	bool updateGameStatus(int gameId);
	bool addAnswerToPlayer(int gameId, string username, int questionId, string answer, bool isCorrect, int answerTime);
};
