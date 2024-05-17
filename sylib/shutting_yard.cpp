#include "shutting_yard.h"
#include<iostream>
#include<stack>
#include<queue>

using namespace std;

ShuttingYard::ShuttingYard(std::istream& syInputStream):  lexer{syInputStream}
{
	return;
}

pair<string, string>ShuttingYard::getNextLexem()
{ 	
	if(syQueue.empty()){		
		pair<string, string> lexem;
		stack<pair<string, string>> syStack;
		op {
		{"opplus", 0}, 
		{"opminus", 0}, 
		{"opmul", 1}
		};
	
		lexem = l.get_next_lexem();

		while(lexem.first != "correct" && lexem.first != "error"){
			
			if(lexem.first == "num" || lexem.first == "id"){
				syQueue.push(lexem);
				lexem = lexer.get_next_lexem();
				continue;
			}

			else if(lexem.first == "lpar"){
				syStack.push(lexem);
				lexem = lexer.get_next_lexem();
				continue;
			
			}

			else if(lexem.first == "rpar"){
				
				while(syStack.top() != "lpar"){
					if(syStack.empty()){
						syQueue.push({"error", ""});
						break;
					}
					if(syStack.top() == "lpar"){
						syStack.pop();
						break;	
					}
				
					syQueue.push(syStack.top());
					syStack.pop();
				}
			}
		}
	}	
}
