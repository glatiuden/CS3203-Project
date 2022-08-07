#pragma once
#include <vector>

using namespace std;

#include "StmtNode.h"

class CFGProcessor {
public:
	static void processCFGs(vector<StmtNode*> CFGs);
private:
	static void processCFGsForNext(vector<StmtNode*> CFGs);
	static void mapCFGNodes(vector<StmtNode*> CFGs);
};
