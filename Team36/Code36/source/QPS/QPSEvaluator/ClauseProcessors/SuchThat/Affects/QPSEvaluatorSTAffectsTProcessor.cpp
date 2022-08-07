#include "QPSEvaluatorSTAffectsTProcessor.h"

QPSEvaluatorSTAffectsTProcessor::QPSEvaluatorSTAffectsTProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTAffectsProcessor(suchThatNode) {};

QPSResults QPSEvaluatorSTAffectsTProcessor::getByTypeAndType(const NodeType& beforeNodeType, const NodeType& afterNodeType) {
	return QPSCommunicator::getAllAffectsTrans();
}

QPSResults QPSEvaluatorSTAffectsTProcessor::getByBefore(const NodeType& beforeNodeType) {
	// MultiClause will later on select the left column.
	return QPSCommunicator::getAllAffectsTrans();
}

QPSResults QPSEvaluatorSTAffectsTProcessor::getByAfter(const NodeType& afterNodeType) {
	// MultiClause will later on select the right column.
	return QPSCommunicator::getAllAffectsTrans();
}

QPSResults QPSEvaluatorSTAffectsTProcessor::getByBefore(const int& stmtNo) {
	return QPSCommunicator::getAffectsBeforeTrans(stmtNo);
}

QPSResults QPSEvaluatorSTAffectsTProcessor::getByAfter(const int& stmtNo) {
	return QPSCommunicator::getAffectsAfterTrans(stmtNo);
}

bool QPSEvaluatorSTAffectsTProcessor::checkIsTrue(const int& beforeStmtNo, const int& afterStmtNo) {
	return QPSCommunicator::checkAffectsTrans(beforeStmtNo, afterStmtNo);
}

bool QPSEvaluatorSTAffectsTProcessor::checkIfExists() {
	return QPSCommunicator::checkIfAffectsExists();
}
