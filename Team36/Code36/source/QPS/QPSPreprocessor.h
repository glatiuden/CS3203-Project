#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "../Token.h"
#include "../Lexer.h"
#include "../QPS/QPSTree/QPSTree.h"
#include "QPSClauseParser.h"

using namespace std;

// Use to verify the type of clause that has been executed
enum class ClauseType {
	NONE,
	SUCHTHAT,
	PATTERN,
	WITH
};

struct QPSPreprocessorResult {
	bool isValid;
	string message;
};

struct QPSPreprocessorElemResult {
	bool isValid = false;
	bool hasAttrName = false;
	Token synonymToken;
	Token attrNameToken;
};

class QPSPreprocessor {
public:
	QPSPreprocessor();
	QPSPreprocessor(QPSClauseParser& qpsClauseParser);
	QPSPreprocessorResult validateQuery(const string& queryStr);
private:
	ClauseType prevRelRefType = ClauseType::NONE;
	QPSClauseParser& qpsClauseParser;
	IterationCount iterationCount = -1;
	bool isInVector(const Token token, const vector<string> v);
	bool isInVector(const NodeType nodeType, const vector<NodeType> v);
	bool isStmtRef(Token token, const Direction direction);
	bool isEntRef(Token token, Lexer& lex, const Direction direction, bool isPattern);
	bool isEntRef(Lexer& lex, const Direction direction, bool isPattern);
	bool isRef(Lexer& lex, const Direction direction);
	bool isAttrName(Lexer& lex, NodeType& test);
	void validateDeclaration(const Token token, Lexer& lex);
	void validateSelect(Lexer& lex);
	void validateResultClause(Token token, Lexer& lex);
	void validateSuchThat(Lexer& lex);
	void validatePattern(Lexer& lex);
	void validateWith(Lexer& lex);
	void validateSynonym(Token token);
	void validateTupleSynonym(const Token token);
	void validateStmtRef(const Token token, const Direction direction);
	void validateEntRef(Lexer& lex, const Direction direction, bool isPattern);
	void validateExpressionSpec(Lexer& lex);
	void validateExpression(vector<Token> expr);
	void validateLeftParenthesis(Token token);
	void validateRightParenthesis(Token token);
	void validateComma(Token token);
	void validateUnderscore(Token token);
	void validateDoubleQuote(Token token);
	void createResultNode(const QPSPreprocessorElemResult elemResult);
	NodeType getArgNodeTypeFromMap(const string value);
	QPSPreprocessorElemResult checkElem(Token token, Lexer& lex);
};
