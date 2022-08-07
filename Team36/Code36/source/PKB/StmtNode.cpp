#include "StmtNode.h"

StmtData StmtNode::getData() {
	return data;
}

StmtNode* StmtNode::getFirstParent() {
	if (!getParents().size()) {
		return nullptr;
	}

	return getParents()[0];
}


vector<StmtNode*> StmtNode::getParents() {
	return parents;
}

vector<StmtNode*> StmtNode::getChildren() {
	return children;
}

void StmtNode::addChild(StmtNode* node) {
	node->parents.push_back(this);
	children.push_back(node);
}

// Replaces the first occurrence of toReplace node in the next vector with the replacement node if exists.
// Otherwise, do nothing.
void StmtNode::replaceChild(StmtNode* toReplace, StmtNode* replacement) {
	for (std::vector<StmtNode*>::size_type i = 0; i != children.size(); ++i) {
		if (children[i] == toReplace) {
			children[i] = replacement;
			replacement->parents.push_back(this);
			return;
		}
	}
}

// Removes the first occurrence of toRemove node in the next vector
// Otherwise, do nothing.
void StmtNode::removeChild(StmtNode* toRemove) {
	auto child = std::find(children.begin(), children.end(), toRemove);
	if (child != children.end()) {
		children.erase(child);
	}
}

bool StmtNode::isDummy() {
	return data.getStmtNo() == -1;
}
