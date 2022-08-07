#pragma once
#include <vector>

#include "../TNode.h"
#include "../Token.h"
#include "../PKB/StmtNode.h"

using namespace std;

class Parser {
public:
	Parser();
	TNode* Parse(const vector<Token>& tokens);
	vector<StmtNode*> createCFGs(TNode* AST);
	TNode* parseExpr(const vector<Token>& tokens, int start, int end);
	void resetStmtNum();
private:
	TNode* parseStmtLst(const vector<Token>& tokens, int start, int end);
	TNode* parseFactor(const vector<Token>& tokens, int start, int end);
	TNode* parseTerm(const vector<Token>& tokens, int start, int end);
	TNode* parseAssign(const vector<Token>& tokens, int start, int end);
	TNode* parseRelFactor(const vector<Token>& tokens, int start, int end);
	TNode* parseRelExpr(const vector<Token>& tokens, int start, int end);
	TNode* parseCondExpr(const vector<Token>& tokens, int start, int end);
	TNode* parseIf(const vector<Token>& tokens, int start, int end);
	TNode* parseWhile(const vector<Token>& tokens, int start, int end);
	TNode* parseSimpleStmt(const vector<Token>& tokens, int start);
	TNode* parseCall(const vector<Token>& tokens, int start);
	TNode* parseProc(const vector<Token>& tokens, int start, int end);
	StmtNode* procToCFG(TNode* proc);
	StmtNode* stmtLstToCFG(TNode* stmtLst);
	StmtNode* stmtToCFG(TNode* stmt, StmtNode* curr);
	StmtNode* whileToCFG(TNode* whileNode, StmtNode* curr);
	StmtNode* ifToCFG(TNode* ifNode, StmtNode* curr);
	StmtNode* mergeCFGNodes(StmtNode* curr, StmtNode* next);
	int stmtNum;
};
