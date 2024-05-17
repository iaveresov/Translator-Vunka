#include "lexer.h"

using namespace std;

char Lexer::instance(char ch){
		if( ch >= 48 && ch <= 57 ){
			return '0';
		}
		else if((ch >=  65 && ch <= 90) || (ch == 95) || (ch >= 97 && ch <= 122)){
			return '1';
		}
		else{
			return ch;
		}
}

//constructor with input stream
Lexer::Lexer(istream &input_stream): stream{input_stream}{
	buffer = ""; 
	state = 0;
	Keywords = {
    	{"int", "kwint"},
	{"char","kwchar"},
    	{"if","kwif"},
    	{"else","kwelse"},
   	{"switch","kwswitch"},
   	{"case","kwcase"},
   	{"while","kwwhile"},
    	{"for","kwfor"},
    	{"return","kwreturn"},
   	{"in","kwin"},
   	{"out","kwout"} 
	};
	return;
}
		
Lexer::Lexer(string filePath): file(filePath), stream(file){
	buffer = "";
	state = 0;
	Keywords = {
    	{"int", "kwint"},
    	{"char","kwchar"},
    	{"if","kwif"},
    	{"else","kwelse"},
    	{"switch","kwswitch"},
    	{"case","kwcase"},
   	{"while","kwwhile"},
    	{"for","kwfor"},
  	{"return","kwreturn"},
   	{"in","kwin"},
    	{"out","kwout"} 
	};		
	return;
}

//next char function
void Lexer::get_next_char(){
	if (buffer != "" && state == 0){
                current_char = buffer[0];
                buffer = "";
        }
        else{
        
		if(stream.get(current_char)){
			return;
		}
		else{	
			current_char = EOF;
		}
	}
}
		

pair<string, string>  Lexer::get_next_lexem(){
	while(true){
		pair<string, string> lexem;
		get_next_char();
		switch(state){
			case 0:
				switch(instance(current_char)){
					case '\t':
					case '\n':
					case ' ':
						continue;
					case '(':
						return pair<string, string>("lpar", "");
					case ')':
						return pair<string, string>("rpar", "");
					case '{':
						return pair<string, string>("lbrace","");
					case '}':
						return pair<string, string>("rbrace", "");
					case ';':
						return pair<string, string>("semicolon", "");
					case ',':
						return pair<string, string>("comma", "");
					case '.':
						return pair<string, string>("colon", "");
					case '*':
						return pair<string, string>("opmul", "");
					case '>':
						return pair<string, string>("opgt", "");
					case '<':
						state = 2;
						continue;
					case '!':
						state = 4;
						continue;
					case '=':
						state = 6;
						continue;
					case '+':
						state = 8;
						continue;
					case '|':
						state =  10;
						continue;
					case '&':
						state = 12;
						continue;
					case '\'':
						state = 14;
						continue;
					case '"':
						state = 18;
						continue;
					case '1': //alphabet symbol
						buffer += current_char;
						state = 21;
						continue;
					case '-':
						buffer += current_char;
						state = 22;
						continue;
					case '0':// digit code in ascii from instance function
						buffer += current_char;
						state = 23;
						continue;
					case EOF:
						return pair<string, string>("correct", "");
					
					default:
						return pair<string, string>("error", "");
					}
			case 2:
				switch(current_char){
					case'=':
						state = 0;
						return pair<string, string>("ople", "");
					default:
						buffer +=  current_char;
						state = 0;
						return pair<string, string>("oplt", "");
				}

			case 4:
				switch(current_char){
					case '=':
				       		state = 0;
						return pair<string, string>("opne", "");
					default:
						buffer += current_char;
						state = 0;
						return pair<string, string>("opnot", "");
				}
			case 6:
				switch(current_char){
					case '=':
						state = 0;
						return pair<string, string>("opeq", "");
					default:
						buffer += current_char;
						state = 0;
						return pair<string, string>("opassign", "");
				}
			case 8: 
				switch(current_char){
					case '+':
						state = 0;
						return pair<string, string>("opinc", "");
					default:
						buffer += current_char;
						state = 0;
						return pair<string, string>("opplus", "");
				}
			case 10:
				switch(current_char){
					case '|':
						state = 0;
						return pair<string, string>("opor", "");
					default:
						return pair<string, string>("error", "");
				}
			case 12:
				switch(current_char){
					case '&':
						state = 0;
						return pair<string, string>("opand", "");
					default:
						return pair<string, string>("error", "");
				}
			case 14:
				switch(current_char){
					case '\'':
						return pair<string, string>("error", "");
					case EOF:
						return pair<string, string>("error", "");
					default:
						state = 16;
						buffer += current_char;
						continue;
				}
			case 16:
				switch(current_char){
					case '\'':
					lexem = {"char", buffer}; 
					state = 0;
					buffer = "";
					return lexem;
				default:
					return pair<string, string>("error", "");
				}
			case 18:
				switch(current_char){
					case EOF:
						buffer = "";
						state = 0;
						return pair<string, string>("error", "");
					case '"':
						state = 0;
						lexem = {"str", buffer};
						buffer = "";
						return lexem;
					default:
						buffer += current_char;
						continue;
				}
			case 21:
				switch(instance(current_char)){
					case '1':
						buffer += current_char;
						continue;
					case '0':
						buffer += current_char;
						continue;
					default:
						if(Keywords.find(buffer) != Keywords.end()){
							lexem = pair<string, string>(Keywords[buffer], "");
							state = 0;
							buffer = current_char;
							return lexem;
						}
						else{
							lexem = {"id", buffer};
							buffer = current_char;
							state = 0;
							return lexem;
						}
				}
			case 22:
				switch(instance(current_char)){
					case '0':
						buffer += current_char;
						state = 23;
						continue;
					default:
						lexem = pair<string, string>("opminus", "");
						buffer = current_char;
						state = 0;
						return lexem;
				}
					
			case 23:
				switch(instance(current_char)){
					case '0':
						buffer += current_char;
						continue;
					default:
						lexem = pair<string, string>("num", buffer);
						buffer = current_char;
						state = 0;
						return lexem;
				}
							
			}
		}		

}	


