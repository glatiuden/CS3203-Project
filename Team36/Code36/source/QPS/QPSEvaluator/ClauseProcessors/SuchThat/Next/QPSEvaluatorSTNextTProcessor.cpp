#include "QPSEvaluatorSTNextTProcessor.h"

QPSEvaluatorSTNextTProcessor::QPSEvaluatorSTNextTProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTNextProcessor(suchThatNode) {};

QPSResults QPSEvaluatorSTNextTProcessor::getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) {
	return QPSCommunicator::getNextTrans(beforeNodeType, afterNodeType);
}

QPSResults QPSEvaluatorSTNextTProcessor::getByBefore(const NodeType& beforeNodeType) {
	return QPSCommunicator::getNextBeforeTrans(beforeNodeType);
}

QPSResults QPSEvaluatorSTNextTProcessor::getByAfter(const NodeType& afterNodeType) {
	return QPSCommunicator::getNextAfterTrans(afterNodeType);
}

QPSResults QPSEvaluatorSTNextTProcessor::getByBefore(const int& stmtNo) {
	return QPSCommunicator::getNextBeforeTrans(stmtNo);
}

QPSResults QPSEvaluatorSTNextTProcessor::getByAfter(const int& stmtNo) {
	return QPSCommunicator::getNextAfterTrans(stmtNo);
}

bool QPSEvaluatorSTNextTProcessor::checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) {
	return QPSCommunicator::checkNextTrans(beforeStmtNo, afterStmtNo);
}

bool QPSEvaluatorSTNextTProcessor::checkIfExists() {
	return QPSCommunicator::checkIfNextExists();
}
