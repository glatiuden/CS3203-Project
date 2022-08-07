#include "StmtTree.h"

StmtNode* StmtTree::getRoot() {
	return root;
}

void StmtTree::linkNode(StmtNode* node1, StmtNode* node2) {
	node1->addChild(node2);
}

vector<StmtData> toVector() {
	vector<StmtData> list;
	/*
	StmtNode* node = root;
	while (node->next != nullptr) {
		
	}
	*/

	return list;
}