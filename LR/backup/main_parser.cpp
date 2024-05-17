#include "./lr_table.cpp"
#include<iostream>
#include<stack>
#include<fstream>


struct Node{
	string value;
	vector<Node*> childrens;
	Node* parent;


};


Node make_parsing_tree(){

}

int main(){
	ifstream in("input.txt");
	stack<int> stmtStack;
	stmtStack.push(0);
	char inp;
	string buffer;
	if(in >> inp){
		buffer += inp;
	}else{
		buffer += '$';
	}

	cout << "start: " << stmtStack.top() << " " << buffer << "\n\n";

	while(true){
		auto& tableString = tableAction[stmtStack.top()];
		for(auto& cellAction: tableString){ 		
			if(cellAction.first == buffer){
				if(cellAction.second.first == "s"){
					stmtStack.push(cellAction.second.second);
					cout << "shift: " << cellAction.second.second << "(" << buffer << ")" << "\n\n";
					if(in >> inp){
						buffer.clear();
						buffer += inp;
					}else{
						buffer.clear();
						buffer += "$";
					}
					break;

				}else if(cellAction.second.first == "r"){
					string reduce_nterm = reduceRules[cellAction.second.second].first;
					cout << "reduce: " << reduceRules[cellAction.second.second].first << " -> ";
					for(string symb: reduceRules[cellAction.second.second].second){
						cout << symb;	
						stmtStack.pop();
					}
					cout << '\n';
					int reduce_stmt = stmtStack.top();
					for(auto& cellGoto: gotoTable[reduce_stmt]){
						if(cellGoto.first == reduce_nterm){
							if(cellGoto.second == -1){
								cout << "error" << '\n';
								return 0;
							}else{
								stmtStack.push(cellGoto.second);
								cout <<"\tpushed: " <<  cellGoto.second << "\n\n";
								break;
							}
						}
					
					}
				}else if(cellAction.second.first == "err"){
					cout << "error" << '\n';
					return 0;
				}else if(cellAction.second.first == "acc"){
					cout << "accept" << '\n';
					return 0;
				}	
			
			}
		
		}
	
	
	}
	return 0;
}
