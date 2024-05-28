#include "./symboltable.h"



void print_data(SymbolData* tstring){
	cout << tstring->code << " " << static_cast<int>(tstring->kind) << " " << static_cast<int>(tstring->type) << " " << tstring->default_val << " " << tstring->scope << " " << tstring->len;
}


int SymbolTable::alloc(int scope){
	string name = "[tmp" + to_string(tmp_code++) + "]";	
	SymbolData* new_string = new SymbolData{max_code++, SymbKind::var, SymbType::Int, "None", scope};
	symtable[name] = new_string;
	return new_string->code;
}
		
int SymbolTable::checkVar(int scope, string name){
	auto ptr = symtable.find(name);
	if(ptr == symtable.end()){
		if(scope != -1){
			return checkVar(GLOBAL_SCOPE, name);
		}
		return -1;
	}
	return  ptr->second->code;
}

int SymbolTable::checkFunc(string name, int len){
	auto ptr = symtable.find(name);
	if(ptr == symtable.end()){
		return -1;
	}

	if(ptr->second->len != len){
		return -1;
	}
	
	return ptr->second->code;
}

int SymbolTable::addVar(string name, int scope, SymbType type, int init){
	int var_code = checkVar(GLOBAL_SCOPE, name);
	int func_code = checkFunc(name, 0);
	if(var_code == -1 and func_code == -1){
		SymbolData* new_string = new SymbolData{max_code++, SymbKind::var, SymbType::Int, to_string(init), scope};
		symtable[name] = new_string;
		return symtable[name]->code;	
	}
	return -1;
}	

int SymbolTable::addFunc(string name, SymbType type){
	int var_code = checkVar(GLOBAL_SCOPE, name);
	int func_code = checkFunc(name, 0);
	if(var_code == -1 and func_code == -1){
		SymbolData* new_string = new SymbolData{max_code++, SymbKind::func, type, "None", GLOBAL_SCOPE, 0};
		symtable[name] = new_string;
		return symtable[name]->code;
	}
	return -1;
}

void SymbolTable::print_table(){
	for(const auto& [name, tstring]: symtable){
		cout << name << " ";
		print_data(tstring);
		cout << '\n';
	}
}



