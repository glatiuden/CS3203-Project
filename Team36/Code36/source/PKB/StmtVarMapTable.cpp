#pragma once
#include "StmtVarMapTable.h"

using namespace std;

namespace {
	vector<StmtData> setToVector(const unordered_set<StmtData, StmtDataHash>& set) {
		vector<StmtData> result{};
		result.reserve(set.size());
		result.insert(result.end(), set.begin(), set.end());
		return result;
	}

	bool isFilterRequired(const NodeType type) {
		return type != NodeType::EMPTY && type != NodeType::WILDCARD && type != NodeType::STMT && type != NodeType::VARIABLE && type != NodeType::INTEGER;
	}

	vector<StmtData> setToVector(const unordered_set<StmtData, StmtDataHash>& set, const NodeType type) {
		if (!isFilterRequired(type)) {
			return setToVector(set);
		}

		vector<StmtData> result{};
		result.reserve(set.size());
		for (const StmtData& sd : set) {
			if (sd.getType() == type) {
				result.push_back(sd);
			}
		}
		return result;
	}

	vector<StmtData> setToVector(const unordered_set<StmtData, StmtDataHash>& set, const string& varName) {
		vector<StmtData> result{};
		result.reserve(set.size());
		for (const StmtData& sd : set) {
			if (sd.getValue() == varName) {
				result.push_back(sd);
			}
		}
		return result;
	}
}

void StmtVarMapTable::insertEntry(const StmtData& data) {
	stmtNoMap[data.getStmtNo()].insert(data);
	typeMap[data.getType()].insert(data);
	if (data.getType() == NodeType::PROCEDURE) {
		procNameMap[data.getProcName()].insert(data);
	}
}

vector<StmtData> StmtVarMapTable::getEntryByStmtNo(int stmtNo) {
	if (!stmtNoMap.count(stmtNo)) {
		return {};
	}

	return setToVector(stmtNoMap[stmtNo]);
}

vector<StmtData> StmtVarMapTable::getEntryByStmtNoAndType(int stmtNo, NodeType type) {
	if (!stmtNoMap.count(stmtNo)) {
		return {};
	}

	return setToVector(stmtNoMap[stmtNo], type);
}

vector<StmtData> StmtVarMapTable::getEntryByType(NodeType type) {
	if (type == NodeType::STMT) {
		return getAllEntries(nullopt);
	}

	if (!typeMap.count(type)) {
		return {};
	}

	return setToVector(typeMap[type]);
}

vector<StmtData> StmtVarMapTable::getEntryByTypeAndVarName(NodeType type, string varName) {
	if (type == NodeType::STMT) {
		return getAllEntries(varName);
	}

	if (!typeMap.count(type)) {
		return {};
	}

	return setToVector(typeMap[type], varName);
}

vector<StmtData> StmtVarMapTable::getEntryByProcName(string procName) {
	if (!procNameMap.count(procName)) {
		return {};
	}

	return setToVector(procNameMap[procName]);
}

vector<StmtData> StmtVarMapTable::getEntryByProcNameAndType(string procName, NodeType type) {
	if (!procNameMap.count(procName)) {
		return {};
	}

	return setToVector(procNameMap[procName], type);
}

bool StmtVarMapTable::checkEntry(const unordered_set<StmtData, StmtDataHash>& set, string varName) {
	for (StmtData varData : set) {
		if (varData.getValue() == varName) {
			return true;
		}
	}

	return false;
}

bool StmtVarMapTable::checkEntry(int stmtNo, string varName) {
	if (!stmtNoMap.count(stmtNo)) {
		return false;
	}

	return checkEntry(stmtNoMap[stmtNo], varName);
}

bool StmtVarMapTable::checkEntry(int stmtNo) {
	return stmtNoMap.count(stmtNo);
}

bool StmtVarMapTable::checkEntry(NodeType type, string varName) {
	if (!typeMap.count(type)) {
		return false;
	}

	return checkEntry(typeMap[type], varName);
}

bool StmtVarMapTable::checkEntry(string procName, string varName) {
	if (!procNameMap.count(procName)) {
		return false;
	}

	return checkEntry(procNameMap[procName], varName);
}

bool StmtVarMapTable::checkEntry(string procName) {
	return procNameMap.count(procName);
}

vector<StmtData> StmtVarMapTable::getAllEntries(const optional<string>& varName) {
	bool hasVarName = varName.has_value();
	vector<StmtData> result{};
	for (auto& [type, stmtDatas] : typeMap) {
		for (StmtData data : stmtDatas) {
			if (hasVarName && varName.value() != data.getValue()) {
				continue;
			}
			result.push_back(data);
		}
	}
	return result;
}

bool StmtVarMapTable::checkIfExists() {
	return !stmtNoMap.empty() || !typeMap.empty() || !procNameMap.empty();
}

void StmtVarMapTable::clearAllEntries() {
	stmtNoMap.clear();
	typeMap.clear();
	procNameMap.clear();
}
