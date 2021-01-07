#include "symtbl.h"
#include <iostream>
using namespace std;

SymbolTable::SymbolTable(string name)
{
	tableName = name;
}

void SymbolTable::enter(Symbol sym)
{
	table.push_back(sym);
}

void ProgramTable::setSymbolic()
{
	for (auto it = tokenList.begin(); it != tokenList.end(); it++) {
		if ((*it).type == Func) {
			it++;
			if ((*it).type == Ident) {
				localList.push_back((*it).text);
				SymbolTable& curList = localList.back();

				it++;
				if ((*it).type != Lparen)
					syntaxFail();

				do {
					it++;

					bool lily_flag = false;
					bool arr_flag = false;

					string name;
					symbol_type type;

					if ((*it).type == Lilly) {
						it++;
						lily_flag = true;
					}

					switch ((*it).type)
					{
					case Integer: type = PAR_INT; break;
					case Float: type = PAR_FLOAT; break;
					case String: type = PAR_STRING; break;
					case Vivid: type = PAR_VIVID; break;
					}
					curList.parameter_count++;
					it++;

					if ((*it).type != Ident)
						syntaxFail();
					vector<Token>::iterator tkIt = it;
					name = (*it).text;
					if (search(curList, name)) {
						insertFail();
					}
					it++;
					if ((*it).type == Lbracket) {
						it++;
						if ((*it).type != Rbracket)
							syntaxFail();
						arr_flag = true;
						it++;
					}
					curList.enter(Symbol(name, type, &(*tkIt), arr_flag, lily_flag));
				} while ((*it).type == Comma);

				if ((*it).type != Rparen)
					syntaxFail();
				it++;

				if ((*it).type != Lbrace)
					syntaxFail();
				do {
					if ((*it).type == Ident) 
						functionIdent(curList, it, (*it).text);
					
					else if ((*it).type != Integer && (*it).type != String && (*it).type != Vivid && (*it).type != Lilly && (*it).type != Float)
						it++;

	   				else
						parseIdent(curList, it);
				} while ((*it).type != Rbrace);
			}
		}
		else {
			if ((*it).type == Ident) 
				functionIdent(globalList, it, (*it).text);
			
			else if ((*it).type != Integer && (*it).type != String && (*it).type != Vivid && (*it).type != Lilly && (*it).type != Float)
				it++;

			else
				parseIdent(globalList, it);
		}
	}
}

bool ProgramTable::search(SymbolTable & curList, string & name)
{
	for (auto symbol : curList.table) {
		if (symbol.name == name)
			return true;
	}
	return false;
}

void ProgramTable::insertFail()
{
	cerr << "같은 이름의 식별자 중복 선언" << endl;
	exit(1);
}

void ProgramTable::syntaxFail()
{
	cerr << "구문 오류" << endl;
	exit(1);
}

void ProgramTable::parseIdent(SymbolTable& curList, vector<Token>::iterator& it)
{
	bool lily_flag = false;
	bool arr_flag = false;

	string name;
	symbol_type type;

	if ((*it).type == Lilly) {
		it++;
		lily_flag = true;
	}

	switch ((*it).type)
	{
	case Integer: type = INT; break;
	case Float: type = FLOAT; break;
	case String: type = STRING; break;
	case Vivid: type = VIVID; break;
		//case Func: type = FUNC; break;
	}

	it++;

	do {
		if ((*it).type != Ident)
			syntaxFail();
		vector<Token>::iterator tkIt = it;
		name = (*it).text;
		if (search(curList, name)) {
			insertFail();
		}
		it++;
		if ((*it).type == Lbracket) {
			it++;
			if ((*it).type != Rbracket)
				syntaxFail();
			arr_flag = true;
		}

		curList.enter(Symbol(name, type, &(*tkIt), arr_flag, lily_flag));
	} while ((*it).type == Comma);
}

void ProgramTable::functionIdent(SymbolTable & curList, vector<Token>::iterator & it, string& name)
{
	vector<Token>::iterator tkIt = it;
	it++;

	if ((*it).type == Lparen) {
		int count = 0;
		while (true) {
			it++;
			if ((*it).type == Ident || (*it).type == ConstNum || (*it).type == ConstString) {
				count++;
				it++;
				if ((*it).type == Comma)
					continue;
			}
			if ((*it).type == Rparen) {
				it++;
				for (auto list : localList) {
					if (list.parameter_count == count && list.tableName == name) {
						curList.enter(Symbol(name, FUNC, &(*tkIt)));
						return;
					}
				}
				syntaxFail();
			}
			else
				syntaxFail();

		}
	}
}
