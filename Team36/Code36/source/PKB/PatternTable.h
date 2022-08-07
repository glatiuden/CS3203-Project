#pragma once

#include "../TNode.h"
#include "StmtData.h"

class PatternTable {
private:
	vector<TNode*> assignRoots;
	vector<StmtData> whileVars;
	vector<StmtData> ifVars;
public:
	PatternTable() {};
	PatternTable(const vector<TNode*>& assignRoots, 
				 const vector<StmtData>& whileVars, 
				 const vector<StmtData>& ifVars) :
		assignRoots(assignRoots), whileVars(whileVars), ifVars(ifVars) {};
	vector<StmtData> getPatternAssignExactMatch(string lhs, TNode* rhs);
	vector<StmtData> getPatternAssignPartialMatch(string lhs, TNode* rhs);
	vector<StmtData> getPatternIf();
	vector<StmtData> getPatternIf(string varName);
	vector<StmtData> getPatternWhile();
	vector<StmtData> getPatternWhile(string varName);
	void clearAllEntries();
};