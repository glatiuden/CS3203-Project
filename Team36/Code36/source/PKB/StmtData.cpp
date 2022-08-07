#include "StmtData.h"
#include "../NodeType.h"

int StmtData::getStmtNo() const {
	return stmtNo;
}

NodeType StmtData::getType() const {
	return type;
}

string StmtData::getValue() const {
	return value;
}

string StmtData::getProcName() const {
	return procName;
}

bool StmtData::operator==(const StmtData& rhs) const {
	if (stmtNo == rhs.stmtNo && type == rhs.type && value == rhs.value) {
		return true;
	}
	return false;
}

