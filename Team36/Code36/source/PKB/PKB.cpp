#include <stdio.h>
#include <iostream>
#include <string>
#include <optional>
#include <functional>
#include <queue>
#include <stack>

using namespace std;

#include "PKB.h"

StmtVarMapTable PKB::usesTable = StmtVarMapTable();
StmtVarMapTable PKB::modifiesTable = StmtVarMapTable();
StmtRsTable PKB::followsTable = StmtRsTable();
StmtRsTable PKB::parentTable = StmtRsTable();
StmtRsTable PKB::callsTable = StmtRsTable();
StmtRsTable PKB::nextTable = StmtRsTable();
AffectsTable PKB::affectsTable = AffectsTable();
PatternTable PKB::patternTable = PatternTable();
unordered_map<NodeType, vector<StmtData>> PKB::allStmtMap = unordered_map<NodeType, vector<StmtData>>();
unordered_map<NodeType, unordered_map<string, string>> PKB::callReadPrintMap = unordered_map<NodeType, unordered_map<string, string>>();
unordered_set<int> PKB::allStmtNos = unordered_set<int>();
Cache PKB::cache = Cache();

/*
General
*/
void PKB::startQuery() {
	cache.clear();
}

unordered_map<NodeType, vector<StmtData>> PKB::getAllStmts() {
	return allStmtMap;
}

unordered_set<int> PKB::getAllStmtNos() {
	return allStmtNos;
}

unordered_map<NodeType, unordered_map<string, string>> PKB::getCallReadPrintMap() {
	return callReadPrintMap;
}

void PKB::clear() {
	PKB::usesTable.clearAllEntries();
	PKB::modifiesTable.clearAllEntries();
	PKB::followsTable.clearAllEntries();
	PKB::parentTable.clearAllEntries();
	PKB::nextTable.clearAllEntries();
	PKB::affectsTable.clearAllEntries();
	PKB::patternTable.clearAllEntries();
	PKB::allStmtMap.clear();
	PKB::allStmtNos.clear();
}

/*
Uses
*/
bool PKB::checkUses(int stmtNo, string varName) {
	return usesTable.checkEntry(stmtNo, varName);
}

bool PKB::checkUses(string procName, string varName) {
	return usesTable.checkEntry(procName, varName);
}

bool PKB::checkUses(NodeType type, string varName) {
	return usesTable.checkEntry(type, varName);
}

bool PKB::checkUses(int stmtNo) {
	return usesTable.checkEntry(stmtNo);
}

bool PKB::checkUses(string procName) {
	return usesTable.checkEntry(procName);
}

vector<StmtData> PKB::getUsesByStmtNo(int stmtNo) {
	return usesTable.getEntryByStmtNo(stmtNo);
}

vector<StmtData> PKB::getUsesByType(NodeType type) {
	return usesTable.getEntryByType(type);
}

vector<StmtData> PKB::getUsesByProcName(string procName) {
	return usesTable.getEntryByProcName(procName);
}

vector<StmtData> PKB::getUsesByStmtNoAndType(int stmtNo, NodeType type) {
	return usesTable.getEntryByStmtNoAndType(stmtNo, type);
}

vector<StmtData> PKB::getUsesByTypeAndVarName(NodeType type, string ident) {
	return usesTable.getEntryByTypeAndVarName(type, ident);
}

vector<StmtData> PKB::getUsesByProcNameAndType(string procName, NodeType type) {
	return usesTable.getEntryByProcNameAndType(procName, type);
}

/*
Modifies
*/
bool PKB::checkModifies(int stmtNo, string varName) {
	return modifiesTable.checkEntry(stmtNo, varName);
}

bool PKB::checkModifies(string procName, string varName) {
	return modifiesTable.checkEntry(procName, varName);
}

bool PKB::checkModifies(NodeType type, string varName) {
	return modifiesTable.checkEntry(type, varName);
}

bool PKB::checkModifies(int stmtNo) {
	return modifiesTable.checkEntry(stmtNo);
}

bool PKB::checkModifies(string procName) {
	return modifiesTable.checkEntry(procName);
}

vector<StmtData> PKB::getModifiesByStmtNo(int stmtNo) {
	return modifiesTable.getEntryByStmtNo(stmtNo);
}

vector<StmtData> PKB::getModifiesByType(NodeType type) {
	return modifiesTable.getEntryByType(type);
}

vector<StmtData> PKB::getModifiesByProcName(string procName) {
	return modifiesTable.getEntryByProcName(procName);
}

vector<StmtData> PKB::getModifiesByStmtNoAndType(int stmtNo, NodeType type) {
	return modifiesTable.getEntryByStmtNoAndType(stmtNo, type);
}

vector<StmtData> PKB::getModifiesByTypeAndVarName(NodeType type, string ident) {
	return modifiesTable.getEntryByTypeAndVarName(type, ident);
}

vector<StmtData> PKB::getModifiesByProcNameAndType(string procName, NodeType type) {
	return modifiesTable.getEntryByProcNameAndType(procName, type);
}

/*
Follows
*/
bool PKB::checkFollows(int beforeStmtNo, int afterStmtNo) {
	return followsTable.checkStmtRs(beforeStmtNo, afterStmtNo);
}

bool PKB::checkFollowsTrans(int beforeStmtNo, int afterStmtNo) {
	return followsTable.checkStmtRsTrans(beforeStmtNo, afterStmtNo);
}

bool PKB::checkIfFollowsExists() {
	return followsTable.checkIfExists();
}

vector<pair<StmtData, StmtData>> PKB::getFollows(NodeType beforeType, NodeType afterType) {
	return followsTable.getStmt(beforeType, afterType);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsTrans(NodeType beforeType, NodeType afterType) {
	return followsTable.getStmtTrans(beforeType, afterType);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsBefore(int stmtNo) {
	return followsTable.getStmtBefore(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsBefore(NodeType type) {
	return followsTable.getStmtBefore(type);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsBeforeTrans(int stmtNo) {
	return followsTable.getStmtBeforeTrans(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsBeforeTrans(NodeType type) {
	return followsTable.getStmtBeforeTrans(type);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsAfter(int stmtNo) {
	return followsTable.getStmtAfter(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsAfter(NodeType type) {
	return followsTable.getStmtAfter(type);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsAfterTrans(int stmtNo) {
	return followsTable.getStmtAfterTrans(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getFollowsAfterTrans(NodeType type) {
	return followsTable.getStmtAfterTrans(type);
}

/*
Parent
*/
bool PKB::checkParent(int beforeStmtNo, int afterStmtNo) {
	return parentTable.checkStmtRs(beforeStmtNo, afterStmtNo);
}

bool PKB::checkParentTrans(int beforeStmtNo, int afterStmtNo) {
	return parentTable.checkStmtRsTrans(beforeStmtNo, afterStmtNo);
}

bool PKB::checkIfParentExists() {
	return parentTable.checkIfExists();
}

vector<pair<StmtData, StmtData>> PKB::getParent(NodeType beforeType, NodeType afterType) {
	return parentTable.getStmt(beforeType, afterType);
}

vector<pair<StmtData, StmtData>> PKB::getParentTrans(NodeType beforeType, NodeType afterType) {
	return parentTable.getStmtTrans(beforeType, afterType);
}

vector<pair<StmtData, StmtData>> PKB::getParentBefore(int stmtNo) {
	return parentTable.getStmtBefore(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getParentBefore(NodeType type) {
	return parentTable.getStmtBefore(type);
}

vector<pair<StmtData, StmtData>> PKB::getParentBeforeTrans(int stmtNo) {
	return parentTable.getStmtBeforeTrans(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getParentBeforeTrans(NodeType type) {
	return parentTable.getStmtBeforeTrans(type);
}

vector<pair<StmtData, StmtData>> PKB::getParentAfter(int stmtNo) {
	return parentTable.getStmtAfter(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getParentAfter(NodeType type) {
	return parentTable.getStmtAfter(type);
}

vector<pair<StmtData, StmtData>> PKB::getParentAfterTrans(int stmtNo) {
	return parentTable.getStmtAfterTrans(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getParentAfterTrans(NodeType type) {
	return parentTable.getStmtAfterTrans(type);
}

/*
Calls
*/
bool PKB::checkCalls(string caller, string callee) {
	return callsTable.checkStmtRs(caller, callee);
}

bool PKB::checkCallsTrans(string caller, string callee) {
	return callsTable.checkStmtRsTrans(caller, callee);
}

bool PKB::checkIfCallsExists() {
	return callsTable.checkIfExists();
}

vector<pair<StmtData, StmtData>> PKB::getAllCalls() {
	return callsTable.getAllStmt();
}

vector<pair<StmtData, StmtData>> PKB::getAllCallsTrans() {
	return callsTable.getAllStmtTrans();
}

vector<pair<StmtData, StmtData>> PKB::getCallsBefore(string callName) {
	return callsTable.getStmtBefore(callName);
}

vector<pair<StmtData, StmtData>> PKB::getCallsBeforeTrans(string callName) {
	return callsTable.getStmtBeforeTrans(callName);
}

vector<pair<StmtData, StmtData>> PKB::getCallsAfter(string callName) {
	return callsTable.getStmtAfter(callName);
}

vector<pair<StmtData, StmtData>> PKB::getCallsAfterTrans(string callName) {
	return callsTable.getStmtAfterTrans(callName);
}


/*
Next
*/
bool PKB::checkNext(int beforeStmtNo, int afterStmtNo) {
	return nextTable.checkStmtRs(beforeStmtNo, afterStmtNo);
}

bool PKB::checkNextTrans(int beforeStmtNo, int afterStmtNo) {
	return nextTable.checkStmtRsTrans(beforeStmtNo, afterStmtNo);
}

bool PKB::checkIfNextExists() {
	return nextTable.checkIfExists();
}

vector<pair<StmtData, StmtData>> PKB::getNext(NodeType beforeType, NodeType afterType) {
	return nextTable.getStmt(beforeType, afterType);
}

vector<pair<StmtData, StmtData>> PKB::getNextTrans(NodeType beforeType, NodeType afterType) {
	return nextTable.getStmtTrans(beforeType, afterType);
}

vector<pair<StmtData, StmtData>> PKB::getNextBefore(int stmtNo) {
	return nextTable.getStmtBefore(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getNextBefore(NodeType type) {
	return nextTable.getStmtBefore(type);
}

vector<pair<StmtData, StmtData>> PKB::getNextBeforeTrans(int stmtNo) {
	return nextTable.getStmtBeforeTrans(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getNextBeforeTrans(NodeType type) {
	return nextTable.getStmtBeforeTrans(type);
}

vector<pair<StmtData, StmtData>> PKB::getNextAfter(int stmtNo) {
	return nextTable.getStmtAfter(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getNextAfter(NodeType type) {
	return nextTable.getStmtAfter(type);
}

vector<pair<StmtData, StmtData>> PKB::getNextAfterTrans(int stmtNo) {
	return nextTable.getStmtAfterTrans(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getNextAfterTrans(NodeType type) {
	return nextTable.getStmtAfterTrans(type);
}


/*
Pattern
*/
vector<StmtData> PKB::getPatternAssignExactMatch(string lhs, TNode* rhs) {
	return patternTable.getPatternAssignExactMatch(lhs, rhs);
}

vector<StmtData> PKB::getPatternAssignPartialMatch(string lhs, TNode* rhs) {
	return patternTable.getPatternAssignPartialMatch(lhs, rhs);
}

vector<StmtData> PKB::getPatternIf() {
	return patternTable.getPatternIf();
}

vector<StmtData> PKB::getPatternIf(string varName) {
	return patternTable.getPatternIf(varName);
}

vector<StmtData> PKB::getPatternWhile() {
	return patternTable.getPatternWhile();
}

vector<StmtData> PKB::getPatternWhile(string varName) {
	return patternTable.getPatternWhile(varName);
}

/*
Affects
*/
bool PKB::checkAffects(int beforeStmtNo, int afterStmtNo) {
	return affectsTable.checkAffects(beforeStmtNo, afterStmtNo);
}

bool PKB::checkAffectsTrans(int beforeStmtNo, int afterStmtNo) {
	return affectsTable.checkAffectsTrans(beforeStmtNo, afterStmtNo);
}

bool PKB::checkIfAffectsExists() {
	return affectsTable.checkIfAffectsExists();
}

vector<pair<StmtData, StmtData>> PKB::getAllAffects() {
	return affectsTable.getAllAffects();
}

vector<pair<StmtData, StmtData>> PKB::getAllAffectsTrans() {
	return affectsTable.getAllAffectsTrans();
}

vector<pair<StmtData, StmtData>> PKB::getAffectsBefore(int stmtNo) {
	return affectsTable.getAffectsBefore(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getAffectsAfter(int stmtNo) {
	return affectsTable.getAffectsAfter(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getAffectsBeforeTrans(int stmtNo) {
	return affectsTable.getAffectsBeforeTrans(stmtNo);
}

vector<pair<StmtData, StmtData>> PKB::getAffectsAfterTrans(int stmtNo) {
	return affectsTable.getAffectsAfterTrans(stmtNo);

}

