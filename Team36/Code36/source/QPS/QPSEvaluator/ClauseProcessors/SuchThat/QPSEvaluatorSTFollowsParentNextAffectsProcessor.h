#pragma once
#include "QPSEvaluatorSuchThatProcessor.h"

using namespace std;

class QPSEvaluatorSTFollowsParentNextAffectsProcessor : public QPSEvaluatorSuchThatProcessor {
public:
	QPSEvaluatorSTFollowsParentNextAffectsProcessor(QPSNode& clauseNode);
	virtual QPSSuchThatResults evaluateSuchThatClause() override;
private:
	virtual QPSResults getByAfter(const NodeType& afterNodeType) = 0;
	virtual QPSResults getByAfter(const int& stmtNo) = 0;
	virtual QPSResults getByBefore(const NodeType& beforeNodeType) = 0;
	virtual QPSResults getByBefore(const int& stmtNo) = 0;
	virtual QPSResults getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) = 0;
	virtual bool checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) = 0;
	virtual bool checkIfExists() = 0;
};
