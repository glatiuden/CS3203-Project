#include <queue>
#include <stack>
#include "AffectsTable.h"
#include "PKB.h"

namespace {
	vector<pair<StmtData, StmtData>> setToVector(const DataPairSet& input) {
		vector<pair<StmtData, StmtData>> results;
		for (pair<StmtData, StmtData> result : input) {
			results.push_back(result);
		}
		return results;
	}
}

vector<StmtNode*> AffectsTable::getAffectNodesBefore(StmtNode* afterNode) {
	// check cache
	vector<StmtNode*> cachedNodes = PKB::cache.getAffectNodesBefore(afterNode);
	if (cachedNodes.size()) {
		return cachedNodes;
	}

	int stmtNo = afterNode->getData().getStmtNo();

	vector<StmtNode*> affectingNodes;
	stack<StmtNode*> cfgStack;
	unordered_set<StmtNode*> visited;
	vector<StmtData> varsNotModified = PKB::getUsesByStmtNo(stmtNo);
	unordered_map<int, vector<StmtData>> varsMap;
	unordered_map<int, unordered_set<StmtNode*>> visitedMap;

	bool multipleParents = afterNode->getParents().size() > 1;
	for (StmtNode* prev : afterNode->getParents()) {
		cfgStack.push(prev);
		visited.insert(prev);
		if (multipleParents) {
			varsMap[prev->getData().getStmtNo()] = varsNotModified;
			visitedMap[prev->getData().getStmtNo()] = visited;
		}
	}

	while (cfgStack.size()) {
		StmtNode* curr = cfgStack.top();
		cfgStack.pop();

		int currStmtNo = curr->getData().getStmtNo();
		if (varsMap.find(currStmtNo) != varsMap.end()) {
			// curr is the start of a different branch, use cached varsNotModified list and visited set
			varsNotModified = varsMap[currStmtNo];
			visited = visitedMap[currStmtNo];
		}

		NodeType currType = curr->getData().getType();
		if (currType == NodeType::ASSIGN || currType == NodeType::READ || currType == NodeType::CALL) {
			vector<StmtData> currVars = PKB::getModifiesByStmtNo(currStmtNo);
			for (StmtData var : currVars) {
				auto commonVar = std::find_if(varsNotModified.begin(), varsNotModified.end(), [var](StmtData data) { return data.getValue() == var.getValue(); });
				if (commonVar != varsNotModified.end()) {
					// curr stmt modifies a var used by afterStmt and the var is not modified by any stmt after curr stmt
					if (currType == NodeType::ASSIGN) {
						// curr stmt is an assign stmt that affects afterStmt
						affectingNodes.push_back(curr);
					}

					varsNotModified.erase(commonVar); // remove var from list

					if (varsNotModified.size() == 0) {
						// all stmts that affect afterStmt have been found
						continue;
					}
				}
			}
		}

		multipleParents = curr->getParents().size() > 1;
		for (StmtNode* prev : curr->getParents()) {
			if (!visited.count(prev)) {
				// next node has not been visited yet
				cfgStack.push(prev);
				visited.insert(prev);
				if (multipleParents) {
					varsMap[prev->getData().getStmtNo()] = varsNotModified;
					visitedMap[prev->getData().getStmtNo()] = visited;
				}
			}
		}
	}

	PKB::cache.cacheAffectNodesBefore(afterNode, affectingNodes);
	return affectingNodes;
}

vector<StmtNode*> AffectsTable::getAffectNodesAfter(StmtNode* beforeNode) {
	// check cache
	vector<StmtNode*> cachedNodes = PKB::cache.getAffectNodesAfter(beforeNode);
	if (cachedNodes.size()) {
		return cachedNodes;
	}

	int stmtNo = beforeNode->getData().getStmtNo();

	vector<StmtNode*> affectedNodes;
	string varName = PKB::getModifiesByStmtNo(stmtNo)[0].getValue();

	queue<StmtNode*> cfgQueue;
	unordered_set<StmtNode*> visited;

	for (StmtNode* child : beforeNode->getChildren()) {
		cfgQueue.push(child);
		visited.insert(child);
	}

	while (cfgQueue.size()) {
		StmtNode* curr = cfgQueue.front();
		cfgQueue.pop();

		NodeType currType = curr->getData().getType();
		if (currType == NodeType::ASSIGN
			&& PKB::checkUses(curr->getData().getStmtNo(), varName)) {
			// curr stmt is affected by beforeStmt
			affectedNodes.push_back(curr);
		}

		if ((currType == NodeType::ASSIGN || currType == NodeType::READ || currType == NodeType::CALL)
			&& PKB::checkModifies(curr->getData().getStmtNo(), varName)) {
			// curr stmt modifies var, so all nodes after this will not be affected
			continue;
		}

		for (StmtNode* child : curr->getChildren()) {
			if (!visited.count(child)) {
				cfgQueue.push(child);
				visited.insert(child);
			}
		}
	}

	PKB::cache.cacheAffectNodesAfter(beforeNode, affectedNodes);
	return affectedNodes;
}

bool AffectsTable::checkIfAffectsExists() {
	for (StmtNode* root : assignCFGNodes) {
		vector<StmtNode*> affectedNodes = getAffectNodesAfter(root);
		if (affectedNodes.size()) {
			return true;
		}
	}
	return false;
}

bool AffectsTable::checkAffects(int beforeStmtNo, int afterStmtNo) {
	StmtNode* beforeNode = allCFGNodesMap[beforeStmtNo];
	string varName = PKB::getModifiesByStmtNo(beforeStmtNo)[0].getValue(); // assign stmt always modifies exactly 1 variable
	if (!PKB::checkUses(afterStmtNo, varName)) {
		// afterStmt does not use a variable that is modified by beforeStmt
		return false;
	}

	vector<StmtNode*> affectedNodes = getAffectNodesAfter(beforeNode);
	for (StmtNode* node : affectedNodes) {
		if (node->getData().getStmtNo() == afterStmtNo) {
			return true;
		}
	}

	return false;
}

bool AffectsTable::checkAffectsTrans(int beforeStmtNo, int afterStmtNo) {
	StmtNode* beforeNode = allCFGNodesMap[beforeStmtNo];

	queue<StmtNode*> cfgQueue;
	unordered_set<StmtNode*> visited;

	cfgQueue.push(beforeNode);
	visited.insert(beforeNode);

	while (cfgQueue.size()) {
		StmtNode* curr = cfgQueue.front();
		cfgQueue.pop();

		vector<StmtNode*> affectedNodes = getAffectNodesAfter(curr);
		for (StmtNode* node : affectedNodes) {
			if (node->getData().getStmtNo() == afterStmtNo) {
				return true;
			}

			if (!visited.count(node)) {
				cfgQueue.push(node);
				visited.insert(node);
			}
		}
	}

	return false;
}

vector<pair<StmtData, StmtData>> AffectsTable::getAllAffects() {
	DataPairSet resultsSet;

	for (StmtNode* root : assignCFGNodes) {
		StmtData rootData = root->getData();

		queue<StmtNode*> cfgQueue;
		unordered_set<StmtNode*> visited;

		vector<StmtNode*> affectedNodes = getAffectNodesAfter(root);
		for (StmtNode* node : affectedNodes) {
			resultsSet.insert({ rootData, node->getData() });
		}
	}

	return setToVector(resultsSet);
}

vector<pair<StmtData, StmtData>> AffectsTable::getAllAffectsTrans() {
	DataPairSet resultsSet;

	for (StmtNode* root : assignCFGNodes) {
		queue<StmtNode*> cfgQueue;
		unordered_set<StmtNode*> visited;

		cfgQueue.push(root);
		visited.insert(root);

		while (cfgQueue.size()) {
			StmtNode* curr = cfgQueue.front();
			cfgQueue.pop();

			vector<StmtNode*> affectedNodes = getAffectNodesAfter(curr);

			for (StmtNode* node : affectedNodes) {
				resultsSet.insert({ root->getData(), node->getData() });

				if (!visited.count(node)) {
					cfgQueue.push(node);
					visited.insert(node);
				}
			}
		}
	}

	return setToVector(resultsSet);
}

vector<pair<StmtData, StmtData>> AffectsTable::getAffectsBefore(int stmtNo) {
	DataPairSet resultsSet;

	StmtNode* afterNode = allCFGNodesMap[stmtNo];
	vector<StmtNode*> affectingNodes = getAffectNodesBefore(afterNode);
	for (StmtNode* node : affectingNodes) {
		resultsSet.insert({ node->getData(), afterNode->getData() });
	}

	return setToVector(resultsSet);
}

vector<pair<StmtData, StmtData>> AffectsTable::getAffectsBeforeTrans(int stmtNo) {
	DataPairSet resultsSet;

	StmtNode* afterNode = allCFGNodesMap[stmtNo];

	stack<StmtNode*> cfgStack;
	unordered_set<StmtNode*> visited;

	cfgStack.push(afterNode);
	visited.insert(afterNode);

	while (cfgStack.size()) {
		StmtNode* curr = cfgStack.top();
		cfgStack.pop();

		vector<StmtNode*> affectingNodes = getAffectNodesBefore(curr);
		for (StmtNode* node : affectingNodes) {
			resultsSet.insert({ node->getData(), afterNode->getData() });

			if (!visited.count(node)) {
				cfgStack.push(node);
				visited.insert(node);
			}
		}
	}

	return setToVector(resultsSet);
}

vector<pair<StmtData, StmtData>> AffectsTable::getAffectsAfter(int stmtNo) {
	DataPairSet resultsSet;

	StmtNode* beforeNode = allCFGNodesMap[stmtNo];

	vector<StmtNode*> affectedNodes = getAffectNodesAfter(beforeNode);
	for (StmtNode* node : affectedNodes) {
		resultsSet.insert({ beforeNode->getData(), node->getData() });
	}

	return setToVector(resultsSet);
}

vector<pair<StmtData, StmtData>> AffectsTable::getAffectsAfterTrans(int stmtNo) {
	DataPairSet resultsSet;

	StmtNode* beforeNode = allCFGNodesMap[stmtNo];

	queue<StmtNode*> cfgQueue;
	unordered_set<StmtNode*> visited;

	cfgQueue.push(beforeNode);
	visited.insert(beforeNode);

	while (cfgQueue.size()) {
		StmtNode* curr = cfgQueue.front();
		cfgQueue.pop();

		vector<StmtNode*> affectedNodes = getAffectNodesAfter(curr);
		for (StmtNode* node : affectedNodes) {
			resultsSet.insert({ beforeNode->getData(), node->getData() });

			if (!visited.count(node)) {
				cfgQueue.push(node);
				visited.insert(node);
			}
		}
	}

	return setToVector(resultsSet);
}

void AffectsTable::clearAllEntries() {
	allCFGNodesMap.clear();
	assignCFGNodes.clear();
}