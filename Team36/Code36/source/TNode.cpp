#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

#include "TNode.h"
#include "Token.h"
#include "CustomException.h"

TNode::TNode() : type{ NodeType::PROGRAM } {}
TNode::TNode(NodeType type) : type{ type } {}
TNode::TNode(NodeType type, TNodeValue  value) : type{ type }, value{ value } {}
TNode::TNode(NodeType type, TNodeValue  value, int stmtNum) : type{ type }, value{ value }, stmtNum{ stmtNum } {}

TNode* TNode::tokenToNode(Token token, int stmtNum) {
	NodeType type;
	string text(token.getValue());
	switch (token.getType()) {
		case Token::Type::DIGIT:
			type = NodeType::CONSTANT;
			break;
		case Token::Type::NAME:
			type = NodeType::VARIABLE;
			break;
		case Token::Type::PLUS:
			type = NodeType::PLUS;
			break;
		case Token::Type::MINUS:
			type = NodeType::MINUS;
			break;
		case Token::Type::MULTIPLY:
			type = NodeType::TIMES;
			break;
		case Token::Type::DIVIDE:
			type = NodeType::DIV;
			break;
		case Token::Type::MODULO:
			type = NodeType::MOD;
			break;
		case Token::Type::EQUAL:
			type = NodeType::ASSIGN;
			break;
		case Token::Type::IF:
			type = NodeType::IF;
			break;
		case Token::Type::WHILE:
			type = NodeType::WHILE;
			break;
		case Token::Type::NOT:
			type = NodeType::NOT;
			break;
		case Token::Type::AND:
			type = NodeType::AND;
			break;
		case Token::Type::OR:
			type = NodeType::OR;
			break;
		case Token::Type::GREATER:
			type = NodeType::GREATER;
			break;
		case Token::Type::GREATER_EQ:
			type = NodeType::GREATER_EQ;
			break;
		case Token::Type::LESS:
			type = NodeType::LESS;
			break;
		case Token::Type::LESS_EQ:
			type = NodeType::LESS_EQ;
			break;
		case Token::Type::DBL_EQUAL:
			type = NodeType::DBL_EQUAL;
			break;
		case Token::Type::NOT_EQUAL:
			type = NodeType::NOT_EQUAL;
			break;
		case Token::Type::READ:
			type = NodeType::READ;
			break;
		case Token::Type::PRINT:
			type = NodeType::PRINT;
			break;
		default:
			throw TNodeException(string("Unrecognised token type could not be converted to TNode"));
	}
	return new TNode(type, token.getValue(), stmtNum);
}

TNode* TNode::createProcNode(string procName) {
	return new TNode(NodeType::PROCEDURE, procName);
}

TNode* TNode::createStmtLstNode() {
	return new TNode(NodeType::STMT_LST);
}

TNode* TNode::createCallNode(string procName, int stmtNum) {
	return new TNode(NodeType::CALL, procName, stmtNum);
}

bool TNode::isEqualExclStmtNo(TNode* tree) {
	if (this == tree) {
		// same pointer
		return true;
	}

	if (type != tree->type) {
		return false;
	}

	if (value != tree->value) {
		return false;
	}

	if (children.size() != tree->children.size()) {
		return false;
	}

	for (std::vector<TNode*>::size_type i = 0; i < children.size(); ++i) {
		if (!(children[i]->isEqualExclStmtNo(tree->children[i]))) {
			return false;
		}
	}

	return true;
}

bool TNode::hasSubtree(TNode* subtree) {
	if (this->isEqualExclStmtNo(subtree)) {
		return true;
	}

	for (std::vector<TNode*>::size_type i = 0; i < children.size(); ++i) {
		if (children[i]->hasSubtree(subtree)) {
			return true;
		}
	}

	return false;
}

void TNode::addChild(TNode *child) { children.push_back(child); }
vector<TNode*> TNode::getChildren() { return children; }
NodeType TNode::getType() { return type; }
TNodeValue TNode::getValue() { return value; }
int TNode::getStmtNum() { return stmtNum; }