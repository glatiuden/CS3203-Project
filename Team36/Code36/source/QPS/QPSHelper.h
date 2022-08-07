#pragma once
#include <string>
#include <unordered_map>
#include "../NodeType.h"

using namespace std;

constexpr auto BOOLEAN_TRUE = "TRUE";
constexpr auto BOOLEAN_FALSE = "FALSE";
constexpr auto BOOLEAN_STR = "BOOLEAN";
constexpr auto WILDCARD_STR = "_";

/**
* Used to identify the type of QPSNode when parsing a PQL query into QPS Tree.
*/
enum class QPSNodeType {
	ROOT, CLAUSE, RESULT,
	FOLLOWS, FOLLOWS_T,
	PARENT, PARENT_T,
	USES,
	MODIFIES,
	PATTERN,
	CALLS, CALLS_T,
	NEXT, NEXT_T,
	AFFECTS, AFFECTS_T,
	SELECTION,
	BOOLEAN,
	WITH,
};

const vector<string> ATTR_NAME = { "procName", "varName", "value", "stmt#" };
const vector<string> DESIGN_ENITITIES = { "stmt", "read", "print", "call", "while", "if", "assign", "variable", "constant", "procedure" };
const vector<NodeType> ALL_SYNONYMS = { NodeType::STMT, NodeType::READ, NodeType::PRINT, NodeType::CALL, NodeType::WHILE, NodeType::IF, NodeType::ASSIGN, NodeType::VARIABLE, NodeType::CONSTANT, NodeType::PROCEDURE };
const vector<NodeType> RELATIONSHIP_SYNONYMS = { NodeType::STMT, NodeType::READ, NodeType::PRINT, NodeType::CALL, NodeType::WHILE, NodeType::IF, NodeType::ASSIGN };
const vector<NodeType> ENTITIES_SYNONYMS = { NodeType::VARIABLE, NodeType::CONSTANT, NodeType::PROCEDURE };
const vector<string> REL_REF = { "Follows", "Parent", "Uses", "Modifies", "Calls", "Next", "Affects" };
const vector<string> REL_REF_STAR = { "Follows", "Parent", "Calls", "Next", "Affects" };
const vector<string> REL_REF_SS = { "Follows", "Parent", "Next", "Affects" };
const vector<string> REL_REF_SE_OR_EE = { "Uses", "Modifies" };
const vector<string> REL_REF_EE = { "Calls" };

const unordered_map<string, QPSNodeType> stringToQPSNodeTypeMap = {
	{"Follows", QPSNodeType::FOLLOWS}, {"Follows*", QPSNodeType::FOLLOWS_T},
	{"Parent", QPSNodeType::PARENT}, {"Parent*", QPSNodeType::PARENT_T},
	{"Uses", QPSNodeType::USES}, {"Modifies", QPSNodeType::MODIFIES},
	{"Pattern", QPSNodeType::PATTERN}, {"Select", QPSNodeType::SELECTION},
	{"Calls", QPSNodeType::CALLS}, {"Calls*", QPSNodeType::CALLS_T},
	{"Next", QPSNodeType::NEXT}, {"Next*", QPSNodeType::NEXT_T},
	{"Affects", QPSNodeType::AFFECTS}, {"Affects*", QPSNodeType::AFFECTS_T},
	{"With", QPSNodeType::WITH}
};

const unordered_map<QPSNodeType, string> QPSNodeTypeToString = {
	{QPSNodeType::FOLLOWS, "Follows"}, {QPSNodeType::FOLLOWS_T, "Follows*"},
	{QPSNodeType::PARENT, "Parent"}, {QPSNodeType::PARENT_T, "Parent*"},
	{QPSNodeType::USES, "Uses"}, {QPSNodeType::MODIFIES, "Modifies"},
	{QPSNodeType::PATTERN, "Pattern"}, {QPSNodeType::SELECTION, "Select"},
	{QPSNodeType::CALLS, "Calls"}, {QPSNodeType::CALLS_T, "Calls*"},
	{QPSNodeType::NEXT, "Next"}, {QPSNodeType::NEXT_T, "Next*"},
	{QPSNodeType::AFFECTS, "Affects"}, {QPSNodeType::AFFECTS_T, "Affects*"},
	{QPSNodeType::WITH, "With"}
};

inline string convertQPSNodeTypetoString(const QPSNodeType& type) {
	return QPSNodeTypeToString.at(type);
}

inline QPSNodeType convertStringtoQPSNodeType(const string& str) {
	return stringToQPSNodeTypeMap.at(str);
}

// Affects and Next allows both left and right arg to be the same
inline bool allowSameArg(const QPSNodeType& type) {
	switch (type) {
		case QPSNodeType::NEXT:
		case QPSNodeType::NEXT_T:
		case QPSNodeType::AFFECTS:
		case QPSNodeType::AFFECTS_T:
			return true;
		default:
			return false;
	}
}

inline bool isSynonym(const NodeType& nodeType) {
	switch (nodeType) {
		case NodeType::STMT:
		case NodeType::READ:
		case NodeType::PRINT:
		case NodeType::WHILE:
		case NodeType::IF:
		case NodeType::ASSIGN:
		case NodeType::VARIABLE:
		case NodeType::CONSTANT:
		case NodeType::PROCEDURE:
		case NodeType::CALL:
			return true;
		default:
			return false;
	}
}

inline bool isStmtSyn(const NodeType& nodeType) {
	switch (nodeType) {
		case NodeType::STMT:
		case NodeType::READ:
		case NodeType::PRINT:
		case NodeType::WHILE:
		case NodeType::IF:
		case NodeType::ASSIGN:
		case NodeType::CALL:
			return true;
		default:
			return false;
	}
}

inline bool isVarNameSyn(const NodeType& nodeType) {
	switch (nodeType) {
		case NodeType::VARIABLE:
		case NodeType::READ:
		case NodeType::PRINT:
			return true;
		default:
			return false;
	}
}

inline bool isProcNameSyn(const NodeType& nodeType) {
	switch (nodeType) {
		case NodeType::CALL:
		case NodeType::PROCEDURE:
			return true;
		default:
			return false;
	}
}

inline bool isValueSyn(const NodeType& nodeType) {
	return nodeType == NodeType::CONSTANT;
}

inline bool isPatternSyn(const NodeType& nodeType) {
	switch (nodeType) {
		case NodeType::ASSIGN:
		case NodeType::IF:
		case NodeType::WHILE:
			return true;
		default:
			return false;
	}
}


inline bool isEmpty(const NodeType& nodeType) {
	return nodeType == NodeType::EMPTY;
}

inline bool checkAttrRefSemantics(const NodeType& synType, const NodeType& attrName) {
	if (isSynonym(synType) && !isEmpty(attrName)) {
		switch (attrName) {
			case NodeType::STMTNUM:
				return isStmtSyn(synType);
			case NodeType::VALUE:
				return isValueSyn(synType);
			case NodeType::VARNAME:
				return isVarNameSyn(synType);
			case NodeType::PROCNAME:
				return isProcNameSyn(synType);
		}
	}
	return true;
}

inline bool isWildcard(const NodeType& nodeType) {
	return nodeType == NodeType::WILDCARD;
}

inline bool isInteger(const NodeType& nodeType) {
	return nodeType == NodeType::INTEGER;
}

inline bool isIdent(const NodeType& nodeType) {
	return nodeType == NodeType::IDENT;
}

inline bool isCallReadPrintSynonym(const NodeType& nodeType) {
	switch (nodeType) {
		case NodeType::CALL:
		case NodeType::READ:
		case NodeType::PRINT:
			return true;
		default:
			return false;
	}
}

inline bool isVarNameOrProcName(const NodeType& attrName) {
	switch (attrName) {
		case NodeType::VARNAME:
		case NodeType::PROCNAME:
			return true;
		default:
			return false;
	}
}

inline bool isSynonymAndInteger(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isSynonym(leftArgType) && isInteger(rightArgType);
}

inline bool isIntegerAndSynonym(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isInteger(leftArgType) && isSynonym(rightArgType);
}

inline bool isIntegerAndInteger(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isInteger(leftArgType) && isInteger(rightArgType);
}

inline bool isSynonymAndSynonym(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isSynonym(leftArgType) && isSynonym(rightArgType);
}

inline bool isSynonymAndIdent(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isSynonym(leftArgType) && isIdent(rightArgType);
}

inline bool isIntegerAndIdent(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isInteger(leftArgType) && isIdent(rightArgType);
}

inline bool isWildcardAndWildcard(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isWildcard(leftArgType) && isWildcard(rightArgType);
}

inline bool isIntegerAndWildcard(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isInteger(leftArgType) && isWildcard(rightArgType);
}

inline bool isWildcardAndInteger(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isWildcard(leftArgType) && isInteger(rightArgType);
}

inline bool isSynonymAndWildcard(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isSynonym(leftArgType) && isWildcard(rightArgType);
}

inline bool isWildcardAndSynonym(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isWildcard(leftArgType) && isSynonym(rightArgType);
}

inline bool isWildcardAndIdent(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isWildcard(leftArgType) && isIdent(rightArgType);
}

inline bool isIdentAndIdent(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isIdent(leftArgType) && isIdent(rightArgType);
}

inline bool isIdentAndSynonym(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isIdent(leftArgType) && isSynonym(rightArgType);
}

inline bool isIdentAndInteger(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isIdent(leftArgType) && isInteger(rightArgType);
}

inline bool isIdentAndWildcard(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isIdent(leftArgType) && isWildcard(rightArgType);
}

inline bool isTrueFalseClause(const NodeType& leftArgType, const NodeType& rightArgType) {
	return isIdentAndWildcard(leftArgType, rightArgType) || isIdentAndInteger(leftArgType, rightArgType)
		|| isIdentAndIdent(leftArgType, rightArgType) || isIntegerAndWildcard(leftArgType, rightArgType)
		|| isIntegerAndInteger(leftArgType, rightArgType) || isIntegerAndIdent(leftArgType, rightArgType)
		|| isWildcardAndInteger(leftArgType, rightArgType) || isWildcardAndIdent(leftArgType, leftArgType)
		|| isWildcardAndWildcard(leftArgType, rightArgType);
}
