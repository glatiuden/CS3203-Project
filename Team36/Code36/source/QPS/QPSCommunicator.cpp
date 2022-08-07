#include <functional>
#include <iostream>
#include <algorithm>

#include "QPSCommunicator.h"
#include "QPSFilter.h"

constexpr auto LEFT = 0;
constexpr auto RIGHT = 1;

using namespace std;

// Modifies
bool QPSCommunicator::checkModifies(int lineNo, string varName) {
	return PKB::checkModifies(lineNo, varName);
}

bool QPSCommunicator::checkModifies(NodeType nodeType, string varName) {
	return PKB::checkModifies(nodeType, varName);
}

bool QPSCommunicator::checkModifies(string procName, string varName) {
	return PKB::checkModifies(procName, varName);
}

bool QPSCommunicator::checkModifies(int stmtNo) {
	return PKB::checkModifies(stmtNo);
}

bool QPSCommunicator::checkModifies(string procName) {
	return PKB::checkModifies(procName);
}

PKBSingleResults QPSCommunicator::getModifiesByProcName(string procName) {
	return PKB::getModifiesByProcName(procName);
}

PKBSingleResults QPSCommunicator::getModifiesByStmtNo(int stmtNo) {
	return PKB::getModifiesByStmtNo(stmtNo);
}

PKBSingleResults QPSCommunicator::getModifiesByType(NodeType nodeType) {
	return PKB::getModifiesByType(nodeType);
}

PKBSingleResults QPSCommunicator::getModifiesByProcNameAndType(string procName, NodeType type) {
	return PKB::getModifiesByProcNameAndType(procName, type);
}

PKBSingleResults QPSCommunicator::getModifiesByStmtNoAndType(int stmtNo, NodeType type) {
	return PKB::getModifiesByStmtNoAndType(stmtNo, type);
}

PKBSingleResults QPSCommunicator::getModifiesByTypeAndVarName(NodeType nodeType, string ident) {
	return PKB::getModifiesByTypeAndVarName(nodeType, ident);
}

// Uses
bool QPSCommunicator::checkUses(int lineNo, string varName) {
	return PKB::checkUses(lineNo, varName);
}

bool QPSCommunicator::checkUses(NodeType nodeType, string varName) {
	return PKB::checkUses(nodeType, varName);
}

bool QPSCommunicator::checkUses(string procName, string varName) {
	return PKB::checkUses(procName, varName);
}

bool QPSCommunicator::checkUses(int stmtNo) {
	return PKB::checkUses(stmtNo);
}

bool QPSCommunicator::checkUses(string procName) {
	return PKB::checkUses(procName);
}

PKBSingleResults QPSCommunicator::getUsesByProcName(string procName) {
	return PKB::getUsesByProcName(procName);
}

PKBSingleResults QPSCommunicator::getUsesByType(NodeType nodeType) {
	return PKB::getUsesByType(nodeType);
}

PKBSingleResults QPSCommunicator::getUsesByStmtNo(int stmtNo) {
	return PKB::getUsesByStmtNo(stmtNo);
}

PKBSingleResults QPSCommunicator::getUsesByProcNameAndType(string procName, NodeType type) {
	return PKB::getUsesByProcNameAndType(procName, type);
}

PKBSingleResults QPSCommunicator::getUsesByStmtNoAndType(int stmtNo, NodeType type) {
	return PKB::getUsesByStmtNoAndType(stmtNo, type);
}

PKBSingleResults QPSCommunicator::getUsesByTypeAndVarName(NodeType nodeType, string ident) {
	return PKB::getUsesByTypeAndVarName(nodeType, ident);
}

// Follows
bool QPSCommunicator::checkFollows(int beforeStmtNo, int afterStmtNo) {
	return PKB::checkFollows(beforeStmtNo, afterStmtNo);
}

bool QPSCommunicator::checkFollowsTrans(int beforeStmtNo, int afterStmtNo) {
	return PKB::checkFollowsTrans(beforeStmtNo, afterStmtNo);
}

bool QPSCommunicator::checkIfFollowsExists() {
	return PKB::checkIfFollowsExists();
}

PKBPairResults QPSCommunicator::getFollows(NodeType beforeType, NodeType afterType) {
	return PKB::getFollows(beforeType, afterType);
}

PKBPairResults QPSCommunicator::getFollowsTrans(NodeType beforeType, NodeType afterType) {
	return PKB::getFollowsTrans(beforeType, afterType);
}

PKBPairResults QPSCommunicator::getFollowsBefore(int stmtNo) {
	return PKB::getFollowsBefore(stmtNo);
}

PKBPairResults QPSCommunicator::getFollowsBefore(NodeType type) {
	return PKB::getFollowsBefore(type);
}

PKBPairResults QPSCommunicator::getFollowsBeforeTrans(int stmtNo) {
	return PKB::getFollowsBeforeTrans(stmtNo);
}

PKBPairResults QPSCommunicator::getFollowsBeforeTrans(NodeType type) {
	return PKB::getFollowsBeforeTrans(type);
}

PKBPairResults QPSCommunicator::getFollowsAfter(int stmtNo) {
	return PKB::getFollowsAfter(stmtNo);
}

PKBPairResults QPSCommunicator::getFollowsAfter(NodeType type) {
	return PKB::getFollowsAfter(type);
}

PKBPairResults QPSCommunicator::getFollowsAfterTrans(int stmtNo) {
	return PKB::getFollowsAfterTrans(stmtNo);
}

PKBPairResults QPSCommunicator::getFollowsAfterTrans(NodeType type) {
	return PKB::getFollowsAfterTrans(type);
}

// Parent
bool QPSCommunicator::checkParent(int beforeStmtNo, int afterStmtNo) {
	return PKB::checkParent(beforeStmtNo, afterStmtNo);
}

bool QPSCommunicator::checkParentTrans(int beforeStmtNo, int afterStmtNo) {
	return PKB::checkParentTrans(beforeStmtNo, afterStmtNo);
}

bool QPSCommunicator::checkIfParentExists() {
	return PKB::checkIfParentExists();
}

PKBPairResults QPSCommunicator::getParent(NodeType beforeType, NodeType afterType) {
	return PKB::getParent(beforeType, afterType);
}

PKBPairResults QPSCommunicator::getParentTrans(NodeType beforeType, NodeType afterType) {
	return PKB::getParentTrans(beforeType, afterType);
}

PKBPairResults QPSCommunicator::getParentBefore(int stmtNo) {
	return PKB::getParentBefore(stmtNo);
}

PKBPairResults QPSCommunicator::getParentBefore(NodeType type) {
	return PKB::getParentBefore(type);
}

PKBPairResults QPSCommunicator::getParentBeforeTrans(int stmtNo) {
	return PKB::getParentBeforeTrans(stmtNo);
}

PKBPairResults QPSCommunicator::getParentBeforeTrans(NodeType type) {
	return PKB::getParentBeforeTrans(type);
}

PKBPairResults QPSCommunicator::getParentAfter(int stmtNo) {
	return PKB::getParentAfter(stmtNo);
}

PKBPairResults QPSCommunicator::getParentAfter(NodeType type) {
	return PKB::getParentAfter(type);
}

PKBPairResults QPSCommunicator::getParentAfterTrans(int stmtNo) {
	return PKB::getParentAfterTrans(stmtNo);
}

PKBPairResults QPSCommunicator::getParentAfterTrans(NodeType type) {
	return PKB::getParentAfterTrans(type);
}

// Next
bool QPSCommunicator::checkNext(int beforeStmtNo, int afterStmtNo) {
	return PKB::checkNext(beforeStmtNo, afterStmtNo);
}

bool QPSCommunicator::checkNextTrans(int beforeStmtNo, int afterStmtNo) {
	return PKB::checkNextTrans(beforeStmtNo, afterStmtNo);
}

bool QPSCommunicator::checkIfNextExists() {
	return PKB::checkIfNextExists();
}

PKBPairResults QPSCommunicator::getNext(NodeType beforeType, NodeType afterType) {
	return PKB::getNext(beforeType, afterType);
}

PKBPairResults QPSCommunicator::getNextTrans(NodeType beforeType, NodeType afterType) {
	return PKB::getNextTrans(beforeType, afterType);
}

PKBPairResults QPSCommunicator::getNextBefore(int stmtNo) {
	return PKB::getNextBefore(stmtNo);
}

PKBPairResults QPSCommunicator::getNextBefore(NodeType type) {
	return PKB::getNextBefore(type);
}

PKBPairResults QPSCommunicator::getNextAfter(int stmtNo) {
	return PKB::getNextAfter(stmtNo);
}

PKBPairResults QPSCommunicator::getNextAfter(NodeType type) {
	return PKB::getNextAfter(type);
}

PKBPairResults QPSCommunicator::getNextBeforeTrans(int stmtNo) {
	return PKB::getNextBeforeTrans(stmtNo);
}

PKBPairResults QPSCommunicator::getNextBeforeTrans(NodeType type) {
	return PKB::getNextBeforeTrans(type);
}

PKBPairResults QPSCommunicator::getNextAfterTrans(int stmtNo) {
	return PKB::getNextAfterTrans(stmtNo);
}

PKBPairResults QPSCommunicator::getNextAfterTrans(NodeType type) {
	return PKB::getNextAfterTrans(type);
}

// Calls
bool QPSCommunicator::checkCalls(string caller, string callee) {
	return PKB::checkCalls(caller, callee);
}

bool QPSCommunicator::checkCallsTrans(string caller, string callee) {
	return PKB::checkCallsTrans(caller, callee);
}

bool QPSCommunicator::checkIfCallsExists() {
	return PKB::checkIfCallsExists();
}

PKBPairResults QPSCommunicator::getByCalleesProcName(string procName) {
	return PKB::getCallsAfter(procName);
}

PKBPairResults QPSCommunicator::getByCallersProcName(string procName) {
	return PKB::getCallsBefore(procName);
}

PKBPairResults QPSCommunicator::getByCallersProcNameTrans(string procName) {
	return PKB::getCallsBeforeTrans(procName);
}

PKBPairResults QPSCommunicator::getByCalleesProcNameTrans(string procName) {
	return PKB::getCallsAfterTrans(procName);
}

PKBPairResults QPSCommunicator::getAllCalls() {
	return PKB::getAllCalls();
}

PKBPairResults QPSCommunicator::getAllCallsTrans() {
	return PKB::getAllCallsTrans();
}


// Affects
bool QPSCommunicator::checkIfAffectsExists() {
	return PKB::checkIfAffectsExists();
}
bool QPSCommunicator::checkAffects(int beforeStmtNo, int afterStmtNo) {
	return PKB::checkAffects(beforeStmtNo, afterStmtNo);
}

bool QPSCommunicator::checkAffectsTrans(int beforeStmtNo, int afterStmtNo) {
	return PKB::checkAffectsTrans(beforeStmtNo, afterStmtNo);
}

PKBPairResults QPSCommunicator::getAllAffects() {
	return PKB::getAllAffects();
}

PKBPairResults QPSCommunicator::getAllAffectsTrans() {
	return PKB::getAllAffectsTrans();
}

PKBPairResults QPSCommunicator::getAffectsBefore(int stmtNo) {
	return PKB::getAffectsBefore(stmtNo);
}


PKBPairResults QPSCommunicator::getAffectsBeforeTrans(int stmtNo) {
	return PKB::getAffectsBeforeTrans(stmtNo);
}


PKBPairResults QPSCommunicator::getAffectsAfter(int stmtNo) {
	return PKB::getAffectsAfter(stmtNo);
}

PKBPairResults QPSCommunicator::getAffectsAfterTrans(int stmtNo) {
	return PKB::getAffectsAfterTrans(stmtNo);
}

// Pattern
PKBSingleResults QPSCommunicator::getAssignmentPatternExactMatch(string lhs, TNode* rhs) {
	return PKB::getPatternAssignExactMatch(lhs, rhs);
}

PKBSingleResults QPSCommunicator::getAssignmentPatternPartialMatch(string lhs, TNode* rhs) {
	return PKB::getPatternAssignPartialMatch(lhs, rhs);
}

PKBSingleResults QPSCommunicator::getIfPatternMatch(string varName) {
	return PKB::getPatternIf(varName);
}

PKBSingleResults QPSCommunicator::getWhilePatternMatch(string varName) {
	return PKB::getPatternWhile(varName);
}

PKBSingleResults QPSCommunicator::getAllIfPattern() {
	return PKB::getPatternIf();
}

PKBSingleResults QPSCommunicator::getAllWhilePattern() {
	return PKB::getPatternWhile();
}

PKBPairResults QPSCommunicator::getWith(ArgNode& leftArg, ArgNode& rightArg) {
	NodeType leftArgType = leftArg.getType();
	NodeType rightArgType = rightArg.getType();
	int direction = LEFT; // Assume is LEFT by default, used to determine the map direction to determine the pair order
	bool traverseOuter = false;
	unordered_map<string, PKBSingleResults> map{};
	ArgNode innerArgNode = leftArg; // Assume inner is LEFT
	ArgNode outerArgNode = rightArg; // Assume outer is RIGHT

	if (isSynonymAndSynonym(leftArgType, rightArgType)) {
		traverseOuter = true;
		populateWithMap(map, leftArg);
	} else if (isSynonym(leftArgType)) {
		populateWithMap(map, leftArg);
	} else if (isSynonym(rightArgType)) {
		direction = RIGHT;
		innerArgNode = rightArg;
		outerArgNode = leftArg;
		populateWithMap(map, rightArg);
	}

	if (!traverseOuter) {
		return processOneSideWithClause(map, outerArgNode);
	} else {
		return processTwoSidesWithClause(map, outerArgNode, direction);
	}
}

// Misc
unordered_map<NodeType, vector<StmtData>> QPSCommunicator::getAllStmts() {
	return PKB::getAllStmts();
}

void QPSCommunicator::populateWithMap(unordered_map<string, PKBSingleResults>& map, const ArgNode& argNode) {
	const NodeType& nodeType = argNode.getType();
	const NodeType& attrName = argNode.getAttrName();
	PKBSingleResults innerData = getAllStmtsByType(nodeType);
	map.reserve(innerData.size());
	for (const StmtData& sd : innerData) {
		// Assume is VALUE or VARNAME by default
		string keyValue = sd.getValue();
		if (attrName == NodeType::STMTNUM) {
			keyValue = to_string(sd.getStmtNo());
		} else if (attrName == NodeType::PROCNAME) {
			keyValue = sd.getProcName();
		}
		map[keyValue].push_back(sd);
	}
}

PKBPairResults QPSCommunicator::processOneSideWithClause(unordered_map<string, PKBSingleResults>& map, const ArgNode& argNode) {
	PKBPairResults results{};
	const NodeType& attrName = argNode.getAttrName();
	const string& argRef = argNode.getRef();
	if (map.count(argRef)) {
		const PKBSingleResults& matchedValues = map[argRef];
		for (const StmtData& sd : matchedValues) {
			results.push_back(make_pair(sd, sd));
		}
	}
	return results;
}

PKBPairResults QPSCommunicator::processTwoSidesWithClause(unordered_map<string, PKBSingleResults>& map, const ArgNode& argNode, const int& direction) {
	PKBPairResults results{};
	results.reserve(map.size());
	PKBSingleResults data = getAllStmtsByType(argNode.getType());
	const NodeType& attrName = argNode.getAttrName();
	for (const StmtData& sd : data) {
		// Assume is VALUE or VARNAME by default
		string& keyValue = sd.getValue();
		if (attrName == NodeType::STMTNUM) {
			keyValue = to_string(sd.getStmtNo());
		} else if (attrName == NodeType::PROCNAME) {
			keyValue = sd.getProcName();
		}

		if (map.count(keyValue)) {
			const PKBSingleResults& matchedValues = map[keyValue];
			for (const StmtData& matchSd : matchedValues) {
				if (direction == LEFT) { // by direction
					results.push_back(make_pair(matchSd, sd));
				} else {
					results.push_back(make_pair(sd, matchSd));
				}
			}
		}
	}
	return results;
}

unordered_set<int> QPSCommunicator::getAllStmtNos() {
	return PKB::getAllStmtNos();
}

// Method to retrieve all values
vector<string> QPSCommunicator::getAllResultsByType(NodeType nodeType, NodeType attrName) {
	if (nodeType == NodeType::STMT) {
		unordered_set<int> allStmtNos = QPSCommunicator::getAllStmtNos();
		vector<string> results(allStmtNos.size());
		int i = 0;
		for (const int& stmtNo : allStmtNos) {
			results[i] = to_string(stmtNo);
			i++;
		}
		return results;
	}

	PKBSingleResults nodeTypeResults = QPSCommunicator::getAllStmts()[nodeType];
	NodeType projectionNodeType = QPSFilter::getProjectionNodeType(nodeType, attrName);
	return QPSFilter::getDisplayValues(nodeTypeResults, projectionNodeType);
}

// Wrapper method to allow selection by STMT Type
PKBSingleResults QPSCommunicator::getAllStmtsByType(NodeType nodeType) {
	if (nodeType == NodeType::STMT) {
		return generateStmtData();
	}
	unordered_map<NodeType, vector<StmtData>> allStmts = getAllStmts();
	return allStmts[nodeType];
}

// Construct dummy StmtData with StmtNo for ease of processing
PKBSingleResults QPSCommunicator::generateStmtData() {
	unordered_set<int> allStmtNos = QPSCommunicator::getAllStmtNos();
	PKBSingleResults stmtDatas(allStmtNos.size());
	int i = 0;
	for (const int& stmtNo : allStmtNos) {
		stmtDatas[i] = StmtData(stmtNo, NodeType::STMT, "");
		i++;
	}
	return stmtDatas;
}

unordered_map<NodeType, unordered_map<string, string>> QPSCommunicator::getCallReadPrintMap() {
	return PKB::getCallReadPrintMap();
}
