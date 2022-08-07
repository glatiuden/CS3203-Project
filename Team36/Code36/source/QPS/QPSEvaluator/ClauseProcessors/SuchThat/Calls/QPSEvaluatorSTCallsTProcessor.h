#pragma once
#include "QPSEvaluatorSTCallsProcessor.h"

using namespace std;

class QPSEvaluatorSTCallsTProcessor : public QPSEvaluatorSTCallsProcessor {
public:
	QPSEvaluatorSTCallsTProcessor(QPSNode& clauseNode);
private:
	virtual QPSResults getAllCalls() override;
	virtual QPSResults getByCallersProcName(const string& procName) override;
	virtual QPSResults getByCalleesProcName(const string& procName) override;
	virtual bool checkIsTrue(const string& procName, const string& procName2) override;
	virtual bool checkIfExists() override;
};
