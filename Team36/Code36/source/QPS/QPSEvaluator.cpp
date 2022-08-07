#include "QPSEvaluator.h"

using namespace std;

QPSEvaluator::QPSEvaluator(QPSTree& qpsTree) : qpsTree(qpsTree) {}

unique_ptr<QPSEvaluatorSuchThatProcessor> QPSEvaluator::getSuchThatClauseProcessor(QPSNode& suchThatNode) {
	const QPSNodeType& nodeType = suchThatNode.getType();
	unique_ptr<QPSEvaluatorSuchThatProcessor> processor{};
	switch (nodeType) {
		case QPSNodeType::MODIFIES:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTModifiesProcessor(suchThatNode));
			break;
		case QPSNodeType::USES:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTUsesProcessor(suchThatNode));
			break;
		case QPSNodeType::FOLLOWS:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTFollowsProcessor(suchThatNode));
			break;
		case QPSNodeType::FOLLOWS_T:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTFollowsTProcessor(suchThatNode));
			break;
		case QPSNodeType::PARENT:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTParentProcessor(suchThatNode));
			break;
		case QPSNodeType::PARENT_T:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTParentTProcessor(suchThatNode));
			break;
		case QPSNodeType::NEXT:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTNextProcessor(suchThatNode));
			break;
		case QPSNodeType::NEXT_T:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTNextTProcessor(suchThatNode));
			break;
		case QPSNodeType::CALLS:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTCallsProcessor(suchThatNode));
			break;
		case QPSNodeType::CALLS_T:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTCallsTProcessor(suchThatNode));
			break;
		case QPSNodeType::AFFECTS:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTAffectsProcessor(suchThatNode));
			break;
		case QPSNodeType::AFFECTS_T:
			processor = unique_ptr<QPSEvaluatorSuchThatProcessor>(new QPSEvaluatorSTAffectsTProcessor(suchThatNode));
			break;
		default:
			break;
	}
	return processor;
}

void QPSEvaluator::retrieveAndDisplayAllResults(const NodeType& nodeType, const NodeType& attrName) {
	if (qpsTree.getIsBooleanSelection()) {
		queryResults.push_back(BOOLEAN_TRUE);
		return;
	}
	vector<string> displayResults = QPSCommunicator::getAllResultsByType(nodeType, attrName);
	queryResults.assign(displayResults.begin(), displayResults.end());
}

void QPSEvaluator::retrieveAndDisplayAllResults(const QPSNode& selectionNode) {
	const LeftArg& selectionArg = selectionNode.getLeftArg();
	const NodeType& selectionNodeType = selectionArg.getType();
	const NodeType& selectionAttrName = selectionArg.getAttrName();
	retrieveAndDisplayAllResults(selectionNodeType, selectionAttrName);
}

vector<string>& QPSEvaluator::returnBooleanOrEmptyResults() {
	if (qpsTree.getIsBooleanSelection()) {
		queryResults.push_back(BOOLEAN_FALSE);
	}
	return queryResults;
}
