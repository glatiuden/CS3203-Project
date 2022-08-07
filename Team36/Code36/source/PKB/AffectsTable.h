#pragma once
#include <vector>
#include <unordered_set>
#include "StmtNode.h"

struct DataPairHash {
	size_t operator()(const pair<StmtData, StmtData>& p) const {
		size_t firstHash = ((hash<int>()(p.first.getStmtNo())
							 ^ (hash<string>()(p.first.getValue()) << 1)) >> 1)
			^ (hash<string>()(p.first.getProcName()) << 1);

		size_t secondHash = ((hash<int>()(p.second.getStmtNo())
							  ^ (hash<string>()(p.second.getValue()) << 1)) >> 1)
			^ (hash<string>()(p.second.getProcName()) << 1);

		return firstHash ^ secondHash;
	}
};

typedef unordered_set<pair<StmtData, StmtData>, DataPairHash> DataPairSet;

class AffectsTable {
private:
	unordered_map<int, StmtNode*> allCFGNodesMap;
	vector<StmtNode*> assignCFGNodes;
	vector<StmtNode*> getAffectNodesBefore(StmtNode* afterNode);
	vector<StmtNode*> getAffectNodesAfter(StmtNode* beforeNode);
public:
	AffectsTable() {};
	AffectsTable(const unordered_map<int, StmtNode*>& allCFGNodesMap, const vector<StmtNode*>& assignCFGNodes) 
		: allCFGNodesMap(allCFGNodesMap), assignCFGNodes(assignCFGNodes) {};
	bool checkAffects(int beforeStmtNo, int afterStmtNo);
	bool checkAffectsTrans(int beforeStmtNo, int afterStmtNo);
	bool checkIfAffectsExists();
	vector<pair<StmtData, StmtData>> getAllAffects();
	vector<pair<StmtData, StmtData>> getAllAffectsTrans();
	vector<pair<StmtData, StmtData>> getAffectsBefore(int stmtNo);
	vector<pair<StmtData, StmtData>> getAffectsAfter(int stmtNo);
	vector<pair<StmtData, StmtData>> getAffectsBeforeTrans(int stmtNo);
	vector<pair<StmtData, StmtData>> getAffectsAfterTrans(int stmtNo);
	void clearAllEntries();
};


