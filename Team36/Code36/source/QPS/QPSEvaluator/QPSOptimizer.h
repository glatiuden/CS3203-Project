#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>

#include "../../PKB/StmtData.h"
#include "../QPSTree/QPSNode.h"
#include "../QPSTree/QPSTree.h"
#include "QPSResultTable.h"

using namespace std;

struct PointerHash {
	size_t operator()(const shared_ptr<QPSNode> val) const {
		static const size_t shift = (size_t)log2(1 + sizeof(QPSNode));
		return (size_t)(val.get()) >> shift;
	}
};

typedef unordered_set<shared_ptr<QPSNode>, PointerHash> NodeSet;
typedef vector<NodeSet> NodeSetGroups;
typedef vector<shared_ptr<QPSNode>> NodeGroup;
typedef vector<NodeGroup> NodeGroups;
typedef unordered_map<string, NodeGroup> SynMap; // A map of all the SYN mapped to a list of nodes containing that SYN
typedef unordered_map<shared_ptr<QPSNode>, NodeSet, PointerHash> QPSNodeGraph; // Graph where each node is a QPSNode and each edge represents that there exists a syn shared between the two nodes

class QPSOptimizer {
public:
	QPSOptimizer();
	NodeGroups getOptimizedGroups(const QPSTree& qpsTree);
	void getOptimizedResultTables(vector<shared_ptr<QPSResultTable>>& resultTables);
	void getOptimizedSelectionResultTable(vector<shared_ptr<QPSResultTable>>& resultTables, function<bool(string)> checkIfSynonymsIsSelection);
private:
	void print(NodeSet&);
	void getTFGroups(const shared_ptr<QPSNode>& qpsNode, NodeGroup& tfNodeGroup, NodeSet& remainingNodes);
	NodeGroup optimizeNodeSet(const NodeSet& nodeSet);
	NodeSetGroups getGroups(const NodeSet& nodeSet);
	SynMap generateSynMap(const NodeSet& nodes);
	QPSNodeGraph generateQPSNodeGraph(const NodeSet& nodes);
};
