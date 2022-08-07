#pragma once
#include <variant>

#include "../QPSEvaluatorProcessor.h"

using namespace std;

typedef variant<QPSResults, QPSSingleResults> QPSSuchThatResults;

class QPSEvaluatorSuchThatProcessor : public QPSEvaluatorProcessor {
public:
	QPSEvaluatorSuchThatProcessor(QPSNode& suchThatNode);
	virtual QPSSuchThatResults evaluateSuchThatClause() = 0;
protected:
	QPSNode& suchThatNode;
};
