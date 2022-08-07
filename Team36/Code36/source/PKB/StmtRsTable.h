#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "StmtNode.h"

using namespace std;

class StmtRsTable {
private:
	vector<StmtNode*> roots;
	unordered_map<int, StmtNode*> stmtNoMap;
	unordered_map<NodeType, unordered_set<StmtNode*>> typeMap;
	unordered_map<string, unordered_set<StmtNode*>> callNameMap;
	unordered_set<StmtNode*> getStmtNodeByType(NodeType type);
	vector<pair<StmtData, StmtData>> getStmtBefore(unordered_set<StmtNode*> node);
	vector<pair<StmtData, StmtData>> getStmtBeforeTrans(unordered_set<StmtNode*> node);
	vector<pair<StmtData, StmtData>> getStmtAfter(unordered_set<StmtNode*> node);
	vector<pair<StmtData, StmtData>> getStmtAfterTrans(unordered_set<StmtNode*> node);
public:
	StmtRsTable() {};
	StmtRsTable(const vector<StmtNode*>& roots,
				const unordered_map<int, StmtNode*>& stmtNoMap,
				const unordered_map<NodeType, unordered_set<StmtNode*>>& typeMap)
		: roots(roots), stmtNoMap(stmtNoMap), typeMap(typeMap) {
	};
	StmtRsTable(const vector<StmtNode*>& roots,
				const unordered_map<string, unordered_set<StmtNode*>>& callNameMap)
		: roots(roots), callNameMap(callNameMap) {
	};
	void insertStmtRs(const vector<StmtData>& stmtRs);
	bool checkStmtRs(int beforeStmtNo, int afterStmtNo);
	bool checkStmtRs(string caller, string callee);
	bool checkStmtRsTrans(int beforeStmtNo, int afterStmtNo);
	bool checkStmtRsTrans(string caller, string callee);
	bool checkIfExists();
	vector<pair<StmtData, StmtData>> getStmt(NodeType beforeType, NodeType afterType);
	vector<pair<StmtData, StmtData>> getStmtTrans(NodeType beforeType, NodeType afterType);
	vector<pair<StmtData, StmtData>> getStmtBefore(int stmtNo);
	vector<pair<StmtData, StmtData>> getStmtBefore(NodeType type);
	vector<pair<StmtData, StmtData>> getStmtBefore(string callName);
	vector<pair<StmtData, StmtData>> getStmtBeforeTrans(int stmtNo);
	vector<pair<StmtData, StmtData>> getStmtBeforeTrans(NodeType type);
	vector<pair<StmtData, StmtData>> getStmtBeforeTrans(string callName);
	vector<pair<StmtData, StmtData>> getStmtAfter(int stmtNo);
	vector<pair<StmtData, StmtData>> getStmtAfter(NodeType type);
	vector<pair<StmtData, StmtData>> getStmtAfter(string callName);
	vector<pair<StmtData, StmtData>> getStmtAfterTrans(int stmtNo);
	vector<pair<StmtData, StmtData>> getStmtAfterTrans(NodeType type);
	vector<pair<StmtData, StmtData>> getStmtAfterTrans(string callName);
	vector<pair<StmtData, StmtData>> getAllStmt();
	vector<pair<StmtData, StmtData>> getAllStmtTrans();
	void clearAllEntries();
};
