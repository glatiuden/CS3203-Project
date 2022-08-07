#include "QPSEvaluatorSTModifiesProcessor.h"

QPSEvaluatorSTModifiesProcessor::QPSEvaluatorSTModifiesProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTModifiesUsesProcessor(suchThatNode) {}

QPSSingleResults QPSEvaluatorSTModifiesProcessor::getByType(const NodeType& nodeType) {
	return QPSCommunicator::getModifiesByType(nodeType);
}

QPSSingleResults QPSEvaluatorSTModifiesProcessor::getByStmtNo(const int& stmtNo) {
	return QPSCommunicator::getModifiesByStmtNo(stmtNo);
}

QPSSingleResults QPSEvaluatorSTModifiesProcessor::getByProcName(const string& procName) {
	return QPSCommunicator::getModifiesByProcName(procName);
}

QPSSingleResults QPSEvaluatorSTModifiesProcessor::getByTypeAndVarName(const NodeType& nodeType, const string& varName) {
	return QPSCommunicator::getModifiesByTypeAndVarName(nodeType, varName);
}

QPSSingleResults QPSEvaluatorSTModifiesProcessor::getByStmtNoAndType(const int& stmtNo, const NodeType& type) {
	return QPSCommunicator::getModifiesByStmtNoAndType(stmtNo, type);
}

QPSSingleResults QPSEvaluatorSTModifiesProcessor::getByProcNameAndType(const string& procName, const NodeType& type) {
	return QPSCommunicator::getModifiesByProcNameAndType(procName, type);
}

bool QPSEvaluatorSTModifiesProcessor::checkIsTrue(const int& stmtNo) {
	return QPSCommunicator::checkModifies(stmtNo);
}

bool QPSEvaluatorSTModifiesProcessor::checkIsTrue(const string& procName) {
	return QPSCommunicator::checkModifies(procName);
}

bool QPSEvaluatorSTModifiesProcessor::checkIsTrue(const int& stmtNo, const string& varName) {
	return QPSCommunicator::checkModifies(stmtNo, varName);
}

bool QPSEvaluatorSTModifiesProcessor::checkIsTrue(const NodeType& nodeType, const string& varName) {
	return QPSCommunicator::checkModifies(nodeType, varName);
}

bool QPSEvaluatorSTModifiesProcessor::checkIsTrue(const string& procName, const string& varName) {
	return QPSCommunicator::checkModifies(procName, varName);
}
