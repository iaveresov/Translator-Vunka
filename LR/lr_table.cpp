#pragma once
#ifndef lr_table_cpp
#define lr_table_cpp
#include<iostream>
#include<fstream>
#include<string>
#include<utility>
#include<vector>
#include<map>

using namespace std;

vector< vector< pair<string, pair<string, int> > >> tableAction = {};
void initTableAction(){
	ifstream fin("tableAction.txt");
	int n;
	fin >> n;
	tableAction.reserve(n);
	for (int i = 0; i < n; ++i)
	{
		int k;
		fin >> k;
		vector< pair<string, pair<string, int> > > tmp;
		tmp.reserve(k);
		for (int j = 0; j < k; ++j)
		{
			string token, action;
			int code;
			fin >> token >> action >> code;
			tmp.push_back({token, {action, code}});
		}
		tableAction.push_back(tmp);
	}
}

vector< vector< pair<string, int> > > gotoTable = {};
void initGotoTable(){
	ifstream fin("gotoTable.txt");
	int n;
	fin >> n;
	gotoTable.reserve(n);
	for (int i = 0; i < n; ++i)
	{
		int k;
		fin >> k;
		vector< pair<string, int> > tmp;
		tmp.reserve(k);
		for (int j = 0; j < k; ++j)
		{
			string rule;
			int state;
			fin >> rule >> state;
			tmp.push_back({rule, state});
		}
		gotoTable.push_back(tmp);
	}
}

vector< pair< string, vector<string> > > reduceRules = {};
void initReduceRules(){
	ifstream fin("reduceRules.txt");
	int n;
	fin >> n;
	reduceRules.reserve(n);
	for (int i = 0; i < n; ++i)
	{
		string rule;
		int k;
		fin >> rule >> k;
		vector< string> tmp;
		tmp.reserve(k);
		for (int j = 0; j < k; ++j)
		{
			string token;
			fin >> token;
			tmp.push_back(token);
		}
		reduceRules.push_back({rule, tmp});
	}
}

#endif