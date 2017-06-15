#include "CGame.h"
#include "Helper.h"
#include "Protocol.h"

using namespace std;

CGame::CGame(SOCKET clientSock, int questionNum, int questionTime)
{
	this->_questionsNo = questionNum;
	this->_questionTime = questionTime;
	this->_clientSock = clientSock;
	this->_currQuestion = 1;
}

CGame::~CGame()
{
}

string question;
vector<string> answers;
int ans;
int timeRemaining;

void CGame::handleQuestion()
{
	//the msg code was recieved already so continue from that point in interpeting the message
	int len = Helper::getIntPartFromSocket(_clientSock, 3);
	if (len == 0)
	{//error, server sends 0 instead of the question
		cout << "Error in sending the question.." << endl;
	}
	else {
		question = Helper::getPartFromSocket(_clientSock, len, 0);
		for (int j = 0; j < 4; j++) answers.push_back(Helper::getPartFromSocket(_clientSock, Helper::getIntPartFromSocket(_clientSock, 3), 0));
		thread t(&CGame::questionDisplay, this);
		t.detach();
		cin >> ans;
		if (ans > 0 && ans < 5)
		{
			timeRemaining = 0;
			string msg = "219" + to_string(ans) + Helper::getPaddedNumber(_questionTime - timeRemaining, 2);
			sendAnswer(msg);
		}
		else
		{
			cout << "Invalid answer number. Sending as if you didn't answer. " << endl;
			timeRemaining = 0;
			string msg = "2195" + Helper::getPaddedNumber(_questionTime - timeRemaining, 2);
			sendAnswer(msg);
		}
	}
}

void CGame::questionDisplay()
{
	timeRemaining = _questionTime;
	while (timeRemaining > 0)
	{
		system("cls");
		cout << "Question number " << _currQuestion << "\n" << question << "			time remaining: " << timeRemaining << endl;
		for (int j = 0; j < 4; j++) cout << j << ". " << answers[j] << endl;
		Sleep(1000);
		timeRemaining--;
	}
	string msg = "2195" + Helper::getPaddedNumber(_questionTime - timeRemaining, 2);
	sendAnswer(msg);
}

void CGame::sendAnswer(string msg)		//219
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
		if (rcvMsg != SERVER_SEND_QUESTION) cout << "Error on recieveing question" << endl;
		else
		{
			_currQuestion++;
			handleQuestion();
		}
	}
}

bool CGame::leaveGame()			//222
{
	return true;
}
