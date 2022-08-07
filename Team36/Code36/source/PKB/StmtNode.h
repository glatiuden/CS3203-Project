#pragma once
#include "StmtData.h"

class StmtNode {
private:
	StmtData data;
	vector<StmtNode*> parents;
	vector<StmtNode*> children;
public:
	StmtNode(const StmtData& data) : data(data) {};
	StmtData getData();
	StmtNode* getFirstParent();
	vector<StmtNode*> getParents();
	vector<StmtNode*> getChildren();
	void addChild(StmtNode* node);
	void replaceChild(StmtNode* toReplace, StmtNode* replacement);
	void removeChild(StmtNode* toRemove);
	bool isDummy();
};
