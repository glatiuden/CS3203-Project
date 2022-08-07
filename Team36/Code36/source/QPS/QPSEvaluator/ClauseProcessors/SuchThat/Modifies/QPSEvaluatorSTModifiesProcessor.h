#pragma once
#include "../QPSEvaluatorSTModifiesUsesProcessor.h"

using namespace std;

class QPSEvaluatorSTModifiesProcessor : public QPSEvaluatorSTModifiesUsesProcessor {
public:
	QPSEvaluatorSTModifiesProcessor(QPSNode& suchThatNode);
private:
	virtual QPSSingleResults getByType(const NodeType& nodeType) override;
	virtual QPSSingleResults getByStmtNo(const int& stmtNo) override;
	virtual QPSSingleResults getByProcName(const string& procName) override;
	virtual QPSSingleResults getByTypeAndVarName(const NodeType& nodeType, const string& varName) override;
	virtual QPSSingleResults getByStmtNoAndType(const int& stmtNo, const NodeType& type) override;
	virtual QPSSingleResults getByProcNameAndType(const string& procName, const NodeType& type) override;
	virtual bool checkIsTrue(const int& stmtNo) override;
	virtual bool checkIsTrue(const string& procName) override;
	virtual bool checkIsTrue(const int& stmtNo, const string& varName) override;
	virtual bool checkIsTrue(const NodeType& nodeType, const string& varName) override;
	virtual bool checkIsTrue(const string& procName, const string& varName) override;
};
