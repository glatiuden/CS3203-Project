#include <ctype.h>
#include "Token.h"
#include "Lexer.h"
#include <iostream>
#include <vector>
#include "CustomException.h"

using namespace std;
Lexer::Lexer(const char* code_p) : code_p{ code_p } {}

Token Lexer::peep() {
	while (isspace(peek())) get();
	char nxt_char = peek();
	
	if (nxt_char == '.') {
		return Token(Token::Type::PERIOD, code_p, 1);
	} else {
		return Token(Token::Type::CHAR, code_p, 1);
	}
}

Token Lexer::next() {
	while (isspace(peek())) get();
	char nxt_char = peek();

	if (isalpha(nxt_char)) {
		return processName();
	}
	if (isdigit(nxt_char)) {
		return processDigit();
	}

	switch (nxt_char) {
		case '#':
			return atom(Token::Type::OCTOTHORP);
		case '.':
			return atom(Token::Type::PERIOD);
		case ',':
			return atom(Token::Type::COMMA);
		case ';':
			return atom(Token::Type::SEMICOLON);
		case '"':
			return atom(Token::Type::DOUBLE_QUOTE);
		case '=':
			return processRelOp();
		case '+':
			return atom(Token::Type::PLUS);
		case '-':
			return atom(Token::Type::MINUS);
		case '*':
			return atom(Token::Type::MULTIPLY);
		case '/':
			return atom(Token::Type::DIVIDE);
		case '%':
			return atom(Token::Type::MODULO);
		case '(':
			return atom(Token::Type::LEFT_PARENTHESIS);
		case ')':
			return atom(Token::Type::RIGHT_PARENTHESIS);
		case '_':
			return atom(Token::Type::UNDERSCORE);
		case '{':
			return atom(Token::Type::LEFT_CURLY);
		case '}':
			return atom(Token::Type::RIGHT_CURLY);
		case '!':
			return processNot();
		case '&':
			return processAndOr(Token::Type::AND);
		case '|':
			return processAndOr(Token::Type::OR);
		case '>':
			return processRelOp();
		case '<':
			return processRelOp();
		case '\0':
			return atom(Token::Type::END_OF_FILE);
		default:
			throw LexerException("Invalid Token");
	}
};

bool Lexer::hasNext() {
	if (peek() == '\0') {
		return false;
	} else {
		return true;
	}
}

// get next() only if avalable else throw error.
Token Lexer::getNext() {
	if (hasNext()) {
		return next();
	} else {
		throw LexerException("End of file reached!");
	}
}


Token Lexer::processName() {
	const char* start = code_p;
	get();
	while (isalpha(peek()) || isdigit(peek())) get();
	return Token(Token::Type::NAME, start, code_p);
};

Token Lexer::processDigit() {
	const char* start = code_p;
	if (get() == '0' && isdigit(peek())) throw LexerException("Start of Digit cannot be 0");
	// Due to this behavior "123abc" is considered 2 tokens , DIGIT NAME.
	while (isdigit(peek())) get();
	return Token(Token::Type::DIGIT, start, code_p);
};

Token Lexer::processNot() {
	const char* start = code_p;
	get();

	if (peek() == '=') {
		get();
		return Token(Token::Type::NOT_EQUAL, start, code_p);
	}

	return Token(Token::Type::NOT, start, code_p);
}

Token Lexer::processAndOr(Token::Type type) {
	const char* start = code_p;
	const char first = get();

	if (first == peek()) {
		get();
		return Token(type, start, code_p);
	}
	throw LexerException("Invalid Token");
}

Token Lexer::processRelOp() {
	const char* start = code_p;
	const char first = get();

	if (peek() == '=') {
		get();
		if (first == '>') {
			return Token(Token::Type::GREATER_EQ, start, code_p);
		}

		if (first == '<') {
			return Token(Token::Type::LESS_EQ, start, code_p);
		}

		return Token(Token::Type::DBL_EQUAL, start, code_p);
	}

	if (first == '>') {
		return Token(Token::Type::GREATER, start, code_p);
	}

	if (first == '<') {
		return Token(Token::Type::LESS, start, code_p);
	}

	return Token(Token::Type::EQUAL, start, code_p);
}

Token Lexer::atom(Token::Type type) { return Token(type, code_p++, 1); }

char Lexer::peek() { return *code_p; }
char Lexer::get() { return *code_p++; }

vector<Token> Lexer::getTokens() {
	vector<Token> v;
	for (
		auto token = this->next();
		token.getType() != Token::Type::END_OF_FILE;
		token = this->next()
		)  v.push_back(token);
	return v;
}