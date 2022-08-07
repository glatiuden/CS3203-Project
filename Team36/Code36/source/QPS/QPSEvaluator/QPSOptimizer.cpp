#include <queue>
#include <iterator>

#include "QPSOptimizer.h"
#include "../QPSHelper.h"

namespace {
	NodeGroup setToVector(NodeSet& set) {
		NodeGroup results;
		results.reserve(set.size());
		for (auto& it = set.begin(); it != set.end(); ) {
			results.push_back(move(set.extract(it++).value()));
		}
		return results;
	}
}

QPSOptimizer::QPSOptimizer() {}

// OPTIMIZATION (1): Optimizing clauses into subgroups
// STEP 1: Remove duplicates
// STEP 2: Form a group with all the T/F clauses
// Add pattern to the remainingNodes for step 3 processing
// STEP 3: Group Remaining Nodes e.g. (syn,syn), (syn, 1), (1,syn)
// STEP 4: Internal sort of the clauses
// STEP 5: Sort the NodeGroups to evaluate bigger nodegroups first to minimize intermediate table sizes
NodeGroups QPSOptimizer::getOptimizedGroups(const QPSTree& qpsTree) {
	NodeGroups finalResults{};
	shared_ptr<QPSNode> suchThatRootNode = qpsTree.getClauseNodePointer();
	shared_ptr<QPSPatternNode> patternRootNode = qpsTree.getPatternNodePointer();
	shared_ptr<QPSNode> withRootNode = qpsTree.getWithNodePointer();

	NodeSet remainingNodes{};
	NodeGroup tfNodeGroup{};
	getTFGroups(suchThatRootNode, tfNodeGroup, remainingNodes);
	getTFGroups(withRootNode, tfNodeGroup, remainingNodes);
	if (!tfNodeGroup.empty()) {
		finalResults.push_back(tfNodeGroup);
	}

	for (int i = 0; i < patternRootNode->getSize(); i++) {
		remainingNodes.insert(patternRootNode->getChildPointer(i));
	}

	NodeSetGroups& groups = getGroups(remainingNodes);
	NodeGroups sortedNodeGroups(groups.size());
	int i = 0;
	for (const NodeSet& ns : groups) {
		sortedNodeGroups[i] = optimizeNodeSet(ns);
		i++;
	}
	sort(sortedNodeGroups.begin(), sortedNodeGroups.end(), [](const NodeGroup& a, const NodeGroup& b) { return a.size() > b.size(); });
	finalResults.insert(finalResults.end(), sortedNodeGroups.begin(), sortedNodeGroups.end());
	return finalResults;
}

void QPSOptimizer::print(NodeSet& nodeSet) {
	for (shared_ptr<QPSNode> node : nodeSet) {
		node->print();
	}
}

// OPTIMIZATION (1.1) : with + singleSyns clauses first, affects last.
/*
* Ranking
* 1. (syn, CONST) or (CONST, syn)
* 2. With Clauses
* 3. Less results first
* 4. As best as possible each syn should be preceded with a clause containing atleast one of its syns
* 5. Next*, Affects/* Last.
*/
NodeGroup QPSOptimizer::optimizeNodeSet(const NodeSet& nodeSet) {
	NodeGroup finalNodeGroup{};
	NodeGroup withNodeGroup{};
	NodeGroup singleSynsNodeGroup{};
	NodeGroup othersNodeGroup{};
	NodeGroup affectsNextTNodeGroups{};
	for (const shared_ptr<QPSNode>& node : nodeSet) {
		switch (node->getType()) {
			case QPSNodeType::WITH:
				withNodeGroup.push_back(node);
				break;
			case QPSNodeType::NEXT_T:
			case QPSNodeType::AFFECTS:
			case QPSNodeType::AFFECTS_T:
				affectsNextTNodeGroups.push_back(node);
				break;
			default:
				if (isSynonymAndSynonym(node->getLeftArgType(), node->getRightArgType())) {
					othersNodeGroup.push_back(node);
				} else {
					singleSynsNodeGroup.push_back(node);
				}
		}
	}

	finalNodeGroup.reserve(nodeSet.size());
	finalNodeGroup.insert(finalNodeGroup.end(), withNodeGroup.begin(), withNodeGroup.end());
	finalNodeGroup.insert(finalNodeGroup.end(), singleSynsNodeGroup.begin(), singleSynsNodeGroup.end());
	finalNodeGroup.insert(finalNodeGroup.end(), othersNodeGroup.begin(), othersNodeGroup.end());
	finalNodeGroup.insert(finalNodeGroup.end(), affectsNextTNodeGroups.begin(), affectsNextTNodeGroups.end());
	return finalNodeGroup;
}

NodeSetGroups QPSOptimizer::getGroups(const NodeSet& nodes) {
	QPSNodeGraph& graph = generateQPSNodeGraph(nodes);
	NodeSetGroups groups{};
	NodeSet processed{};
	for (const shared_ptr<QPSNode>& node : nodes) {
		if (processed.count(node)) {
			continue;
		}

		NodeSet visited{};
		queue<shared_ptr<QPSNode>> q({ node });
		while (!q.empty()) {
			shared_ptr<QPSNode> curr = q.front();
			q.pop();
			if (visited.count(curr)) {
				continue;
			}

			visited.insert(curr);
			for (const shared_ptr<QPSNode>& i : graph[curr]) {
				q.push(i);
			}
		}

		for (const shared_ptr<QPSNode>& v : visited) {
			processed.insert(v);
		}

		groups.push_back(visited);
	}
	return groups;
}

SynMap QPSOptimizer::generateSynMap(const NodeSet& nodes) {
	SynMap synMap{};
	for (const shared_ptr<QPSNode>& node : nodes) {
		const ArgNodeRef& leftArgRef = node->getLeftArgRef();
		const ArgNodeRef& rightArgRef = node->getRightArgRef();

		if (isSynonym(node->getLeftArgType())) {
			synMap[leftArgRef].push_back(node);
		}

		if (isSynonym(node->getRightArgType())) {
			synMap[rightArgRef].push_back(node);
		}
	}
	return synMap;
}

QPSNodeGraph QPSOptimizer::generateQPSNodeGraph(const NodeSet& nodes) {
	SynMap& synMap = generateSynMap(nodes);
	QPSNodeGraph graph{};
	for (const shared_ptr<QPSNode>& node : nodes) {
		const ArgNodeRef& leftArgRef = node->getLeftArgRef();
		const ArgNodeRef& rightArgRef = node->getRightArgRef();

		if (isSynonym(node->getLeftArgType())) {
			const NodeGroup& leftNodeGroup = synMap[leftArgRef];
			for (const shared_ptr<QPSNode>& i : leftNodeGroup) {
				if (node != i) {
					graph[node].insert(i);
				}
			}
		}

		if (isSynonym(node->getRightArgType())) {
			const NodeGroup& rightNodeGroup = synMap[rightArgRef];
			for (const shared_ptr<QPSNode>& i : rightNodeGroup) {
				if (node != i) {
					graph[node].insert(i);
				}
			}
		}
	}
	return graph;
}

void QPSOptimizer::getTFGroups(const shared_ptr<QPSNode>& qpsNode, NodeGroup& tfNodeGroup, NodeSet& remainingNodes) {
	NodeSet tfNodeSet{};
	for (int i = 0; i < qpsNode->getSize(); i++) {
		const shared_ptr<QPSNode>& childNode = qpsNode->getChildPointer(i);
		const NodeType& leftArgType = childNode->getLeftArgType();
		const NodeType& rightArgType = childNode->getRightArgType();
		if (isTrueFalseClause(leftArgType, rightArgType)) {
			tfNodeSet.insert(childNode);
			continue;
		}
		remainingNodes.insert(childNode);
	}
	tfNodeGroup.insert(tfNodeGroup.end(), tfNodeSet.begin(), tfNodeSet.end());
}

// OPTIMIZATION (2): as much as possible evaluate clauses preceded by clauses that share syns. sorted by size.
void QPSOptimizer::getOptimizedResultTables(vector<shared_ptr<QPSResultTable>>& resultTables) {
	sort(resultTables.begin(), resultTables.end(), [](shared_ptr<QPSResultTable> a, shared_ptr<QPSResultTable> b) { return a->size() < b->size(); });

	vector<shared_ptr<QPSResultTable>> q{};
	q.reserve(resultTables.size());
	unordered_set<string> activatedSyns{};

	while (!resultTables.empty()) {
		if (q.empty()) {
			const shared_ptr<QPSResultTable>& currResultTable = resultTables.front();
			q.push_back(currResultTable);
			const vector<Column>& cols = currResultTable->getColumns();
			copy(cols.begin(), cols.end(), inserter(activatedSyns, activatedSyns.end()));
			resultTables.erase(resultTables.begin());
			continue;
		}

		bool isInActivatedSet = false;
		for (auto& it = resultTables.begin(); it != resultTables.end(); it++) {
			const shared_ptr<QPSResultTable>& currResultTable = *it;
			const vector<Column>& cols = currResultTable->getColumns();
			for (const Column& c : cols) {
				isInActivatedSet = isInActivatedSet || activatedSyns.count(c);
			}

			if (isInActivatedSet) {
				copy(cols.begin(), cols.end(), inserter(activatedSyns, activatedSyns.end()));
				q.push_back(currResultTable);
				resultTables.erase(it);
				break;
			}
		}
	}

	resultTables.clear();
	resultTables.insert(resultTables.end(), q.begin(), q.end());
}

// OPTIMIZATION (3): Drop columns & tables that are not involved in selection after the remaining tables are formed
void QPSOptimizer::getOptimizedSelectionResultTable(vector<shared_ptr<QPSResultTable>>& resultTables, function<bool(string)> checkIfSynonymsIsSelection) {
	vector<shared_ptr<QPSResultTable>> finalResultTables{};
	for (const shared_ptr<QPSResultTable>& resultTable : resultTables) {
		int matchedCount = 0;
		const vector<string>& columns = resultTable->getColumns();
		vector<string> columnsToBeDrop{};
		for (const string& column : columns) {
			if (checkIfSynonymsIsSelection(column)) {
				matchedCount++;
			} else {
				columnsToBeDrop.push_back(column);
			}
		}

		if (matchedCount == columns.size()) {
			finalResultTables.push_back(resultTable);
		} else if (columnsToBeDrop.size() > 0 && columnsToBeDrop.size() != columns.size()) {
			resultTable->dropColumns(columnsToBeDrop);
			finalResultTables.push_back(resultTable);
		}
	}

	resultTables.clear();
	if (finalResultTables.empty()) {
		return;
	}
	resultTables.insert(resultTables.end(), finalResultTables.begin(), finalResultTables.end());
}
