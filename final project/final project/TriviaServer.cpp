#include "TriviaServer.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include "Helper.h"
#include <iostream>
#include <chrono>
#include <mutex>

//CV:
#include <condition_variable>
std::condition_variable cv;

using namespace std;

TriviaServer::TriviaServer()
{
	// notice that we step out to the global namespace
	// for the resolution of the function socket
	
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	_serverSocket = ::socket(AF_INET,  SOCK_STREAM,  IPPROTO_TCP); 
	_roomIdSequence = 0;
	if (_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
	_db = DataBase();
	if (!_db.open())
		throw exception("Unable to open database");
	thread msgHandler(&TriviaServer::handleRecievedMessages, this); //start a thread that will handle the messages in _queRcvMessages
	msgHandler.detach();
}

TriviaServer::~TriviaServer()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		::closesocket(_serverSocket);
	}
	catch (...) {}
}

void TriviaServer::serve(int port)
{
	
	bindAndListen(port);
	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		cout << "Waiting for client connection request" << endl;
		accept();
	}
}

void TriviaServer::accept()
{
	// this accepts the client and create a specific socket from server to this client
	SOCKET client_socket = ::accept(_serverSocket, NULL, NULL);

	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	cout << "Client accepted. Server and client can speak" << endl;

	// the function that handle the conversation with the client
	thread t(&TriviaServer::clientHandler, this, client_socket);
	t.detach();
}

void TriviaServer::bindAndListen(int port)
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (::bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (::listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	cout << "Listening on port " << port << endl;

}

void TriviaServer::clientHandler(SOCKET clientSocket)
{
	int msgCode = Helper::getMessageTypeCode(clientSocket);
	while (msgCode != 0 && msgCode != 299)
	{
		RecievedMessage* msg = buildRecieveMessage(clientSocket,msgCode);
		addRecievedMessage(msg);
		msgCode = Helper::getMessageTypeCode(clientSocket);
	}
	try
	{		
		// Closing the socket (in the level of the TCP protocol)
		closesocket(clientSocket); 
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		closesocket(clientSocket);
	}
}

/*		User connection functions		*//////////////////////////////////////////////////////////////////////

bool TriviaServer::handleSignup(RecievedMessage* msg) //203
{
	string username = msg->getValues()[0];
	string password = msg->getValues()[1];
	string email = msg->getValues()[2];
	if (Validator::isPasswordValid(password) == false)			Helper::sendData(msg->getSock(), SERVER_SIGN_UP_PASS_ILEGAL);
	else if (Validator::isUsernameValid(username) == false)		Helper::sendData(msg->getSock(), SERVER_SIGN_UP_USERNAME_ILEGAL);
	else if (_db.isUserExists(username))						Helper::sendData(msg->getSock(), SERVER_SIGN_UP_USERNAME_ALLREDY_EXIST);
	else if (_db.addNewUser(username, password, email))			{
		Helper::sendData(msg->getSock(), SERVER_SIGN_UP_SUCCESS);
		return true;
	}
	else Helper::sendData(msg->getSock(), SERVER_SIGN_UP_OTHER);
	return false;
}

User* TriviaServer::handleSignin(RecievedMessage* msg) //200
{
	string username = msg->getValues()[0];
	string password = msg->getValues()[1];
	SOCKET socket = msg->getSock();
	if (!_db.isUserAndPassMatch(username, password))		{
		Helper::sendData(msg->getSock(), SERVER_SIGN_IN_WRONG_DETAILS);
		return nullptr;
	}
	if (getUserByName(username) != nullptr)					{
		Helper::sendData(msg->getSock(), SERVER_SIGN_IN_ALLREADY_CONNECTAED);
		return nullptr;
	}
	User* u = new User(username, socket);
	_connectedUsers.insert(pair<SOCKET, User*>(socket, u));
	if (username == "Admin" && password == "A1d2m3i4n5")
		u->send(SERVER_SIGN_IN_SUCCESS_ADMIN);
	else
		u->send(SERVER_SIGN_IN_SUCCESS);
	return u;
}

void TriviaServer::handleSignout(RecievedMessage* msg) //201
{
	SOCKET sock = msg->getSock();
	map<SOCKET, User*>::iterator it;
	for (it = _connectedUsers.begin(); it->first != sock; it++){}
	if (it != _connectedUsers.end())
	{
		if (it->second->getRoom()) {
			handleLeaveRoom(msg);
			handleCloseRoom(msg);
		}
		if (it->second->getGame()) handleLeaveGame(msg);
		_connectedUsers.erase(it);
	}
	else cout << "No such user." << endl; //error info
}

void TriviaServer::safeDeleteUser(RecievedMessage* msg) //299
{
	try
	{
		handleSignout(msg);
		closesocket(msg->getSock());
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
}

/*		Game related functions		*//////////////////////////////////////////////////////////////////////

void TriviaServer::handleLeaveGame(RecievedMessage* msg) //222
{
	bool leaveSuccess = msg->getUser()->leaveGame();
	if (leaveSuccess)
	{
		delete msg->getUser()->getGame();
	}
}

void TriviaServer::handleStartGame(RecievedMessage* msg) //217
{
	Room* room = msg->getUser()->getRoom();
	Game* newGame = nullptr;
	try
	{
		newGame = new Game(room->getUsers(), room->getQuestionNo(), _db);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
	for (int i = 0; i < (int)room->getUsers().size(); i++)
	{
		room->getUsers()[i]->setGame(newGame);
	}
	map<int, Room*>::iterator it;
	for (it = _roomsList.begin(); it->second != room; it++){} //find the room in the map
	if (it != _roomsList.end())
	{
		_roomsList.erase(it);
		_roomIdSequence--;
	}
	newGame->sendFirstQuestion();
}

/*		Room related functions		*//////////////////////////////////////////////////////////////////////

bool TriviaServer::handleCreateRoom(RecievedMessage* msg) //213
{
	User* user = msg->getUser();
	if (user == NULL) return false; //no user found
	string roomName = msg->getValues()[0];
	int maxUsers = atoi(msg->getValues()[1].c_str());
	int questionsNo = atoi(msg->getValues()[2].c_str());
	int questionTime = atoi(msg->getValues()[3].c_str());
	if (user->createRoom(++_roomIdSequence, roomName, maxUsers, questionsNo, questionTime))
	{
		_roomsList.insert(pair<int, Room*>(_roomIdSequence, user->getRoom()));
		return true; //success, the code was already sent in user->createRoom();
	}
	return false;
}

bool TriviaServer::handleJoinRoom(RecievedMessage* msg) //209
{
	User* user = msg->getUser();
	if (user == NULL) return false; //no user found
	int roomId = atoi(msg->getValues()[0].c_str());
	Room* room = getRoomById(roomId);
	if (room == NULL) user->send(SERVER_JOIN_ROOM_NOT_EXIST);
	if (user->joinRoom(room))
	{
		_roomsList.insert(pair<int, Room*>(_roomIdSequence, user->getRoom()));
		return true; //success
	}
	return false;
}

bool TriviaServer::handleLeaveRoom(RecievedMessage* msg) //211
{
	User* user = msg->getUser();
	if (user == NULL) return false; //no user found

	Room* room = user->getRoom();
	map<int, Room*>::iterator it;
	for (it = _roomsList.begin(); it->second != room; it++){}
	if (it != _roomsList.end())
	{
		user->leaveRoom();
		return true;
	}
	else return false; //no such room
}

bool TriviaServer::handleCloseRoom(RecievedMessage* msg) //215
{
	User* user = msg->getUser();
	if (user == NULL) return false; //no user found

	Room* room = user->getRoom();
	map<int, Room*>::iterator it;
	for (it = _roomsList.begin(); it->second != room; it++){}
	if (it != _roomsList.end())
	{
		if (user->closeRoom() != -1)
			_roomsList.erase(it);
		_roomIdSequence--;
		return true;
	}
	else return false; //no such room
}

void TriviaServer::handleGetUsersInRoom(RecievedMessage* msg) //207
{
	int roomId = atoi(msg->getValues()[0].c_str());
	Room* room = getRoomById(roomId);
	string sendMsg = SERVER_USER_IN_ROOM;
	if (room)
	{
		sendMsg = room->getUsersListMessage();
	}
	else sendMsg += "0";
	msg->getUser()->send(sendMsg);
}

void TriviaServer::handleGetRooms(RecievedMessage* msg) //205
{
	string sendMsg = SERVER_GET_EXIST_ROOMS;

	sendMsg += Helper::getPaddedNumber(_roomIdSequence, 4);
	for (int i = 0; i < _roomIdSequence; i++)
	{
		int id = _roomsList[i+1]->getId();
		sendMsg += Helper::getPaddedNumber(id, 4);	//room id
		sendMsg += Helper::getPaddedNumber(_roomsList.find(id)->second->getName().length(), 2); //room name length
		sendMsg += _roomsList.find(id)->second->getName();
	}
	msg->getUser()->send(sendMsg);
}

void TriviaServer::handleGetBestScores(RecievedMessage* msg) //223
{
	map<int, string> bestScores = _db.getBestScores();
	map<int, string> ::iterator it = bestScores.begin();
	string sendMsg = SERVER_BEST_SCORES;
	if (bestScores.size() == 0) sendMsg += "00"; //size 0 = no more users (none)
	else if (bestScores.size() == 1)
	{
		sendMsg = sendMsg + Helper::getPaddedNumber(it->second.size(), 2) + it->second + Helper::getPaddedNumber(it->first, 3);
		sendMsg += "00"; //size 0 = no more users 
	}
	else if (bestScores.size() == 2)
	{
		sendMsg = sendMsg + Helper::getPaddedNumber(it->second.size(), 2) + it->second + Helper::getPaddedNumber(it->first, 3); 
		it++; //move 'it' forward once
		sendMsg = sendMsg + Helper::getPaddedNumber(it->second.size(), 2) + it->second + Helper::getPaddedNumber(it->first, 3);
		sendMsg += "00"; //size 0 = no more users 
	}
	else
	{
		sendMsg = sendMsg + Helper::getPaddedNumber(it->second.size(), 2) + it->second + Helper::getPaddedNumber(it->first, 3);
		it++; //move 'it' forward once
		sendMsg = sendMsg + Helper::getPaddedNumber(it->second.size(), 2) + it->second + Helper::getPaddedNumber(it->first, 3);
		it++; //move 'it' forward once
		sendMsg = sendMsg + Helper::getPaddedNumber(it->second.size(), 2) + it->second + Helper::getPaddedNumber(it->first, 3);
	}
	msg->getUser()->send(sendMsg);
}

void TriviaServer::handleGetPersonalStatus(RecievedMessage* msg) //225
{
	vector<string> stats = _db.getPersonalStatus(msg->getUser()->getUsername());
	string sendMsg = SERVER_PERSONAL_STATUS + stats[0] + stats[1] + stats[2] + stats[3];
	msg->getUser()->send(sendMsg);
}

void TriviaServer::handleAddQuestion(RecievedMessage* msg) //225
{
	string sendMsg;
	if (_db.insertQuestion(msg->getValues()[0], msg->getValues()[1], msg->getValues()[2], msg->getValues()[3], msg->getValues()[4]))
		sendMsg = SERVER_ADD_QUESTION_SUCCESS;
	else
		sendMsg = SERVER_ADD_QUESTION_FAIL;
	msg->getUser()->send(sendMsg);
}

void TriviaServer::handleQuestionCount(RecievedMessage* msg)
{
	int numOfQuestions = _db.questionCount();
	msg->getUser()->send(SERVER_QUESTION_COUNT + Helper::getPaddedNumber(numOfQuestions, 2)); //will be -1 if it fails
}

void TriviaServer::handlePlayerAnswer(RecievedMessage* msg) //219
{
	User* user = msg->getUser();
	Game* game = user->getGame();
	int qNum = atoi(msg->getValues()[0].c_str());
	int time = atoi(msg->getValues()[1].c_str());
	if (game)
	{
		int gameContinues = game->handleAnswerFromUser(msg->getUser(), qNum, time);
		if (!gameContinues) delete game; //game ended.
	}
}

/*		support 'get' functions		*//////////////////////////////////////////////////////////////////////

Room* TriviaServer::getRoomById(int roomId)
{
	return ((_roomsList.find(roomId) == _roomsList.end()) ? NULL : _roomsList[roomId]);
}

User* TriviaServer::getUserByName(string username)
{
	for (map<SOCKET, User*>::iterator i = _connectedUsers.begin(); i != _connectedUsers.end(); i++)
	{
		if (i->second->getUsername() == username) return i->second;
	}
	return nullptr;
}

User* TriviaServer::getUserBySocket(SOCKET client_socket)
{
	map<SOCKET, User*>::iterator i;
	for(i = _connectedUsers.begin(); i != _connectedUsers.end() && i->first != client_socket; i++){};
	return i->second;
}

RecievedMessage* TriviaServer::buildRecieveMessage(SOCKET client_socket, int msgCode)
{
	vector<string> values;
	User* user = nullptr;
	if (msgCode == CLIENT_SIGN_IN || msgCode == CLIENT_SIGN_UP)
	{
		values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //username
		values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //password
		if (msgCode == CLIENT_SIGN_UP) values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //email
	}
	else
	{
		user = getUserBySocket(client_socket);
		if (msgCode == CLIENT_JOIN_EXISTING_ROOM || msgCode == CLIENT_USERS_IN_ROOM)
		{
			values.push_back(Helper::getStringPartFromSocket(client_socket, 4));
		}
		else if (msgCode == CLIENT_CREATE_NEW_ROOM)
		{
			values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //room name
			values.push_back(Helper::getStringPartFromSocket(client_socket, 1)); //number of players
			values.push_back(Helper::getStringPartFromSocket(client_socket, 2)); //number of questions
			values.push_back(Helper::getStringPartFromSocket(client_socket, 2)); //time per question
		}
		else if (msgCode == CLIENT_ANSWER)
		{
			values.push_back(Helper::getStringPartFromSocket(client_socket, 1)); //answer number
			values.push_back(Helper::getStringPartFromSocket(client_socket, 2)); //time taken
		}
		else if (msgCode == CLIENT_ADD_QUESTION)
		{
			values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //question
			values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //answer 1
			values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //answer 2
			values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //answer 3
			values.push_back(Helper::getStringPartFromSocket(client_socket, Helper::getIntPartFromSocket(client_socket, 2))); //answer 4
		}
	}
	RecievedMessage* msg = new RecievedMessage(client_socket, msgCode, values);
	if (msgCode != CLIENT_SIGN_IN && msgCode != CLIENT_SIGN_UP) msg->setUser(user); //these two get the user in 'values'.
	cout << "Recieved: " << msg->getSock();
	for (int i = 0; i < msg->getValues().size(); i++) cout << " " << msg->getValues()[i];
	cout << endl;
	return msg;
}

void TriviaServer::addRecievedMessage(RecievedMessage* msg)
{
	unique_lock<mutex> lck(this->_mtxRecievedMessages, defer_lock);
	lck.lock();
	this->_queRcvMessages.push(msg);
	lck.unlock();
	cv.notify_all();
}

void TriviaServer::handleRecievedMessages()
{
	RecievedMessage* msg;
	unique_lock<mutex> lck(this->_mtxRecievedMessages, defer_lock);
	while (true)
	{
		lck.lock(); //usage of _queRcvMessages starts
		while (this->_queRcvMessages.empty())
		{
			cv.wait(lck);
		}
		msg = this->_queRcvMessages.front();
		this->_queRcvMessages.pop();
		lck.unlock(); //usage of _queRcvMessages ends
		try
		{
			if (msg->getMessageCode() == CLIENT_SIGN_IN)
			{
				handleSignin(msg);
			}
			else if (msg->getMessageCode() == CLIENT_SIGN_UP)
			{
				handleSignup(msg);
			}
			else if (msg->getMessageCode() == CLIENT_SIGN_OUT)
			{
				handleSignout(msg);
			}
			else if (msg->getMessageCode() == CLIENT_LEAVE_GAME)
			{
				handleLeaveGame(msg);
			}
			else if (msg->getMessageCode() == CLIENT_START_GAME)
			{
				handleStartGame(msg);
			}
			else if (msg->getMessageCode() == CLIENT_ANSWER)
			{
				handlePlayerAnswer(msg);
			}
			else if (msg->getMessageCode() == CLIENT_CREATE_NEW_ROOM)
			{
				handleCreateRoom(msg);
			}
			else if (msg->getMessageCode() == CLIENT_CLOSE_ROOM)
			{
				handleCloseRoom(msg);
			}
			else if (msg->getMessageCode() == CLIENT_JOIN_EXISTING_ROOM)
			{
				handleJoinRoom(msg);
			}
			else if (msg->getMessageCode() == CLIENT_LEAVE_ROOM)
			{
				handleLeaveRoom(msg);
			}
			else if (msg->getMessageCode() == CLIENT_USERS_IN_ROOM)
			{
				handleGetUsersInRoom(msg);
			}
			else if (msg->getMessageCode() == CLIENT_GET_EXIST_ROOMS)
			{
				handleGetRooms(msg);
			}
			else if (msg->getMessageCode() == CLIENT_BEST_SCORE)
			{
				handleGetBestScores(msg);
			}
			else if (msg->getMessageCode() == CLIENT_STATUS)
			{
				handleGetPersonalStatus(msg);
			}
			else if (msg->getMessageCode() == CLIENT_ADD_QUESTION)
			{
				handleAddQuestion(msg);
			}
			else if (msg->getMessageCode() == CLIENT_GET_QUESTION_COUNT)
			{
				handleQuestionCount(msg);
			}
			else if (msg->getMessageCode() == CLIENT_EXIT)
			{
				safeDeleteUser(msg);
			}
		}
		catch (exception ex)
		{
			safeDeleteUser(msg);
			cout << ex.what() << endl;
		}
	}
}