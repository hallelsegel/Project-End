#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <thread>
#include <stdlib.h>
using namespace std;

class CGame
{
public:
	CGame(SOCKET, int, int);
	~CGame();

	void sendAnswer(string msg);	//219
	bool leaveGame();				//222
	void handleQuestion();
	void questionDisplay();

private:
	SOCKET _clientSock;
	int _questionTime;
	int _questionsNo;
	int _currQuestion;
};