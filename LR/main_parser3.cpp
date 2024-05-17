#include "../lexlib/lexer.h"
#include "./lr_table.cpp"

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


struct Node{
	string value;
	Node* parent;
	vector<Node*> childs;

};

void print(Node* root, vector<int> not_last = {}){
	int strSize = not_last.size();
	
	if(strSize == 0){
		cout << root->value << '\n';
	}
	
	else if(strSize == 1){

		if(not_last[0] == 0){
			cout << "└";
			cout << root->value << '\n';	
		}else{
			cout << "├";
			cout << root->value << '\n';
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
		
		cout << root->value << '\n';
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
		return new Node{"error"};
	}

	while(true){
		auto& tableString = tableAction[stmtStack.top()];		
		for(auto& cellAction: tableString){
			if(cellAction.first == lexem.first){
				if(cellAction.second.first == "s"){
					Node* node = new Node{lexem.first};
					treeStack.push(node);
					stmtStack.push(cellAction.second.second);
					if(lexem.first != "correct"){
						lexem = l.get_next_lexem();
						if(lexem.first == "error"){
							return new Node{"error"};
						}
					}
					break;
				}
				else if(cellAction.second.first == "r"){
					string reduceNterm = reduceRules[cellAction.second.second].first;
					Node* node = new Node{reduceNterm};
					if(reduceRules[cellAction.second.second].second.size() == 1 && reduceRules[cellAction.second.second].second[0] == "epsilon"){
						Node* child = new Node{"epsilon", node};
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
								Node* error = new Node{"err"};	
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
					Node* error = new Node{"err"};
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
	Node* root = make_tree();
	print(root);
	return 0;

}
