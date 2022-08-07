#pragma once
#include <vector>
#include "StmtNode.h"

class StmtTree {
private:
	StmtNode* root;

public:
	StmtTree(StmtNode* root) : root(root) {};
	StmtNode* getRoot();
	void linkNode(StmtNode* parent, StmtNode* child);
	vector<StmtData> toVector();
};