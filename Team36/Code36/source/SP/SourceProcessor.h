#pragma once
#include <vector>

#include "../TNode.h"
#include "../PKB/StmtNode.h"

using namespace std;

class SourceProcessor {
public:
	SourceProcessor();
	void processSource(const char*, TNode* &AST, vector<StmtNode*> &CFGs);
};
