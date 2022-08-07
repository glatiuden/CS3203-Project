#pragma once
#include <string>
#include <vector>
#include <memory>

#include "ArgNode.h"
#include "../QPSHelper.h"
#include "../../NodeType.h"

using namespace std;

typedef ArgNode LeftArg;
typedef ArgNode RightArg;
typedef int Index;

class QPSNode {
public:
	QPSNode();
	QPSNode(const QPSNodeType& type);
	QPSNode(const QPSNodeType& type, const LeftArg& leftArg);
	QPSNode(const QPSNodeType& type, const LeftArg& leftArg, const RightArg& rightArg);

	virtual int getSize() const;
	QPSNode getParent() const;
	QPSNode getChild() const;
	QPSNode& getChild(Index index) const;
	shared_ptr<QPSNode> getChildPointer(Index index);
	QPSNodeType getType() const;
	LeftArg getLeftArg() const;
	RightArg getRightArg() const;
	LeftArg& getLeftArgReference();
	RightArg& getRightArgReference();
	NodeType getLeftArgType() const;
	NodeType getRightArgType() const;
	ArgNodeRef getLeftArgRef() const;
	ArgNodeRef getRightArgRef() const;
	NodeType getLeftAttrName() const;
	NodeType getRightAttrName() const;

	void addChild(QPSNode* _child);
	void setParent(QPSNode* _parent);
	void setType(const QPSNodeType type);
	void setLeftArg(const LeftArg& _leftArg);
	void setArgs(const LeftArg& _leftArg, const RightArg& _rightArg);
	virtual void print();
	void clear();
	bool operator==(const QPSNode& rhs) const;
	bool operator!=(const QPSNode& rhs) const;
	size_t operator()(const QPSNode& k) const {
		return ((hash<QPSNodeType>()(k.getType()) ^ (hash<string>()(k.getLeftArg().getRef()) << 1)) >> 1) ^ (hash<string>()(k.getRightArg().getRef()) << 1);
	}
protected:
	QPSNode* parent{}; // Used for Child Node to trace the parent node
	vector<QPSNode*> children{}; // Used for the Root Node to trace other nodes
	QPSNodeType type{}; // Identify the type of node
	LeftArg leftArg{};
	RightArg rightArg{};
};
