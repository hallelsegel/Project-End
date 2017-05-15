#pragma once
#include <list>
#include "CAlbum.h"
#include "CUser.h"
#include "CDataAccess.h"

struct sqlite3;

class CDatabaseAccess : public CDataAccess
{
private:
	albums _albums;
	users _users;
	sqlite3* _sqldb;
	int _lastId;
	CPicture _topTaggedPicture;

	albums _albumsOfUser;
	pictures _picturesOfUser;

	bool fileExistsOnDisk(const string& filename);
	bool initDatabase();

public:
	CDatabaseAccess();
	~CDatabaseAccess();

	bool open() override;
	void close() override;
	void clear() override;

	// album related
	const albums& getAlbums() override;
	const albums& getAlbumsOfUser(int userId) override;
	void insertAlbum(CAlbum& album) override;
	void deleteAlbum(string albumName) override;
	bool albumExists(string albumName) override;
	CAlbum* openAlbum(string albumName) override;
	void closeAlbum(CAlbum *pAlbum) override;

	// picture related
	void addPictureToAlbum(int albumId, CPicture& picture) override;
	void removePictureFromAlbum(int albumId, int pictureId) override;
	void tagUserInPicture(CPicture& picture, int userId) override;
	void untagUserInPicture(CPicture& picture, int userId) override;
	bool isUserTaggedInPicture(const CPicture& picture, int userId) override;

	// user related
	const users& getUsers() override;
	void addUser(CUser& user) override;
	void deleteUser(string userName) override;
	bool userExists(string userName) override;
	bool userExists(int userId) override;
	CUser* getUser(int userId) override;

	// user statistics
	int countAlbumsOwnedOfUser(int userId) override;
	int countAlbumsTaggedOfUser(int userId) override;
	int countTagsOfUser(int userId) override;
	float averageTagsPerAlbumOfUser(int userId) override;

	// queries
	const CUser* getTopTaggedUser() override;
	const CPicture* getTopTaggedPicture() override;
	const pictures& getTaggedPicturesOfUser(int userId) override;

	// sql related
	void setLastId(char* lastId);
	void resetLastId();
};
