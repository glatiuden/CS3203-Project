#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "../QPS/QPSTree/QPSTree.h"

constexpr auto PATTERN_SYN = 0;
constexpr auto LEFT_ARG = 1;
constexpr auto RIGHT_ARG = 2;

typedef string DeclarationSynonym;
typedef int Direction; // To determine SYN_ASSIGN, LEFT_ARG or RIGHT_ARG
typedef int IterationCount;
typedef vector<Token> PatternTokens;
typedef pair<NodeType, ArgNodeRef> NodePair;
typedef tuple<NodeType, ArgNodeRef, NodeType> NodeTuple;
typedef pair<PartialMatching, PatternTokens> PatternExpPair;

typedef unordered_map<DeclarationSynonym, NodeType> SelectionNodesMap;
typedef unordered_map<IterationCount, QPSNodeType> AbstractionsMap;
typedef unordered_map<IterationCount, unordered_map<Direction, NodePair>> NodesMap;
typedef unordered_map<IterationCount, pair<PartialMatching, PatternTokens>> PatternExpMap;
typedef unordered_map<IterationCount, unordered_map<Direction, NodeTuple>> WithNodesMap;
typedef vector<pair<DeclarationSynonym, NodeType>> SelectionMap;

class QPSClauseParser {
public:
	void validateSemanticsAndParseTree(QPSTree& qpsTree);
	NodeType getArgNodeTypeFromDeclarationMap(const DeclarationSynonym& declarationSynonym);
	void addSelectionDeclaration(const DeclarationSynonym& declarationSynonym, const NodeType& declarationNodeType);
	void addSelectionAttrName(const DeclarationSynonym& declarationSynonym, const NodeType& attrName);
	void addSuchThatNodeType(const IterationCount& index, const QPSNodeType& qpsNodeType);
	void addSuchThatNodeArgument(const IterationCount& index, const Direction& direction, const NodePair& value);
	void addPatternNodeArgument(const IterationCount& index, const Direction& direction, const NodePair& value);
	void addPatternExpression(const IterationCount& index, const PatternExpPair& value);
	void addWithNodeArgument(const IterationCount& index, const Direction& direction, const NodeTuple& value);
	void setIsSemanticErrorTrue();
	bool getIsSemanticError();
	void clear();
private:
	SelectionNodesMap declarationMap{}; // Storing the declaration of synonyms.
	AbstractionsMap suchThatNodeTypeMap{}; // Storing the abstraction type of the Such That clause
	SelectionMap selectionSynonyms{}; // Storing of Selection Synonyms with AttrName
	NodesMap suchThatNodeArgumentsMap{}; // Storing the arguments of Such That Nodes
	NodesMap patternNodeArgumentsMap{}; // Storing the arguments of Pattern Nodes
	PatternExpMap patternExpressionsMap{}; // Storing the pattern expression if any
	WithNodesMap withNodeArgumentsMap{}; // Storing the arguments of With Nodes
	bool isBoolean = false;
	bool isSemanticError = false;

	void parseSuchThatNodes(QPSTree& qpsTree);
	void validateSuchThatNode(const QPSNodeType& qpsNodeType, const NodeType& leftNodeType, const NodeType& rightNodeType);
	void validateModifiesUsesNode(const NodeType& leftNodeType, const NodeType& rightNodeType);
	void validateCallsNode(const NodeType& leftNodeType, const NodeType& rightNodeType);
	void validateIsRecursiveNode(const QPSNodeType& qpsNodeType, const ArgNodeRef& leftArgRef, const ArgNodeRef& rightArgRef);
	void validateFollowsParentNextNode(const NodeType& leftNodeType, const NodeType& rightNodeType);
	void validateAffectsNode(const NodeType& leftNodeType, const NodeType& rightNodeType);

	void parsePatternNodes(QPSTree& qpsTree);
	void validateSynArgPatternNode(const NodeType& patternNodeType);
	void validateLeftArgPatternNode(const NodeType& leftNodeType);
	void validateSemanticArgPatternNode(const NodeType& patternNodeType, const NodeType& semanticSynNodeType);

	void parseWithNodes(QPSTree& qpsTree);
	void validateWithNode(const NodeType& nodeType, const NodeType& attrName);

	void parseSelectionNodes(QPSTree& qpsTree);
};
