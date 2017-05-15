#include "stdafx.h"
#include "sqlite3.h"
#include "CDatabaseAccess.h"

#define RETURN_IF_INVALID				if (_sqldb == nullptr) return;
#define RETURN_RES_IF_INVALID(res)		if (_sqldb == nullptr) return res;


static int sqlExecCallback(void* param, int argc, char** argv, char** azColName)
{/* The Callback function (for storing information recieved by SELECT in objects, used in the exec that calls SELECET) that
 covers the ID column only, if it are available (true for all callbacks). This will be primarily used to determine if an object 
 exists (because we only need to know if anything is returned)*/
	CDatabaseAccess *dbAccess = (CDatabaseAccess *)param;
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

static int listUsersCallback(void* param, int argc, char** argv, char** azColName)
{/* The Callback function (for storing information recieved by SELECT in objects) for the USERS table: takes ID and NAME, and stores them. */
	users *usersList = static_cast<users *>(param);

	CUser currentUser;

	for(int i=0; i<argc; i++) {
		if (string(azColName[i]).compare("ID") == 0) {
			currentUser.setId(atoi(argv[i]));
		}
		else if (string(azColName[i]).compare("NAME") == 0) {
			currentUser.setName(argv[i]);
		}
	}
	usersList->push_back(currentUser);

	return 0;
}

static int listAlbumsCallback(void* param, int argc, char** argv, char** azColName)
{/* The Callback function (for storing information recieved by SELECT in objects) for the ALBUMS table: takes ID, NAME
 CREATION_DATE and USER_ID and stored them. */
	albums *usersList = static_cast<albums *>(param);

	CAlbum currentAlbum;

	for(int i=0; i<argc; i++) {
		if (string(azColName[i]).compare("ID") == 0) {
			currentAlbum.setId(atoi(argv[i]));
		}
		else if (string(azColName[i]).compare("NAME") == 0) {
			currentAlbum.setName(argv[i]);
		}
		else if (string(azColName[i]).compare("CREATION_DATE") == 0)
		{
			currentAlbum.setCreationDate(argv[i]);
		}
		else if (string(azColName[i]).compare("USER_ID") == 0)
		{
			currentAlbum.setUserId(atoi(argv[i]));
		}
	}
	usersList->push_back(currentAlbum);

	return 0;
}

static int listPicturesCallback(void* param, int argc, char** argv, char** azColName)
{/* The Callback function (for storing information recieved by SELECT in objects) for the PICTURES table: takes ID, NAME
	LOCATION, and CREATION_DATE and stores them. */
	pictures *picsList = static_cast<pictures *>(param);

	CPicture currentPicture;

	for(int i=0; i<argc; i++) {
		if (string(azColName[i]).compare("ID") == 0) {
			currentPicture.setId(atoi(argv[i]));
		}
		else if (string(azColName[i]).compare("NAME") == 0) {
			currentPicture.setName(argv[i]);
		}
		else if (string(azColName[i]).compare("LOCATION") == 0) {
			currentPicture.setLocation(argv[i]);
		}
		else if (string(azColName[i]).compare("CREATION_DATE") == 0)
		{
			currentPicture.setCreationDate(argv[i]);
		}
	}
	picsList->push_back(currentPicture);

	return 0;
}

static int listUserTagsCallback(void* param, int argc, char** argv, char** azColName)
{/* The Callback function (for storing information recieved by SELECT in objects) for the TAGS table: takes USER_ID only and stores it. */
	set<int> *usersList = (set<int> *)param;

	for(int i=0; i<argc; i++) {
		if (string(azColName[i]).compare("USER_ID") == 0) {
			usersList->insert(atoi(argv[i]));
		}
	}

	return 0;
}

CDatabaseAccess::CDatabaseAccess() : _sqldb(nullptr)
{/* c'tor */
}

CDatabaseAccess::~CDatabaseAccess()
{/* d'tor */
	close();
}

const albums& CDatabaseAccess::getAlbums()
{/* regular SELECT sql command (sent using the exec function) from albums, stores all available albums. uses the callback 
	for albums from above. */
	_albums.clear();
	RETURN_RES_IF_INVALID(_albums);

	string sqlStatement;
	char *errMessage = nullptr;

	sqlStatement = "SELECT * FROM ALBUMS;";

	resetLastId();
	sqlite3_exec(_sqldb, sqlStatement.c_str(), listAlbumsCallback, &_albums, &errMessage);

	return _albums;
}

const albums& CDatabaseAccess::getAlbumsOfUser(int userId)
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


void CDatabaseAccess::insertAlbum(CAlbum& album)
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

void CDatabaseAccess::deleteAlbum(string albumName)
{/* DELETE an album from the ALBUMS table according to a selected name. */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;

	sqlStatement = "DELETE FROM ALBUMS WHERE NAME='" + albumName + "';";

	sqlite3_exec(_sqldb, sqlStatement.c_str(), nullptr, this, &errMessage);
}

bool CDatabaseAccess::albumExists(string albumName)
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

CAlbum* CDatabaseAccess::openAlbum(string albumName)
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

void CDatabaseAccess::closeAlbum(CAlbum* pAlbum)
{/*I guess this is only a placeholder because the RAM based class needed to close the objects, but we don't because its all in
 the file instead of the RAM (that needs to be cleaned) and they wanted this to work with the same files the original did. */
}

void CDatabaseAccess::addPictureToAlbum(int albumId, CPicture& picture)
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

void CDatabaseAccess::removePictureFromAlbum(int albumId, int pictureId)
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
	
void CDatabaseAccess::tagUserInPicture(CPicture& picture, int userId)
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

void CDatabaseAccess::untagUserInPicture(CPicture& picture, int userId)
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

bool CDatabaseAccess::isUserTaggedInPicture(const CPicture& picture, int userId)
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

const users& CDatabaseAccess::getUsers()
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

void CDatabaseAccess::addUser(CUser& user)
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

void CDatabaseAccess::deleteUser(string userName)
{/* DELETE a picture from the USERS table according to a selected name. */
	RETURN_IF_INVALID;

	string sqlStatement;
	char *errMessage = nullptr;

	sqlStatement = "DELETE FROM USERS WHERE NAME='" + userName + "';";

	sqlite3_exec(_sqldb, sqlStatement.c_str(), sqlExecCallback, this, &errMessage);
}

bool CDatabaseAccess::userExists(string userName)
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

bool CDatabaseAccess::userExists(int userId)
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

CUser* CDatabaseAccess::getUser(int userId)
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

int CDatabaseAccess::countAlbumsOwnedOfUser(int userId) 
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

int CDatabaseAccess::countAlbumsTaggedOfUser(int userId)
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

int CDatabaseAccess::countTagsOfUser(int userId)
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

float CDatabaseAccess::averageTagsPerAlbumOfUser(int userId)
{/* another non-sql function, calls the function that counts the tags of a user and returns the average tags per user,
	using the album-counting function (if there are any albums). */
	float albumsOfTaggedUser = static_cast<float>(countAlbumsTaggedOfUser(userId));
	if (albumsOfTaggedUser == 0)
		return albumsOfTaggedUser;

	return static_cast<float>(countTagsOfUser(userId)) / albumsOfTaggedUser;
}

const CUser* CDatabaseAccess::getTopTaggedUser()
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

const CPicture* CDatabaseAccess::getTopTaggedPicture()
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

const pictures& CDatabaseAccess::getTaggedPicturesOfUser(int userId)
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

void CDatabaseAccess::setLastId(char* lastId)
{/* setter for the private member _lastid */
	_lastId = atoi(lastId);
}

void CDatabaseAccess::resetLastId()
{/* resetter for the private member _lastid (insert -1) */
	_lastId = -1;
}

bool CDatabaseAccess::open()
{/* starter for the database manager, including: opening the gallery db file if it exists, and calling the initiator for it if not. */
	// check if file exists
	string filename = "galleryDB.sqlite";
	bool fileExisted = fileExistsOnDisk(filename);

	// try to open the database
	int res = sqlite3_open(filename.c_str(), &_sqldb);
	if (res != SQLITE_OK) {
		_sqldb = nullptr;
		return false;
	}

	// db is open, check if we need to init it
	if (fileExisted){

		/**			sql/c++ tutorial exercises:			**/
		char* errmsg = nullptr;
			//insertion:

		res = sqlite3_exec(_sqldb, "INSERT INTO USERS (NAME) VALUES ('Noam Batito'); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO ALBUMS (NAME, CREATION_DATE, USER_ID) VALUES ('itsa me batito', '06/04/17'"
			", 15); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('Bathitub'"
			", 'Batito's tub', 06/04/17, 12); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (31, 4); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (31, 7); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('Batito Salad'"
			", 'Batito's kitchen', 06/04/17, 12); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (32, 4); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (32, 3); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		//2nd user
		res = sqlite3_exec(_sqldb, "INSERT INTO USERS (NAME) VALUES ('Amit Malka'); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO ALBUMS (NAME, CREATION_DATE, USER_ID) VALUES ('Malka in da haus', '06/04/17'"
			", 16); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('bloddy ell'"
			", 'literally ell', 06/04/17, 13); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (33, 8); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (33, 2); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;

		res = sqlite3_exec(_sqldb, "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('Malkware'"
			", 'Hacker lab', 06/04/17, 13); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (34, 5); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (34, 6); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		//3rd user
		res = sqlite3_exec(_sqldb, "INSERT INTO USERS (NAME) VALUES ('Singsongpingpongdingdong'); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO ALBUMS (NAME, CREATION_DATE, USER_ID) VALUES ('So Small Mr child', '06/04/17'"
			", 17); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('lmao u dumb'"
			", 'low prio', 06/04/17, 14); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (35, 2); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (35, 1); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;

		res = sqlite3_exec(_sqldb, "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('Shamefur dispuray'"
			", 'high prio city', 06/04/17, 14); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (36, 3); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (36, 6); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;

		//updating
		res = sqlite3_exec(_sqldb, "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES ('My Femily'"
			", 'Our Home <3', 06/04/17, 6); ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "UPDATE PICTURES set NAME = 'My Family' where PREFIX='My Femily'; ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "UPDATE PICTURES set LOCATION = 'C:\\Users\\User\\Documents\\magshimim\\marieCuriePics\\MarieCurie3.jpg' where ID=20; ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;


		//Deletion
		res = sqlite3_exec(_sqldb, "DELETE FROM USERS where NAME='Amit Malka'; ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "DELETE FROM ALBUMS where NAME='Malka in da haus'; ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		res = sqlite3_exec(_sqldb, "DELETE FROM PICTURES where ALBUM_ID=13; ", nullptr, nullptr, &errmsg);
		if (res != SQLITE_OK) return false;
		/**	Done **/
		
		
		
		return true;
	}

	// need to init, lets create the tables and some other stuff...
	return initDatabase();
}

void CDatabaseAccess::close()
{/* close the file and clear the private member that holds it, _sqldb. */
	RETURN_IF_INVALID;

	sqlite3_close(_sqldb);
	_sqldb = nullptr;
}

void CDatabaseAccess::clear()
{/* clear the whole database, using DELETE commands. */
	RETURN_IF_INVALID;

	char *sqlStatement;
	char *errMessage = nullptr;

	sqlStatement = "DELETE FROM ALBUMS;"	\
		"DELETE FROM PICTURES;"				\
		"DELETE FROM USERS;"				\
		"DELETE FROM TAGS;";

	sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
}

bool CDatabaseAccess::fileExistsOnDisk(const string& filename)
{/* checks if a file called [filename] exists on disc, like the name says. */
	struct stat buffer;   
	return (stat(filename.c_str(), &buffer) == 0); 
}

bool CDatabaseAccess::initDatabase()
{ /* Initiates the database in case it didn't exist beforehand, meaning creating all of the tables. */
	char *sqlStatement;
	char *errMessage = nullptr;
	int res;

	// create Users table
	sqlStatement = "CREATE TABLE USERS("				\
		"ID    INTEGER   PRIMARY KEY   AUTOINCREMENT,"	\
		"NAME  TEXT					NOT NULL);";

	res = sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		return false;

	// create Albums table
	sqlStatement = "CREATE TABLE ALBUMS("							\
		"ID			     INTEGER   PRIMARY KEY	AUTOINCREMENT,"   	\
		"NAME			 TEXT					NOT NULL,"			\
		"CREATION_DATE   TEXT					NOT NULL,"          \
		"USER_ID         INT                    NOT NULL,"          \
		"FOREIGN KEY(USER_ID)      REFERENCES   USER(ID));";

	res = sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		return false;



	// create Pictures table
	sqlStatement = "CREATE TABLE PICTURES("							\
         "ID			 INTEGER   PRIMARY KEY		AUTOINCREMENT,"	\
         "NAME			 TEXT						NOT NULL,"		\
         "LOCATION		 TEXT						NOT NULL,"		\
		 "CREATION_DATE  TEXT						NOT NULL,"		\
         "ALBUM_ID		 INT						NOT NULL,"      \
		 "FOREIGN KEY(ALBUM_ID)    REFERENCES       ALBUM(ID));";

	res = sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		return false;

	// create Tags table
	sqlStatement = "CREATE TABLE TAGS("						     	\
         "ID			INTEGER      PRIMARY KEY   AUTOINCREMENT,"	\
         "PICTURE_ID	INT						   NOT NULL,"		\
         "USER_ID		INT						   NOT NULL,"       \
		 "FOREIGN KEY(PICTURE_ID)    REFERENCES    PICTURES(ID),"    \
	     "FOREIGN KEY(USER_ID)       REFERENCES    USERS(ID));";

	res = sqlite3_exec(_sqldb, sqlStatement, nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
		return false;

	return true;
}
