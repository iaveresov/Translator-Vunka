#include "../lexlib/lexer.h"
#include "./lr_table.cpp"
#include "./symtable/symboltable.h"

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


#include<stack>

using namespace std;

SymbolTable symtab;


struct Node{
	pair<string, string> value;
	Node* parent;
	int scope = -1;
	vector<int> atr;
	vector<Node*> childs;
};


bool IsLeaf(Node* node){
	return node->childs.size() == 0 ? true : false;

}

void generateAtoms(Node* node){
	if(node->value == "E"){ //prod 1
		Node* E7 = node->childs[0];
		
		E7->scope = node->scope;
		
		generateAtoms(E7);
		auto q = E7->atr[0];
		E->atr.push_back(q) // p = q
	
	}
	else if(node->value == "E7"){	//prod 2
		Node* E6 = node->childs[0];
		Node* E7_ = node->childs[1];
		
		E6->scope = node->scope;
		E7_->scope = node->scope;
		
		generateAtoms(E6);
		auto q = E6->atr[0];
		E7_->atr.push_back(q); //r = q
		
		generateAtoms(E7_);
		s = E7_->atr[1];
		E7->atr.push_back(s);//p = s	
		
	
	}
	else if(node->value == "E7'"){
		if(IsLeaf(node)){//prod 4
			auto p = node->atr[0];
			node->atr.push_back(p);//q = p
			return;
		}
		//prod 3
		Node* E6 = node->childs[1];
		Node* E7_= node->childs[2];
		
		E6->scope = node->scope;
		E7_->scope = node->scope;
		
		generateAtoms(E6);
		auto r = E6->atr[0];
		auto s = symtab.alloc(E7_->scope)//s = alloc(C)
		
		generateAtoms(E7_);
		auto t = E7_->atr[1];
		node->atr.push_back(t);//q = t
		
		auto p = E7_->atr[0];

		cout << "(" << "OR" << p << r << s << ")" << '\n';	
	}
	else if(node->value == "E6"){//prod 5
		Node* E5 = node->childs[0];
		Node* E6_ = node->childs[1];
		
		E5->scope = node->scope;
		E6_->scope = node->scope;
		
		generateAtoms(E5);
		auto q = E5->atr[0];
		E6_->atr.push_back(q); //r = q
		
		generateAtoms(E6_);
		s = E6_->atr[1];
		E6->atr.push_back(s);//p = s	
	
	}
	else if(node->value == "E6'"){
		if(IsLeaf(node)){//prod 4
			auto p = node->atr[0];
			node->atr.push_back(p);//q = p
			return;
		}
		//prod 3
		Node* E5 = node->childs[1];
		Node* E6_= node->childs[2];
		
		E5->scope = node->scope;
		E6_->scope = node->scope;
		
		generateAtoms(E5);
		auto r = E5->atr[0];
		auto s = symtab.alloc(E6_->scope)//s = alloc(C)
		
		generateAtoms(E6_);
		auto t = E6_->atr[1];
		node->atr.push_back(t);//q = t
		
		auto p = E6_->atr[0];

		cout << "(" << "AND" << p << r << s << ")" << '\n';

	}
	return;
}



void print(Node* root, vector<int> not_last = {}){
	int strSize = not_last.size();
	
	if(strSize == 0){
		cout << root->value.first << "(" << root->value.second << ")"<<'\n';
	}
	
	else if(strSize == 1){

		if(not_last[0] == 0){
			cout << "└";
			cout << root->value.first << "(" << root->value.second << ")"<< '\n';	
		}else{
			cout << "├";
			cout << root->value.first <<"(" <<  root->value.second << ")" << '\n';
		}
	}
	
	else{
		for(int i = 0; i < strSize; ++i){
			if(not_last[i] == 1){
				if(i == strSize - 1){
					cout << "├";
				}
				else{
					cout << "│";
				}
			}else{
				if(i == strSize - 1){
					cout << "└";
				}
				else{
					cout << " "; 
				}
			
			}
		
		}
		
		cout << root->value.first << "(" <<  root->value.second << ")" << '\n';
	}

	int layerSize = root->childs.size();
	
	

	for(int i = 0; i < layerSize; ++i){
		if(i == layerSize - 1){
			vector<int> down_vector = not_last;
			down_vector.push_back(0);
			print(root->childs[i],down_vector);
		}
		else{
			vector<int> down_vector = not_last;
			down_vector.push_back(1);
			print(root->childs[i], down_vector);
		}

	}
}
	




Node* make_tree(){
	Lexer l("input.txt");
	stack<int> stmtStack;
	stmtStack.push(0);
	stack<Node*> treeStack;
	
	pair<string, string> lexem = l.get_next_lexem();
	
	if(lexem.first == "error"){
		return new Node{{"error", ""}};
	}

	while(true){
		auto& tableString = tableAction[stmtStack.top()];		
		for(auto& cellAction: tableString){
			if(cellAction.first == lexem.first){
				if(cellAction.second.first == "s"){
					Node* node = new Node{lexem};
					treeStack.push(node);
					stmtStack.push(cellAction.second.second);
					if(lexem.first != "correct"){
						lexem = l.get_next_lexem();
						if(lexem.first == "error"){
							return new Node{{"error", ""}};
						}
					}
					break;
				}
				else if(cellAction.second.first == "r"){
					string reduceNterm = reduceRules[cellAction.second.second].first;
					Node* node = new Node{{reduceNterm, ""}};
					if(reduceRules[cellAction.second.second].second.size() == 1 && reduceRules[cellAction.second.second].second[0] == "epsilon"){
						Node* child = new Node{{"epsilon",""}, node};
						auto it = node->childs.begin();
						node->childs.insert(it, child);
						treeStack.push(node);
					}else{		
						for(string symb: reduceRules[cellAction.second.second].second){
							stmtStack.pop();
							auto it = node->childs.begin();
							Node* child = treeStack.top();
							node->childs.insert(it, child);
							child->parent = node;
							treeStack.pop();
						}
						treeStack.push(node);
					}
					int reduceStmt = stmtStack.top();
					for(auto& cellGoto: gotoTable[reduceStmt]){
						if(cellGoto.first == reduceNterm){
							if(cellGoto.second == -1){
								Node* error = new Node{{"err",""}};	
								return error;
							}
							else{
								stmtStack.push(cellGoto.second);
								break;
							}
						
						}
					
					}
				}
				else if(cellAction.second.first == "err"){
					Node* error = new Node{{"err", ""}};
					return error;
				}
				else if(cellAction.second.first == "acc"){
					 return treeStack.top();
				}
			
			}
		}
	
	}
}



int main(){
	initTableAction();
	initGotoTable();
	initReduceRules();
	
	Node* root = make_tree();
	print(root);
	return 0;

}
