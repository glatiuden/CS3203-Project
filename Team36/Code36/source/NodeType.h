#pragma once
#include <string>
#include <unordered_map>

using namespace std;

/**
 * Used to identify the type of arguments.
 */
enum class NodeType {
	VARIABLE,
	CONSTANT,
	PLUS,
	MINUS,
	TIMES,
	DIV,
	MOD,
	NOT,
	AND,
	OR,
	GREATER,
	GREATER_EQ,
	LESS,
	LESS_EQ,
	DBL_EQUAL,
	NOT_EQUAL,
	EXPR,
	ASSIGN,
	STMT_LST,
	IF,
	WHILE,
	LEFT_CURLY,
	RIGHT_CURLY,
	READ,
	PRINT,
	CALL,
	PROGRAM,
	STMT,
	PROCEDURE,
	IDENT, // Used to identify entRef
	INTEGER, // Used to identify stmtRef
	PATTERN, // Used to identify expression-spec
	WILDCARD, // Used to identify wildcard ("_")
	PROCNAME, // Used to identify synonym.procName
	VARNAME, // Used to identify synonym.varName
	VALUE, // Used to identify synonym.value
	STMTNUM, // Used to identify synonym.stmt#
	EMPTY,
	BOOLEAN
};

const unordered_map<string, NodeType> stringToNodeTypeMap = {
	{"stmt", NodeType::STMT}, {"read", NodeType::READ},
	{"print", NodeType::PRINT}, {"while", NodeType::WHILE},
	{"if", NodeType::IF}, {"assign", NodeType::ASSIGN},
	{"variable", NodeType::VARIABLE}, {"constant", NodeType::CONSTANT},
	{"procedure", NodeType::PROCEDURE}, {"ident", NodeType::IDENT},
	{"integer", NodeType::INTEGER}, {"pattern", NodeType::PATTERN},
	{"_", NodeType::WILDCARD}, {"call", NodeType::CALL},
	{"procName", NodeType::PROCNAME}, {"varName", NodeType::VARNAME},
	{"value", NodeType::VALUE}, {"stmt#", NodeType::STMTNUM},
};

inline NodeType convertStringToNodeType(const string& typeString) {
	return stringToNodeTypeMap.at(typeString);
}

const unordered_map<NodeType, string> attrNameToString = {
	{NodeType::PROCNAME, "procName"}, {NodeType::VARNAME, "varName"},
	{NodeType::VALUE, "value"}, {NodeType::STMTNUM, "stmt#"}, {NodeType::EMPTY, ""},
};

inline string convertAttrNameToString(const NodeType& type) {
	return attrNameToString.at(type);
}
