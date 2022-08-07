#pragma once
#include <vector>

#include "Token.h"

class Lexer {
	public:
	Lexer(const char*);
	Token next();
	Token peep();
	bool hasNext();
	Token getNext();
	std::vector<Token> getTokens();

	private:
	Token processName();
	Token processDigit();
	Token processNot();
	Token processAndOr(Token::Type type);
	Token processRelOp();
	Token atom(Token::Type type);

	char peek();
	char get();
	const char* code_p = nullptr;
};

