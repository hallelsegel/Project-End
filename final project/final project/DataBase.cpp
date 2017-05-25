#include "stdafx.h"
#include "sqlite3.h"
#include "DataBase.h"
#include <ctime>
#define RETURN_IF_INVALID				if (_sqldb == nullptr) return;
#define RETURN_RES_IF_INVALID(res)		if (_sqldb == nullptr) return res;


DataBase::DataBase()
{
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

	resetLastId();
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return _lastId != -1;
}

bool DataBase::addNewUser(string username, string password, string email)
{/* adds a new row to the T_USERS table with these values. */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	char buff[10];

	sqlStatement = "INSERT INTO T_USERS (USERNAME, PASSWORD, EMAIL) "	\
		"VALUES ('" + username + "', '" + password + "', " + email + "); "	\
		"SELECT * FROM T_USERS WHERE USERNAME='" + username + "';";

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	if (_lastId == -1)
		return false;
	return true;
}

bool DataBase::isUserAndPassMatch(string username, string password)
{/* Checks if a user with this username AND password exists in the database. */
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "SELECT * FROM T_USERS WHERE USERNAME='" + username + "' AND WHERE PASSWORD='" + password + "';";

	resetLastId();
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return _lastId != -1;
}

vector<Question*> DataBase::initQuestion(int questionNo)
{/* initiates a vector with a [questionNo] number of questions drawn randomly from the database. */
	vector<Question*> questionsV;
	string sqlStatement;
	char *errMessage = nullptr;
	char buff[10];

	sqlStatement = "SELECT * FROM T_QUESTIONS ORDER BY RANDOM() LIMIT " + string(_itoa(questionNo, buff, 10)) + ";";
	resetLastId();
	sqlite3_exec(_sqldb, sqlStatement.c_str(), callbackQuestions, questionsV, &errMessage);
	
	return;
}

int DataBase::insertNewGame()
{
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	char buff[10];
	sqlStatement = "INSERT INTO T_GAMES (STATUS, START_TIME, END_TIME) "	\
		"VALUES (0, time('now'), time('now'); "	\
		"SELECT * FROM T_GAMES WHERE START_TIME=time('now');";  //time('now') is the current time (h/m/s), is temporarily inserted to END_TIME.

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	if (_lastId == -1)
		return false;
	return true;
}

bool DataBase::updateGameStatus(int gameId)
{
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	int res;
	char buff[10];
	
	sqlStatement = "UPDATE T_GAMES SET END_TIME = date('now') WHERE GAME_ID = " + string(_itoa(gameId, buff, 10)) + ";";
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	if (_lastId == -1)
		return false;
	return true;
}

bool DataBase::addAnswerToPlayer(int gameId, string username, int questionId, string answer, bool isCorrect, int answerTime)
{
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	int res;
	char buff[10];
	
	sqlStatement = "INSERT INTO T_PLAYERS_ANSWERS (PLAYER_ANSWER, IS_CORRECT, GAME_ID, USERNAME, QUESTION_ID, ANSWER_TIME)" \
		"VALUES(" + answer + ", " + string(_itoa(isCorrect, buff, 10)) + ", " + string(_itoa(gameId, buff, 10)) + ", "		\
		+ username + ", " + string(_itoa(questionId, buff, 10)) + ", " + string(_itoa(answerTime, buff, 10)) + "); "		\
		+ "SELECT * FROM T_PLAYERS_ANSWETS WHERE GAME_ID = " + string(_itoa(gameId, buff, 10)) + "AND WHERE USERNAME ="		\
		+ username + "AND WHERE QUESTION_ID =" + string(_itoa(questionId, buff, 10)) +									 ";";
	//first insert with all required values and then call select where its the right question for the right user in the right
	//game to check success.
	
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	if (_lastId == -1)
		return false;
	return true;
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
		"ANS4			 TEXT						NOT NULL			  ;";

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

	return true;
}

void DataBase::setLastId(char* lastId)
{/* setter for the private member _lastid */
	_lastId = atoi(lastId);
}

void DataBase::resetLastId()
{/* resetter for the private member _lastid (insert -1) */
	_lastId = -1;
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

	// db is open, check if we need to init it
	if (fileExisted){

		char* errmsg = nullptr;
		//insertion:

		res = sqlite3_exec(_sqldb, "INSERT INTO USERS (NAME) VALUES ('Noam Batito'); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		//updating
		res = sqlite3_exec(_sqldb, "DELETE FROM PICTURES where ALBUM_ID=13; ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		/**	Done **/



		return true;
	}

	// need to init, lets create the tables and some other stuff...
	return initDatabase();
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


int DataBase::callbackCount(void* param, int argc, char** argv, char** azColName)
{
	string sqlStatement;
	char *errMessage = nullptr;
	int res;
	char buff[10];

	sqlStatement = "SELECT COUNT(*) FROM TAGS WHERE USER_ID=" + string(_itoa(userId, buff, 10)) + ";";

	int albumsCount;

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlInt, &albumsCount, &errMessage);
	if (res != SQLITE_OK)
		return -1;

	return albumsCount;
}

int DataBase::callbackQuestions(void* param, int argc, char** argv, char** azColName)
{
	questions *questionsList = static_cast<questions *>(param);
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
	Question currentQuestion(id, question, corrAns, ans2, ans3, ans4);
	questionsList->push_back(currentQuestion);
	return 0;
}

int DataBase::callbackBestScores(void* param, int argc, char** argv, char** azColName)
{

}

int DataBase::callbackPersonalStatus(void* param, int argc, char** argv, char** azColName)
{

}

static int sqlExecCallback(void* param, int argc, char** argv, char** azColName)
{/* The Callback function (for storing information recieved by SELECT in objects, used in the exec that calls SELECET) that
 covers the ID column only, if it are available (true for all callbacks). This will be primarily used to determine if an object 
 exists (because we only need to know if anything is returned)*/
	DataBase *dbAccess = (DataBase *)param;
	dbAccess->resetLastId();

	int i;
	for(i=0; i<argc; i++) {
		if (string(azColName[i]).compare("ID") == 0) { //store in _lastid if it is the ID column
			dbAccess->setLastId(argv[i]);
		}
	}
	return 0;
}

static int sqlTopId(void* param, int argc, char** argv, char** azColName)
{/* used to determine highest value for the "top tagged user/picture" commands (used on every one, if higher value
	new one is saved).	this is a like a callback function, but its for using COUNT. */
	pair<int, int> *topId = (pair<int, int> *)param; 
	if (argc != 2) 
		return 0;

	int curCount = atoi(argv[1]);
	if (curCount < topId->second) //check for higher count
		return 0;

	topId->first = atoi(argv[0]);
	topId->second = curCount;

		return 0;
}

static int sqlInt(void* params, int argc, char** argv, char** azColName)
{/* used to count albums and tags of a user (called until reaching end), similar to the last function.*/
	int *num = (int*)params;

	if (argc != 1)
		return 0;

	*num = atoi(argv[0]);

	return 0;
}


const albums& DataBase::getAlbumsOfUser(int userId)
{/* SELECT albums of a certain user, using the WHERE sql command (sent using the exec function) from albums. */
	_albumsOfUser.clear();
	RETURN_RES_IF_INVALID(_albums);

	string sqlStatement;
	char *errMessage = nullptr;

	char buff[10];
	sqlStatement = "SELECT * FROM ALBUMS WHERE USER_ID=" + string(_itoa(userId, buff, 10)) + ";"; //sql command with the chosen username 
	//implemented in.

	resetLastId();
	sqlite3_exec(_sqldb, sqlStatement.c_str(), listAlbumsCallback, &_albumsOfUser, &errMessage);

	return _albumsOfUser;
}


void DataBase::insertAlbum(CAlbum& album)
{/* INSERT an album to the ALBUMS table, using the sql command and a CAlbum object that the information
	for the album comes from. Using SELECT after with the minimal callback (the first) to determine the INSERT success. */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	album.setCreationDateNow();

	char buff[10];

	sqlStatement = "INSERT INTO ALBUMS (NAME, CREATION_DATE, USER_ID) "	\
        "VALUES ('" + album.getName() + "', '" + album.getCreationDate() + "', " + string(_itoa(album.getUserId(), buff, 10)) + "); "	\
		"SELECT * FROM ALBUMS WHERE NAME='" + album.getName() + "';";

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return;

	if (_lastId == -1)
		return;

	album.setId(_lastId);
}

void DataBase::deleteAlbum(string albumName)
{/* DELETE an album from the ALBUMS table according to a selected name. */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;

	sqlStatement = "DELETE FROM ALBUMS WHERE NAME='" + albumName + "';";

	sqlite3_exec(_sqldb, sqlStatement.c_str(), nullptr, this, &errMessage);
}

bool DataBase::albumExists(string albumName)
{/* same as used in the INSERT function, determines if the album exists using the minimal callback function. */
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "SELECT * FROM ALBUMS WHERE NAME='" + albumName + "';";

	resetLastId();
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return _lastId != -1;
}

CAlbum* DataBase::openAlbum(string albumName)
{/* Preperation of a certain album for the album-specific-function: checks the album's validity and queries for all of its
	pictures and tags. */
	RETURN_RES_IF_INVALID(nullptr);

	getAlbums();
	if (_albums.size() == 0)
		return nullptr;

	albums_iter iter = _albums.begin();
	while (iter != _albums.end())
	{
		if (iter->getName().compare(albumName) == 0)
			break;
		++iter;
	}

	if (iter == _albums.end())
		return nullptr;

	string sqlStatement;
	char *errMessage = nullptr;

	char buff[10];
	sqlStatement = "SELECT * FROM PICTURES WHERE ALBUM_ID=" + string(_itoa(iter->getId(), buff, 10)) + ";";
	sqlite3_exec(_sqldb, sqlStatement.c_str(), listPicturesCallback, &iter->getPicturesRef(), &errMessage);

	pictures& pictures = iter->getPicturesRef();
	pictures_iter picIter = pictures.begin();
	for (; picIter != pictures.end(); ++picIter)
	{
		sqlStatement = "SELECT * FROM TAGS WHERE PICTURE_ID=" + string(_itoa(picIter->getId(), buff, 10)) + ";";
		sqlite3_exec(_sqldb, sqlStatement.c_str(), listUserTagsCallback, &picIter->getUserTagsRef(), &errMessage);
	}

	return &*iter;
}

void DataBase::closeAlbum(CAlbum* pAlbum)
{/*I guess this is only a placeholder because the RAM based class needed to close the objects, but we don't because its all in
 the file instead of the RAM (that needs to be cleaned) and they wanted this to work with the same files the original did. */
}

void DataBase::addPictureToAlbum(int albumId, CPicture& picture)
{/* add a picture, using the INSERT sql command, and a CPicture object for the values of the new line in the PICTURES table,
	except for ALBUM_ID that is given as parameter. also checks if it exists for confirmation usingthe minimal callback function*/
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	picture.setCreationDateNow();

	char buff[10];
	sqlStatement = "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ("	\
		"'" + picture.getName() + "', "					\
		"'" + picture.getLocation() + "', "				\
        "'" + picture.getCreationDate() + "', "			\
        "" + string(_itoa(albumId, buff, 10)) + ");"	\
		"SELECT * FROM PICTURES WHERE NAME='" + picture.getName() + "';";

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return;

	if (_lastId == -1)
		return;

	picture.setId(_lastId);

	albums_iter iter = _albums.begin();
	while (iter != _albums.end())
	{
		if (iter->getId() == albumId)
			iter->addPicture(picture);
		++iter;
	}
}

void DataBase::removePictureFromAlbum(int albumId, int pictureId)
{/* DELETE an picture from the PICTURES table according to a selected name, then deletes all the tags that had to do
	with it (since it doesn't exist anymore). */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	char buff[10];
	sqlStatement = "DELETE FROM PICTURES WHERE ID=" + string(_itoa(pictureId, buff, 10)) + ";";
	sqlite3_exec(_sqldb, sqlStatement.c_str(), nullptr, this, &errMessage);

	sqlStatement = "DELETE FROM TAGS WHERE PICTURE_ID=" + string(_itoa(pictureId, buff, 10)) + ";";
	sqlite3_exec(_sqldb, sqlStatement.c_str(), nullptr, this, &errMessage);
}
	
void DataBase::tagUserInPicture(CPicture& picture, int userId)
{/* adds a tag of a user using the INSERT commend for the TAGS table and a given int for the user id. */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	char buff[10];
	string picIdStr = string(_itoa(picture.getId(), buff, 10));
	string userIdStr = string(_itoa(userId, buff, 10));

	sqlStatement = "INSERT INTO TAGS (PICTURE_ID, USER_ID) "	\
        "VALUES (" + picIdStr + ", " + userIdStr + ");";

	sqlite3_exec(_sqldb, sqlStatement.c_str(), nullptr, this, &errMessage);

	picture.tagUser(userId);
}

void DataBase::untagUserInPicture(CPicture& picture, int userId)
{/* remove a tag from a user, which is just to DELETE it from the table. also checks if it still exists 
 for confirmation usingthe minimal callback function.*/
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	char buff[10];
	string picIdStr = string(_itoa(picture.getId(), buff, 10));
	string userIdStr = string(_itoa(userId, buff, 10));

	sqlStatement = "DELETE FROM TAGS WHERE PICTURE_ID=" + picIdStr + " AND USER_ID=" + userIdStr + ";";

	sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);

	picture.untagUser(userId);
}

bool DataBase::isUserTaggedInPicture(const CPicture& picture, int userId)
{/* checks if a user is tagged in a picture, which means checking if a tag that points to the given picture id and the given user id
 exists using the minimal callback function*/
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	char buff[10];

	resetLastId();
	sqlStatement = "SELECT * FROM TAGS WHERE PICTURE_ID=" + string(_itoa(picture.getId(), buff, 10)) + " AND USER_ID=" + string(_itoa(userId, buff, 10)) + ";";
	sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);

	return _lastId != -1;
}

const users& DataBase::getUsers()
{/* regular SELECT command on the USERS table, queries for all available users and stores them in CUser objects
 using the user callback function. */
	_users.clear();
	RETURN_RES_IF_INVALID(_users);

	string sqlStatement;
	char *errMessage = nullptr;

	sqlStatement = "SELECT * FROM USERS;";

	resetLastId();
	sqlite3_exec(_sqldb, sqlStatement.c_str(), listUsersCallback, &_users, &errMessage);

	return _users;
}

void DataBase::addUser(CUser& user)
{/* INSERT a user to the USERS table, using the sql command and a CUser object that the information
	for the user comes from. Using SELECT after with the minimal callback to determine the INSERT success. */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "INSERT INTO USERS (NAME) "	\
        "VALUES ('" + user.getName() + "');"	\
		"SELECT * FROM USERS WHERE NAME='" + user.getName() + "';";

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return;

	if (_lastId == -1)
		return;

	user.setId(_lastId);
}

void DataBase::deleteUser(string userName)
{/* DELETE a picture from the USERS table according to a selected name. */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;

	sqlStatement = "DELETE FROM USERS WHERE NAME='" + userName + "';";

	sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
}

bool DataBase::userExists(string userName)
{/* Checks if a user exists using the minimal callback function using his name as a string. */
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "SELECT * FROM USERS WHERE NAME='" + userName + "';";

	resetLastId();
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return _lastId != -1;
}

bool DataBase::userExists(int userId)
{/* Checks if a user exists using the minimal callback function using his id (int). */
	RETURN_RES_IF_INVALID(false);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	char buff[10];
	sqlStatement = "SELECT * FROM USERS WHERE ID=" + string(_itoa(userId, buff, 10)) + ";";

	resetLastId();
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return _lastId != -1;
}

CUser* DataBase::getUser(int userId)
{/* actual non-sql function! gets the id of the wanted user and returns a pointer to the right one from the users object-list. */
	RETURN_RES_IF_INVALID(nullptr);

	getUsers();
	if (_users.size() == 0)
		return nullptr;

	users_iter iter = _users.begin();
	while (iter != _users.end())
	{
		if (iter->getId() == userId)
			return &*iter;
		++iter;
	}

	return nullptr;
}

int DataBase::countAlbumsOwnedOfUser(int userId) 
{/* Count the albums of a certain user by using the exec with the sqlInt function from above.*/
	RETURN_RES_IF_INVALID(-1);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;
	char buff[10];

	sqlStatement = "SELECT COUNT(*) FROM ALBUMS WHERE USER_ID=" + string(_itoa(userId, buff, 10)) + ";";

	int albumsCount;

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlInt, &albumsCount, &errMessage);
	if (res != SQLITE_OK)
		return -1;

	return albumsCount;
}

int DataBase::countAlbumsTaggedOfUser(int userId)
{/* tags to a certain user in the his albums by using the exec with the sqlInt function from above. */
	RETURN_RES_IF_INVALID(-1);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;
	char buff[10];

	sqlStatement = "SELECT COUNT(DISTINCT ALBUM_ID) FROM ALBUMS "                    \
		           "INNER JOIN PICTURES ON PICTURES.ALBUM_ID = ALBUMS.ID "			 \
				   "LEFT JOIN TAGS ON PICTURES.ID = TAGS.PICTURE_ID "				 \
				   "WHERE TAGS.USER_ID =" + string(_itoa(userId, buff, 10)) + ";";

	int albumsCount;

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlInt, &albumsCount, &errMessage);
	if (res != SQLITE_OK)
		return -1;

	return albumsCount;
}

int DataBase::countTagsOfUser(int userId)
{/* Count the tags to a certain user by using the exec with the sqlInt function from above. */
	RETURN_RES_IF_INVALID(-1);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;
	char buff[10];

	sqlStatement = "SELECT COUNT(*) FROM TAGS WHERE USER_ID=" + string(_itoa(userId, buff, 10)) + ";";

	int albumsCount;

	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlInt, &albumsCount, &errMessage);
	if (res != SQLITE_OK)
		return -1;

	return albumsCount;
}

float DataBase::averageTagsPerAlbumOfUser(int userId)
{/* another non-sql function, calls the function that counts the tags of a user and returns the average tags per user,
	using the album-counting function (if there are any albums). */
	float albumsOfTaggedUser = static_cast<float>(countAlbumsTaggedOfUser(userId));
	if (albumsOfTaggedUser == 0)
		return albumsOfTaggedUser;

	return static_cast<float>(countTagsOfUser(userId)) / albumsOfTaggedUser;
}

const CUser* DataBase::getTopTaggedUser()
{/* counts tags for the users and returns the one with the most, using the sqlTopId function from the start. */
	RETURN_RES_IF_INVALID(nullptr);

	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "select USER_ID, count(*) from tags group by USER_ID;";

	pair<int, int> tagsPair;
	tagsPair.first = -1;
	tagsPair.second = -1;
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlTopId, &tagsPair, &errMessage);
	if (res != SQLITE_OK)
		return nullptr;

	if (tagsPair.first == -1)
		return nullptr;

	return getUser(tagsPair.first);
}

const CPicture* DataBase::getTopTaggedPicture()
{/* counts tags for the all pictures and returns the one with the most, using the sqlTopId function from the start. also 
	queries (using the pictures callback) for that picture and puts it in the private member _topTaggedPicture. */
	RETURN_RES_IF_INVALID(nullptr);
	
	string sqlStatement;
	char *errMessage = nullptr;
	int res;

	sqlStatement = "select PICTURE_ID, count(*) from tags group by PICTURE_ID;";

	pair<int, int> tagsPair;
	tagsPair.first = -1;
	tagsPair.second = -1;
	res = sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlTopId, &tagsPair, &errMessage);
	if (res != SQLITE_OK)
		return nullptr;

	if (tagsPair.first == -1)
		return nullptr;

	char buff[10];
	pictures picturesList;
	sqlStatement = "SELECT * FROM PICTURES WHERE ID=" + string(_itoa(tagsPair.first, buff, 10)) + ";";
	sqlite3_exec(_sqldb, sqlStatement.c_str(), listPicturesCallback, &picturesList, &errMessage); //query for the picture (in case it wasn't queried yet?)
	if (res != SQLITE_OK)
		return nullptr;

	if (picturesList.size() == 0)
		return nullptr;

	pictures_iter iter = picturesList.begin();
	while (iter != picturesList.end())
	{
		if (iter->getId() == tagsPair.first) //go through the pictures in the list until you find the one you just queried for
		{
			_topTaggedPicture = *iter;
			return &_topTaggedPicture; //insert to the private member _topTaggedPicture
		}
		++iter;
	}

	return nullptr;
}

const pictures& DataBase::getTaggedPicturesOfUser(int userId)
{/* queries for pictures that are tagged to a certain user using the pictures callback function. */
	_picturesOfUser.clear();
	RETURN_RES_IF_INVALID(_picturesOfUser);

	string sqlStatement;
	char *errMessage = nullptr;

	char buff[10];
	sqlStatement = "SELECT PICTURES.* FROM PICTURES "                                 \
			       "LEFT JOIN TAGS ON PICTURES.ID = TAGS.PICTURE_ID "				 \
		           "WHERE TAGS.USER_ID =" + string(_itoa(userId, buff, 10)) + ";";

	int res = sqlite3_exec(_sqldb, sqlStatement.c_str(), listPicturesCallback, &_picturesOfUser, &errMessage);

	return _picturesOfUser;
}
