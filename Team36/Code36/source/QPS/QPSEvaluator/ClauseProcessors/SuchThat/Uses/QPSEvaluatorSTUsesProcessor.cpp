#include "QPSEvaluatorSTUsesProcessor.h"

QPSEvaluatorSTUsesProcessor::QPSEvaluatorSTUsesProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTModifiesUsesProcessor(suchThatNode) {}

QPSSingleResults QPSEvaluatorSTUsesProcessor::getByType(const NodeType& nodeType) {
	return QPSCommunicator::getUsesByType(nodeType);
}

QPSSingleResults QPSEvaluatorSTUsesProcessor::getByStmtNo(const int& stmtNo) {
	return QPSCommunicator::getUsesByStmtNo(stmtNo);
}

QPSSingleResults QPSEvaluatorSTUsesProcessor::getByProcName(const string& procName) {
	return QPSCommunicator::getUsesByProcName(procName);
}

QPSSingleResults QPSEvaluatorSTUsesProcessor::getByTypeAndVarName(const NodeType& nodeType, const string& varName) {
	return QPSCommunicator::getUsesByTypeAndVarName(nodeType, varName);
}

QPSSingleResults QPSEvaluatorSTUsesProcessor::getByStmtNoAndType(const int& stmtNo, const NodeType& type) {
	return QPSCommunicator::getUsesByStmtNoAndType(stmtNo, type);
}

QPSSingleResults QPSEvaluatorSTUsesProcessor::getByProcNameAndType(const string& procName, const NodeType& type) {
	return QPSCommunicator::getUsesByProcNameAndType(procName, type);
}

bool QPSEvaluatorSTUsesProcessor::checkIsTrue(const int& stmtNo) {
	return QPSCommunicator::checkUses(stmtNo);
}

bool QPSEvaluatorSTUsesProcessor::checkIsTrue(const string& procName) {
	return QPSCommunicator::checkUses(procName);
}

bool QPSEvaluatorSTUsesProcessor::checkIsTrue(const int& stmtNo, const string& varName) {
	return QPSCommunicator::checkUses(stmtNo, varName);
}

bool QPSEvaluatorSTUsesProcessor::checkIsTrue(const NodeType& nodeType, const string& varName) {
	return QPSCommunicator::checkUses(nodeType, varName);
}

bool QPSEvaluatorSTUsesProcessor::checkIsTrue(const string& procName, const string& varName) {
	return QPSCommunicator::checkUses(procName, varName);
}
