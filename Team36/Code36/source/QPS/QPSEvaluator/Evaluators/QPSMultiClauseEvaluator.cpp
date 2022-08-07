#include <algorithm>
#include <functional>
#include <execution>

#include "../../../CustomException.h"
#include "QPSMultiClauseEvaluator.h"

QPSMultiClauseEvaluator::QPSMultiClauseEvaluator(QPSTree& qpsTree) : QPSEvaluator(qpsTree) {};

vector<string> QPSMultiClauseEvaluator::evaluateQuery() {
	const QPSNode& resultRootNode = qpsTree.getResultNode();
	const QPSNode& clauseRootNode = qpsTree.getClauseNode();
	const QPSPatternNode& patternRootNode = qpsTree.getPatternNode();
	const QPSNode& withRootNode = qpsTree.getWithNode();
	try {
		bool isTupleClause = resultRootNode.getSize() > 1 && clauseRootNode.getSize() == 0 && patternRootNode.getSize() == 0 && withRootNode.getSize() == 0;
		if (isTupleClause) {
			evaluateTupleClause(resultRootNode);
		} else {
			optimizer = QPSOptimizer();
			evaluateMultiClause(optimizer.getOptimizedGroups(qpsTree));
		}
		return queryResults;
	} catch (QPSEvaluatorNonTargetSelectionException&) {
		return evaluateSelectionClause();
	} catch (QPSEvaluatorException&) {
		return returnBooleanOrEmptyResults();
	}
}

void QPSMultiClauseEvaluator::evaluateTupleClause(const QPSNode& selectionRootNode) {
	const shared_ptr<QPSResultTable>& finalResultTable = addTuplesColumns(shared_ptr<QPSResultTable>(new QPSResultTable()));
	finalResultTable->getResultsByColumns(selectionRootNode, queryResults);
}

void QPSMultiClauseEvaluator::evaluateSuchThatClause(QPSNode& qpsNode, vector<shared_ptr<QPSResultTable>>& resultTables) {
	unique_ptr<QPSEvaluatorSuchThatProcessor> processor = getSuchThatClauseProcessor(qpsNode);
	QPSSuchThatResults resultsFromQPS = processor->evaluateSuchThatClause();
	const bool& isGetAll = processor->getIsGetAll();
	if (!isGetAll) {
		const bool& isInvalid = processor->getIsInvalid();
		auto qpsSingleResults = get_if<QPSSingleResults>(&resultsFromQPS);
		auto qpsPairResults = get_if<QPSResults>(&resultsFromQPS);
		const bool isEmpty = qpsSingleResults != nullptr && qpsSingleResults->empty() || qpsPairResults != nullptr && qpsPairResults->empty();
		if (isInvalid || isEmpty) {
			throw QPSEvaluatorException("[QPSMultiClauseEvaluator] Empty Such That Clause Results");
		}

		ArgNode suchThatTableColumns[2] = { qpsNode.getLeftArgReference(), qpsNode.getRightArgReference() };
		shared_ptr<QPSResultTable> suchThatTable{};
		if (qpsNode.getType() == QPSNodeType::MODIFIES || qpsNode.getType() == QPSNodeType::USES) {
			suchThatTable = make_shared<QPSResultTable>(QPSResultTable(suchThatTableColumns, get<QPSSingleResults>(resultsFromQPS)));
		} else {
			suchThatTable = make_shared<QPSResultTable>(QPSResultTable(suchThatTableColumns, get<QPSResults>(resultsFromQPS)));
		}

		if (!suchThatTable->isEmpty()) {
			resultTables.push_back(suchThatTable);
		}
	}
}

void QPSMultiClauseEvaluator::evaluatePatternClause(QPSPatternNode& qpsNode, vector<shared_ptr<QPSResultTable>>& resultTables) {
	QPSSingleResults resultsFromQPS = QPSEvaluatorPatternProcessor(qpsNode).evaluatePatternClause();
	if (resultsFromQPS.empty()) {
		throw QPSEvaluatorException("[QPSMultiClauseEvaluator] Empty Pattern Clause Results");
	}

	ArgNode patternTableColumns[2] = { qpsNode.getLeftArgReference(), qpsNode.getRightArgReference() };
	const shared_ptr<QPSResultTable>& patternTable = make_shared<QPSResultTable>(QPSResultTable(patternTableColumns, resultsFromQPS));
	if (!patternTable->isEmpty()) {
		resultTables.push_back(patternTable);
	}
}

void QPSMultiClauseEvaluator::evaluateWithClause(QPSNode& qpsNode, vector<shared_ptr<QPSResultTable>>& resultTables) {
	QPSEvaluatorWithProcessor processor(qpsNode);
	QPSResults resultsFromQPS = processor.evaluateWithClause();
	const bool& isResultTrue = processor.getIsResultTrue();
	if (!isResultTrue) {
		if (resultsFromQPS.empty()) {
			throw QPSEvaluatorException("Empty Clause Results");
		}

		ArgNode withTableColumns[2] = { qpsNode.getLeftArgReference(), qpsNode.getRightArgReference() };
		const shared_ptr<QPSResultTable>& withTable = make_shared<QPSResultTable>(QPSResultTable(withTableColumns, resultsFromQPS));
		if (!withTable->isEmpty()) {
			resultTables.push_back(withTable);
		}
	}
}

shared_ptr<QPSResultTable> QPSMultiClauseEvaluator::processResultTables(const vector<shared_ptr<QPSResultTable>>& resultTables) {
	shared_ptr<QPSResultTable> finalResultTable = resultTables.front(); // Set the first table as initial table
	for (size_t i = 1; i < resultTables.size(); i++) {
		finalResultTable = finalResultTable->processIntermediateTable(resultTables.at(i));
		if (finalResultTable->isEmpty()) {
			throw QPSEvaluatorException("Empty ResultTable");
		}
	}
	return finalResultTable;
}

void QPSMultiClauseEvaluator::processNode(const shared_ptr<QPSNode>& qpsNode, vector<shared_ptr<QPSResultTable>>& resultTables) {
	switch (qpsNode->getType()) {
		case QPSNodeType::PATTERN:
			evaluatePatternClause(dynamic_cast<QPSPatternNode&>(*qpsNode), resultTables);
			break;
		case QPSNodeType::WITH:
			evaluateWithClause(*qpsNode, resultTables);
			break;
		default:
			evaluateSuchThatClause(*qpsNode, resultTables);
			break;
	}
}

vector<string>& QPSMultiClauseEvaluator::evaluateSelectionClause() {
	const QPSNode& resultNode = qpsTree.getResultNode();
	if (resultNode.getSize() == 1) {
		const QPSNode& childNode = resultNode.getChild();
		retrieveAndDisplayAllResults(childNode);
	} else {
		evaluateTupleClause(resultNode);
	}
	return queryResults;
}

/* The method will evaluate groups which has been sorted and grouped by the Optimizer.
* If any of the results from PKB is false, the Evaluator will throw QPSEvaluatorException.
* After evaluating each subgroups, the optimizer will attempt to drop the columns or tables that are not involved in SELECTION.
* If the vector of ResultTable is empty and yet no exception thrown (for empty results), this means no intermediate table is generated due to it only contains
* True/False clauses or trivial WITH clauses, which is in fact a non-target selection or non-constraint query.
* Otherwise, all the intermediate tables generated from each subgroup will undergo merging again, which will give us the final result table.
* @param nodeGroups Optimized QPSNodes Groups
*/
void QPSMultiClauseEvaluator::evaluateMultiClause(const NodeGroups& nodeGroups) {
	vector<shared_ptr<QPSResultTable>> processedClauseResultTables{};
	for (const NodeGroup& nodeGroup : nodeGroups) {
		vector<shared_ptr<QPSResultTable>> resultTables{};
		for (const shared_ptr<QPSNode>& node : nodeGroup) {
			processNode(node, resultTables);
		}

		if (!resultTables.empty()) {
			optimizer.getOptimizedResultTables(resultTables);
			processedClauseResultTables.push_back(processResultTables(resultTables));
		}
	}

	if (processedClauseResultTables.size() > 1) {
		auto checkFunc = bind(&QPSTree::checkIfSynonymsIsSelection, qpsTree, placeholders::_1);
		optimizer.getOptimizedSelectionResultTable(processedClauseResultTables, checkFunc);
	}

	if (processedClauseResultTables.empty()) {
		throw QPSEvaluatorNonTargetSelectionException("Non-Target Selection");
	}

	shared_ptr<QPSResultTable> evaluationResult = processResultTables(processedClauseResultTables);
	const bool& isNonTargetSelection = qpsTree.getIsNonTargetSelection();
	if (!evaluationResult->isEmpty() && isNonTargetSelection) {
		throw QPSEvaluatorNonTargetSelectionException("Non-Target Selection");
	}

	evaluationResult = addTuplesColumns(evaluationResult);
	evaluationResult->getResultsByColumns(qpsTree.getResultNode(), queryResults);
}

shared_ptr<QPSResultTable> QPSMultiClauseEvaluator::addTuplesColumns(shared_ptr<QPSResultTable>& resultTable) {
	const QPSNode& selectionRootNode = qpsTree.getResultNode();
	for (int i = 0; i < selectionRootNode.getSize(); i++) {
		const QPSNode& childNode = selectionRootNode.getChild(i);
		const LeftArg& selectionArg = childNode.getLeftArg();
		const NodeType& selectionArgType = selectionArg.getType();
		const ArgNodeRef& selectionArgRef = selectionArg.getRef();
		if (!resultTable->checkIfColumnExists(selectionArgRef)) {
			vector<StmtData> tupleData = QPSCommunicator::getAllStmtsByType(selectionArgType);
			if (tupleData.empty()) {
				throw QPSEvaluatorException("Empty ResultTable");
			}

			const shared_ptr<QPSResultTable>& dataTable = make_shared<QPSResultTable>(QPSResultTable(selectionArg, tupleData));
			if (resultTable->isEmpty()) {
				resultTable = dataTable;
			} else {
				resultTable = resultTable->processIntermediateTable(dataTable);
			}
		}
	}
	return shared_ptr<QPSResultTable>(resultTable);
}
