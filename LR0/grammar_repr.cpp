#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Rule{
	private:
		string head;
		vector<string> body;

	public:
		Rule(const string& ruleHead, const vector<string>& ruleTerms): head{ruleHead}, body{ruleTerms} {}

		void print(){
			cout << head << " -> ";
			
			for(const string& term : body){
				cout << term;
			
			}
		
			cout << '\n';
			return;
		}
		

};

class Grammar{
	private:
		vector<Rule> rules;

	public:
		Grammar(const vector<Rule> grammarRules): rules{grammarRules} {}
		
		vector<Rule> getRules(){
			return rules;
		}

		void print(){
			for(Rule rule: rules){
				rule.print();
			}
			return;
		}

};

Grammar grammarExpanding(const Grammar& grammar){
	vector<Rule> new_rules = grammar.getRules();
	string firstProd = new_rules.front();
	first

}



int main(){
	Rule plusrule("E", {"E", "+", "T", ";"});
	Rule multrule("T", {"T", "*", "F"});
	Grammar expr({plusrule, multrule});
	expr.print();
	return 0;
}
