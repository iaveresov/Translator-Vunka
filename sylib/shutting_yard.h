#ifndef LEX_SHUTTING_YADR_H
#define LEX_SHUTTING_YADR_H

#include "../lexlib/lexer.h"
#include<iostream>
#include<string>
#include<fstream>
#include<utility>
#include<queue>

class ShuttingYard {
	public:
		ShuttingYard(std::istream&);
		std::pair<std::string, std::string> getNextLexem();
	private:
		std::queue<std::string, std::string> syQueue;
		Lexer lexer;
};


#endif
