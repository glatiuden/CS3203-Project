#pragma once
#include "StmtNode.h"

class Cache {
private:
	unordered_map<StmtNode*, vector<StmtNode*>> affectNodesBeforeMap;
	unordered_map<StmtNode*, vector<StmtNode*>> affectNodesAfterMap;
public:
	void clear();
	vector<StmtNode*> Cache::getAffectNodesBefore(StmtNode* afterNode);
	vector<StmtNode*> Cache::getAffectNodesAfter(StmtNode* beforeNode);
	void cacheAffectNodesBefore(StmtNode* afterNode, const vector<StmtNode*>& nodes);
	void cacheAffectNodesAfter(StmtNode* beforeNode, const vector<StmtNode*>& nodes);
};