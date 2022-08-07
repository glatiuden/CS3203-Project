#pragma once
#include "../../QPSEvaluator.h"
#include "../QPSResultTable.h"
#include "../QPSOptimizer.h"

using namespace std;

class QPSMultiClauseEvaluator : public QPSEvaluator {
public:
	QPSMultiClauseEvaluator(QPSTree& qpsTree);
private:
	QPSOptimizer optimizer{};
	virtual vector<string> evaluateQuery() override;
	void evaluateTupleClause(const QPSNode& selectionRootNode);
	void evaluateMultiClause(const NodeGroups& nodeSetGroups);
	shared_ptr<QPSResultTable> addTuplesColumns(shared_ptr<QPSResultTable>& resultTable);

	void evaluateSuchThatClause(QPSNode& qpsNode, vector<shared_ptr<QPSResultTable>>& resultTables);
	void evaluatePatternClause(QPSPatternNode& qpsNode, vector<shared_ptr<QPSResultTable>>& resultTables);
	void evaluateWithClause(QPSNode& qpsNode, vector<shared_ptr<QPSResultTable>>& resultTables);
	void processNode(const shared_ptr<QPSNode>& qpsNode, vector<shared_ptr<QPSResultTable>>& resultTables);
	shared_ptr<QPSResultTable> processResultTables(const vector<shared_ptr<QPSResultTable>>& resultTables);
	vector<string>& evaluateSelectionClause();
};
