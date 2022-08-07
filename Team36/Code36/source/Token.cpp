#include "Token.h"

#include <string>
#include <stdexcept>

#include "Token.h"

using namespace std;

Token::Token() {}
Token::Token(Type type, const char* beg, size_t len) : type{ type }, value(beg, len) {}
Token::Token(Type type, const char* beg, const char* end) : type{ type }, value(beg, distance(beg, end)) {}

Token::Type Token::getType() const { return type; }
string Token::getValue() const { return value; }

bool Token::isName() { return type == Token::Type::NAME; }

bool Token::isDigit() { return type == Token::Type::DIGIT; }

bool Token::isAnd() { return type == Token::Type::AND; }

bool Token::isOr() { return type == Token::Type::OR; }

bool Token::isPeriod() { return type == Token::Type::PERIOD; }

bool Token::isComma() { return type == Token::Type::COMMA; }

bool Token::isSemicolon() { return type == Token::Type::SEMICOLON; }

bool Token::isDoubleQuote() { return type == Token::Type::DOUBLE_QUOTE; }

bool Token::isEqual() { return type == Token::Type::EQUAL; }

bool Token::isPlus() { return type == Token::Type::PLUS; }

bool Token::isMinus() { return type == Token::Type::MINUS; }

bool Token::isMultiply() { return type == Token::Type::MULTIPLY; }

bool Token::isDivide() { return type == Token::Type::DIVIDE; }

bool Token::isModulo() { return type == Token::Type::MODULO; }

bool Token::isUnderscore() { return type == Token::Type::UNDERSCORE; }

bool Token::isLAngle() { return type == Token::Type::LESS; }

bool Token::isRAngle() { return type == Token::Type::GREATER; }

bool Token::isLParam() { return type == Token::Type::LEFT_PARENTHESIS; }

bool Token::isRParam() { return type == Token::Type::RIGHT_PARENTHESIS; }

bool Token::isLCurly() { return type == Token::Type::LEFT_CURLY; }

bool Token::isRCurly() { return type == Token::Type::RIGHT_CURLY; }

bool Token::isNot() { return type == Token::Type::NOT; }

bool Token::isConditionalOp() {
	// Possible Optimization : set isCondOp flag when token is instantiated
	switch (type) {
		case Token::Type::GREATER:
		case Token::Type::GREATER_EQ:
		case Token::Type::LESS:
		case Token::Type::LESS_EQ:
		case Token::Type::DBL_EQUAL:
		case Token::Type::NOT_EQUAL:
			return true;
		default:
			return false;
	}
}

void Token::setType(Token::Type t) { type = t; }
