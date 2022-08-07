#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <queue>
#include <algorithm>
#include <functional>

#include "StmtRsTable.h"
#include "StmtNode.h"

using namespace std;

void StmtRsTable::insertStmtRs(const vector<StmtData>& stmtRs) {
	if (stmtRs.size() == 0) {
		return;
	}

	StmtNode* node = nullptr;
	for (size_t i = 0; i < stmtRs.size(); i++) {
		if (!node) {
			node = new StmtNode(stmtRs[i]);
		} else {
			StmtNode* next = new StmtNode(stmtRs[i]);
			node->addChild(next);
			node = next;
		}

		stmtNoMap[node->getData().getStmtNo()] = node;
		typeMap[node->getData().getType()].insert(node);
	}
}

bool StmtRsTable::checkStmtRs(int beforeStmtNo, int afterStmtNo) {
	if (!stmtNoMap.count(afterStmtNo) || !stmtNoMap.count(beforeStmtNo)) {
		return false;
	}

	StmtNode* beforeNode = stmtNoMap[beforeStmtNo];
	StmtNode* afterNode = stmtNoMap[afterStmtNo];

	if (beforeNode->getChildren().size() < afterNode->getParents().size()) {
		for (StmtNode* child : beforeNode->getChildren()) {
			if (child == afterNode) {
				return true;
			}
		}
	} else {
		for (StmtNode* parent : afterNode->getParents()) {
			if (parent == beforeNode) {
				return true;
			}
		}
	}
	return false;
}

bool StmtRsTable::checkStmtRs(string caller, string callee) {
	if (!callNameMap.count(caller) || !callNameMap.count(callee)) {
		return false;
	}

	unordered_set<StmtNode*> callerNodes = callNameMap[caller];
	unordered_set <StmtNode*> calleeNodes = callNameMap[callee];
	
	for (StmtNode* node : calleeNodes) {
		StmtNode* curr = node->getFirstParent();
		if (callerNodes.count(curr)) {
			return true;
		}
	}
	return false;
}

bool StmtRsTable::checkStmtRsTrans(int beforeStmtNo, int afterStmtNo) {
	if (!stmtNoMap.count(beforeStmtNo) || !stmtNoMap.count(afterStmtNo)) {
		return false;
	}

	StmtNode* node = stmtNoMap[afterStmtNo];
	StmtNode* target = stmtNoMap[beforeStmtNo];
	queue<StmtNode*> q;
	unordered_set<StmtNode*> visited;
	q.push(node);
	visited.insert(node);

	while (q.size()) {
		StmtNode* curr = q.front();
		q.pop();

		for (StmtNode* parent : curr->getParents()) {
			if (parent == target) {
				return true;
			}

			if (visited.find(parent) == visited.end()) {
				// parent node has not been visited yet
				q.push(parent);
				visited.insert(parent);
			}
		}
	}

	return false;
}

bool StmtRsTable::checkStmtRsTrans(string caller, string callee) {
	if (!callNameMap.count(caller) || !callNameMap.count(callee)) {
		return false;
	}

	unordered_set<StmtNode*> callerNodes = callNameMap[caller];
	unordered_set <StmtNode*> calleeNodes = callNameMap[callee];

	for (StmtNode* node : calleeNodes) {
		StmtNode* curr = node->getFirstParent();
		while (curr) {
			if (callerNodes.count(curr)) {
				return true;
			}
			curr = curr->getFirstParent();
		}
	}
	return false;
}

bool StmtRsTable::checkIfExists() {
	return !stmtNoMap.empty() || !typeMap.empty() || !callNameMap.empty();
}

unordered_set<StmtNode*> StmtRsTable::getStmtNodeByType(NodeType type) {
	unordered_set<StmtNode*> nodes;
	if (type == NodeType::STMT) {
		for (auto& it : stmtNoMap) {
			nodes.insert(it.second);
		}
	} else {
		nodes = typeMap[type];
	}
	return nodes;
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmt(NodeType beforeType, NodeType afterType) {
	if (beforeType != NodeType::STMT && !typeMap.count(beforeType) 
		|| afterType != NodeType::STMT && !typeMap.count(afterType)) {
		return {};
	}

	vector<pair<StmtData, StmtData>> result;

	unordered_set<StmtNode*> nodes = getStmtNodeByType(afterType);
	for (StmtNode* node : nodes) {
		for (StmtNode* parent : node->getParents()) {
			if (beforeType == NodeType::STMT
				|| beforeType == parent->getData().getType()) {
				pair<StmtData, StmtData> resultPair{ parent->getData(), node->getData() };
				result.push_back(resultPair);
			}
		}
	}

	return result;
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtTrans(NodeType beforeType, NodeType afterType) {
	if (beforeType != NodeType::STMT && !typeMap.count(beforeType)
		|| afterType != NodeType::STMT && !typeMap.count(afterType)) {
		return {};
	}

	vector<pair<StmtData, StmtData>> result;

	unordered_set<StmtNode*> nodes = getStmtNodeByType(afterType);
	for (StmtNode* node : nodes) {
		queue<StmtNode*> q;
		unordered_set<StmtNode*> visited;

		for (StmtNode* parent : node->getParents()) {
			q.push(parent);
			visited.insert(parent);
		}

		while (q.size()) {
			StmtNode* curr = q.front();
			q.pop();

			if (beforeType == NodeType::STMT
				|| beforeType == curr->getData().getType()) {
				pair<StmtData, StmtData> resultPair{ curr->getData(), node->getData() };
				result.push_back(resultPair);
			}

			for (StmtNode* parent : curr->getParents()) {
				if (visited.find(parent) == visited.end()) {
					// parent node has not been visited yet
					q.push(parent);
					visited.insert(parent);
				}
			}
		}
	}

	reverse(result.begin(), result.end());
	return result;
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtBefore(unordered_set<StmtNode*> nodes) {
	vector<pair<StmtData, StmtData>> result;
	for (StmtNode* node : nodes) {
		for (StmtNode* parent : node->getParents()) {
			pair<StmtData, StmtData> resultPair{ parent->getData(), node->getData() };
			result.push_back(resultPair);
		}
	}
	return result;
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtBeforeTrans(unordered_set<StmtNode*> nodes) {
	vector<pair<StmtData, StmtData>> result;
	
	for (StmtNode* node : nodes) {
		queue<StmtNode*> q;
		unordered_set<StmtNode*> visited;

		for (StmtNode* parent : node->getParents()) {
			q.push(parent);
			visited.insert(parent);
		}

		while (q.size()) {
			StmtNode* curr = q.front();
			q.pop();

			pair<StmtData, StmtData> resultPair{ curr->getData(), node->getData() };
			result.push_back(resultPair);

			for (StmtNode* parent : curr->getParents()) {
				if (visited.find(parent) == visited.end()) {
					// parent node has not been visited yet
					q.push(parent);
					visited.insert(parent);
				}
			}
		}
	}

	reverse(result.begin(), result.end());
	return result;
}


vector<pair<StmtData, StmtData>> StmtRsTable::getStmtAfter(unordered_set<StmtNode*> nodes) {
	vector<pair<StmtData, StmtData>> result;
	for (StmtNode* node : nodes) {
		for (StmtNode* child : node->getChildren()) {
			if (child->getData().getType() == NodeType::EMPTY) {
				// child node is a dummy node (for CFG)
				continue;
			}

			pair<StmtData, StmtData> resultPair{ node->getData(), child->getData() };
			result.push_back(resultPair);
		}
	}
	return result;
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtAfterTrans(unordered_set<StmtNode*> nodes) {
	vector<pair<StmtData, StmtData>> result;

	for (StmtNode* root : nodes) {
		queue<StmtNode*> q;
		unordered_set<StmtNode*> visited;

		for (StmtNode* child : root->getChildren()) {
			if (child->getData().getType() == NodeType::EMPTY) {
				// child node is a dummy node (for CFG)
				continue;
			}

			q.push(child);
			visited.insert(child);
		}

		while (q.size()) {
			StmtNode* curr = q.front();
			q.pop();

			pair<StmtData, StmtData> resultPair{ root->getData(), curr->getData() };
			result.push_back(resultPair);

			for (StmtNode* child : curr->getChildren()) {
				if (child->getData().getType() == NodeType::EMPTY) {
					// child node is a dummy node (for CFG)
					continue;
				}

				if (visited.find(child) == visited.end()) {
					// child node has not been visited yet
					q.push(child);
					visited.insert(child);
				}
			}
		}
	}

	return result;
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtBefore(int stmtNo) {
	if (!stmtNoMap.count(stmtNo)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { stmtNoMap[stmtNo] };
	return getStmtBefore(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtBefore(NodeType type) {
	if (type != NodeType::STMT && !typeMap.count(type)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { getStmtNodeByType(type) };
	return getStmtBefore(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtBefore(string callName) {
	if (!callNameMap.count(callName)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { callNameMap[callName] };
	return getStmtBefore(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtBeforeTrans(int stmtNo) {
	if (!stmtNoMap.count(stmtNo)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { stmtNoMap[stmtNo] };
	return getStmtBeforeTrans(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtBeforeTrans(NodeType type) {
	if (type != NodeType::STMT && !typeMap.count(type)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { getStmtNodeByType(type) };
	return getStmtBeforeTrans(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtBeforeTrans(string callName) {
	if (!callNameMap.count(callName)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { callNameMap[callName] };
	return getStmtBeforeTrans(nodeSet);
}


vector<pair<StmtData, StmtData>> StmtRsTable::getStmtAfter(int stmtNo) {
	if (!stmtNoMap.count(stmtNo)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { stmtNoMap[stmtNo] };
	return getStmtAfter(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtAfter(NodeType type) {
	if (type != NodeType::STMT && !typeMap.count(type)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = getStmtNodeByType(type);
	return getStmtAfter(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtAfter(string callName) {
	if (!callNameMap.count(callName)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { callNameMap[callName] };
	return getStmtAfter(nodeSet);
}


vector<pair<StmtData, StmtData>> StmtRsTable::getStmtAfterTrans(int stmtNo) {
	if (!stmtNoMap.count(stmtNo)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { stmtNoMap[stmtNo] };
	return getStmtAfterTrans(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtAfterTrans(NodeType type) {
	if (type != NodeType::STMT && !typeMap.count(type)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = getStmtNodeByType(type);
	return getStmtAfterTrans(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getStmtAfterTrans(string callName) {
	if (!callNameMap.count(callName)) {
		return {};
	}

	unordered_set<StmtNode*> nodeSet = { callNameMap[callName] };
	return getStmtAfterTrans(nodeSet);
}

vector<pair<StmtData, StmtData>> StmtRsTable::getAllStmt() {
	vector<pair<StmtData, StmtData>> result;
	for (StmtNode* root : roots) {	
		queue<StmtNode*> q;
		q.push(root);

		while (q.size()) {
			StmtNode* curr = q.front();
			q.pop();
			for (StmtNode* child : curr->getChildren()) {
				pair<StmtData, StmtData> resultPair{ curr->getData(), child->getData() };
				result.push_back(resultPair);
				q.push(child);
			}
		}
	}
	return result;
}

vector<pair<StmtData, StmtData>> StmtRsTable::getAllStmtTrans() {
	vector<pair<StmtData, StmtData>> result;
	
	function<void(StmtNode*, vector<StmtNode*>)> dfs = [&dfs, &result](StmtNode* node, vector<StmtNode*> parents) {
		for (StmtNode* parent : parents) {
			pair<StmtData, StmtData> resultPair = { parent->getData(), node->getData() };
			result.push_back(resultPair);
		}
		
		vector<StmtNode*> parentCopy = parents;
		parentCopy.push_back(node);

		for (StmtNode* child : node->getChildren()) {
			dfs(child, parentCopy);
		}
	};
	
	vector<StmtNode*> parentList;
	for (StmtNode* root : roots) {
		dfs(root, parentList);
	}

	return result;
}

void StmtRsTable::clearAllEntries() {
	stmtNoMap.clear();
	roots.clear();
}
