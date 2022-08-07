#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <optional>

#include "StmtData.h"

using namespace std;

struct StmtDataHash {
	size_t hashFunction(const string& key) const {
		const long p = 31;
		const double m = 1e9 + 9;
		long long hash_value = 0;
		long long p_pow = 1;
		for (char c : key) {
			hash_value = (hash_value + (c - 'a' + 1) * p_pow) % int(m);
			p_pow = (p_pow * p) % int(m);
		}
		return (size_t)hash_value;
	}
	size_t
		operator()(const StmtData& obj) const {

		size_t result = hash<int>()(obj.getStmtNo())
			^ hash<NodeType>()(obj.getType())
			^ hashFunction(obj.getProcName())
			^ hash<string>()(obj.getValue());

		return result;
	}
};

class StmtVarMapTable {
private:
	unordered_map<int, unordered_set<StmtData, StmtDataHash>> stmtNoMap;
	unordered_map<NodeType, unordered_set<StmtData, StmtDataHash>> typeMap;
	unordered_map<string, unordered_set<StmtData, StmtDataHash>> procNameMap;
	bool StmtVarMapTable::checkEntry(const unordered_set<StmtData, StmtDataHash>& set, string varName);
	vector<StmtData> getAllEntries(const optional<string>& varName);
public:
	void insertEntry(const StmtData& data);
	vector<StmtData> getEntryByStmtNo(int stmtNo);
	vector<StmtData> getEntryByStmtNoAndType(int stmtNo, NodeType type);
	vector<StmtData> getEntryByType(NodeType type);
	vector<StmtData> getEntryByTypeAndVarName(NodeType type, string varName);
	vector<StmtData> getEntryByProcName(string procName);
	vector<StmtData> getEntryByProcNameAndType(string procName, NodeType type);
	bool checkEntry(int stmtNo, string varName);
	bool checkEntry(int stmtNo);
	bool checkEntry(NodeType type, string varName);
	bool checkEntry(string procName, string varName);
	bool checkEntry(string procName);
	bool checkIfExists();
	void clearAllEntries();
};
