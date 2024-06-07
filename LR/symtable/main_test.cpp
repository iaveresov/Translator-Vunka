#include "symboltable.h"

int main(){
	SymbolTable symtab;
	symtab.alloc(-1);
	cout << symtab.addFunc("f", SymbType::Int) << '\n';
	cout << symtab.addFunc("f", SymbType::Int) << '\n';
	cout << symtab.addVar("f", -1, SymbType::Int) << '\n';
	cout << symtab.addVar("zhopa", -1, SymbType::Int, string{"123"}) << '\n';
	
	symtab.print_table();
	symtab.set_len(10, 1);
	return 0;
}
