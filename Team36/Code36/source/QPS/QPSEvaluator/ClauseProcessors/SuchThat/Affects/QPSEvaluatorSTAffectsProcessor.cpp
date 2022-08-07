#include "QPSEvaluatorSTAffectsProcessor.h"

QPSEvaluatorSTAffectsProcessor::QPSEvaluatorSTAffectsProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTFollowsParentNextAffectsProcessor(suchThatNode) {};

QPSResults QPSEvaluatorSTAffectsProcessor::getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) {
	return QPSCommunicator::getAllAffects();
}

QPSResults QPSEvaluatorSTAffectsProcessor::getByBefore(const NodeType& beforeNodeType) {
	// MultiClause will later on select the left column.
	return QPSCommunicator::getAllAffects();
}

QPSResults QPSEvaluatorSTAffectsProcessor::getByAfter(const NodeType& afterNodeType) {
	// MultiClause will later on select the right column.
	return QPSCommunicator::getAllAffects();
}

QPSResults QPSEvaluatorSTAffectsProcessor::getByBefore(const int& stmtNo) {
	return QPSCommunicator::getAffectsBefore(stmtNo);
}

QPSResults QPSEvaluatorSTAffectsProcessor::getByAfter(const int& stmtNo) {
	return QPSCommunicator::getAffectsAfter(stmtNo);
}

bool QPSEvaluatorSTAffectsProcessor::checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) {
	return QPSCommunicator::checkAffects(beforeStmtNo, afterStmtNo);
}

bool QPSEvaluatorSTAffectsProcessor::checkIfExists() {
	return QPSCommunicator::checkIfAffectsExists();
}
