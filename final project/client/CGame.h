#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <condition_variable>
using namespace std;

class CGame
{
public:
	CGame(SOCKET, int, int, condition_variable*);
	~CGame();

	void sendAnswer(string msg);	//219
	bool leaveGame();				//222
	void handleQuestion();
	void questionDisplay();

private:
	condition_variable* _whenDone;
	SOCKET _clientSock;
	int _questionTime;
	int _questionsNo;
	int _currQuestion;
};