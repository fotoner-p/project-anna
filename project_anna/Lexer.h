#pragma once
#include<fstream>
#include<string>
#include<vector>
using namespace std;

enum TokenType
{
	Plus = 1, Minus, Multi, Divi, Mod, Inc, Dec,			//Arithmetic operators
	Equal, NotEq, Less, LessEq, Great, GreatEq,				//relational operators
	LogiNot, LogiAnd, LogiOr,								//Logical operators
	BitNot, BitAnd, BitOr, BitXor, BitLShift, BitRShift,	//Bitwise operators

	AddAss, SubAss, MultiAss, DiviAss, ModAss,				//Compound assignment operators 1
	BAndAss, BOrAss, BXorAss, BLShiftAss, BRShiftAss,		//Compound assignment operators 2

	Assign, Comma, Dot, OneQ, DblQ, Semicolon, Import,		//special char1
	Lparen, Rparen, Lbrace, Rbrace, Lbracket, Rbracket,		//special char2

	If, Else, Switch, Case, For, While, On, Off, Func,		//reserved word
	Return, Integer, Float, String, Lilly, Vivid, ENTER, 	//reserved word
	Ident, ConstNum, ConstString, None1, None2,				//For inter processing1
	Letter, Digit, EofTkn, Others, END_list,				//For inter processing2

	PLEASURE //^^
};

struct KeyWord {
	const char *keyName;
	TokenType keyKind;
};

const KeyWord wordTable[] = {
	{"if" ,		 If		  }, {"else",  Else    },
	{"switch",   Switch	  }, {"case",  Case	   },
	{"for",		 For	  }, {"while", While   },
	{"on",		 On		  }, {"off",   Off	   },
	{"func",	 Func	  }, {"return",Return  },
	{"int",		 Integer  }, {"float", Float   },
	{"string",	 String	  }, {"lily",  Lilly    },
	{"vivid",	 Vivid	  }, {"ENTER", ENTER   },
	{"PLEASURE", PLEASURE }, {"",	   END_list }
};

struct Operator {
	const char *keyName;
	TokenType keyKind;
};

const Operator operTable[] = {
	{"//",  None1		}, {"/**/",  None2		},
	{"(",   Lparen		}, {")",     Rparen		},
	{"{",   Lbrace		}, {"}",     Rbrace		},
	{"[",   Lbracket	}, {"]",     Rbracket	},
	{"+",   Plus		}, {"-",     Minus		},
	{"*",   Multi		}, {"/",     Divi		},
	{"%",   Mod			}, {"++",    Inc		},
	{"--",  Dec			}, {"==",    Equal		},
	{"!=",  NotEq		}, {"<",     Less		},
	{"<=",  LessEq		}, {">",     Great		},
	{">=",  GreatEq		}, {"!",     LogiNot	},
	{"&&",  LogiAnd		}, {"||",    LogiOr		},
	{"~",   BitNot		}, {"&",     BitAnd		},
	{"|",   BitOr		}, {"^",     BitXor		},
	{"<<",  BitLShift	}, {">>",    BitRShift	},
	{"+=",  AddAss		}, {"-=",    SubAss		},
	{"*=",  MultiAss	}, {"/=",    DiviAss	},
	{"%=",  ModAss		}, {"&=",    BAndAss	},
	{"|=",  BOrAss		}, {"^=",    BXorAss	},
	{"<<=", BLShiftAss	}, {">>=",   BRShiftAss	},
	{"=",	Assign		}, {",",     Comma		},
	{".",	Dot			}, {"'",     OneQ		},
	{"\"",	DblQ		}, {"->",	 Import		},
	{";",	Semicolon	}, {"",		 END_list	}
};

struct Token
{
	TokenType type;
	string text;
	string data;

	Token() {
		type = Others;
		text = "";
		data = "";
	}
	Token(TokenType k, const string& s, const string& d = "") : type(k), text(s), data(d) {}
};


class Lexer
{
public:
	Lexer() = delete;
	Lexer(const string& path);
	void scanning();

	vector<Token> token_list;

	~Lexer();
private:
	int next_char();
	bool is_operator(int c1, int c2);
	Token token_scan();
	Token digit_scan(string& str, int& ch);
	Token float_scan(string& str, int& ch);
	void scanning_fail(const string& str);
	TokenType get_type(const string& s);

	int cur_input;
	TokenType char_type[256];
	ifstream fin;
};

