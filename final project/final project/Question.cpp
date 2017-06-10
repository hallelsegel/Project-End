#include "Question.h"
#include <string>
#include <time.h>
using namespace std;


Question::Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4)
{
	_id = id;
	_question = question;
	_answers.resize(4);
	vector<string> temp = { correctAnswer, answer2, answer3, answer4 };
	int i;
	for (int j = 0; j < 4; j++)
	{
		i = rand() % (4-j);
		if (i = 0) _correctPlace = j;
		_answers[j] = temp[i];
		temp.erase(temp.begin() + i);
	}
}

Question::~Question()
{

}

string Question::getQuestion()
{
	return _question;
}

string* Question::getAnswers()
{
	string ans[4] = { _answers[0], _answers[1], _answers[2], _answers[3] };
	return ans;
}

int Question::getCorrectAnswerIndex()
{
	return _correctPlace;
}

int Question::getId()
{
	return _id;
}