#include "QPSEvaluatorSTParentProcessor.h"

QPSEvaluatorSTParentProcessor::QPSEvaluatorSTParentProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTFollowsParentNextAffectsProcessor(suchThatNode) {};

QPSResults QPSEvaluatorSTParentProcessor::getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) {
	return QPSCommunicator::getParent(beforeNodeType, afterNodeType);
}

QPSResults QPSEvaluatorSTParentProcessor::getByBefore(const NodeType& beforeNodeType) {
	return QPSCommunicator::getParentBefore(beforeNodeType);
}

QPSResults QPSEvaluatorSTParentProcessor::getByAfter(const NodeType& afterNodeType) {
	return QPSCommunicator::getParentAfter(afterNodeType);
}

QPSResults QPSEvaluatorSTParentProcessor::getByBefore(const int& stmtNo) {
	return QPSCommunicator::getParentBefore(stmtNo);
}

QPSResults QPSEvaluatorSTParentProcessor::getByAfter(const int& stmtNo) {
	return QPSCommunicator::getParentAfter(stmtNo);
}

bool QPSEvaluatorSTParentProcessor::checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) {
	return QPSCommunicator::checkParent(beforeStmtNo, afterStmtNo);
}

bool QPSEvaluatorSTParentProcessor::checkIfExists() {
	return QPSCommunicator::checkIfParentExists();
}
