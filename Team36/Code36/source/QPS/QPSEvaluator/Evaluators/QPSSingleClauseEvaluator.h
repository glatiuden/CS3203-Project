#pragma once
#include "../../QPSEvaluator.h"

using namespace std;

class QPSSingleClauseEvaluator : public QPSEvaluator {
public:
	QPSSingleClauseEvaluator(QPSTree& qpsTree);
private:
	virtual vector<string> evaluateQuery() override;
	void evaluateSingleSelectionClause(QPSNode&);
	void evaluateSinglePatternClause(QPSNode&, QPSPatternNode&);
	void evaluateSingleSuchThatClause(QPSNode&, QPSNode&);
	void evaluateSingleWithClause(QPSNode&, QPSNode&);
};
