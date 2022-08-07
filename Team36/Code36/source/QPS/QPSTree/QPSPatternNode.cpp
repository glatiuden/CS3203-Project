#include <iostream>

#include "QPSPatternNode.h"

constexpr auto EMPTY_EXPRRESSION = nullptr;

QPSPatternNode::QPSPatternNode() : QPSNode(QPSNodeType::PATTERN), isPartialMatching(false) {}

QPSPatternNode::QPSPatternNode(const LeftArg& leftArg, const PatternSynArg& patternSynArg, const SemanticArg& semanticArg)
	: QPSNode(QPSNodeType::PATTERN, leftArg, patternSynArg), semanticArg(semanticArg), isPartialMatching(false) {
}

QPSPatternNode::QPSPatternNode(const LeftArg& leftArg, const PatternSynArg& patternSynArg, const PartialMatching& isPartialMatching, const SemanticArg& semanticArg)
	: QPSNode(QPSNodeType::PATTERN, leftArg, patternSynArg), isPartialMatching(isPartialMatching), semanticArg(semanticArg) {
}

QPSPatternNode::QPSPatternNode(const LeftArg& leftArg, const PatternSynArg& patternSynArg, const PartialMatching& isPartialMatching, const optional<TNode*>& expression, const SemanticArg& semanticArg)
	: QPSNode(QPSNodeType::PATTERN, leftArg, patternSynArg), isPartialMatching(isPartialMatching), expression(expression), semanticArg(semanticArg) {
}

int QPSPatternNode::getSize() const {
	return children.size();
}

QPSPatternNode QPSPatternNode::getChild() const {
	return *children.front();
}

QPSPatternNode QPSPatternNode::getChild(const Index& index) const {
	return *children.at(index);
}

shared_ptr<QPSPatternNode> QPSPatternNode::getChildPointer(const Index& index) {
	return shared_ptr<QPSPatternNode>(children.at(index));
}

PartialMatching QPSPatternNode::getIsPartialMatching() const {
	return isPartialMatching;
}

void QPSPatternNode::addChild(QPSPatternNode* _child) {
	children.push_back(_child);
}

void QPSPatternNode::setIsPartialMatching(const PartialMatching& _isPartialMatching) {
	isPartialMatching = _isPartialMatching;
}

PatternSynArg QPSPatternNode::getPatternSynArg() const {
	return rightArg;
}

ArgNodeType QPSPatternNode::getPatternSynArgNodeType() {
	return rightArg.getType();
}

ArgNodeRef QPSPatternNode::getPatternSynArgRef() {
	return rightArg.getRef();
}

NodeType QPSPatternNode::getSemanticArgType() {
	return semanticArg.getType();
}

bool QPSPatternNode::hasExpression() {
	return expression.has_value();
}

TNode* QPSPatternNode::getExpression() {
	if (hasExpression()) {
		return expression.value();
	}
	return EMPTY_EXPRRESSION;
}

void QPSPatternNode::print() {
	string typeString = convertQPSNodeTypetoString(type);
	cout << "Node Type: " << typeString;
	cout << ", " << "Left Arg: " << leftArg.getRef();
	cout << ", " << "Syn Arg: " << rightArg.getRef();
}

bool QPSPatternNode::operator==(const QPSPatternNode& rhs) const {
	return type == rhs.type && leftArg == rhs.leftArg && rightArg == rhs.rightArg && isPartialMatching == rhs.isPartialMatching;
}
