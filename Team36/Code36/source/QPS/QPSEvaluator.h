#pragma once
#include "QPSTree/QPSTree.h"
#include "QPSCommunicator.h"
#include "../CustomException.h"
#include "QPSEvaluator/ClauseProcessors/ClauseProcessors.h"

using namespace std;

class QPSEvaluator {
public:
	QPSEvaluator(QPSTree& qpsTree);
	virtual vector<string> evaluateQuery() = 0;
protected:
	QPSTree& qpsTree;
	vector<string> queryResults{};
	unique_ptr<QPSEvaluatorSuchThatProcessor> getSuchThatClauseProcessor(QPSNode& suchThatNode);
	void retrieveAndDisplayAllResults(const NodeType& nodeType, const NodeType& attrName);
	void retrieveAndDisplayAllResults(const QPSNode& selectionNode);
	vector<string>& returnBooleanOrEmptyResults();
};
