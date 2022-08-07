#pragma once
#include "../PKB/PKB.h"
#include "../PKB/StmtData.h"
#include "QPSTree/QPSNode.h"

using namespace std;

typedef vector<pair<StmtData, StmtData>> PKBPairResults;
typedef vector<StmtData> PKBSingleResults;

class QPSCommunicator {
public:
	static bool checkModifies(int stmtNo, string varName);
	static bool checkModifies(NodeType nodeType, string varName);
	static bool checkModifies(string procName, string varName);
	static bool checkModifies(int stmtNo);
	static bool checkModifies(string procName);
	static PKBSingleResults getModifiesByProcName(string procName);
	static PKBSingleResults getModifiesByStmtNo(int stmtNo);
	static PKBSingleResults getModifiesByType(NodeType nodeType);
	static PKBSingleResults getModifiesByProcNameAndType(string procName, NodeType type);
	static PKBSingleResults getModifiesByStmtNoAndType(int stmtNo, NodeType type);
	static PKBSingleResults getModifiesByTypeAndVarName(NodeType nodeType, string ident);

	static bool checkUses(int stmtNo, string varName);
	static bool checkUses(NodeType nodeType, string varName);
	static bool checkUses(string procName, string varName);
	static bool checkUses(int stmtNo);
	static bool checkUses(string procName);
	static PKBSingleResults getUsesByProcName(string procName);
	static PKBSingleResults getUsesByType(NodeType nodeType);
	static PKBSingleResults getUsesByStmtNo(int stmtNo);
	static PKBSingleResults getUsesByProcNameAndType(string procName, NodeType type);
	static PKBSingleResults getUsesByStmtNoAndType(int stmtNo, NodeType type);
	static PKBSingleResults getUsesByTypeAndVarName(NodeType nodeType, string ident);

	static bool checkFollows(int beforeStmtNo, int afterStmtNo);
	static bool checkFollowsTrans(int beforeStmtNo, int afterStmtNo);
	static bool checkIfFollowsExists();
	static PKBPairResults getFollows(NodeType beforeType, NodeType afterType);
	static PKBPairResults getFollowsTrans(NodeType beforeType, NodeType afterType);
	static PKBPairResults getFollowsBefore(int stmtNo);
	static PKBPairResults getFollowsBefore(NodeType type);
	static PKBPairResults getFollowsBeforeTrans(int stmtNo);
	static PKBPairResults getFollowsBeforeTrans(NodeType type);
	static PKBPairResults getFollowsAfter(int stmtNo);
	static PKBPairResults getFollowsAfter(NodeType type);
	static PKBPairResults getFollowsAfterTrans(int stmtNo);
	static PKBPairResults getFollowsAfterTrans(NodeType type);

	static bool checkParent(int beforeStmtNo, int afterStmtNo);
	static bool checkParentTrans(int beforeStmtNo, int afterStmtNo);
	static bool checkIfParentExists();
	static PKBPairResults getParent(NodeType beforeType, NodeType afterType);
	static PKBPairResults getParentTrans(NodeType beforeType, NodeType afterType);
	static PKBPairResults getParentBefore(int stmtNo);
	static PKBPairResults getParentBefore(NodeType type);
	static PKBPairResults getParentBeforeTrans(int stmtNo);
	static PKBPairResults getParentBeforeTrans(NodeType type);
	static PKBPairResults getParentAfter(int stmtNo);
	static PKBPairResults getParentAfter(NodeType type);
	static PKBPairResults getParentAfterTrans(int stmtNo);
	static PKBPairResults getParentAfterTrans(NodeType type);

	static bool checkNext(int beforeStmtNo, int afterStmtNo);
	static bool checkNextTrans(int beforeStmtNo, int afterStmtNo);
	static bool checkIfNextExists();
	static PKBPairResults getNext(NodeType beforeType, NodeType afterType);
	static PKBPairResults getNextTrans(NodeType beforeType, NodeType afterType);
	static PKBPairResults getNextBefore(int stmtNo);
	static PKBPairResults getNextBefore(NodeType type);
	static PKBPairResults getNextAfter(int stmtNo);
	static PKBPairResults getNextAfter(NodeType type);
	static PKBPairResults getNextBeforeTrans(int stmtNo);
	static PKBPairResults getNextBeforeTrans(NodeType type);
	static PKBPairResults getNextAfterTrans(int stmtNo);
	static PKBPairResults getNextAfterTrans(NodeType type);

	static bool checkCalls(string procName, string procName2);
	static bool checkCallsTrans(string procName, string procName2);
	static bool checkIfCallsExists();
	static PKBPairResults getByCalleesProcName(string procName);
	static PKBPairResults getByCallersProcName(string procName);
	static PKBPairResults getByCallersProcNameTrans(string procName);
	static PKBPairResults getByCalleesProcNameTrans(string procName);
	static PKBPairResults getAllCalls();
	static PKBPairResults getAllCallsTrans();

	static bool checkIfAffectsExists();
	static bool checkAffects(int beforeStmtNo, int afterStmtNo);
	static bool checkAffectsTrans(int beforeStmtNo, int afterStmtNo);
	static PKBPairResults getAllAffects();
	static PKBPairResults getAllAffectsTrans();
	static PKBPairResults getAffectsBefore(int stmtNo);
	static PKBPairResults getAffectsBeforeTrans(int stmtNo);
	static PKBPairResults getAffectsAfter(int stmtNo);
	static PKBPairResults getAffectsAfterTrans(int stmtNo);

	static PKBSingleResults getAssignmentPatternExactMatch(string lhs, TNode* rhs);
	static PKBSingleResults getAssignmentPatternPartialMatch(string lhs, TNode* rhs);
	static PKBSingleResults getIfPatternMatch(string varName);
	static PKBSingleResults getWhilePatternMatch(string varName);
	static PKBSingleResults getAllIfPattern();
	static PKBSingleResults getAllWhilePattern();

	static PKBPairResults getWith(ArgNode& leftArg, ArgNode& rightArg);
	static unordered_map<NodeType, unordered_map<string, string>> getCallReadPrintMap();
	static PKBSingleResults getAllStmtsByType(NodeType nodeType);
	static vector<string> getAllResultsByType(NodeType nodeType, NodeType attrName);
private:
	static PKBSingleResults generateStmtData();
	static unordered_set<int> getAllStmtNos();
	static unordered_map<NodeType, vector<StmtData>> getAllStmts();
	static void populateWithMap(unordered_map<string, PKBSingleResults>& map, const ArgNode& argNode);
	static PKBPairResults processOneSideWithClause(unordered_map<string, PKBSingleResults>& map, const ArgNode& argNode);
	static PKBPairResults processTwoSidesWithClause(unordered_map<string, PKBSingleResults>& map, const ArgNode& argNode, const int& direction);
};
