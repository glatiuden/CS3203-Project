#include "CustomException.h"

SourceProcessorException::SourceProcessorException(const string& m) : exception(m.c_str()) {}

LexerException::LexerException(const string& m) : SourceProcessorException(m) {}

SourceValidatorException::SourceValidatorException(const string& m) : SourceProcessorException(m) {}

ParserException::ParserException(const string& m) : SourceProcessorException(m) {}

QPSException::QPSException(const string& m) : exception(m.c_str()) {}

QPSValidatorException::QPSValidatorException(const string& m) : QPSException(m) {}

QPSClauseParserException::QPSClauseParserException(const string& m) : QPSException(m), isBoolean(false) {}

QPSClauseParserException::QPSClauseParserException(const string& m, const bool& isBoolean) : QPSException(m), isBoolean(isBoolean) {}

QPSEvaluatorException::QPSEvaluatorException(const string& m) : QPSException(m) {}

QPSTreeException::QPSTreeException(const string& m) : QPSException(m) {}

QPSEvaluatorNonTargetSelectionException::QPSEvaluatorNonTargetSelectionException(const string& m) : QPSException(m) {}

TNodeException::TNodeException(const string& m) : exception(m.c_str()) {}
