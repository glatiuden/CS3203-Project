#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#include "PKB.h"
#include "CFGProcessor.h"
void CFGProcessor::processCFGs(vector<StmtNode*> CFGs) {
	processCFGsForNext(CFGs);
	mapCFGNodes(CFGs);
}

void CFGProcessor::processCFGsForNext(vector<StmtNode*> CFGs) {
	queue<StmtNode*> cfgQueue;
	unordered_set<StmtNode*> visited;
	unordered_map<int, StmtNode*> nodeMap;
	unordered_map<NodeType, unordered_set<StmtNode*>> typeMap;
	for (StmtNode* node : CFGs) {
		if (node->isDummy()) {
			// CFG is empty
			continue;
		}
		cfgQueue.push(node);
		visited.insert(node);
		nodeMap[node->getData().getStmtNo()] = node;
		typeMap[node->getData().getType()].insert(node);

		while (cfgQueue.size()) {
			StmtNode* curr = cfgQueue.front();
			cfgQueue.pop();

			for (StmtNode* next : curr->getChildren()) {
				nodeMap[next->getData().getStmtNo()] = next;
				typeMap[next->getData().getType()].insert(next);

				if (visited.find(next) == visited.end()) {
					// next node has not been visited yet
					cfgQueue.push(next);
					visited.insert(next);
				}
			}
		}
	}
	PKB::nextTable = StmtRsTable(CFGs, nodeMap, typeMap);
}

void CFGProcessor::mapCFGNodes(vector<StmtNode*> CFGs) {
	unordered_map<int, StmtNode*> allCFGNodesMap;
	vector<StmtNode*> assignCFGNodes;

	queue<StmtNode*> cfgQueue;
	unordered_set<StmtNode*> visited;
	for (StmtNode* node : CFGs) {
		if (node->isDummy()) {
			// CFG is empty
			continue;
		}
		cfgQueue.push(node);
		visited.insert(node);

		while (cfgQueue.size()) {
			StmtNode* curr = cfgQueue.front();
			cfgQueue.pop();

			(allCFGNodesMap)[curr->getData().getStmtNo()] = curr;
			if (curr->getData().getType() == NodeType::ASSIGN) {
				assignCFGNodes.push_back(curr);
			}

			for (StmtNode* child : curr->getChildren()) {
				if (visited.find(child) == visited.end()) {
					// child node has not been visited yet
					cfgQueue.push(child);
					visited.insert(child);
				}
			}
		}
	}

	PKB::affectsTable = AffectsTable(allCFGNodesMap, assignCFGNodes);
}
