#include <string>
#include <vector>
#include <iostream>

#include "QPSTree.h"
#include "../QPSFilter.h"
#include "../../CustomException.h"

using namespace std;

QPSTree::QPSTree() : rootNode(new QPSNode(QPSNodeType::ROOT)), resultNode(new QPSNode(QPSNodeType::RESULT)), clauseNode(new QPSNode(QPSNodeType::CLAUSE)), patternNode(new QPSPatternNode()), withNode(new QPSNode(QPSNodeType::WITH)) {
	linkNode(rootNode, resultNode);
	linkNode(rootNode, clauseNode);
	linkNode(rootNode, patternNode);
	linkNode(rootNode, withNode);
	selectionVector.reserve(10);
	selectionMap.reserve(10);
	synonymMap.reserve(10);
}

void QPSTree::addResultNode(QPSNode* _resultNode) {
	LeftArg& selectionArg = _resultNode->getLeftArg();
	if (isResultNode(_resultNode->getType())) {
		linkNode(resultNode, _resultNode);
		setSelectionArgType(selectionArg);
	} else {
		throw QPSTreeException("Invalid Node Type. Expected: Result Type Node");
	}
}

void QPSTree::addClauseNode(QPSNode* _clauseNode) {
	if (isClauseNode(_clauseNode->getType())) {
		linkNode(clauseNode, _clauseNode);
	} else {
		throw QPSTreeException("Invalid Node Type. Expected: Clause Type Node");
	}
}

void QPSTree::addPatternNode(QPSPatternNode* _patternNode) {
	if (isPatternNode(_patternNode->getType())) {
		linkPatternNode(patternNode, _patternNode);
	} else {
		throw QPSTreeException("Invalid Node Type. Expected: Pattern Type Node");
	}
}

void QPSTree::addWithNode(QPSNode* _withNode) {
	if (isWithNode(_withNode->getType())) {
		linkNode(withNode, _withNode);
	} else {
		throw QPSTreeException("Invalid Node Type. Expected: With Type Node");
	}
}

void QPSTree::addToSynonymMap(const string& synonymValue, const NodeType& synonymNodeType) {
	if (isSynonym(synonymNodeType) && !synonymMap.count(synonymValue)) {
		synonymMap.insert(synonymValue);
	}
}

void QPSTree::setSelectionArgType(ArgNode& argNode) {
	isBooleanSelection = argNode.getType() == NodeType::BOOLEAN;
	if (!isBooleanSelection) {
		addToSelectionMap(argNode.getStrRef());
	}
}

QPSNode QPSTree::getRoot() const {
	return *rootNode;
}

QPSNode QPSTree::getResultNode() const {
	return *resultNode;
}

QPSNode QPSTree::getClauseNode() const {
	return *clauseNode;
}

QPSPatternNode QPSTree::getPatternNode() const {
	return *patternNode;
}

QPSNode QPSTree::getWithNode() const {
	return *withNode;
}

shared_ptr<QPSNode> QPSTree::getClauseNodePointer() const {
	return shared_ptr<QPSNode>(clauseNode);
}

shared_ptr<QPSPatternNode> QPSTree::getPatternNodePointer() const {
	return shared_ptr<QPSPatternNode>(patternNode);
}

shared_ptr<QPSNode> QPSTree::getWithNodePointer() const {
	return shared_ptr<QPSNode>(withNode);
}

void QPSTree::printTree() {
	cout << "Printing Query Tree... " << endl;
	cout << "Result Nodes: " << endl;
	for (int i = 0; i < resultNode->getSize(); i++) {
		resultNode->getChild(i).print();
	}
	cout << "Clause Nodes: " << endl;
	for (int i = 0; i < clauseNode->getSize(); i++) {
		clauseNode->getChild(i).print();
	}
	cout << "Pattern Nodes: " << endl;
	for (int i = 0; i < patternNode->getSize(); i++) {
		patternNode->getChild(i).print();
	}
	cout << "With Nodes: " << endl;
	for (int i = 0; i < withNode->getSize(); i++) {
		withNode->getChild(i).print();
	}
	cout << "Is Non-Target Selection: " << endl;
	if (isNonTargetSelection) {
		cout << "TRUE" << endl;
	} else {
		cout << "FALSE" << endl;
	}
}

void QPSTree::clearTree() {
	resultNode->clear();
	clauseNode->clear();
}

void QPSTree::addToSelectionMap(const string& selectionSyn) {
	selectionMap.insert(selectionSyn);
	selectionVector.push_back(selectionSyn);
}

void QPSTree::linkNode(QPSNode* parent_node, QPSNode* child_node) {
	child_node->setParent(parent_node);
	parent_node->addChild(child_node);
}

void QPSTree::linkPatternNode(QPSPatternNode* parent_node, QPSPatternNode* child_node) {
	child_node->setParent(parent_node);
	parent_node->addChild(child_node);
}

bool QPSTree::isResultNode(const QPSNodeType& type) {
	return type == QPSNodeType::SELECTION;
}

bool QPSTree::isClauseNode(const QPSNodeType& type) {
	switch (type) {
		case QPSNodeType::FOLLOWS:
		case QPSNodeType::FOLLOWS_T:
		case QPSNodeType::PARENT:
		case QPSNodeType::PARENT_T:
		case QPSNodeType::USES:
		case QPSNodeType::MODIFIES:
		case QPSNodeType::CALLS:
		case QPSNodeType::CALLS_T:
		case QPSNodeType::NEXT:
		case QPSNodeType::NEXT_T:
		case QPSNodeType::AFFECTS:
		case QPSNodeType::AFFECTS_T:
			return true;
		default:
			return false;
	}
}

bool QPSTree::isPatternNode(const QPSNodeType& type) {
	return type == QPSNodeType::PATTERN;
}

bool QPSTree::isWithNode(const QPSNodeType& type) {
	return type == QPSNodeType::WITH;
}

bool QPSTree::checkIfSynonymExist(const string& synonymValue) const {
	return synonymMap.count(synonymValue);
}

bool QPSTree::checkIfSynonymsIsSelection(const string& synonymValue) const {
	return selectionMap.count(synonymValue);
}

bool QPSTree::getIsNonTargetSelection() const {
	if (isBooleanSelection) {
		return true;
	}

	for (const string& selectionSyn : selectionVector) {
		if (synonymMap.count(selectionSyn)) {
			return false;
		}
	}

	return true;
}

bool QPSTree::getIsSingleClause() const {
	bool isSelectionOnly = resultNode->getSize() == 1 && clauseNode->getSize() == 0 && patternNode->getSize() == 0 && withNode->getSize() == 0;
	bool isSuchThatOnly = resultNode->getSize() == 1 && clauseNode->getSize() == 1 && patternNode->getSize() == 0 && withNode->getSize() == 0;
	bool isPatternOnly = resultNode->getSize() == 1 && clauseNode->getSize() == 0 && patternNode->getSize() == 1 && withNode->getSize() == 0;
	bool isWithOnly = resultNode->getSize() == 1 && clauseNode->getSize() == 0 && patternNode->getSize() == 0 && withNode->getSize() == 1;
	bool isSingleClause = isSelectionOnly || isSuchThatOnly || isPatternOnly || isWithOnly;
	return isSingleClause;
}

bool QPSTree::getIsBooleanSelection() const {
	return isBooleanSelection;
}
