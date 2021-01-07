#include <iostream>
#include "Lexer.h"
using namespace std;

Lexer::Lexer(const string & path)
{
	cur_input = 0;
	fin.open(path, ios::in);
	if (!fin.is_open()) {
		cerr << "Open Fail" << endl;
		exit(1);
	}
	//char type init
	for (int i = 0; i < 256; ++i) char_type[i] = Others;
	for (int i = '0'; i < '9'; ++i) char_type[i] = Digit;
	for (int i = 'A'; i < 'Z'; ++i) char_type[i] = Letter;
	for (int i = 'a'; i < 'z'; ++i) char_type[i] = Letter;
	char_type['_'] = Letter;
	char_type['('] = Lparen;	char_type[')'] = Rparen;
	char_type['{'] = Lbrace;	char_type['}'] = Rbrace;
	char_type['['] = Lbracket;	char_type[']'] = Rbracket;
	char_type['<'] = Less;		char_type['>'] = Great;
	char_type['+'] = Plus;		char_type['-'] = Minus;
	char_type['*'] = Multi;		char_type['/'] = Divi;
	char_type['_'] = Letter;	char_type['='] = Assign;
	char_type[','] = Comma;		char_type['"'] = DblQ; 
	char_type['\''] = OneQ;		char_type['.'] = Dot;
}

void Lexer::scanning()
{
	for (Token token = token_scan(); token.type != EofTkn; token = token_scan()) {
		if (token.type == None1 || token.type == None2)
			continue;
		token_list.push_back(token);
	}
}

Token Lexer::token_scan()
{
	TokenType kt;
	string str;
	int ch0;
	static int ch = ' ';

	while (isspace(ch))
		ch = next_char();

	if (ch == EOF)
		return Token(EofTkn, str);

	switch (char_type[ch]) {
	case Letter: //식별자 or 키워드
		str += ch;
		ch = next_char();

		while (char_type[ch] == Letter || char_type[ch] == Digit) {
			str += ch;
			ch = next_char();
		}
		break;

	case Plus:
		str += ch;
		ch = next_char();

		switch (char_type[ch])
		{
		case Equal:
			str += ch;
			ch = next_char();
			break;
		case Plus:
			str += ch;
			ch = next_char();
			break;
		case Digit:
			return digit_scan(str, ch);
		case Dot:
			return float_scan(str, ch);
		}
		break;

	case Dot:
		str += ch;
		ch = next_char();
		switch (char_type[ch]) 
		{
		case Digit:
			return float_scan(str, ch);
		}
		break;

	case Minus:
		str += ch;
		ch = next_char();

		switch (char_type[ch])
		{
		case Equal:	// -=
			str += ch;
			ch = next_char();
			break;
		case Minus:	// --
			str += ch;
			ch = next_char();
			break;
		case Great:
			str += ch;
			ch = next_char();
			break;
		case Digit:	// -(int)
			return digit_scan(str, ch);

		case Dot:	// -.*
			return float_scan(str, ch);
		}
		break;

	case Digit:
		return digit_scan(str, ch);

	case Divi:
		str += ch;
		ch = next_char();
		switch (char_type[ch])
		{
		case Divi: // 한 줄 주석
			str += ch;
			while (next_char() != '\n');
			ch = next_char();
			break;
		case Multi: // 여러 줄 주석
			str += ch;
			for (ch = next_char(); ch != EOF; ch = next_char()) {
				if (ch == '*') {
					ch = next_char();
					if (ch == '/') {
						str += "*/";
						ch = next_char();
						break;
					}
				}
			}
			break;
		case Equal: // /= 
			str += ch;
			ch = next_char();
			break;
		}
		break;

	case DblQ:
		for (ch = next_char(); ch != EOF && ch != '\n' && ch != '"'; ch = next_char())
			str += ch;

		if (ch != '"') {
			cerr << "문자열 리터럴을 닫지 않음" << endl;
			exit(1);
		}

		ch = next_char();
		return Token(ConstString, str, str);

	case Less: // <
		str += ch;
		ch = next_char();

		switch (char_type[ch]) {
		case Less: // <<
			str += ch;
			ch = next_char();
			if (ch == '=') { // <<=
				str += ch;
				ch = next_char();
			}
			break;

		case Equal: // <=
			str += ch;
			ch = next_char();
			break;
		}
		break;

	case Great: // >
		str += ch;
		ch = next_char();

		switch (char_type[ch]) {
		case Great: // >>
			str += ch;
			ch = next_char();
			if (ch == '=') { // >>=
				str += ch;
				ch = next_char();
			}
			break;

		case Equal: // >=
			str += ch;
			ch = next_char();
			break;
		}
		break;

	default:
		str += ch;
		ch0 = ch;
		ch = next_char();

		if (is_operator(ch0, ch)) {
			str += ch;
			ch = next_char();
		}
	}
	kt = get_type(str);                     /* 종류 설정 */

	if (kt == Others)
		scanning_fail(str);

	return Token(kt, str);
}

Token Lexer::digit_scan(string& str, int& ch)
{
	str += ch;
	ch = next_char();

	while (char_type[ch] == Digit) {
		str += ch;
		ch = next_char();
	}

	if (ch != '.')
		return Token(ConstNum, str, str);

	return float_scan(str, ch);
}

Token Lexer::float_scan(string& str, int& ch)
{
	str += ch;
	ch = next_char();

	while (char_type[ch] == Digit) {
		str += ch;
		ch = next_char();
	}

	if (ch == 'e' || ch == 'E') {
		str += ch;
		ch = next_char();

		switch (char_type[ch]) {
		case Plus:
			str += ch;
			ch = next_char();
			if (char_type[ch] == Digit) {
				str += ch;
				ch = next_char();
				while (char_type[ch] == Digit) {
					str += ch;
					ch = next_char();
				}
			}
			break;
		case Minus:
			str += ch;
			ch = next_char();
			if (char_type[ch] == Digit) {
				str += ch;
				ch = next_char();
				while (char_type[ch] == Digit) {
					str += ch;
					ch = next_char();
				}
			}
			break;
		case Digit:
			str += ch;
			ch = next_char();
			while (char_type[ch] == Digit) {
				str += ch;
				ch = next_char();
			}
			break;
		default:
			scanning_fail(str);
		}
		return Token(ConstNum, str, str);
	}
	else
		return Token(ConstNum, str, str);
}

void Lexer::scanning_fail(const string& str)
{
	cerr << "잘못된 토큰: " << str << endl;
	exit(1);
}

Lexer::~Lexer()
{
	fin.close();
}

bool Lexer::is_operator(int c1, int c2)
{
	char s[] = "    ";

	if (c1 == '\0' || c2 == '\0')
		return false;

	s[1] = c1;
	s[2] = c2;

	return strstr(" *= && || |= != ^= ", s) != NULL;
}

int Lexer::next_char()
{
	static int c = 0;
	if (c == EOF)
		return c;

	if ((c = fin.get()) == EOF)
		fin.close();  /* 종료 */

	return c;
}

TokenType Lexer::get_type(const string & s)
{
	if (char_type[s[0]] == Letter) {
		for (int i = 0; wordTable[i].keyKind != END_list; i++) {
			if (s == wordTable[i].keyName)
				return wordTable[i].keyKind;
		}
	}
	else {
		for (int i = 0; operTable[i].keyKind != END_list; i++) {
			if (s == operTable[i].keyName)
				return operTable[i].keyKind;
		}
	}
	if (char_type[s[0]] == Letter) return Ident;
	if (char_type[s[0]] == Digit)  return ConstNum;
	return Others;
}
