#pragma once
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include<iostream>
#include<stack>
#include<unordered_map>
#include<sstream>

using namespace std;


enum class SymbType{
	Int,
	Char,
};

enum class SymbKind{
	var,
	func,
};


struct SymbolData{
	int code;
	SymbKind kind;
	SymbType type;
	string default_val = "None";
	int scope = -1;
	int len = -1;
};


void print_data(SymbolData* tstring, string data);


class SymbolTable{
	private:
		int GLOBAL_SCOPE = -1;
		int tmp_code = 0;
		int max_code = 0;
		int max_context = 0;
		unordered_map<string, SymbolData*> symtable;
		stack<int> scope_stack;

	public:
		int alloc(int scope);

		int checkVar(int scope, string name);

		int checkFunc(string name, int len);

		int addVar(string name, int scope, SymbType type,  string init="");

		int addFunc(string name, SymbType type );
		
		void set_len(int n, int scope);

		void print_table();

};
#endif
