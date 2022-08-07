#pragma once
#include "../QPSEvaluatorSuchThatProcessor.h"

using namespace std;

class QPSEvaluatorSTCallsProcessor : public QPSEvaluatorSuchThatProcessor {
public:
	QPSEvaluatorSTCallsProcessor(QPSNode& clauseNode);
	virtual QPSSuchThatResults evaluateSuchThatClause() override;
private:
	virtual QPSResults getAllCalls();
	virtual QPSResults getByCallersProcName(const string& procName);
	virtual QPSResults getByCalleesProcName(const string& procName);
	virtual bool checkIsTrue(const string& procName, const string& procName2);
	virtual bool checkIfExists();
};
