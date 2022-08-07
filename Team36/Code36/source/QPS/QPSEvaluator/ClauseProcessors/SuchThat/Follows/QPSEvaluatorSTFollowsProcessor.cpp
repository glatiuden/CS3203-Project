#include "QPSEvaluatorSTFollowsProcessor.h"

QPSEvaluatorSTFollowsProcessor::QPSEvaluatorSTFollowsProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTFollowsParentNextAffectsProcessor(suchThatNode) { };

QPSResults QPSEvaluatorSTFollowsProcessor::getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) {
	return QPSCommunicator::getFollows(beforeNodeType, afterNodeType);
}

QPSResults QPSEvaluatorSTFollowsProcessor::getByBefore(const NodeType& beforeNodeType) {
	return QPSCommunicator::getFollowsBefore(beforeNodeType);
}

QPSResults QPSEvaluatorSTFollowsProcessor::getByAfter(const NodeType& afterNodeType) {
	return QPSCommunicator::getFollowsAfter(afterNodeType);
}

QPSResults QPSEvaluatorSTFollowsProcessor::getByBefore(const int& stmtNo) {
	return QPSCommunicator::getFollowsBefore(stmtNo);
}

QPSResults QPSEvaluatorSTFollowsProcessor::getByAfter(const int& stmtNo) {
	return QPSCommunicator::getFollowsAfter(stmtNo);
}

bool QPSEvaluatorSTFollowsProcessor::checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) {
	return QPSCommunicator::checkFollows(beforeStmtNo, afterStmtNo);
}

bool QPSEvaluatorSTFollowsProcessor::checkIfExists() {
	return QPSCommunicator::checkIfFollowsExists();
}
