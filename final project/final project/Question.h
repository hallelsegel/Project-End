#pragma once
#include <list>
#include <vector>
#include <string.h> 

using namespace std;

class Question
{
private:
	string _question;
	vector<string> _answers;
	int _id;
	int _correctPlace;
public:
	Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4);
	~Question();
	};
