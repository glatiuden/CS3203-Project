#include "QPSEvaluatorSelectionProcessor.h"

QPSEvaluatorSelectionProcessor::QPSEvaluatorSelectionProcessor(QPSNode& selectionNode) : selectionNode(selectionNode) {};

vector<string> QPSEvaluatorSelectionProcessor::evaluateSelectionClause() {
	const NodeType& selectionArgType = selectionNode.getLeftArgType();
	const NodeType& selectionAttrName = selectionNode.getLeftAttrName();
	if (selectionArgType == NodeType::BOOLEAN) {
		return { BOOLEAN_TRUE };
	}
	return QPSCommunicator::getAllResultsByType(selectionArgType, selectionAttrName);
}
