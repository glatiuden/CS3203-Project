#include "QPSEvaluatorSTNextProcessor.h"

QPSEvaluatorSTNextProcessor::QPSEvaluatorSTNextProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTFollowsParentNextAffectsProcessor(suchThatNode) {};

QPSResults QPSEvaluatorSTNextProcessor::getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) {
	return QPSCommunicator::getNext(beforeNodeType, afterNodeType);
}

QPSResults QPSEvaluatorSTNextProcessor::getByBefore(const NodeType& beforeNodeType) {
	return QPSCommunicator::getNextBefore(beforeNodeType);
}

QPSResults QPSEvaluatorSTNextProcessor::getByAfter(const NodeType& afterNodeType) {
	return QPSCommunicator::getNextAfter(afterNodeType);
}

QPSResults QPSEvaluatorSTNextProcessor::getByBefore(const int& stmtNo) {
	return QPSCommunicator::getNextBefore(stmtNo);
}

QPSResults QPSEvaluatorSTNextProcessor::getByAfter(const int& stmtNo) {
	return QPSCommunicator::getNextAfter(stmtNo);
}

bool QPSEvaluatorSTNextProcessor::checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) {
	return QPSCommunicator::checkNext(beforeStmtNo, afterStmtNo);
}

bool QPSEvaluatorSTNextProcessor::checkIfExists() {
	return QPSCommunicator::checkIfNextExists();
}
