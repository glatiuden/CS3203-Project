#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <optional>
#include <functional>

using namespace std;

#include "PKB.h"
#include "ASTProcessor.h"
#include "StmtVarMapTable.h"
#include "StmtRsTable.h"
#include "StmtData.h"
#include "../TNode.h"

namespace {
	unordered_map<string, TNode*> generateProcMap(TNode* root) {
		unordered_map<string, TNode*> result;
		for (TNode* child : root->getChildren()) {
			if (child->getType() == NodeType::PROCEDURE) {
				result[child->getValue()] = child;
			}
		}
		return result;
	}
}

void ASTProcessor::processAST(TNode* root) {
	processASTForPattern(root);
	processASTForFollows(root);
	processASTForParents(root);
	processASTForCalls(root);
	processASTForUsesModifies(root);
	processASTForStatements(root);
}

void ASTProcessor::processASTForPattern(TNode* root) {
	queue<TNode*> astQueue;
	astQueue.push(root);

	vector<TNode*> assignRoots;
	vector<StmtData> ifVars;
	vector<StmtData> whileVars;

	function<void(TNode*, NodeType containerType)> dfsCondVars = [&dfsCondVars, &ifVars, &whileVars](TNode* node, NodeType containerType) {
		if (node->getType() == NodeType::VARIABLE) {
			StmtData sd(node->getStmtNum(), node->getType(), node->getValue());
			if (containerType == NodeType::IF) {
				ifVars.push_back(sd);
			} else if (containerType == NodeType::WHILE) {
				whileVars.push_back(sd);
			}
		}

		for (TNode* child : node->getChildren()) {
			dfsCondVars(child, containerType);
		}
	};

	while (astQueue.size()) {
		TNode* curr = astQueue.front();
		astQueue.pop();

		for (TNode* child : curr->getChildren()) {
			switch (child->getType()) {
				case NodeType::ASSIGN:
				{
					assignRoots.push_back(child);
					break;
				}
				case NodeType::IF:
				case NodeType::WHILE:
				{
					TNode* condNode = child->getChildren()[0];
					dfsCondVars(condNode, child->getType());
					astQueue.push(child);
					break;
				}
				case NodeType::STMT_LST:
				case NodeType::PROCEDURE:
				{
					astQueue.push(child);
					break;
				}
			}
		}
	}

	PKB::patternTable = PatternTable(assignRoots, whileVars, ifVars);
}

void ASTProcessor::processASTForFollows(TNode* root) {
	queue<TNode*> astQueue;
	astQueue.push(root);

	while (astQueue.size()) {
		TNode* curr = astQueue.front();
		astQueue.pop();

		vector<StmtData> followList;
		bool isStmtList = (curr->getType() == NodeType::STMT_LST);

		for (TNode* child : curr->getChildren()) {
			StmtData data(child->getStmtNum(), child->getType(), child->getValue());

			if (isStmtList) {
				followList.push_back(data);
			}

			if (child->getType() == NodeType::IF
				|| child->getType() == NodeType::WHILE
				|| child->getType() == NodeType::STMT_LST
				|| child->getType() == NodeType::PROCEDURE) {
				astQueue.push(child);
			}
		}

		if (isStmtList) {
			PKB::followsTable.insertStmtRs(followList);
		}
	}
}

void ASTProcessor::processASTForParents(TNode* root) {
	vector<StmtNode*> roots;
	unordered_map<int, StmtNode*> stmtNoMap;
	unordered_map<NodeType, unordered_set<StmtNode*>> typeMap;
	function<void(TNode*, StmtNode*)> dfs = [&dfs, &roots, &stmtNoMap, &typeMap](TNode* node, StmtNode* stmtNode) {
		switch (node->getType()) {
			case NodeType::PROGRAM:
			case NodeType::PROCEDURE:
			case NodeType::STMT_LST:
			{
				for (TNode* child : node->getChildren()) {
					dfs(child, stmtNode);
				}
				break;
			}
			case NodeType::WHILE:
			case NodeType::IF:
			{
				StmtNode* sn = new StmtNode(StmtData(node->getStmtNum(), node->getType(), node->getValue()));
				stmtNoMap[node->getStmtNum()] = sn;
				typeMap[node->getType()].insert(sn);
				if (stmtNode) {
					stmtNode->addChild(sn);
				} else {
					roots.push_back(sn);
				}

				for (TNode* child : node->getChildren()) {
					dfs(child, sn);
				}
				break;
			}
			case NodeType::ASSIGN:
			case NodeType::READ:
			case NodeType::PRINT:
			case NodeType::CALL:
				if (stmtNode) {
					StmtNode* sn = new StmtNode(StmtData(node->getStmtNum(), node->getType(), node->getValue()));
					stmtNoMap[node->getStmtNum()] = sn;
					typeMap[node->getType()].insert(sn);
					stmtNode->addChild(sn);
				}
				break;
		}
	};
	dfs(root, nullptr);
	PKB::parentTable = StmtRsTable(roots, stmtNoMap, typeMap);
}

void ASTProcessor::processASTForCalls(TNode* root) {
	vector<StmtNode*> roots;
	unordered_map<string, unordered_set<StmtNode*>> callNameMap;

	unordered_map<string, TNode*> procMap = generateProcMap(root);

	function<void(TNode*, StmtNode*)> dfs = [&dfs, &roots, &callNameMap, &procMap](TNode* node, StmtNode* stmtNode) {
		switch (node->getType()) {
			case NodeType::PROGRAM:
			case NodeType::WHILE:
			case NodeType::IF:
			case NodeType::STMT_LST:
			{
				for (TNode* child : node->getChildren()) {
					dfs(child, stmtNode);
				}
				break;
			}
			case NodeType::PROCEDURE:
			case NodeType::CALL:
			{
				StmtNode* sn = new StmtNode(StmtData(node->getStmtNum(), NodeType::CALL,
													 node->getValue(), node->getValue()));
				callNameMap[node->getValue()].insert(sn);

				if (stmtNode) {
					stmtNode->addChild(sn);
				} else {
					roots.push_back(sn);
				}

				TNode* procNode = procMap[node->getValue()];
				for (TNode* child : procNode->getChildren()) {
					dfs(child, sn);
				}
			}
		}
	};
	dfs(root, nullptr);
	PKB::callsTable = StmtRsTable(roots, callNameMap);
}


void ASTProcessor::processASTForUsesModifies(TNode* root) {
	unordered_map<string, TNode*> procMap = generateProcMap(root);

	auto insert = [](StmtVarMapTable* table, vector<TNode*> containers, StmtData data) {
		table->insertEntry(data);
		for (TNode* container : containers) {
			StmtData sd;
			if (container->getType() == NodeType::PROCEDURE || container->getType() == NodeType::CALL) {
				sd = StmtData(container->getStmtNum(), container->getType(), data.getValue(), container->getValue());
			} else {
				sd = StmtData(container->getStmtNum(), container->getType(), data.getValue());
			}
			table->insertEntry(sd);
		}
	};

	function<void(vector<TNode*>, TNode*, NodeType)> dfsUsedVars = [&dfsUsedVars, &insert](vector<TNode*> containers, TNode* node, NodeType type) {
		if (node->getType() == NodeType::VARIABLE) {
			StmtData sd(node->getStmtNum(), type, node->getValue());
			insert(&PKB::usesTable, containers, sd);
		}

		for (TNode* child : node->getChildren()) {
			dfsUsedVars(containers, child, type);
		}
	};

	function<void(vector<TNode*>, TNode*)> dfs = [&dfs, &procMap, &insert, &dfsUsedVars](vector<TNode*> containers, TNode* node) {
		switch (node->getType()) {
			case NodeType::PROGRAM:
			case NodeType::STMT_LST:
			{
				for (TNode* child : node->getChildren()) {
					dfs(containers, child);
				}
				break;
			}
			case NodeType::PROCEDURE:
			{
				vector<TNode*> containersCopy = containers;
				containersCopy.push_back(node);
				for (TNode* child : node->getChildren()) {
					dfs(containersCopy, child);
				}
				break;
			}
			case NodeType::CALL:
			{
				string procName = node->getValue();
				TNode* procNode = procMap[procName];

				vector<TNode*> containersCopy = containers;
				containersCopy.push_back(node);
				for (TNode* child : procNode->getChildren()) {
					dfs(containersCopy, child);
				}
				break;
			}
			case NodeType::IF:
			case NodeType::WHILE:
			{
				TNode* condition = node->getChildren()[0];
				dfsUsedVars(containers, condition, node->getType());

				vector<TNode*> containersCopy = containers;
				containersCopy.push_back(node);
				for (TNode* child : node->getChildren()) {
					dfs(containersCopy, child);
				}
				break;
			}
			case NodeType::PRINT:
			{
				TNode* child = node->getChildren()[0];
				StmtData sd(child->getStmtNum(), NodeType::PRINT, child->getValue());
				insert(&PKB::usesTable, containers, sd);
				break;
			}
			case NodeType::READ:
			{
				TNode* child = node->getChildren()[0];
				StmtData sd(child->getStmtNum(), NodeType::READ, child->getValue());
				insert(&PKB::modifiesTable, containers, sd);
				break;
			}
			case NodeType::ASSIGN:
			{
				TNode* lhsNode = node->getChildren()[0];
				StmtData sd(lhsNode->getStmtNum(), NodeType::ASSIGN, lhsNode->getValue());
				insert(&PKB::modifiesTable, containers, sd);

				TNode* rhsNode = node->getChildren()[1];
				dfsUsedVars(containers, rhsNode, NodeType::ASSIGN);
				break;
			}
		}
	};
	vector<TNode*> initialContainers;
	dfs(initialContainers, root);
}

void ASTProcessor::processASTForStatements(TNode* root) {
	NodeType type = root->getType();
	if (type == NodeType::PROCEDURE
		|| type == NodeType::WHILE
		|| type == NodeType::IF
		|| type == NodeType::READ
		|| type == NodeType::PRINT
		|| type == NodeType::ASSIGN
		|| type == NodeType::VARIABLE
		|| type == NodeType::CONSTANT
		|| type == NodeType::CALL
		) {
		StmtData sd{};
		if (type == NodeType::PROCEDURE || type == NodeType::CALL) {
			int stmtNum = root->getStmtNum();
			string procName = root->getValue();
			sd = StmtData(stmtNum, root->getType(), "", procName);
			if (type == NodeType::CALL) {
				PKB::callReadPrintMap[type][to_string(stmtNum)] = procName;
			}
		} else if (type == NodeType::READ || type == NodeType::PRINT) {
			TNode* child = root->getChildren()[0];
			int stmtNum = child->getStmtNum();
			string varName = child->getValue();
			sd = StmtData(stmtNum, root->getType(), varName);
			PKB::callReadPrintMap[type][to_string(stmtNum)] = varName;
		} else {
			sd = StmtData(root->getStmtNum(), root->getType(), root->getValue());
		}
		PKB::allStmtMap[type].push_back(sd);

		if (root->getStmtNum() > 0) {
			PKB::allStmtNos.insert(root->getStmtNum());
		}
	}

	for (TNode* child : root->getChildren()) {
		processASTForStatements(child);
	}
}
