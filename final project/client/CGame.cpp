#include "CGame.h"
#include "Helper.h"
#include "Protocol.h"

using namespace std;

CGame::CGame(SOCKET clientSock, int questionNum, int questionTime, condition_variable* cv)
{
	this->_questionsNo = questionNum;
	this->_questionTime = questionTime;
	this->_clientSock = clientSock;
	this->_currQuestion = 1;
	this->_whenDone = cv;
}

CGame::~CGame()
{
}

//globals
string question;
vector<string> answers;
string ans;
int timeRemaining;
mutex TRLock;

void CGame::handleQuestion()
{
	//the msg code was recieved already so continue from that point in interpeting the message
 	int len = Helper::getIntPartFromSocket(_clientSock, 3);
	if (len == 0)
	{//error, server sends 0 instead of the question
		cout << "Error in sending the question.." << endl;
	}
	else
	{
		question = Helper::getPartFromSocket(_clientSock, len, 0);
		answers.clear();
		for (int j = 0; j < 4; j++) answers.push_back(Helper::getPartFromSocket(_clientSock, Helper::getIntPartFromSocket(_clientSock, 3), 0));
		thread t(&CGame::questionDisplay, this);
		t.detach();
		cin >> ans; //the instructions as to what should be entered are in the display function
		if (atoi(ans.c_str()) > 0 && atoi(ans.c_str()) < 5)
		{
			cout << "					answer : " << ans << endl;
			TRLock.lock();
			string msg = "219" + ans + Helper::getPaddedNumber(_questionTime - timeRemaining, 2);
			timeRemaining = -1; //stop questionDisplay()
			TRLock.unlock();
			sendAnswer(msg);
		}
		else if (ans == "L")
		{
			TRLock.lock();
			timeRemaining = -1; //stop questionDisplay()
			TRLock.unlock();
			leaveGame();
		}
		else
		{
			cout << "Invalid answer number. Sending as if you didn't answer. " << endl;
			TRLock.lock();
			string msg = "2195" + Helper::getPaddedNumber(_questionTime - timeRemaining, 2);
			timeRemaining = -1; //stop questionDisplay()
			TRLock.unlock(); 
			sendAnswer(msg);
		}
	}
}

void CGame::questionDisplay()
{
	TRLock.lock();
	timeRemaining = _questionTime;
	TRLock.unlock();
	while (timeRemaining > 0)
	{
		TRLock.lock();
		system("cls");
		cout << "(To leave the game enter 'L')" << "\n" << "Question number " << _currQuestion << "\n" << question << "				Time remaining: " << timeRemaining << endl;
		for (int j = 0; j < 4; j++) cout << (j + 1) << ". " << answers[j] << endl;
		timeRemaining--;
		TRLock.unlock();
		Sleep(1000);
	}
	TRLock.lock();
	if (timeRemaining == 0) //time's up
	{
		string msg = "2195" + Helper::getPaddedNumber(_questionTime, 2); //no answer
		timeRemaining--;
		sendAnswer(msg);
	}
	TRLock.unlock();
}	

void CGame::sendAnswer(string msg)	//219
{
	send(_clientSock, msg.c_str(), msg.size(), 0);
	string rcvMsg = Helper::getPartFromSocket(_clientSock, 3, 0);
	if (rcvMsg != SERVER_ANSWER) cout << "Error in communication" << endl;
	else
	{
		bool correct = Helper::getIntPartFromSocket(_clientSock, 1);
		if (correct) {
			cout << "Correct! Well done!" << endl;
			system("color AF"); //just some fancy shit
			Sleep(500);
			system("color 0F");
		}
		else {
			cout << "Incorrect... " << endl;
			system("color CF"); //just some fancy shit
			Sleep(500);
			system("color 0F");
		}
		rcvMsg = Helper::getPartFromSocket(_clientSock, 3, 0);
		if (rcvMsg == SERVER_SEND_QUESTION)
		{
			_currQuestion++;
			handleQuestion();
		}
		else if (rcvMsg == SERVER_END_GAME && _currQuestion == _questionsNo) //game ended
		{
			cout << "The game ended! Scores: out of " << _questionsNo << " questions" << endl;
			int playerNum = Helper::getIntPartFromSocket(_clientSock, 1);
			for (int i = 0; i < playerNum; i++)
			{
				cout << Helper::getPartFromSocket(_clientSock, Helper::getIntPartFromSocket(_clientSock, 2), 0);
				cout << " answered correctly on " << Helper::getIntPartFromSocket(_clientSock, 2) << " questions" << endl;
			}
			system("PAUSE");
			_whenDone->notify_all();
		}
		else
		{
			cout << "Error on recieveing question" << endl;
		}
	}
}

bool CGame::leaveGame()			//222
{
	send(_clientSock, "222", 3, 0);
	_whenDone->notify_all();
	return true;
}
