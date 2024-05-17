#include "shutting_yard.h"
#include<iostream>
#include<utility>
#include<string>

int main(){
	std::pair<std::string, std::string> lexem;
	ShuttingYard yard(std::cin);
	yard.getNextLexem();
	return 0;
}
