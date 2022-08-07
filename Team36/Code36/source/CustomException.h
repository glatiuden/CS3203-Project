#pragma once
#include <exception>
#include <string>

using namespace std;

struct SourceProcessorException : exception {
	SourceProcessorException(const string& m);
};

struct LexerException : SourceProcessorException {
	LexerException(const string& m);
};

struct SourceValidatorException : SourceProcessorException {
	SourceValidatorException(const string& m);
};

struct ParserException : SourceProcessorException {
	ParserException(const string& m);
};

struct QPSException : exception {
	QPSException(const string& m);
};

struct QPSValidatorException : QPSException {
	QPSValidatorException(const string& m);
};

struct QPSClauseParserException : QPSException {
	QPSClauseParserException(const string& m);
	QPSClauseParserException(const string& m, const bool& isBoolean);
	bool isBoolean = false;
};

struct QPSEvaluatorException : QPSException {
	QPSEvaluatorException(const string& m);
};

struct QPSEvaluatorNonTargetSelectionException : QPSException {
	QPSEvaluatorNonTargetSelectionException(const string& m);
};

struct QPSTreeException : QPSException {
	QPSTreeException(const string& m);
};

struct TNodeException : exception {
	TNodeException(const string& m);
};
