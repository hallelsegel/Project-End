#include "Game.h"
#include "user.h"
#include "Helper.h"
#include "DataBase.h"
#include "Question.h"
#include <iostream>

Game::Game(const vector<User*>& players, int questionsNo, DataBase& db) : _db(db)
{
	this->_question_no = questionsNo;
	this->_currentTurnAnswers = 0;
	this->_currQuestionIndex = 0;
	if (insertGameToDB())
	{
		initQuestionsFromDB();
	}
	else
	{
		throw exception("ERROR: Failed to insert a new game.");
	}

	this->_players = players;

	for (int i = 0; i < _players.size(); i++)
	{
		_results.insert(pair<string, int>(_players[i]->getUsername(), 0));
		_players[i]->setGame(this);
	}
}

Game::~Game()
{
	for (int i = 0; i < _questions.size(); i++)
	{
		delete _questions[i];
	}

	_questions.clear();
	_players.clear();
	_results.clear();
}

void Game::handleFinishGame()
{
	this->_db.updateGameStatus(this->_ID);
	for (int i = 0; i < this->_players.size(); i++)
	{
		try
		{
			this->_players[i]->send(SERVER_END_GAME);
			this->_players[i]->setGame(nullptr);
		}
		catch (exception ex)
		{
			cout << ex.what() << endl;
		}
	}
}

void Game::sendFirstQuestion()
{
	sendQuestionToAllUsers();
}

bool Game::handleNextTurn()
{
	if (this->_players.size() > 0) // Checks if there are players in the game
	{
		if (this->_currentTurnAnswers = this->_players.size()) // Checks if everyone answered the given question yet.
		{
			if (this->_currQuestionIndex = this->_question_no) // Checks if its the last question of the game.
			{
				handleFinishGame();
				return (false);
			}
			this->_currQuestionIndex++;
			sendQuestionToAllUsers();
		}
		// Do nthing until everyone answers.
	}
	else
	{
		handleFinishGame();
		return (false);
	}
	return (true);
}

bool Game::handleAnswerFromUser(User* user, int answerNo, int time)
{
	stringstream  msg;
	msg << SERVER_ANSWER;
	bool isCorrect = false;
	this->_currentTurnAnswers++;
	if (answerNo = this->_questions[this->_currQuestionIndex]->getCorrectAnswerIndex())
	{
		this->_results[user->getUsername()]++;
		isCorrect = true;
	}
	try
	{
		this->_db.addAnswerToPlayer(this->_ID, user->getUsername(), this->_currQuestionIndex, answerNo == 5 ? "" : _questions[_currQuestionIndex]->getAnswers()[answerNo], isCorrect, time);
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	msg << (isCorrect ? 1 : 0);
	user->send(msg.str());
	return handleNextTurn();
}

bool Game::leaveGame(User* currUser)
{
	vector<User*>::iterator i;
	for (i = _players.begin(); i < _players.end() && *i != currUser; i++);
	_players.erase(i);
	return handleNextTurn();
}

bool Game::insertGameToDB()
{
	this->_ID = this->_db.insertNewGame();
	return this->_ID != -1;
}

void Game::initQuestionsFromDB()
{
	this->_questions = this->_db.initQuestion(this->_question_no);
}

void Game::sendQuestionToAllUsers()
{
	stringstream msg;
	string que = _questions[_currQuestionIndex]->getQuestion(), *ans = _questions[_currQuestionIndex]->getAnswers();
	this->_currentTurnAnswers = 0;

	if (que.length() > 0)
	{                   // Building the message:
		msg << SERVER_SEND_QUESTION << Helper::getPaddedNumber(que.length(), 3) << que << Helper::getPaddedNumber(ans[0].length(), 3) << ans[0] << Helper::getPaddedNumber(ans[1].length(), 3) << ans[1] << Helper::getPaddedNumber(ans[2].length(), 3) << ans[2] << Helper::getPaddedNumber(ans[3].length(), 3) << ans[3];
		for (int i = 0; i < _players.size(); i++) // Sending the message:
		{
			try
			{
				_players[i]->send(msg.str());
			}
			catch (exception e)
			{
				cout << e.what() << endl;
			}
		}
	}
}

// GETS
int Game::getId()
{
	return this->_ID;
}

// SETS
