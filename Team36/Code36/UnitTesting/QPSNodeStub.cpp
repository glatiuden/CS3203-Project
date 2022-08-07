#pragma once
#include "QPS/QPSTree/QPSNode.h"
#include "ArgNodeStub.cpp"

typedef ArgNode LeftArg;
typedef ArgNode RightArg;

class QPSNodeStub : public QPSNode {
public:
	QPSNodeStub::QPSNodeStub() {}
	QPSNodeStub::QPSNodeStub(QPSNodeType type) : QPSNode(type) {}
	QPSNodeStub::QPSNodeStub(QPSNodeType type, LeftArg leftArg) : QPSNode(type, leftArg) {}
	QPSNodeStub::QPSNodeStub(QPSNodeType type, LeftArg leftArg, RightArg rightArg) : QPSNode(type, leftArg, rightArg) {}

	bool QPSNodeStub::operator==(const QPSNodeStub& rhs) const {
		if (type == rhs.type && leftArg == rhs.leftArg && rightArg == rhs.rightArg) {
			return true;
		}
		return false;
	}

	int QPSNodeStub::getSize() const {
		return children.size();
	}

	QPSNodeType QPSNodeStub::getType() const {
		return type;
	}

	LeftArg* QPSNodeStub::getLeftArg() {
		return &leftArg;
	}

	RightArg* QPSNodeStub::getRightArg() {
		return &rightArg;
	}

	LeftArg QPSNodeStub::getLeftArgValue() const {
		return leftArg;
	}

	RightArg QPSNodeStub::getRightArgValue() const {
		return rightArg;
	}

	void QPSNodeStub::addChild(QPSNodeStub* _child) {
		children.push_back(_child);
	}

	void QPSNodeStub::setParent(QPSNodeStub* _parent) {
		parent = _parent;
	}

	void QPSNodeStub::setType(const QPSNodeType _type) {
		type = _type;
	}

	void QPSNodeStub::setLeftArg(const LeftArg _leftArg) {
		leftArg = _leftArg;
	}

	void QPSNodeStub::setArgs(const LeftArg _leftArg, const RightArg _rightArg) {
		leftArg = _leftArg;
		rightArg = _rightArg;
	}

	void QPSNodeStub::clear() {
		children.clear();
	}
};
