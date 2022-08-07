#pragma once
#include "QPSEvaluatorProcessor.h"
#include "../../QPSTree/QPSPatternNode.h"

using namespace std;

class QPSEvaluatorPatternProcessor : public QPSEvaluatorProcessor {
public:
	QPSEvaluatorPatternProcessor(QPSPatternNode& patternNode);
	QPSSingleResults evaluatePatternClause();
private:
	QPSPatternNode& patternNode;
	QPSSingleResults processAssign();
	QPSSingleResults processIf();
	QPSSingleResults processWhile();
};
