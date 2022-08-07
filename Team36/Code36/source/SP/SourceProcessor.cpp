#include "SourceProcessor.h"

#include "../Lexer.h"
#include "Parser.h"
#include "SourceValidator.h"

SourceProcessor::SourceProcessor() = default;

void SourceProcessor::processSource(const char* source, TNode * &tree, vector<StmtNode*> &CFGs) {
	Lexer lex(source);
	Parser parser;
	vector<Token> tokens;

	tokens = lex.getTokens();
	SourceValidator(tokens).validate();
	tree = parser.Parse(tokens);
	CFGs = parser.createCFGs(tree);

	return;
}
