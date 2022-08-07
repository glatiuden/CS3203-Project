#include "QPSEvaluatorSTModifiesUsesProcessor.h"

QPSEvaluatorSTModifiesUsesProcessor::QPSEvaluatorSTModifiesUsesProcessor(QPSNode& suchThatNode) : QPSEvaluatorSuchThatProcessor(suchThatNode) {}

QPSSuchThatResults QPSEvaluatorSTModifiesUsesProcessor::evaluateSuchThatClause() {
	const NodeType& leftArgType = suchThatNode.getLeftArgType();
	const NodeType& rightArgType = suchThatNode.getRightArgType();
	const ArgNodeRef& leftArgRef = suchThatNode.getLeftArgRef();
	const ArgNodeRef& rightArgRef = suchThatNode.getRightArgRef();

	// ModifiesS and UsesS
	if (isSynonymAndSynonym(leftArgType, rightArgType)) {
		return getByType(leftArgType);
	}

	if (isIntegerAndSynonym(leftArgType, rightArgType)) {
		return getByStmtNoAndType(stoi(leftArgRef), rightArgType);
	}

	if (isIntegerAndWildcard(leftArgType, rightArgType)) {
		setIsGetAll(checkIsTrue(stoi(leftArgRef)));
	}

	if (isIntegerAndIdent(leftArgType, rightArgType)) {
		setIsGetAll(checkIsTrue(stoi(leftArgRef), rightArgRef));
	}

	if (isSynonymAndWildcard(leftArgType, rightArgType)) {
		return getByType(leftArgType);
	}

	if (isSynonymAndIdent(leftArgType, rightArgType)) {
		return getByTypeAndVarName(leftArgType, rightArgRef);
	}

	// ModifiesP and UsesP
	if (isIdentAndIdent(leftArgType, rightArgType)) {
		setIsGetAll(checkIsTrue(leftArgRef, rightArgRef));
	}

	if (isIdentAndWildcard(leftArgType, rightArgType)) {
		setIsGetAll(checkIsTrue(leftArgRef));
	}

	if (isIdentAndSynonym(leftArgType, rightArgType)) {
		return getByProcNameAndType(leftArgRef, rightArgType);
	}

	if (!isInvalid && !isGetAll) {
		setIsInvalid(true);
	}

	return EMPTY_RESULT;
};
