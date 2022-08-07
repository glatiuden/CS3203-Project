#include "QPSEvaluatorSTFollowsParentNextAffectsProcessor.h"

QPSEvaluatorSTFollowsParentNextAffectsProcessor::QPSEvaluatorSTFollowsParentNextAffectsProcessor(QPSNode& suchThatNode) : QPSEvaluatorSuchThatProcessor(suchThatNode) {};

QPSSuchThatResults QPSEvaluatorSTFollowsParentNextAffectsProcessor::evaluateSuchThatClause() {
	const NodeType& leftArgType = suchThatNode.getLeftArgType();
	const NodeType& rightArgType = suchThatNode.getRightArgType();
	const ArgNodeRef& leftArgRef = suchThatNode.getLeftArgRef();
	const ArgNodeRef& rightArgRef = suchThatNode.getRightArgRef();

	if (isSynonymAndSynonym(leftArgType, rightArgType)) {
		if (leftArgRef == rightArgRef) {
			return QPSFilter::getSameValuePairs(getByTypeAndType(leftArgType, rightArgType));
		} else {
			return getByTypeAndType(leftArgType, rightArgType);
		}
	}

	if (isSynonymAndWildcard(leftArgType, rightArgType)) {
		return getByAfter(leftArgType);
	}

	if (isSynonymAndInteger(leftArgType, rightArgType)) {
		return QPSFilter::filterRawResults(getByBefore(stoi(rightArgRef)), FIRST, leftArgType);
	}

	if (isWildcardAndSynonym(leftArgType, rightArgType)) {
		return getByBefore(rightArgType);
	}

	if (isWildcardAndInteger(leftArgType, rightArgType)) {
		setIsGetAll(!getByBefore(stoi(rightArgRef)).empty());
	}

	if (isIntegerAndSynonym(leftArgType, rightArgType)) {
		return QPSFilter::filterRawResults(getByAfter(stoi(leftArgRef)), SECOND, rightArgType);
	}

	if (isIntegerAndWildcard(leftArgType, rightArgType)) {
		setIsGetAll(!getByAfter(stoi(leftArgRef)).empty());
	}

	if (isIntegerAndInteger(leftArgType, rightArgType)) {
		setIsGetAll(checkIsTrue(stoi(leftArgRef), stoi(rightArgRef)));
	}

	if (isWildcardAndWildcard(leftArgType, rightArgType)) {
		setIsGetAll(checkIfExists());
	}

	if (!isInvalid && !isGetAll) {
		setIsInvalid(true);
	}

	return EMPTY_RESULT;
}
