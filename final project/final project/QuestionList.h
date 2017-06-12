#pragma once
#include <string>
#include <list>

class QuestionList
{
public:
	list<string> _list;
	QuestionList(){
		//q1
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('What is the airspeed velocity of an unladen swallow?', 'What do you mean? African or European swallow?',"
			"'10 meters/second', '15 km/h', 'Huh? I... I dont know that. '); ");
		//q2
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('What... is the capital of Assyria?', 'I dont know that. Auuuuuuuugh!!',"
			"'Aššur', 'Nineveh', 'Damascus'); ");
		//q3
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('If you could decribe yourself in 3 words, how flipping sad would that be?', "
			"'Pretty damn sad.', 'Not at all!', 'A bit, I guess', 'Not too bad. <- my answer as well ;)'); ");
		//q4
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('What was Red Hot Chilly Peppers original name?', 'Miraculously Majestic Masters of Mayhem',"
			"'Averagely Hot Peppers', 'Red Hot Chilly Peppers', 'The Mars Volta'); ");
		//q5
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('A group of Ravens is called:', 'An Unkindness', 'A Coal mine', 'A Party', 'A Flock'); ");
		//q6
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('Taphephobia is the fear of:', 'Being buried alive', 'Typhoons', 'Tea parties', 'Clowns'); ");
		//q7
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('How many eggs does a female mackerel lay at a time?', '500,000', '1.3 million', '450-500', '23,000'); ");
		//q8
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
"VALUES ('Which of the following is wrong?', 'Beakine means bird-like', 'Feline means cat-like', "
"'Ursine means bear-like', 'Canine means dog-like'); ");
		//q9
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('What is a tittle?', 'The dot above i and j', 'A breed of cats', 'A british woman', 'A plastic hoelace'); ");
		//q10
		_list.push_back("INSERT INTO T_QUESTIONS (QUESTION, CORRECT_ANS, ANS2, ANS3, ANS4)"
			" VALUES ('What is an aglet?', 'The plastic at the end of a shoelace', 'The middle button on a mouse', "
			"'Scrambled eggs', 'A sweatband brand'); ");
	}
	~QuestionList(){};
};