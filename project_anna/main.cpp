#include<iostream>
#include<string>
#include<iomanip>
#include"Lexer.h"
#include"symtbl.h"
using namespace std;

int main(int argc, char * argv[]) {
	if (argc == 1)
		return 1;

	Lexer lex(argv[1]);
	lex.scanning();
	cout << "text          : type : data" << endl;
	cout << "---------------------------" << endl;
	
	for (Token token : lex.token_list) 
		cout << left << setw(11) << token.text << right << setw(5) << ": " << setw(4) << right<< token.type << " : " << token.data << endl;
	
	ProgramTable sym(lex.token_list);
	sym.setSymbolic();

	cout << endl << "*GLOBAL SYMBOL LIST*" << endl;
	for (auto symbol : sym.globalList.table) 
		cout << left << setw(16) <<  symbol.name << ": " << symbol.type  << " : [PTR: " << symbol.ptr << " ]" << endl;;
	
	cout << endl << "*LOCAL SYMBOL LIST*" << endl;
	for (auto list : sym.localList) {
		cout << setfill('-') << '{' << setw(23) << list.tableName + '}' <<  left << "<Parameter count: " << list.parameter_count << '>' << endl;
		cout << setfill(' ');
		for (auto symbol : list.table) 
			cout << left << setw(16) << symbol.name << ": " << symbol.type << " : [PTR: "<< symbol.ptr << " ]" << endl;;
		
		cout << endl;
	}

	return 0;
}