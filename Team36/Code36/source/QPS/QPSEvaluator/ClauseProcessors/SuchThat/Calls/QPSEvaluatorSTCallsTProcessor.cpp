#include "QPSEvaluatorSTCallsTProcessor.h"

QPSEvaluatorSTCallsTProcessor::QPSEvaluatorSTCallsTProcessor(QPSNode& suchThatNode) : QPSEvaluatorSTCallsProcessor(suchThatNode) {}

QPSResults QPSEvaluatorSTCallsTProcessor::getAllCalls() {
	return QPSCommunicator::getAllCallsTrans();
}

QPSResults QPSEvaluatorSTCallsTProcessor::getByCallersProcName(const string& procName) {
	return QPSCommunicator::getByCallersProcNameTrans(procName);
}

QPSResults QPSEvaluatorSTCallsTProcessor::getByCalleesProcName(const string& procName) {
	return QPSCommunicator::getByCalleesProcNameTrans(procName);
}

bool QPSEvaluatorSTCallsTProcessor::checkIsTrue(const string& procName, const string& procName2) {
	return QPSCommunicator::checkCallsTrans(procName, procName2);;
}

bool QPSEvaluatorSTCallsTProcessor::checkIfExists() {
	return QPSCommunicator::checkIfCallsExists();
}
