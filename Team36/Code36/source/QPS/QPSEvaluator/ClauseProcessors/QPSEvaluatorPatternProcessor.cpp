#include "../../QPSFilter.h"
#include "QPSEvaluatorPatternProcessor.h"

QPSEvaluatorPatternProcessor::QPSEvaluatorPatternProcessor(QPSPatternNode& patternNode) : patternNode(patternNode) {};

QPSSingleResults QPSEvaluatorPatternProcessor::evaluatePatternClause() {
	const NodeType& patternSynArgType = patternNode.getPatternSynArgNodeType();
	switch (patternSynArgType) {
		case NodeType::ASSIGN:
			return processAssign();
		case NodeType::IF:
			return processIf();
		case NodeType::WHILE:
			return processWhile();
		default:
			return EMPTY_RESULT;
	}
}

QPSSingleResults QPSEvaluatorPatternProcessor::processAssign() {
	TNode* exprAST = patternNode.getExpression();
	ArgNodeRef& leftPatternRef = patternNode.getLeftArgRef();
	if (!isIdent(patternNode.getLeftArgType())) {
		leftPatternRef = WILDCARD_STR;
	}

	if (patternNode.getIsPartialMatching()) {
		return QPSCommunicator::getAssignmentPatternPartialMatch(leftPatternRef, exprAST);
	}

	return QPSCommunicator::getAssignmentPatternExactMatch(leftPatternRef, exprAST);
}

QPSSingleResults QPSEvaluatorPatternProcessor::processIf() {
	const NodeType& leftArgType = patternNode.getLeftArgType();
	if (isSynonym(leftArgType) || isWildcard(leftArgType)) {
		return QPSCommunicator::getAllIfPattern();
	}
	return QPSCommunicator::getIfPatternMatch(patternNode.getLeftArgRef());
}

QPSSingleResults QPSEvaluatorPatternProcessor::processWhile() {
	const NodeType& leftArgType = patternNode.getLeftArgType();
	if (isSynonym(leftArgType) || isWildcard(leftArgType)) {
		return QPSCommunicator::getAllWhilePattern();
	}

	return QPSCommunicator::getWhilePatternMatch(patternNode.getLeftArgRef());
}
