#include "QPSSingleClauseEvaluator.h"

QPSSingleClauseEvaluator::QPSSingleClauseEvaluator(QPSTree& qpsTree) : QPSEvaluator(qpsTree) {};

vector<string> QPSSingleClauseEvaluator::evaluateQuery() {
	try {
		QPSNode& resultRootNode = qpsTree.getResultNode();
		QPSNode& clauseRootNode = qpsTree.getClauseNode();
		QPSPatternNode& patternRootNode = qpsTree.getPatternNode();
		QPSNode& withRootNode = qpsTree.getWithNode();

		bool isSelectionOnly = resultRootNode.getSize() == 1 && clauseRootNode.getSize() == 0 && patternRootNode.getSize() == 0 && withRootNode.getSize() == 0;
		bool isSuchThatOnly = clauseRootNode.getSize() == 1 && patternRootNode.getSize() == 0 && withRootNode.getSize() == 0;
		bool isPatternOnly = clauseRootNode.getSize() == 0 && patternRootNode.getSize() == 1 && withRootNode.getSize() == 0;
		bool isWithOnly = clauseRootNode.getSize() == 0 && patternRootNode.getSize() == 0 && withRootNode.getSize() == 1;

		QPSNode& selectionNode = qpsTree.getResultNode().getChild();
		if (isSelectionOnly) {
			evaluateSingleSelectionClause(selectionNode);
		} else if (isSuchThatOnly) {
			QPSNode& suchThatNode = clauseRootNode.getChild();
			evaluateSingleSuchThatClause(selectionNode, suchThatNode);
		} else if (isPatternOnly) {
			QPSPatternNode& patternNode = patternRootNode.getChild();
			evaluateSinglePatternClause(selectionNode, patternNode);
		} else if (isWithOnly) {
			QPSNode& withNode = withRootNode.getChild();
			evaluateSingleWithClause(selectionNode, withNode);
		}
		return queryResults;
	} catch (QPSEvaluatorException&) {
		return returnBooleanOrEmptyResults();
	}
}

void QPSSingleClauseEvaluator::evaluateSingleSelectionClause(QPSNode& selectionNode) {
	vector<string> selectionClauseResults = QPSEvaluatorSelectionProcessor(selectionNode).evaluateSelectionClause();
	unordered_set<string> setResults{};
	setResults.reserve(selectionClauseResults.size());
	for (const string& str : selectionClauseResults) {
		setResults.insert(str);
	}
	QPSFilter::setToVector(queryResults, setResults);
}

void QPSSingleClauseEvaluator::evaluateSingleSuchThatClause(QPSNode& selectionNode, QPSNode& suchThatNode) {
	const ArgNodeType& selectionArgType = selectionNode.getLeftArgType();
	const ArgAttrName& selectionAttrName = selectionNode.getLeftAttrName();

	unique_ptr<QPSEvaluatorSuchThatProcessor> processor = getSuchThatClauseProcessor(suchThatNode);
	QPSSuchThatResults resultsFromQPS = processor->evaluateSuchThatClause();
	auto qpsSingleResults = get_if<QPSSingleResults>(&resultsFromQPS);
	auto qpsPairResults = get_if<QPSResults>(&resultsFromQPS);
	bool isGetAll = processor->getIsGetAll();
	bool isInvalid = processor->getIsInvalid();
	bool isEmpty = qpsSingleResults != nullptr && qpsSingleResults->empty() || qpsPairResults != nullptr && qpsPairResults->empty();
	// If no results or is invalid, stop processing and return.
	if (!isGetAll && (isInvalid || isEmpty)) {
		throw QPSEvaluatorException("Empty Clause Results");
	}

	const bool& isNonTargetSelection = qpsTree.getIsNonTargetSelection();
	if (isGetAll || isNonTargetSelection) {
		retrieveAndDisplayAllResults(selectionArgType, selectionAttrName);
		return;
	}

	vector<StmtData> suchThatClauseResults{};
	if (suchThatNode.getType() == QPSNodeType::MODIFIES || suchThatNode.getType() == QPSNodeType::USES) {
		suchThatClauseResults = get<QPSSingleResults>(resultsFromQPS);
	} else {
		QPSResults& pairResults = get<QPSResults>(resultsFromQPS);
		const ArgNode& rightArg = suchThatNode.getRightArg();
		const ArgNode& selectionArg = selectionNode.getLeftArg();
		int pairNo = FIRST;
		bool isSelectionRightArg = selectionArg == rightArg;
		if (isSelectionRightArg) {
			pairNo = SECOND;
		}
		suchThatClauseResults = QPSFilter::getVectorFromPairVector(pairResults, pairNo);
	}

	NodeType projectionNodeType = QPSFilter::getProjectionNodeType(selectionArgType, selectionAttrName);
	QPSFilter::getDisplayValues(queryResults, suchThatClauseResults, projectionNodeType);
}

void QPSSingleClauseEvaluator::evaluateSinglePatternClause(QPSNode& selectionNode, QPSPatternNode& patternNode) {
	QPSSingleResults patternClauseResults = QPSEvaluatorPatternProcessor(patternNode).evaluatePatternClause();
	if (patternClauseResults.empty()) {
		throw QPSEvaluatorException("Empty Clause Results");
	}

	const NodeType& selectionArgType = selectionNode.getLeftArgType();
	const NodeType& selectionAttrName = selectionNode.getLeftAttrName();
	bool isNonTargetSelection = qpsTree.getIsNonTargetSelection();
	if (isNonTargetSelection) {
		retrieveAndDisplayAllResults(selectionArgType, selectionAttrName);
		return;
	}

	NodeType projectionNodeType = QPSFilter::getProjectionNodeType(selectionArgType, selectionAttrName);
	QPSFilter::getDisplayValues(queryResults, patternClauseResults, projectionNodeType);
}

void QPSSingleClauseEvaluator::evaluateSingleWithClause(QPSNode& selectionNode, QPSNode& withNode) {
	QPSEvaluatorWithProcessor processor(withNode);
	QPSResults resultsFromQPS = processor.evaluateWithClause();
	const bool& isResultTrue = processor.getIsResultTrue();
	if (!isResultTrue && resultsFromQPS.empty()) {
		throw QPSEvaluatorException("Empty Clause Results");
	}

	const NodeType& selectionArgType = selectionNode.getLeftArgType();
	const NodeType& selectionAttrName = selectionNode.getLeftAttrName();

	const bool& isNonTargetSelection = qpsTree.getIsNonTargetSelection();
	if (isNonTargetSelection || isResultTrue) {
		retrieveAndDisplayAllResults(selectionArgType, selectionAttrName);
		return;
	}

	int pairNo = FIRST;
	if (selectionNode.getLeftArgRef() == withNode.getRightArgRef()) {
		pairNo = SECOND;
	}

	vector<StmtData> withClauseResults = QPSFilter::getVectorFromPairVector(resultsFromQPS, pairNo);
	NodeType projectionNodeType = QPSFilter::getProjectionNodeType(selectionArgType, selectionAttrName);
	QPSFilter::getDisplayValues(queryResults, withClauseResults, projectionNodeType);
}
