#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

#include "QPSNode.h"
#include "QPSTree.h"
#include "QPSPatternNode.h"

class QPSTree {
public:
	QPSTree();
	void addResultNode(QPSNode* _resultNode);
	void addClauseNode(QPSNode* _clauseNode);
	void addPatternNode(QPSPatternNode* _patternNode);
	void addWithNode(QPSNode* _withNode);
	void addToSynonymMap(const string& synonymValue, const NodeType& synonymNodeType);
	void addToSelectionMap(const string& selectionSyn);
	void setSelectionArgType(ArgNode& argNode);

	QPSNode getRoot() const;
	QPSNode getResultNode() const;
	QPSNode getClauseNode() const;
	QPSPatternNode getPatternNode() const;
	QPSNode getWithNode() const;
	shared_ptr<QPSNode> getClauseNodePointer() const;
	shared_ptr<QPSPatternNode> getPatternNodePointer() const;
	shared_ptr<QPSNode> getWithNodePointer() const;

	bool checkIfSynonymExist(const string& synonymValue) const;
	bool checkIfSynonymsIsSelection(const string& synonymValue) const;
	bool getIsBooleanSelection() const;
	bool getIsNonTargetSelection() const;
	bool getIsSingleClause() const;

	void printTree();
	void clearTree();
private:
	QPSNode* rootNode{};
	QPSNode* resultNode{};
	QPSNode* clauseNode{};
	QPSPatternNode* patternNode{};
	QPSNode* withNode{};
	unordered_set<string> synonymMap{};
	unordered_set<string> selectionMap{};
	vector<string> selectionVector{};
	void linkNode(QPSNode* parent_node, QPSNode* child_node);
	void linkPatternNode(QPSPatternNode* parent_node, QPSPatternNode* child_node);
	bool isResultNode(const QPSNodeType& type);
	bool isClauseNode(const QPSNodeType& type);
	bool isPatternNode(const QPSNodeType& type);
	bool isWithNode(const QPSNodeType& type);
	bool isNonTargetSelection = false;
	bool isBooleanSelection = false;
};
