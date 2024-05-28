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

map<string,string> comparsionAtoms = {{"opeq", "EQ"}, {"opne", "NE"}, {"opgt", "GT"}, {"oplt", "LT"}, {"ople", "LE"}};

map<string, string> arifmAtoms = {{"opplus", "ADD"}, {"opminus", "SUB"}, {"opmul", "MUL"}};

SymbolTable symtab;

int new_label = 0;

struct Node{
	pair<string, string> value;
	Node* parent;
	int scope = -1;
	vector<int> atr;
	vector<Node*> childs;
	vector<string> satr;
};


bool IsEpsBorn(Node* node){
	Node* child = node->childs[0];
	if(child->value.first == "epsilon"){
		return true;
	}
	return false;
}

void generateAtoms(Node* node){
	if(node->value.first == "E"){
		Node* E7 = node->childs[0];
		E7->scope = node->scope;
		
		generateAtoms(E7);
		int q = E7->atr[0];
		
		node->atr.push_back(q);//p = q
	
	}else if(node->value.first == "E7"){
		Node* E6 =  node->childs[0];
		Node* E7_ = node->childs[1];
		
		E6->scope = node->scope;
		E7_->scope = node->scope;
		

		generateAtoms(E6);
	
		int q = E6->atr[0];
		E7_->atr.push_back(q);//r = q 
		generateAtoms(E7_);
		
		int s = E7_->atr[1];
		node->atr.push_back(s);//p = s

		return;
		
	}
	else if(node->value.first == "E6"){
		Node* E5 =  node->childs[0];
		Node* E6_ = node->childs[1];
		
		E5->scope = node->scope;
		E6_->scope = node->scope;
		

		generateAtoms(E5);
	
		int q = E5->atr[0];
		E6_->atr.push_back(q);//r = q
		generateAtoms(E6_);
		
		int s = E6_->atr[1];
		node->atr.push_back(s);//p = s

		return;
	}
	else if(node->value.first == "E7'"){
		if(IsEpsBorn(node)){
			int q = node->atr[0];
			node->atr.push_back(q);//p = q
			return;
		}
		
		Node* E6 = node->childs[1];
		Node* E7_ = node->childs[2];
		
		
		E6->scope = node->scope;
		E7_->scope = node->scope;
			
		int p = node->atr[0];
		
		generateAtoms(E6);
		int r = E6->atr[0];
		
		int s = symtab.alloc(node->scope);

		cout << "<" << "OR, " << p << ", " << r << ", " << s << ">" << '\n'; 
		E7_->atr.push_back(s);
		generateAtoms(E7_);
		int t = E7_->atr[1];
		node->atr.push_back(t);//q = t
	}
	
	else if(node->value.first == "E6'"){
		if(IsEpsBorn(node)){
			int q = node->atr[0];
			node->atr.push_back(q);
			return;
		}
		
		Node* E5 = node->childs[1];
		Node* E6_ = node->childs[2];
		
		
		E5->scope = node->scope;
		E6_->scope = node->scope;
			
		int p = node->atr[0];
		
		generateAtoms(E5);
		int r = E5->atr[0];
		
		int s = symtab.alloc(node->scope);
		
		
		cout << "<" << "AND, " << p << ", " << r << ", " << s << ">" << '\n'; 
		E6_->atr.push_back(s);
		generateAtoms(E6_);
		int t = E6_->atr[1];
		node->atr.push_back(t);
	}

	else if(node->value.first == "E5"){
		Node* E4 =  node->childs[0];
		Node* E5_ = node->childs[1];
		
		E4->scope = node->scope;
		E5_->scope = node->scope;
		

		generateAtoms(E4);
	
		int q = E4->atr[0];
		E5_->atr.push_back(q);
		generateAtoms(E5_);
		
		int s = E5_->atr[1];
		node->atr.push_back(s);

		return;
		
	}
	else if(node->value.first == "E4"){
		Node* E3 =  node->childs[0];
		Node* E4_ = node->childs[1];
		
		E3->scope = node->scope;
		E4_->scope = node->scope;
		

		generateAtoms(E3);
	
		int q = E3->atr[0];
		E4_->atr.push_back(q);
		generateAtoms(E4_);
		
		int s = E4_->atr[1];
		node->atr.push_back(s);

		return;
		
	}

	else if(node->value.first == "E5'"){
		if(IsEpsBorn(node)){
			int q = node->atr[0];
			node->atr.push_back(q);
			return;
		}
		string op = node->childs[0]->value.first;
		Node* E4 = node->childs[1];		
		
		E4->scope = node->scope;
			
		int p = node->atr[0];
		
		generateAtoms(E4);
		int r = E4->atr[0];
		
		int s = symtab.alloc(node->scope);
		
		int label = new_label++;
		cout << "<" << "MOV, " << "1, " << " , " << s << ">" << '\n';
		cout << "<" <<  comparsionAtoms[op] << ", " << p << ", " << r << ", " << label <<  ">" << '\n';
	      	cout << "<" << "MOV, " << "0, " << " , " << s << ">" << '\n';	
		cout << "<" << "LBL, " << " ," << "  ," << label << ">" << '\n'; 
		
		node->atr.push_back(s);//q = s
	}

	else if(node->value.first == "E4'"){
		if(IsEpsBorn(node)){
			int q = node->atr[0];
			node->atr.push_back(q);
			return;
		}
		string op = node->childs[0]->value.first;
		Node* E3 = node->childs[1];
		Node* E4_ = node->childs[2];
		
	
		E3->scope = node->scope;
		E4_->scope = node->scope;
			
		int p = node->atr[0];
		
		generateAtoms(E3);
		int r = E3->atr[0];
		
		int s = symtab.alloc(node->scope);
		
		
		cout << "<" << arifmAtoms[op] << ", " << p << ", " << r << ", " << s << ">" << '\n'; 
		E4_->atr.push_back(s);
		generateAtoms(E4_);
		int t = E4_->atr[1];
		node->atr.push_back(t);
	}

	else if(node->value.first == "E3"){
		Node* E2 =  node->childs[0];
		Node* E3_ = node->childs[1];
		
		E2->scope = node->scope;
		E3_->scope = node->scope;
		

		generateAtoms(E2);
	
		int q = E2->atr[0];
		E3_->atr.push_back(q);
		generateAtoms(E3_);
		
		int s = E3_->atr[1];
		node->atr.push_back(s);

		return;
		
	}

	else if(node->value.first == "E3'"){
		if(IsEpsBorn(node)){
			int q = node->atr[0];
			node->atr.push_back(q);
			return;
		}
		string op = node->childs[0]->value.first;
		Node* E2 = node->childs[1];
		Node* E3_ = node->childs[2];
		
	
		E2->scope = node->scope;
		E3_->scope = node->scope;
			
		int p = node->atr[0];
		
		generateAtoms(E2);
		int r = E2->atr[0];
		
		int s = symtab.alloc(node->scope);
		
		
		cout << "<" << arifmAtoms[op] << ", " << p << ", " << r << ", " << s << ">" << '\n'; 
		E3_->atr.push_back(s);
		generateAtoms(E3_);
		int t = E3_->atr[1];
		node->atr.push_back(t);
	}
	
	else if(node->value.first == "E2"){
		if(node->childs.size() == 2){
			Node* E1 = node->childs[1];
			E1->scope = node->scope;
			
			generateAtoms(E1);
			int q = E1->atr[0];
			int r  = symtab.alloc(node->scope);

			cout << "<" << "NOT" << ", " << q << ", " << " " << ", " << r << ">";
			return;
		}
		
		Node* E1 = node->childs[0];

		E1->scope = node->scope;

		generateAtoms(E1);

		int q = E1->atr[0];

		node->atr.push_back(q);

	}
	else if(node->value.first == "E1"){
		if(node->childs[0]->value.first == "lpar"){
			Node* E = node->childs[1];
			E->scope = node->scope;
			
			generateAtoms(E);
			
			int q = E->atr[0];
			node->atr.push_back(q);
		}
		else if(node->childs[0]->value.first == "num"){
			
			string num_val = node->childs[0]->value.second;
			int num_val_int = stoi(num_val);
			node->atr.push_back(num_val_int);
		}
		else if(node->childs[0]->value.first == "opinc"){
			string id = node->childs[1]->value.first;
			
			int q = symtab.checkVar(node->scope, id);

			
			cout << "<" << "ADD" << ", "<< q << ", " << "1, " << q << ">" << '\n';
			node->atr.push_back(q);
		}
		else if(node->childs[0]->value.first == "id"){
			Node* id = node->childs[0];
			Node* E1_ = node->childs[1];
			
			E1_->scope = node->scope;

			E1_->satr.push_back(id->value.second);
			generateAtoms(E1_);
			int s = E1_->atr[0];
			node->atr.push_back(s);
		}
	}
	else if(node->value.first == "E1'"){
		if(IsEpsBorn(node)){
			string p = node->satr[0];
			int q =  symtab.checkVar(node->scope, p);
			node->atr.push_back(q);
			return;
		}
		else if(node->childs[0]->value.first == "opinc"){
			string p = node->satr[0];
			
			int s = symtab.checkVar(node->scope, p);		
			int r = symtab.alloc(node->scope);
			
			node->atr.push_back(r);
		
			cout << "<" << "MOV" << ", " << s << ", " << " " << ", " << r << ">" << '\n'; 
			cout << "<" << "ADD" << ", " << s << ", " << "1" << ", " << s << ">" << '\n';
		}
		else if(node->childs[0]->value.first == "lpar"){
			Node* ArgList = node->childs[1];
			ArgList->scope = node->scope;

			string p = node->satr[0];
			cout << p << '\n';	
			generateAtoms(ArgList);
			
			int n = ArgList->atr[0];
			int r = symtab.alloc(node->scope);
			
			int s = symtab.checkFunc(p, n);
			
			node->atr.push_back(r);
			cout << "<" << "CALL" << ", " << s << ", " << r << ">" << '\n';
		}

	}
	else if(node->value.first == "ArgList"){
		if(IsEpsBorn(node)){
			int n = 0;
			node->atr.push_back(n);
			return;
		}	
		
		Node* E = node->childs[0];
		Node* ArgList_ = node->childs[1];
	
		E->scope = node->scope;
		ArgList_->scope = node->scope;
		
		generateAtoms(E);
		int p = E->atr[0];		
		
		cout << "<" << "PARAM" << ", " << " " << ", " << p << ">" << '\n';
		
		generateAtoms(ArgList_);
		int m = ArgList_->atr[0];

		node->atr.push_back(++m);

	}
	else if(node->value.first == "ArgList'"){
		if(IsEpsBorn(node)){
			int n = 0;
			node->atr.push_back(n);
			return;
		}
		
		Node* E =  node->childs[1];
		Node* ArgList_ = node->childs[2];
		
		E->scope = node->scope;
		ArgList_->scope = node->scope;
	
		generateAtoms(E);
		int p = E->atr[0];
		
		cout << "<" << "PARAM" << ", " << " " << ", " << p << ">" << '\n';
		
		generateAtoms(ArgList_);
		int m = ArgList_->atr[0];
		
		node->atr.push_back(++m);
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
	symtab.addVar("a", -1, SymbType::Int);
	symtab.addVar("b", -1, SymbType::Int);
	symtab.addVar("c", -1, SymbType::Int);
	symtab.addVar("d", -1 , SymbType::Int);
	symtab.addFunc("func", SymbType::Int);
	cout << "-----------------------------------------------" << '\n';
	generateAtoms(root);
	cout << "-----------------------------------------------" << '\n';
	symtab.print_table();
	return 0;

}
