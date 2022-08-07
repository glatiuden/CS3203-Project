#include "QPSEvaluatorSTFollowsTProcessor.h"

QPSEvaluatorSTFollowsTProcessor::QPSEvaluatorSTFollowsTProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTFollowsProcessor(suchThatNode) {};

QPSResults QPSEvaluatorSTFollowsTProcessor::getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) {
	return QPSCommunicator::getFollowsTrans(beforeNodeType, afterNodeType);
}

QPSResults QPSEvaluatorSTFollowsTProcessor::getByBefore(const NodeType& beforeNodeType) {
	return QPSCommunicator::getFollowsBeforeTrans(beforeNodeType);
}

QPSResults QPSEvaluatorSTFollowsTProcessor::getByAfter(const NodeType& afterNodeType) {
	return QPSCommunicator::getFollowsAfterTrans(afterNodeType);
}

QPSResults QPSEvaluatorSTFollowsTProcessor::getByBefore(const int& stmtNo) {
	return QPSCommunicator::getFollowsBeforeTrans(stmtNo);
}

QPSResults QPSEvaluatorSTFollowsTProcessor::getByAfter(const int& stmtNo) {
	return QPSCommunicator::getFollowsAfterTrans(stmtNo);
}

bool QPSEvaluatorSTFollowsTProcessor::checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) {
	return QPSCommunicator::checkFollowsTrans(beforeStmtNo, afterStmtNo);
}

bool QPSEvaluatorSTFollowsTProcessor::checkIfExists() {
	return QPSCommunicator::checkIfFollowsExists();
}
