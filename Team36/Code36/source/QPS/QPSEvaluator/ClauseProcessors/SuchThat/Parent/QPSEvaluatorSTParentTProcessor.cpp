#include "QPSEvaluatorSTParentTProcessor.h"

QPSEvaluatorSTParentTProcessor::QPSEvaluatorSTParentTProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTParentProcessor(suchThatNode) {};

QPSResults QPSEvaluatorSTParentTProcessor::getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) {
	return QPSCommunicator::getParentTrans(beforeNodeType, afterNodeType);
}

QPSResults QPSEvaluatorSTParentTProcessor::getByBefore(const NodeType& beforeNodeType) {
	return QPSCommunicator::getParentBeforeTrans(beforeNodeType);
}

QPSResults QPSEvaluatorSTParentTProcessor::getByAfter(const NodeType& afterNodeType) {
	return QPSCommunicator::getParentAfterTrans(afterNodeType);
}

QPSResults QPSEvaluatorSTParentTProcessor::getByBefore(const int& stmtNo) {
	return QPSCommunicator::getParentBeforeTrans(stmtNo);
}

QPSResults QPSEvaluatorSTParentTProcessor::getByAfter(const int& stmtNo) {
	return QPSCommunicator::getParentAfterTrans(stmtNo);
}

bool QPSEvaluatorSTParentTProcessor::checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) {
	return QPSCommunicator::checkParentTrans(beforeStmtNo, afterStmtNo);
}

bool QPSEvaluatorSTParentTProcessor::checkIfExists() {
	return QPSCommunicator::checkIfParentExists();
}