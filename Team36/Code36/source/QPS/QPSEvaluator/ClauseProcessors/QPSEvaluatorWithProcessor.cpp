#include "QPSEvaluatorWithProcessor.h"

QPSEvaluatorWithProcessor::QPSEvaluatorWithProcessor(QPSNode& withNode) : withNode(withNode) {};

QPSResults QPSEvaluatorWithProcessor::evaluateWithClause() {
	const NodeType& leftArgType = withNode.getLeftArgType();
	const NodeType& rightArgType = withNode.getRightArgType();

	if (isIntegerAndInteger(leftArgType, rightArgType) || isIdentAndIdent(leftArgType, rightArgType)) {
		const ArgNodeRef& leftArgRef = withNode.getLeftArgRef();
		const ArgNodeRef& rightArgRef = withNode.getRightArgRef();
		setIsResultTrue(leftArgRef == rightArgRef);
		return EMPTY_RESULT;
	}

	return QPSCommunicator::getWith(withNode.getLeftArg(), withNode.getRightArg());
}

void QPSEvaluatorWithProcessor::setIsResultTrue(bool _isResultTrue) {
	isResultTrue = _isResultTrue;
}

bool QPSEvaluatorWithProcessor::getIsResultTrue() {
	return isResultTrue;
}
