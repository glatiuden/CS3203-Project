#pragma once
#include "QPSEvaluatorSTAffectsProcessor.h"

using namespace std;

class QPSEvaluatorSTAffectsTProcessor : public QPSEvaluatorSTAffectsProcessor {
public:
	QPSEvaluatorSTAffectsTProcessor(QPSNode& suchThatNode);
private:
	virtual QPSResults getByAfter(const NodeType& afterNodeType) override;
	virtual QPSResults getByAfter(const int& stmtNo) override;
	virtual QPSResults getByBefore(const NodeType& beforeNodeType) override;
	virtual QPSResults getByBefore(const int& stmtNo) override;
	virtual QPSResults getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) override;
	virtual bool checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) override;
	virtual bool checkIfExists() override;
};
