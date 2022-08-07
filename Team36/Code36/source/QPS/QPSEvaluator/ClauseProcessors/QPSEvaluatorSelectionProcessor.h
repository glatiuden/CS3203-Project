#pragma once
#include "QPSEvaluatorProcessor.h"

using namespace std;

class QPSEvaluatorSelectionProcessor : public QPSEvaluatorProcessor {
public:
	QPSEvaluatorSelectionProcessor(QPSNode& selectionNode);
	vector<string> evaluateSelectionClause();
private:
	QPSNode& selectionNode;
};
