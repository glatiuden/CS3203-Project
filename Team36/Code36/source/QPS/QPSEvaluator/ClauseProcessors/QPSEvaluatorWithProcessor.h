#pragma once
#include "QPSEvaluatorProcessor.h"

using namespace std;

class QPSEvaluatorWithProcessor : public QPSEvaluatorProcessor {
public:
	QPSEvaluatorWithProcessor(QPSNode& withNode);
	QPSResults evaluateWithClause();
	void setIsResultTrue(bool);
	bool getIsResultTrue();
private:
	QPSNode& withNode;
	bool isResultTrue = false;
};
