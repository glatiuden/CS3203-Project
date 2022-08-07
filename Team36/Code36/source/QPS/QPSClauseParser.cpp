#include "../SP/Parser.h"
#include "../CustomException.h"
#include "QPSClauseParser.h"

void QPSClauseParser::validateSemanticsAndParseTree(QPSTree& qpsTree) {
	if (isSemanticError) {
		throw QPSClauseParserException("[QPSClauseParser] Semantic Error encountered while parsing the query", isBoolean);
	}

	parseSuchThatNodes(qpsTree);
	parsePatternNodes(qpsTree);
	parseWithNodes(qpsTree);
	parseSelectionNodes(qpsTree);
}

NodeType QPSClauseParser::getArgNodeTypeFromDeclarationMap(const DeclarationSynonym& declarationSynonym) {
	try {
		return declarationMap.at(declarationSynonym);
	} catch (const exception&) {
		if (declarationSynonym == BOOLEAN_STR) {
			isBoolean = true;
			return NodeType::BOOLEAN;
		} else {
			setIsSemanticErrorTrue();
			return NodeType::EMPTY;
		}
	}
}

void QPSClauseParser::addSelectionDeclaration(const DeclarationSynonym& declarationSynonym, const NodeType& declarationNodeType) {
	if (!declarationMap.empty() && declarationMap.count(declarationSynonym)) {
		setIsSemanticErrorTrue();
		return; // Do not override
	}
	declarationMap[declarationSynonym] = declarationNodeType;
}

void QPSClauseParser::addSelectionAttrName(const DeclarationSynonym& declarationSynonym, const NodeType& attrName) {
	selectionSynonyms.push_back(make_pair(declarationSynonym, attrName));
}

void QPSClauseParser::addSuchThatNodeType(const IterationCount& index, const QPSNodeType& qpsNodeType) {
	suchThatNodeTypeMap[index] = qpsNodeType;
}

void QPSClauseParser::addSuchThatNodeArgument(const IterationCount& index, const Direction& direction, const NodePair& value) {
	suchThatNodeArgumentsMap[index][direction] = value;
}

void QPSClauseParser::addPatternNodeArgument(const IterationCount& index, const Direction& direction, const NodePair& value) {
	patternNodeArgumentsMap[index][direction] = value;
}

void QPSClauseParser::addPatternExpression(const IterationCount& index, const PatternExpPair& value) {
	patternExpressionsMap[index] = value;
}

void QPSClauseParser::addWithNodeArgument(const IterationCount& index, const Direction& direction, const NodeTuple& value) {
	withNodeArgumentsMap[index][direction] = value;
}

void QPSClauseParser::setIsSemanticErrorTrue() {
	isSemanticError = true;
}

bool QPSClauseParser::getIsSemanticError() {
	return isSemanticError;
}

void QPSClauseParser::clear() {
	declarationMap.clear();
	suchThatNodeTypeMap.clear();
	selectionSynonyms.clear();
	suchThatNodeArgumentsMap.clear();
	patternNodeArgumentsMap.clear();
	patternExpressionsMap.clear();
	withNodeArgumentsMap.clear();
}

void QPSClauseParser::parseSuchThatNodes(QPSTree& qpsTree) {
	for (const auto& [index, nodeData] : suchThatNodeArgumentsMap) {
		const auto& [leftNodeType, leftNodeValue] = nodeData.at(LEFT_ARG);
		ArgNode leftArgNode(leftNodeType, leftNodeValue);

		const auto& [rightNodeType, rightNodeValue] = nodeData.at(RIGHT_ARG);
		ArgNode rightArgNode(rightNodeType, rightNodeValue);

		QPSNodeType suchThatNodeType = suchThatNodeTypeMap[index];
		validateSuchThatNode(suchThatNodeType, leftNodeType, rightNodeType);
		if (isSynonymAndSynonym(leftNodeType, rightNodeType)) {
			validateIsRecursiveNode(suchThatNodeType, leftNodeValue, rightNodeValue);
		}

		QPSNode* suchThatNode = new QPSNode(suchThatNodeType, leftArgNode, rightArgNode);
		qpsTree.addClauseNode(suchThatNode);
		qpsTree.addToSynonymMap(leftNodeValue, leftNodeType);
		qpsTree.addToSynonymMap(rightNodeValue, rightNodeType);
	}
}

void QPSClauseParser::validateSuchThatNode(const QPSNodeType& qpsNodeType, const NodeType& leftNodeType, const NodeType& rightNodeType) {
	switch (qpsNodeType) {
		case QPSNodeType::MODIFIES:
		case QPSNodeType::USES:
			validateModifiesUsesNode(leftNodeType, rightNodeType);
			break;
		case QPSNodeType::CALLS:
		case QPSNodeType::CALLS_T:
			validateCallsNode(leftNodeType, rightNodeType);
			break;
		case QPSNodeType::FOLLOWS:
		case QPSNodeType::FOLLOWS_T:
		case QPSNodeType::PARENT:
		case QPSNodeType::PARENT_T:
		case QPSNodeType::NEXT:
		case QPSNodeType::NEXT_T:
			validateFollowsParentNextNode(leftNodeType, rightNodeType);
			break;
		case QPSNodeType::AFFECTS:
		case QPSNodeType::AFFECTS_T:
			validateAffectsNode(leftNodeType, rightNodeType);
			break;
	}
}

void QPSClauseParser::validateModifiesUsesNode(const NodeType& leftNodeType, const NodeType& rightNodeType) {
	// Invalid: (v, v1), (SYN, s1)
	bool isBothVars = leftNodeType == NodeType::VARIABLE && rightNodeType == NodeType::VARIABLE;
	bool isRightNotVar = isSynonym(rightNodeType) && rightNodeType != NodeType::VARIABLE;
	bool isLeftWildcard = isWildcard(leftNodeType);
	if (isBothVars || isRightNotVar || isLeftWildcard) {
		throw QPSClauseParserException("[QPSClauseParser] Invalid SUCH THAT modifies/uses clause", isBoolean);
	}
}

void QPSClauseParser::validateCallsNode(const NodeType& leftNodeType, const NodeType& rightNodeType) {
	bool isLeftNotProc = isSynonym(leftNodeType) && leftNodeType != NodeType::PROCEDURE;
	bool isRightNotProc = isSynonym(rightNodeType) && rightNodeType != NodeType::PROCEDURE;
	if (isLeftNotProc || isRightNotProc) {
		throw QPSClauseParserException("[QPSClauseParser] Invalid SUCH THAT calls clause", isBoolean);
	}
}

void QPSClauseParser::validateIsRecursiveNode(const QPSNodeType& qpsNodeType, const ArgNodeRef& leftArgRef, const ArgNodeRef& rightArgRef) {
	bool isRecursive = leftArgRef == rightArgRef;
	if (isRecursive) {
		if (!allowSameArg(qpsNodeType)) { // Only Next & Affects is allowed
			throw QPSClauseParserException("[QPSClauseParser] Recursion not allowed", isBoolean);
		}
	}
}

void QPSClauseParser::validateFollowsParentNextNode(const NodeType& leftNodeType, const NodeType& rightNodeType) {
	bool isLeftNotStmtSyn = isSynonym(leftNodeType) && !isStmtSyn(leftNodeType);
	bool isRightNotStmtSyn = isSynonym(rightNodeType) && !isStmtSyn(rightNodeType);
	if (isLeftNotStmtSyn || isRightNotStmtSyn) {
		throw QPSClauseParserException("[QPSClauseParser] Invalid SUCH THAT follows/parent/next clause", isBoolean);
	}
}

void QPSClauseParser::validateAffectsNode(const NodeType& leftNodeType, const NodeType& rightNodeType) {
	bool isLeftNotAssignOrStmt = isSynonym(leftNodeType) && leftNodeType != NodeType::ASSIGN && leftNodeType != NodeType::STMT;
	bool isRightNotAssignOrStmt = isSynonym(rightNodeType) && rightNodeType != NodeType::ASSIGN && rightNodeType != NodeType::STMT;
	if (isLeftNotAssignOrStmt || isRightNotAssignOrStmt) {
		throw QPSClauseParserException("[QPSClauseParser] Invalid SUCH THAT affects clause", isBoolean);
	}
}

void QPSClauseParser::parsePatternNodes(QPSTree& qpsTree) {
	for (const auto& [index, nodeData] : patternNodeArgumentsMap) {
		const NodePair& patternSynPair = nodeData.at(PATTERN_SYN);
		const auto& [synNodeType, synNodeValue] = patternSynPair;
		validateSynArgPatternNode(synNodeType);
		PatternSynArg synNode(synNodeType, synNodeValue);

		const NodePair& leftArgPair = nodeData.at(LEFT_ARG);
		const auto& [leftNodeType, leftNodeValue] = leftArgPair;
		validateLeftArgPatternNode(leftNodeType);
		LeftArg leftArgNode(leftNodeType, leftNodeValue);

		const NodePair& rightArgPair = nodeData.at(RIGHT_ARG);
		const auto& [rightNodeType, rightNodeValue] = rightArgPair;
		validateSemanticArgPatternNode(synNodeType, rightNodeType);
		RightArg rightArgNode(rightNodeType, rightNodeValue);

		QPSPatternNode* patternNode{};
		if (patternExpressionsMap.count(index)) {
			const PatternExpPair& patternExp = patternExpressionsMap[index];
			const auto& [isPM, tokens] = patternExp;
			// Generate AST
			optional<TNode*> expression = Parser().parseExpr(tokens, 0, tokens.size() - 1);
			patternNode = new QPSPatternNode(leftArgNode, synNode, isPM, expression, rightArgNode);
		} else {
			patternNode = new QPSPatternNode(leftArgNode, synNode, rightArgNode);
		}

		qpsTree.addPatternNode(patternNode);
		qpsTree.addToSynonymMap(leftNodeValue, leftNodeType);
		qpsTree.addToSynonymMap(synNodeValue, synNodeType);
	}
}

void QPSClauseParser::validateSynArgPatternNode(const NodeType& patternNodeType) {
	if (!isPatternSyn(patternNodeType)) {
		throw QPSClauseParserException("[QPSClauseParser] Invalid PATTERN clause", isBoolean);
	}
}

void QPSClauseParser::validateLeftArgPatternNode(const NodeType& leftNodeType) {
	if (isSynonym(leftNodeType) && leftNodeType != NodeType::VARIABLE) {
		throw QPSClauseParserException("[QPSClauseParser] Invalid PATTERN clause", isBoolean);
	}
}

void QPSClauseParser::validateSemanticArgPatternNode(const NodeType& patternNodeType, const NodeType& semanticSynNodeType) {
	bool result = false;
	switch (patternNodeType) {
		case NodeType::ASSIGN:
		case NodeType::WHILE:
			result = !isEmpty(semanticSynNodeType);
			break;
		case NodeType::IF:
			result = isWildcard(semanticSynNodeType);
			break;
	}

	if (!result) {
		throw QPSClauseParserException("[QPSClauseParser] Invalid PATTERN clause", isBoolean);
	}
}

void QPSClauseParser::parseWithNodes(QPSTree& qpsTree) {
	for (const auto& [index, nodeData] : withNodeArgumentsMap) {
		const auto& [leftNodeType, leftNodeValue, leftAttrName] = nodeData.at(LEFT_ARG);
		validateWithNode(leftNodeType, leftAttrName);
		ArgNode leftArgNode(leftNodeType, leftNodeValue, leftAttrName);

		const auto& [rightNodeType, rightNodeValue, rightAttrName] = nodeData.at(RIGHT_ARG);
		validateWithNode(rightNodeType, rightAttrName);
		ArgNode rightArgNode(rightNodeType, rightNodeValue, rightAttrName);

		QPSNode* withNode = new QPSNode(QPSNodeType::WITH, leftArgNode, rightArgNode);
		qpsTree.addWithNode(withNode);
		qpsTree.addToSynonymMap(leftNodeValue, leftNodeType);
		qpsTree.addToSynonymMap(rightNodeValue, rightNodeType);
	}
}

void QPSClauseParser::validateWithNode(const NodeType& nodeType, const NodeType& attrName) {
	bool isAttrRefValid = checkAttrRefSemantics(nodeType, attrName);
	if (isWildcard(nodeType) || !isAttrRefValid) {
		throw QPSClauseParserException("[QPSClauseParser] WITH clause contains invalid attrname", isBoolean);
	}
}

void QPSClauseParser::parseSelectionNodes(QPSTree& qpsTree) {
	for (const pair<DeclarationSynonym, NodeType>& selectionPair : selectionSynonyms) {
		const auto& [selectionSynonym, selectionAttrName] = selectionPair;
		const NodeType& selectionNodeType = getArgNodeTypeFromDeclarationMap(selectionSynonym);

		if (!checkAttrRefSemantics(selectionNodeType, selectionAttrName)) {
			throw QPSClauseParserException("[QPSClauseParser] Selection contains mismatch attrname", isBoolean);
		}

		ArgNode selectionArgNode(selectionNodeType, selectionSynonym, selectionAttrName);
		QPSNode* selectionNode = new QPSNode(QPSNodeType::SELECTION, selectionArgNode);
		qpsTree.addResultNode(selectionNode);
	}
}
