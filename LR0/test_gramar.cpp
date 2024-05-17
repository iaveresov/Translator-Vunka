#include<iostream>
#include<vector>
#include<string>
#include<stdarg.h>


using namespace std;

class Rule{
	private:
		string head;
		vector<string> body;
	public:
		Rule(string ruleHead, ...){

			head = ruleHead;
			body = ruleBody;
			return;
		}

		void print(){
			cout << head << " -> ";
			for(string term : body){
				cout << term;
			}
	
			cout << '\n';
			return;
		}

};

int main(){
	Rule plusrule("E" , {"E", "+", "T", ";"});
	plusrule.print();
	Rule multrule("T", {"T", "*", "F", ";"});
	multrule.print();
	return 0;
}
