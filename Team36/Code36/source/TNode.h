#pragma once
#include <string>
#include <vector>

#include "Token.h"
#include "NodeType.h"

using namespace std;

typedef string TNodeValue;

class TNode {
public:
	TNode();

	static TNode* tokenToNode(Token token, int stmtNum);
	static TNode* createProcNode(string procName);
	static TNode* createStmtLstNode();
	static TNode* createCallNode(string procName, int stmtNum);
	bool isEqualExclStmtNo(TNode* tree);
	bool hasSubtree(TNode* subtree);
	void addChild(TNode* child);
	vector<TNode*> getChildren();
	NodeType getType();
	TNodeValue getValue();
	int getStmtNum();

private:
	vector<TNode*> children;
	NodeType type;
	TNodeValue value = "";
	int stmtNum = -1;
	TNode(NodeType type);
	TNode(NodeType type, TNodeValue value);
	TNode(NodeType type, TNodeValue value, int stmtNum);
};
