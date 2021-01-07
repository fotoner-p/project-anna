#pragma onc0e
#include<string>
#include<vector>
#include"Lexer.h"

enum symbol_type {
	NONE, INT, FLOAT, STRING, VIVID, PAR_INT, PAR_FLOAT, PAR_STRING, PAR_VIVID, FUNC, IDENT,
};

struct Symbol
{
	string name;
	symbol_type type;
	bool arr_flag = false;
	bool lily_flag = false;
	Token * ptr;
	Symbol(const string& n, symbol_type t, Token * it) :name(n), type(t), ptr(it) {}
	Symbol(const string& n, symbol_type t, Token * it, bool af, bool lf)
		:name(n), type(t), ptr(it), arr_flag(af), lily_flag(lf){}
};

class SymbolTable {
public:
	SymbolTable() = delete;
	SymbolTable(string name);
	void enter(Symbol sym);
	int parameter_count = 0;
	vector<Symbol> table;
	string tableName;
};

class ProgramTable {
public:
	ProgramTable() = delete;
	ProgramTable(vector<Token>& tkList) : globalList("top"), tokenList(tkList) {};
	void setSymbolic();

	SymbolTable globalList;
	vector<SymbolTable> localList;

private:
	bool search(SymbolTable& curList, string& name);
	void insertFail();
	void syntaxFail();
	void parseIdent(SymbolTable& curList, vector<Token>::iterator& it);
	void functionIdent(SymbolTable& curList, vector<Token>::iterator& it, string& name);
	vector<Token>& tokenList;
};