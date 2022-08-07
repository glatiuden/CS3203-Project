#pragma once
#include "PatternTable.h"

vector<StmtData> PatternTable::getPatternAssignExactMatch(string lhs, TNode* rhs) {
	vector<StmtData> result;

	for (TNode* root : assignRoots) {
		TNode* lhsNode = root->getChildren()[0];
		TNode* rhsNode = root->getChildren()[1];
		if (lhsNode->getValue() != lhs && lhs != "_") {
			continue;
		}

		StmtData lhsData(lhsNode->getStmtNum(), lhsNode->getType(), lhsNode->getValue());
		if (!rhs || rhsNode->isEqualExclStmtNo(rhs)) {
			result.push_back(lhsData);
		}
	}

	return result;
}
vector<StmtData> PatternTable::getPatternAssignPartialMatch(string lhs, TNode* rhs) {
	vector<StmtData> result;

	for (TNode* root : assignRoots) {
		TNode* lhsNode = root->getChildren()[0];
		TNode* rhsNode = root->getChildren()[1];
		if (lhsNode->getValue() != lhs && lhs != "_") {
			continue;
		}

		StmtData lhsData(lhsNode->getStmtNum(), lhsNode->getType(), lhsNode->getValue());
		if (!rhs || rhsNode->hasSubtree(rhs)) {
			result.push_back(lhsData);
		}
	}
	return result;
}

vector<StmtData> PatternTable::getPatternIf() {
	return ifVars;
}

vector<StmtData> PatternTable::getPatternIf(string varName) {
	vector<StmtData> result;
	for (StmtData sd : ifVars) {
		if (sd.getValue() == varName) {
			result.push_back(sd);
		}
	}
	return result;
}

vector<StmtData> PatternTable::getPatternWhile() {
	return whileVars;
}

vector<StmtData> PatternTable::getPatternWhile(string varName) {
	vector<StmtData> result;
	for (StmtData sd : whileVars) {
		if (sd.getValue() == varName) {
			result.push_back(sd);
		}
	}
	return result;
}

void PatternTable::clearAllEntries() {
	assignRoots.clear();
	whileVars.clear();
	ifVars.clear();
}