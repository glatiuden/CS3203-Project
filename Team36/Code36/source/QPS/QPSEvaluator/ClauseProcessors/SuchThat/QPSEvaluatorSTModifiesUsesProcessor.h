#pragma once
#include "QPSEvaluatorSuchThatProcessor.h"

using namespace std;

class QPSEvaluatorSTModifiesUsesProcessor : public QPSEvaluatorSuchThatProcessor {
public:
	QPSEvaluatorSTModifiesUsesProcessor(QPSNode& suchThatNode);
	virtual QPSSuchThatResults evaluateSuchThatClause() override;
private:
	virtual QPSSingleResults getByType(const NodeType& nodeType) = 0;
	virtual QPSSingleResults getByStmtNo(const int& stmtNo) = 0;
	virtual QPSSingleResults getByProcName(const string& procName) = 0;
	virtual QPSSingleResults getByTypeAndVarName(const NodeType& nodeType, const string& varName) = 0;
	virtual QPSSingleResults getByStmtNoAndType(const int& stmtNo, const NodeType& type) = 0;
	virtual QPSSingleResults getByProcNameAndType(const string& procName, const NodeType& type) = 0;
	virtual bool checkIsTrue(const int& stmtNo) = 0;
	virtual bool checkIsTrue(const string& procName) = 0;
	virtual bool checkIsTrue(const int& stmtNo, const string& varName) = 0;
	virtual bool checkIsTrue(const NodeType& nodeType, const string& varName) = 0;
	virtual bool checkIsTrue(const string& procName, const string& varName) = 0;
};
