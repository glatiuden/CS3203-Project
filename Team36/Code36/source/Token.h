#pragma once
#include <string>

class Token {
public:
	enum class Type {
		// PEEP
		CHAR,

		// BASIC
		DIGIT,
		NAME,

		// KEYWORDS
		PROCEDURE,
		READ,
		PRINT,
		CALL,
		WHILE,
		IF,
		THEN,
		ELSE,

		// SYMBOLS
		OCTOTHORP,
		PERIOD,
		COMMA,
		SEMICOLON,
		DOUBLE_QUOTE,
		LEFT_PARENTHESIS,
		RIGHT_PARENTHESIS,
		LEFT_CURLY,
		RIGHT_CURLY,

		// MATH OPERATORS
		EQUAL,
		PLUS,
		MINUS,
		MULTIPLY,
		DIVIDE,
		MODULO,
		UNDERSCORE,

		// CMP OPERATORS
		NOT,
		AND,
		OR,
		GREATER,
		GREATER_EQ,
		LESS,
		LESS_EQ,
		DBL_EQUAL,
		NOT_EQUAL,

		END_OF_FILE
	};

	Token();
	Token(Type, const char*, std::size_t);
	Token(Type, const char*, const char*);

	Type getType() const;
	std::string getValue() const;

	// Helper Functions
	bool isName();
	bool isDigit();
	bool isAnd();
	bool isOr();
	bool isPeriod();
	bool isComma();
	bool isSemicolon();
	bool isDoubleQuote();
	bool isEqual();
	bool isPlus();
	bool isMinus();
	bool isMultiply();
	bool isDivide();
	bool isModulo();
	bool isUnderscore();
	bool isLAngle();
	bool isRAngle();
	bool isLParam();
	bool isRParam();
	bool isLCurly();
	bool isRCurly();

	bool isNot();
	bool isConditionalOp();

	void setType(Type t);

private:
	Type type{};
	std::string value{};
};
