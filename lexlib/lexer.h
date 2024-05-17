#pragma once
#ifndef lexer_h
#define lexer_h
#ifndef utils
#define utils
#include<string>
#include<map>
#include<utility>
#include<fstream>
#include<map>
#include<vector>
#include<iostream>
#endif

class Lexer{
	private:
		std::istream &stream;
		std::ifstream file;
		char current_char;
		int state;
		std::string buffer;
		std::map<std::string, std::string> Keywords;
		char instance(char ch);
	public:
		Lexer(std::istream &input_stream);
		Lexer(std::string filePath);
		void get_next_char();
		std::pair<std::string, std::string> get_next_lexem();
};
#endif
