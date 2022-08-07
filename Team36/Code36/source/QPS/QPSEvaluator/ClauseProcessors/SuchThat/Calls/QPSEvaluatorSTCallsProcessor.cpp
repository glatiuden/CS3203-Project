#include "QPSEvaluatorSTCallsProcessor.h"

QPSEvaluatorSTCallsProcessor::QPSEvaluatorSTCallsProcessor(QPSNode& suchThatNode) : QPSEvaluatorSuchThatProcessor(suchThatNode) {}

QPSSuchThatResults QPSEvaluatorSTCallsProcessor::evaluateSuchThatClause() {
	const NodeType& leftArgType = suchThatNode.getLeftArgType();
	const NodeType& rightArgType = suchThatNode.getRightArgType();
	const ArgNodeRef& leftArgRef = suchThatNode.getLeftArgRef();
	const ArgNodeRef& rightArgRef = suchThatNode.getRightArgRef();

	if (isSynonymAndSynonym(leftArgType, rightArgType)) {
		return getAllCalls();
	}

	if (isSynonymAndWildcard(leftArgType, rightArgType)) {
		return getAllCalls();
	}

	if (isWildcardAndSynonym(leftArgType, rightArgType)) {
		return getAllCalls();
	}

	if (isSynonymAndIdent(leftArgType, rightArgType)) {
		return getByCallersProcName(rightArgRef);
	}

	if (isIdentAndSynonym(leftArgType, rightArgType)) {
		return getByCalleesProcName(leftArgRef);
	}

	if (isIdentAndWildcard(leftArgType, rightArgType)) {
		return getByCalleesProcName(leftArgRef);
	}

	if (isIdentAndIdent(leftArgType, rightArgType)) {
		setIsGetAll(checkIsTrue(leftArgRef, rightArgRef));
	}

	if (isWildcardAndWildcard(leftArgType, rightArgType)) {
		setIsGetAll(checkIfExists());
	}

	if (!isInvalid && !isGetAll) {
		setIsInvalid(true);
	}

	return EMPTY_RESULT;
}

QPSResults QPSEvaluatorSTCallsProcessor::getAllCalls() {
	return QPSCommunicator::getAllCalls();
}

QPSResults QPSEvaluatorSTCallsProcessor::getByCallersProcName(const string& procName) {
	return QPSCommunicator::getByCallersProcName(procName);
}

QPSResults QPSEvaluatorSTCallsProcessor::getByCalleesProcName(const string& procName) {
	return QPSCommunicator::getByCalleesProcName(procName);
}

bool QPSEvaluatorSTCallsProcessor::checkIsTrue(const string& procName, const string& procName2) {
	return QPSCommunicator::checkCalls(procName, procName2);;
}

bool QPSEvaluatorSTCallsProcessor::checkIfExists() {
	return QPSCommunicator::checkIfCallsExists();
}
