#pragma once
#include "../NodeType.h"

class StmtData {
private:
	int stmtNo;
	NodeType type;
	string value;
	string procName;
public:
	StmtData() : stmtNo(-1), type(NodeType::EMPTY), value(""), procName("") {};
	StmtData(int stmtNo, NodeType type, string value) : stmtNo(stmtNo), type(type), value(value), procName("") {};
	StmtData(int stmtNo, NodeType type, string value, string procName) : stmtNo(stmtNo), type(type), value(value), procName(procName) {};
	int getStmtNo() const;
	NodeType getType() const;
	string getValue() const;
	string getProcName() const;
	bool operator==(const StmtData& rhs) const;
};
