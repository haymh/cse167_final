#ifndef LSTRING_H_
#define LSTRING_H_
#include <vector>
#include <string>
using namespace std;

/*
+ Turn left by angle ��, using rotation matrix RU(��).
? Turn right by angle ��, using rotation matrix RU(?��).
& Pitch down by angle ��, using rotation matrix RL(��).
�� Pitch up by angle ��, using rotation matrix RL(?��).
\ Roll left by angle ��, using rotation matrix RH(��).
/ Roll right by angle ��, using rotation matrix RH(?��).
| Turn around, using rotation matrix RU(180?).
*/

#define MyRule vector<Rule*> 

struct Rule{
	Rule(char s, string w, double p){
		symbol = s;
		word = w;
		prob = p;
	}
	char symbol;
	string word;
	double prob;
};


class Lstring{


public:
	Lstring(string a, string v);
	void addRule(Rule*);
	void addVariables(string v);
	void generate();
	string getGeneration(int index);
	string getCurrent();
private:
	MyRule rules;
	string axiom;
	string current;
	string V;
	vector<string>generations;


	bool isVariable(char);
	MyRule searchRules(char c);
};

#endif