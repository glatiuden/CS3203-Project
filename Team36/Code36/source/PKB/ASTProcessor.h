#pragma once

class TNode;

class ASTProcessor {
public:
	static void processAST(TNode* root);
	static void processASTForPattern(TNode* root);
	static void processASTForFollows(TNode* root);
	static void processASTForParents(TNode* root);
	static void processASTForCalls(TNode* root);
	static void processASTForUsesModifies(TNode* root);
	static void processASTForStatements(TNode* root);
};