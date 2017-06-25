 #include "stdafx.h"
#include "sqlite3.h"
#include "DataBase.h"
#include "Helper.h"
#include "QuestionList.h"
#include <ctime>
#include <iostream>
#include <sstream>

#pragma warning(disable:4996)
#define RETURN_IF_INVALID				if (_sqldb == nullptr) return;
#define RETURN_RES_IF_INVALID(res)		if (_sqldb == nullptr) return res;



DataBase::DataBase()
{
	srand(time(NULL)); //reset time so that the random inside Question::Question() comes out different every time
	this->open();
	//c'tor
}

DataBase::~DataBase()
{
	//d'tor
}

bool DataBase::isUserExists(string username)
{/* Checks if a user with this name exists in the database. */
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "SELECT * FROM T_USERS WHERE USERNAME='" + username + "';";

	resetLastUM();
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallbackUM, this, &errMessage);
	if (res != SQLITE_OK)
	{
		throw exception(errMessage);
		return false;
	}
	return (_lastUsername != "-1");
}

bool DataBase::addNewUser(string username, string password, string email)
{/* adds a new row to the T_USERS table with these values. */
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "INSERT INTO T_USERS (USERNAME, PASSWORD, EMAIL) "	\
		"VALUES ('" + username + "', '" + password + "', '" + email + "'); "	\
		"SELECT * FROM T_USERS WHERE USERNAME='" + username + "';";

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallbackUM, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return (_lastUsername != "-1");
}

bool DataBase::isUserAndPassMatch(string username, string password)
{/* Checks if a user with this username AND password exists in the database. */
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "SELECT * FROM T_USERS WHERE USERNAME='" + username + "' AND PASSWORD='" + password + "';";

	resetLastUM();
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallbackUM, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return (_lastUsername != "-1");
}

vector<Question*> DataBase::initQuestion(int questionNo)
{/* initiates a vector with a [questionNo] number of questions drawn randomly from the database. */
	vector<Question*> questionsV;
	string sqlStatement;
	char *errMessage = nullptr;
	char buff[10];
	sqlStatement = "SELECT * FROM T_QUESTIONS ORDER BY RANDOM() LIMIT " + string(_itoa(questionNo, buff, 10)) + ";";
	resetLastId();
	
	sqlite3_exec(_sqldb, sqlStatement.c_str(), callbackQuestions, &questionsV, &errMessage);
	return questionsV;
}

map<int, string> DataBase::getBestScores()
{
	string sqlStatement;
	map<int, string> scores;
	vector<string> users;
	char *errMessage = nullptr;
	char buff[10];
	int i = 0, correct = 0, wrong = 0;
	sqlStatement = "SELECT DISTINCT USERNAME FROM T_PLAYERS_ANSWERS;";
	sqlite3_exec(_sqldb, sqlStatement.c_str(), callbackUsers, &users, &errMessage);

	for (i = 0; i < users.size(); i++)
	{
		correct = atoi(this->getPersonalStatus(users[i])[1].c_str());
		wrong = atoi(this->getPersonalStatus(users[i])[2].c_str());
		scores.insert(pair<int, string>(correct * 100 / (correct + wrong), users[i])); //score: percent of correct answers
	}
	return scores;
}

vector<string> DataBase::getPersonalStatus(string username)
{
	string sqlStatement;
	vector<string> stats;
	char *errMessage = nullptr;
	int res, numOfGames, numOfCorrect, numOfWrong, avgTime;
	sqlStatement = "select count(distinct game_id) from t_players_answers where username = '" + username + "';";
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), callbackCount, &numOfGames, &errMessage);
	stats.push_back(Helper::getPaddedNumber(numOfGames, 4));
	if (numOfGames == 0) {
		stats.push_back(Helper::getPaddedNumber(numOfGames, 6)); //fill with 0's, no games
		stats.push_back(Helper::getPaddedNumber(numOfGames, 6)); //fill with 0's, no games
		stats.push_back(Helper::getPaddedNumber(numOfGames, 4)); //fill with 0's, no games
		return stats;
	}

	sqlStatement = "select count() from t_players_answers where username = '" + username + "' AND IS_CORRECT = 1;";
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), callbackCount, &numOfCorrect, &errMessage);
	stats.push_back(Helper::getPaddedNumber(numOfCorrect, 6));

	sqlStatement = "select count() from t_players_answers where username = '" + username + "' AND IS_CORRECT = 0;";
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), callbackCount, &numOfWrong, &errMessage);
	stats.push_back(Helper::getPaddedNumber(numOfWrong, 6));

	sqlStatement = "select sum(answer_time) from t_players_answers where username = '" + username + "';";
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), callbackCount, &avgTime, &errMessage);
	stats.push_back(Helper::getPaddedNumber((int)(avgTime * 100.0 / (numOfWrong + numOfCorrect)), 4));

	return stats;
}

int DataBase::questionCount()
{
	string sqlStatement;
	char *errMessage = nullptr;
	int res, numOfQuestions;
	sqlStatement = "select count(*) from t_questions;";
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), callbackCount, &numOfQuestions, &errMessage);
	if (res != SQLITE_OK)
		return -1;
	return numOfQuestions;
}

bool DataBase::insertQuestion(string question, string answer1, string answer2, string answer3, string answer4)
{
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4) VALUES ('" + question + "', '" + answer1 +
		"', '" + answer2 + "', '" + answer3 + "', '" + answer4 + "'); SELECT * FROM T_QUESTIONS WHERE QUESTION='" + question + "';"; //to check if worked

	resetLastId();
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallbackID, this, &errMessage);
	if (res != SQLITE_OK)
	{
		throw exception(errMessage);
		return false;
	}
	return (_lastId != -1);
}

int DataBase::insertNewGame()
{
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "INSERT INTO T_GAMES (STATUS, START_TIME, END_TIME) "	\
		"VALUES (0, time('now'), time('now')); "	\
		"SELECT * FROM T_GAMES WHERE START_TIME=time('now');";  //time('now') is the current time (h/m/s), is temporarily inserted to END_TIME.

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallbackID, this, &errMessage);
	if (res != SQLITE_OK)
		return false;
	else return _lastId;
}

bool DataBase::updateGameStatus(int gameId)
{
	RETURN_RES_IF_INVALID(false);
	
	string sqlStatement;
	char *errMessage = nullptr;
	int res;
	char buff[10];
	sqlStatement = "UPDATE T_GAMES SET END_TIME = time('now') WHERE GAME_ID = " + string(_itoa(gameId, buff, 10)) + ";";
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallbackID, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return (_lastId != -1);
}

bool DataBase::addAnswerToPlayer(int gameId, string username, int questionId, string answer, bool isCorrect, int answerTime)
{
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;
	char buff[10];
	if (answer == "") answer = "no answer";
	sqlStatement = "INSERT INTO T_PLAYERS_ANSWERS (PLAYER_ANSWER, IS_CORRECT, GAME_ID, USERNAME, QUESTION_ID, ANSWER_TIME)" \
		"VALUES('" + answer + "', " + string(_itoa(isCorrect, buff, 10)) + ", " + string(_itoa(gameId, buff, 10)) + ", '"		\
		+ username + "', " + string(_itoa(questionId, buff, 10)) + ", " + string(_itoa(answerTime, buff, 10)) + "); "		\
		+ "SELECT * FROM T_PLAYERS_ANSWERS WHERE GAME_ID = " + string(_itoa(gameId, buff, 10)) + " AND USERNAME = '"		\
		+ username + "' AND QUESTION_ID =" + string(_itoa(questionId, buff, 10)) +									 ";";
	//first insert with all required values and then call select where its the right question for the right user in the right
	//game to check success.
	
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallbackID, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return (_lastId != -1);
}

void DataBase::setLastId(char* lastId)
{/* setter for the private member _lastid */
	_lastId = atoi(lastId);
}

void DataBase::resetLastId()
{/* resetter for the private member _lastid (insert -1) */
	_lastId = -1;
}

void DataBase::setLastUM(char* lastUM)
{
	_lastUsername = lastUM;
}

void DataBase::resetLastUM()
{
	_lastUsername = "-1";
}

bool DataBase::open()
{/* starter for the database manager, including: opening the trivia database file if it exists, and calling the initiator for it if not. */
	// check if file exists
	string filename = "triviaDB.sqlite";
	bool fileExisted = fileExistsOnDisk(filename);

	// try to open the database
	int res = sqlite3_open(filename.c_str(), &_sqldb);
	if (res != SQLITE_OK) {
		_sqldb = nullptr;
		return false;
	}

	// db is open, check if we need to init it (it didn't exist before)
	if (fileExisted){
		/**	Done **/
		return true;
	}
	else	// need to init, lets create the tables and some other stuff...
	return initDatabase();
}

bool DataBase::initDatabase()
{ /* Initiates the database in case it didn't exist beforehand, meaning creating all of the tables. */
	char *sqlStatement;
	char *errMessage = nullptr;
	int res;

	// create users table
	sqlStatement = "CREATE TABLE T_USERS("				\
		"USERNAME  TEXT PRIMARY KEY		NOT NULL	,"	\
		"PASSWORD  TEXT	NOT NULL					,"	\
		"EMAIL     TEXT	NOT NULL)					;";

	res = sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		return false;

	// create games table
	sqlStatement = "CREATE TABLE T_GAMES("							\
		"GAME_ID	     INTEGER   PRIMARY KEY	AUTOINCREMENT NOT NULL	,"	\
		"STATUS			 INTEGER   NOT NULL								,"	\
		"START_TIME	     DATETIME  NOT NULL								,"  \
		"END_TIME        DATETIME)										;";

	res = sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		return false;

	// create questions table
	sqlStatement = "CREATE TABLE T_QUESTIONS("						\
		"QUESTION_ID	 INTEGER   PRIMARY KEY		AUTOINCREMENT NOT NULL,"	\
		"QUESTION		 TEXT						NOT NULL			  ,"	\
		"CORRECT_ANS	 TEXT						NOT NULL			  ,"	\
		"ANS2			 TEXT						NOT NULL			  ,"	\
		"ANS3			 TEXT						NOT NULL			  ,"	\
		"ANS4			 TEXT						NOT NULL)			  ;";

	res = sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		return false;

	// create answers table
	sqlStatement = "CREATE TABLE T_PLAYERS_ANSWERS("							 	\
		"PLAYER_ANSWER		TEXT						NOT NULL				,"	\
		"IS_CORRECT			INTEGER						NOT NULL				,"	\
		"GAME_ID			INTEGER						NOT NULL				,"  \
		"USERNAME			TEXT						NOT NULL				,"  \
		"QUESTION_ID		INTEGER						NOT NULL				,"  \
		"ANSWER_TIME		INTEGER						NOT NULL				,"  \
		"FOREIGN KEY(GAME_ID)		REFERENCES    T_GAMES(GAME_ID)				,"	\
		"FOREIGN KEY(USERNAME)		REFERENCES    T_USERS(USERNAME)				,"	\
		"FOREIGN KEY(QUESTION_ID)   REFERENCES    T_QUESTIONS(QUESTION_ID))		;";

	res = sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		return false;

	/* Add questions so games can start: */
	QuestionList ql; //the question list
	for (list<string>::iterator i = ql._list.begin(); i != ql._list.end(); i++)
	{
		res = sqlite3_exec(_sqldb, i->c_str(), nullptr, nullptr, &errMessage);
		if (res != SQLITE_OK) return false;
	}
	return true;
}

void DataBase::close()
{/* close the file and clear the private member that holds it, _sqldb. */
	RETURN_IF_INVALID;

	sqlite3_close(_sqldb);
	_sqldb = nullptr;
}

void DataBase::clear()
{/* clear the whole database, using DELETE commands. */
	RETURN_IF_INVALID;

	char *sqlStatement;
	char *errMessage = nullptr;

	sqlStatement = "DELETE FROM T_GAMES;"	\
		"DELETE FROM T_QUESTIONS;"				\
		"DELETE FROM T_USERS;"				\
		"DELETE FROM T_PLAYERS_ANSWERS;";

	sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
}

bool DataBase::fileExistsOnDisk(const string& filename)
{/* checks if a file called [filename] exists on disc, like the name says. */
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}

int DataBase::callbackUsers(void* param, int argc, char** argv, char** azColName)
{
	vector<string>* users = (static_cast<vector<string>*>(param));
	string question, corrAns, ans2, ans3, ans4;
	for (int i = 0; i < argc; i++) {
		if (string(azColName[i]).compare("USERNAME") == 0) {
			users->push_back(argv[i]);
		}
		return 0;
	}
	return 1;
}

int DataBase::callbackQuestions(void* param, int argc, char** argv, char** azColName)
{
	int id;
	string question, corrAns, ans2, ans3, ans4;
	for (int i = 0; i<argc; i++) {
		if (string(azColName[i]).compare("QUESTION_ID") == 0) {
			id = atoi(argv[i]);
		}
		else if (string(azColName[i]).compare("QUESTION") == 0) {
			question = argv[i];
		}
		else if (string(azColName[i]).compare("CORRECT_ANS") == 0) {
			corrAns = argv[i];
		}
		else if (string(azColName[i]).compare("ANS2") == 0) {
			ans2 = argv[i];
		}
		else if (string(azColName[i]).compare("ANS3") == 0) {
			ans3 = argv[i];
		}
		else if (string(azColName[i]).compare("ANS4") == 0) {
			ans4 = argv[i];
		}
	}
	Question* currentQuestion = new Question(id, question, corrAns, ans2, ans3, ans4);
	(static_cast<vector<Question*>*>(param))->push_back(currentQuestion);
	return 0;
}

int DataBase::sqlExecCallbackID(void* param, int argc, char** argv, char** azColName)
{/* The Callback function (for storing information recieved by SELECT in objects, used in the exec that calls SELECET) that
 covers the ID column only, if it are available (true for all callbacks). This will be primarily used to determine if an object 
 exists (because we only need to know if anything is returned)*/
	DataBase *dbAccess = (DataBase *)param;
	dbAccess->resetLastId();
	int i;
	for(i=0; i<argc; i++) {
		if (strstr(azColName[i],"ID") != nullptr) { //store in _lastid if it is the ID column
			dbAccess->setLastId(argv[i]);
		}
	}
	return 0;
}

int DataBase::sqlExecCallbackUM(void* param, int argc, char** argv, char** azColName)
{/* The Callback function (for storing information recieved by SELECT in objects, used in the exec that calls SELECET) that
 covers the ID column only, if it are available (true for all callbacks). This will be primarily used to determine if an object
 exists (because we only need to know if anything is returned)*/
	DataBase *dbAccess = (DataBase *)param;
	dbAccess->resetLastUM();
	int i;
	for (i = 0; i<argc; i++) {
		if (strstr(azColName[i], "USERNAME") != nullptr) { //store in _lastUM if it is the username column
			dbAccess->setLastUM(argv[i]);
		}
	}
	return 0;
}

int DataBase::callbackCount(void* params, int argc, char** argv, char** azColName)
{/* used to count albums and tags of a user (called until reaching end), similar to the last function.*/
	int *num = (int*)params;

	if (argc != 1)
		return 0;

	*num = atoi(argv[0]);

	return 0;
}