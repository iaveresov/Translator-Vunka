#include<iostream>
#include<cstdarg>
#include<string>

using namespace std;

string print(int n, ...){
	string result = "";
	va_list strings;
	va_start(strings, n);
	for(int i = 0; i < n; ++i){
		cout << va_arg(strings, char*);
	
	}
	
	va_end(strings);
	return result;

}


int main(){

	cout << print(1,"a") << '\n';
	return 0;
}
