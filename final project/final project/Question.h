#pragma once
#include <list>
#include <vector>
#include <string.h>

using namespace std;
class Question;
typedef list<Question> questions;
typedef questions::iterator users_iter;
typedef questions::const_iterator users_const_iter;


class Question
{
public:
	Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4);
	~Question();
	// Gets
	string getQuestion();
	string* getAnswers();
	int getCorrectAnswerIndex();
	int getId();

private:
	string _question;
	vector<string> _answers;
	int _id;
	int _correctPlace;

};
