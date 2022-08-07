#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <optional>
#include "StmtVarMapTable.h"
#include "StmtRsTable.h"
#include "AffectsTable.h"
#include "PatternTable.h"
#include "StmtData.h"
#include "../TNode.h"
#include "Cache.h"

using namespace std;

class VarTable;

class Var;

class PKB {
public:
	static StmtVarMapTable usesTable;
	static StmtVarMapTable modifiesTable;
	static StmtRsTable followsTable;
	static StmtRsTable parentTable;
	static StmtRsTable nextTable;
	static StmtRsTable callsTable;
	static AffectsTable affectsTable;
	static PatternTable patternTable;
	static unordered_map<NodeType, vector<StmtData>> allStmtMap;
	static unordered_map<NodeType, unordered_map<string, string>> callReadPrintMap;
	static unordered_set<int> allStmtNos;
	static Cache cache;

	// General
	static void PKB::startQuery();
	static unordered_map<NodeType, vector<StmtData>> PKB::getAllStmts();
	static unordered_set<int> getAllStmtNos();
	static unordered_map<NodeType, unordered_map<string, string>> getCallReadPrintMap();
	static void clear();

	// Uses
	static bool checkUses(int stmtNo, string varName);
	static bool checkUses(string procName, string varName);
	static bool checkUses(NodeType type, string varName);
	static bool checkUses(int stmtNo);
	static bool checkUses(string procName);
	static vector<StmtData> getUsesByStmtNo(int stmtNo);
	static vector<StmtData> getUsesByType(NodeType type);
	static vector<StmtData> getUsesByProcName(string procName);
	static vector<StmtData> getUsesByStmtNoAndType(int stmtNo, NodeType type);
	static vector<StmtData> getUsesByTypeAndVarName(NodeType type, string ident);
	static vector<StmtData> getUsesByProcNameAndType(string procName, NodeType type);

	// Modifies
	static bool checkModifies(int stmtNo, string varName);
	static bool checkModifies(string procName, string varName);
	static bool checkModifies(NodeType type, string varName);
	static bool checkModifies(int stmtNo);
	static bool checkModifies(string procName);
	static vector<StmtData> getModifiesByStmtNo(int stmtNo);
	static vector<StmtData> getModifiesByType(NodeType type);
	static vector<StmtData> getModifiesByProcName(string procName);
	static vector<StmtData> getModifiesByStmtNoAndType(int stmtNo, NodeType type);
	static vector<StmtData> getModifiesByTypeAndVarName(NodeType type, string ident);
	static vector<StmtData> getModifiesByProcNameAndType(string procName, NodeType type);

	// Follows
	static bool checkFollows(int beforeStmtNo, int afterStmtNo);
	static bool checkFollowsTrans(int beforeStmtNo, int afterStmtNo);
	static bool checkIfFollowsExists();
	static vector<pair<StmtData, StmtData>> getFollows(NodeType beforeType, NodeType afterType);
	static vector<pair<StmtData, StmtData>> getFollowsTrans(NodeType beforeType, NodeType afterType);
	static vector<pair<StmtData, StmtData>> getFollowsBefore(int stmtNo);
	static vector<pair<StmtData, StmtData>> getFollowsBefore(NodeType type);
	static vector<pair<StmtData, StmtData>> getFollowsBeforeTrans(int stmtNo);
	static vector<pair<StmtData, StmtData>> getFollowsBeforeTrans(NodeType type);
	static vector<pair<StmtData, StmtData>> getFollowsAfter(int stmtNo);
	static vector<pair<StmtData, StmtData>> getFollowsAfter(NodeType type);
	static vector<pair<StmtData, StmtData>> getFollowsAfterTrans(int stmtNo);
	static vector<pair<StmtData, StmtData>> getFollowsAfterTrans(NodeType type);

	// Parent
	static bool checkParent(int beforeStmtNo, int afterStmtNo);
	static bool checkParentTrans(int beforeStmtNo, int afterStmtNo);
	static bool checkIfParentExists();
	static vector<pair<StmtData, StmtData>> getParent(NodeType beforeType, NodeType afterType);
	static vector<pair<StmtData, StmtData>> getParentTrans(NodeType beforeType, NodeType afterType);
	static vector<pair<StmtData, StmtData>> getParentBefore(int stmtNo);
	static vector<pair<StmtData, StmtData>> getParentBefore(NodeType type);
	static vector<pair<StmtData, StmtData>> getParentBeforeTrans(int stmtNo);
	static vector<pair<StmtData, StmtData>> getParentBeforeTrans(NodeType type);
	static vector<pair<StmtData, StmtData>> getParentAfter(int stmtNo);
	static vector<pair<StmtData, StmtData>> getParentAfter(NodeType type);
	static vector<pair<StmtData, StmtData>> getParentAfterTrans(int stmtNo);
	static vector<pair<StmtData, StmtData>> getParentAfterTrans(NodeType type);

	// Calls
	static bool checkCalls(string caller, string callee);
	static bool checkCallsTrans(string caller, string callee);
	static bool checkIfCallsExists();
	static vector<pair<StmtData, StmtData>> getAllCalls();
	static vector<pair<StmtData, StmtData>> getAllCallsTrans();
	static vector<pair<StmtData, StmtData>> getCallsBefore(string callName);
	static vector<pair<StmtData, StmtData>> getCallsBeforeTrans(string callName);
	static vector<pair<StmtData, StmtData>> getCallsAfter(string callName);
	static vector<pair<StmtData, StmtData>> getCallsAfterTrans(string callName);

	// Pattern
	static vector<StmtData> getPatternAssignExactMatch(string lhs, TNode* rhs);
	static vector<StmtData> getPatternAssignPartialMatch(string lhs, TNode* rhs);
	static vector<StmtData> getPatternIf();
	static vector<StmtData> getPatternIf(string varName);
	static vector<StmtData> getPatternWhile();
	static vector<StmtData> getPatternWhile(string varName);

	// Next
	static bool checkNext(int beforeStmtNo, int afterStmtNo);
	static bool checkNextTrans(int beforeStmtNo, int afterStmtNo);
	static bool checkIfNextExists();
	static vector<pair<StmtData, StmtData>> getNext(NodeType beforeType, NodeType afterType);
	static vector<pair<StmtData, StmtData>> getNextTrans(NodeType beforeType, NodeType afterType);
	static vector<pair<StmtData, StmtData>> getNextBefore(int stmtNo);
	static vector<pair<StmtData, StmtData>> getNextBefore(NodeType type);
	static vector<pair<StmtData, StmtData>> getNextAfter(int stmtNo);
	static vector<pair<StmtData, StmtData>> getNextAfter(NodeType type);
	static vector<pair<StmtData, StmtData>> getNextBeforeTrans(int stmtNo);
	static vector<pair<StmtData, StmtData>> getNextBeforeTrans(NodeType type);
	static vector<pair<StmtData, StmtData>> getNextAfterTrans(int stmtNo);
	static vector<pair<StmtData, StmtData>> getNextAfterTrans(NodeType type);

	// Affects
	static bool checkAffects(int beforeStmtNo, int afterStmtNo);
	static bool checkAffectsTrans(int beforeStmtNo, int afterStmtNo);
	static bool checkIfAffectsExists();
	static vector<pair<StmtData, StmtData>> getAllAffects();
	static vector<pair<StmtData, StmtData>> getAllAffectsTrans();
	static vector<pair<StmtData, StmtData>> getAffectsBefore(int stmtNo);
	static vector<pair<StmtData, StmtData>> getAffectsAfter(int stmtNo);
	static vector<pair<StmtData, StmtData>> getAffectsBeforeTrans(int stmtNo);
	static vector<pair<StmtData, StmtData>> getAffectsAfterTrans(int stmtNo);
};
