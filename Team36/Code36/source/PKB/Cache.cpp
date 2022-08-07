#include "Cache.h"

void Cache::clear() {
	affectNodesBeforeMap.clear();
	affectNodesAfterMap.clear();
}

vector<StmtNode*> Cache::getAffectNodesBefore(StmtNode* afterNode) {
	if (!affectNodesBeforeMap.count(afterNode)) {
		return {};
	}
	return affectNodesBeforeMap[afterNode];
}

vector<StmtNode*> Cache::getAffectNodesAfter(StmtNode* beforeNode) {
	if (!affectNodesAfterMap.count(beforeNode)) {
		return {};
	}
	return affectNodesAfterMap[beforeNode];
}

void Cache::cacheAffectNodesBefore(StmtNode* afterNode, const vector<StmtNode*>& nodes) {
	affectNodesBeforeMap[afterNode] = nodes;
}

void Cache::cacheAffectNodesAfter(StmtNode* beforeNode, const vector<StmtNode*>& nodes) {
	affectNodesAfterMap[beforeNode] = nodes;
}
