#include <iostream>

#include "QPSNode.h"

QPSNode::QPSNode() {}

QPSNode::QPSNode(const QPSNodeType& type) : type{ type } {}

/*
* Used for Selection
*/
QPSNode::QPSNode(const QPSNodeType& type, const LeftArg& leftArg) : type{ type }, leftArg{ leftArg } {}

/*
* Used for Such-That and With
*/
QPSNode::QPSNode(const QPSNodeType& type, const LeftArg& leftArg, const RightArg& rightArg) : type{ type }, leftArg{ leftArg }, rightArg{ rightArg } {}

int QPSNode::getSize() const {
	return children.size();
}

QPSNode QPSNode::getChild() const {
	return *children.front();
}

QPSNode& QPSNode::getChild(Index index) const {
	return *children.at(index);
}

shared_ptr<QPSNode> QPSNode::getChildPointer(Index index) {
	return shared_ptr<QPSNode>(children.at(index));
}

QPSNode QPSNode::getParent() const {
	return *parent;
}

QPSNodeType QPSNode::getType() const {
	return type;
}

LeftArg QPSNode::getLeftArg() const {
	return leftArg;
}

RightArg QPSNode::getRightArg() const {
	return rightArg;
}

LeftArg& QPSNode::getLeftArgReference() {
	return leftArg;
}

RightArg& QPSNode::getRightArgReference() {
	return rightArg;
}

NodeType QPSNode::getLeftArgType() const {
	return leftArg.getType();
}

NodeType QPSNode::getRightArgType() const {
	return rightArg.getType();
}

ArgNodeRef QPSNode::getLeftArgRef() const {
	return leftArg.getRef();
}

ArgNodeRef QPSNode::getRightArgRef() const {
	return rightArg.getRef();
}

NodeType QPSNode::getLeftAttrName() const {
	return leftArg.getAttrName();
}

NodeType QPSNode::getRightAttrName() const {
	return rightArg.getAttrName();
}

void QPSNode::addChild(QPSNode* _child) {
	children.push_back(_child);
}

void QPSNode::setParent(QPSNode* _parent) {
	parent = _parent;
}

void QPSNode::setType(const QPSNodeType _type) {
	type = _type;
}

void QPSNode::setLeftArg(const LeftArg& _leftArg) {
	leftArg = _leftArg;
}

void QPSNode::setArgs(const LeftArg& _leftArg, const RightArg& _rightArg) {
	leftArg = _leftArg;
	rightArg = _rightArg;
}

void QPSNode::clear() {
	children.clear();
}

void QPSNode::print() {
	string type_string = convertQPSNodeTypetoString(type);
	cout << "Node Type: " << type_string;
	cout << ", " << "Left Arg: " << leftArg.getRef();
	bool hasRightArg = !rightArg.getRef().empty();
	if (hasRightArg) {
		cout << ", " << "Right Arg: " << rightArg.getRef() << endl;
	} else {
		cout << endl;
	}
}

bool QPSNode::operator==(const QPSNode& rhs) const {
	return type == rhs.type && leftArg == rhs.leftArg && rightArg == rhs.rightArg;
}

bool QPSNode::operator!=(const QPSNode& rhs) const {
	return !(*this == rhs);
}
