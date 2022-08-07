#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Parser.h"
#include "../CustomException.h"

Parser::Parser() : stmtNum{ 1 } {}

TNode* Parser::Parse(const vector<Token>& tokens) {
	TNode* program = new TNode();
	if (tokens.size() == 0) return program;
	int start = 0;
	for (std::vector<Token>::size_type i = 1; i != tokens.size(); ++i) {
		if (tokens[i].getType() == Token::Type::PROCEDURE) {
			program->addChild(parseProc(tokens, start, int(i) - 1));
			start = i;
		}
	}
	program->addChild(parseProc(tokens, start, int(tokens.size()) - 1));

	return program;
}

void Parser::resetStmtNum() { stmtNum = 1; }

TNode* Parser::parseProc(const vector<Token>& tokens, int start, int end) {
	TNode* proc = TNode::createProcNode(tokens[start + 1].getValue());
	TNode* stmtLst = parseStmtLst(tokens, start + 3, end - 1);
	proc->addChild(stmtLst);

	return proc;
}

TNode* Parser::parseStmtLst(const vector<Token>& tokens, int start, int end) {
	TNode* stmtLst = TNode::createStmtLstNode();
	for (int i = start; i <= end; i++) {
		if (tokens[i].getType() == Token::Type::NAME) {
			// assign statement
			int endOfAssign = tokens.size();
			for (int j = i + 1; j <= end; j++) {
				// look for end of assign statement
				if (tokens[j].getType() == Token::Type::SEMICOLON) {
					endOfAssign = j;
					break;
				}
			}
			stmtLst->addChild(parseAssign(tokens, i, endOfAssign));
			i = endOfAssign;
		} else if (tokens[i].getType() == Token::Type::IF) {
			// if statement
			int endOfIf = tokens.size();
			int curlyCount = 0;
			bool foundElse = false;
			for (int j = i + 1; j <= end; j++) {
				// look for end of if statement
				if (tokens[j].getType() == Token::Type::LEFT_CURLY) {
					curlyCount++;
				} else if (tokens[j].getType() == Token::Type::RIGHT_CURLY) {
					if (curlyCount > 1 || !foundElse) {
						curlyCount--;
					} else {
						endOfIf = j;
						break;
					}
				} else if (tokens[j].getType() == Token::Type::ELSE && curlyCount == 0) {
					foundElse = true;
				}
			}
			stmtLst->addChild(parseIf(tokens, i, endOfIf));
			i = endOfIf;
		} else if (tokens[i].getType() == Token::Type::WHILE) {
			// while statement
			int endOfWhile = tokens.size();
			int curlyCount = 0;
			for (int j = i + 1; j <= end; j++) {
				// look for end of while statement
				if (tokens[j].getType() == Token::Type::LEFT_CURLY) {
					curlyCount++;
				} else if (tokens[j].getType() == Token::Type::RIGHT_CURLY) {
					if (curlyCount > 1) {
						curlyCount--;
					} else {
						endOfWhile = j;
						break;
					}
				}
			}
			stmtLst->addChild(parseWhile(tokens, i, endOfWhile));
			i = endOfWhile;
		} else if (tokens[i].getType() == Token::Type::READ || tokens[i].getType() == Token::Type::PRINT) {
			// read/print statement
			stmtLst->addChild(parseSimpleStmt(tokens, i));
			i = i + 2;
		} else if (tokens[i].getType() == Token::Type::CALL) {
			// call statement
			stmtLst->addChild(parseCall(tokens, i));
			i = i + 2;
		} else {
			// start of statement not recognised
			throw ParserException(string("Unrecognised start of statement"));
		}
	}
	return stmtLst;
}

TNode* Parser::parseFactor(const vector<Token>& tokens, int start, int end) {
	if (start >= end) {
		// factor is var_name or const_value
		return TNode::tokenToNode(tokens[start], stmtNum);
	}

	// factor is expr surrounded by parentheses
	return parseExpr(tokens, start + 1, end - 1);
}

TNode* Parser::parseTerm(const vector<Token>& tokens, int start, int end) {
	if (start >= end) {
		// term is a var_name or const_value
		return TNode::tokenToNode(tokens[start], stmtNum);
	}

	int opIndex = -1;
	int parenthesisCount = 0;
	for (int i = end; i >= start; i--) {
		if (tokens[i].getType() == Token::Type::LEFT_PARENTHESIS) {
			parenthesisCount++;
		} else if (tokens[i].getType() == Token::Type::RIGHT_PARENTHESIS) {
			parenthesisCount--;
		} else if ((tokens[i].getType() == Token::Type::MULTIPLY || tokens[i].getType() == Token::Type::DIVIDE || tokens[i].getType() == Token::Type::MODULO) && parenthesisCount == 0) {
			opIndex = i;
			break;
		}
	}

	if (opIndex == -1) {
		// term is a factor
		return parseFactor(tokens, start, end);
	}

	TNode* op = TNode::tokenToNode(tokens[opIndex], stmtNum);
	TNode* term = parseTerm(tokens, start, opIndex - 1);
	TNode* factor = parseFactor(tokens, opIndex + 1, end);
	op->addChild(term);
	op->addChild(factor);

	return op;
}

TNode* Parser::parseExpr(const vector<Token>& tokens, int start, int end) {
	if (start >= end) {
		// expr is a var_name or const_value
		return TNode::tokenToNode(tokens[start], stmtNum);
	}

	int opIndex = -1;
	int parenthesisCount = 0;
	for (int i = end; i >= start; i--) {
		if (tokens[i].getType() == Token::Type::LEFT_PARENTHESIS) {
			parenthesisCount++;
		} else if (tokens[i].getType() == Token::Type::RIGHT_PARENTHESIS) {
			parenthesisCount--;
		} else if ((tokens[i].getType() == Token::Type::PLUS || tokens[i].getType() == Token::Type::MINUS) && parenthesisCount == 0) {
			opIndex = i;
			break;
		}
	}

	if (opIndex == -1) {
		// expr is a term
		return parseTerm(tokens, start, end);
	}

	TNode* op = TNode::tokenToNode(tokens[opIndex], stmtNum);
	TNode* expr = parseExpr(tokens, start, opIndex - 1);
	TNode* term = parseTerm(tokens, opIndex + 1, end);
	op->addChild(expr);
	op->addChild(term);

	return op;

}

TNode* Parser::parseAssign(const vector<Token>& tokens, int start, int end) {
	TNode* varName = TNode::tokenToNode(tokens[start], stmtNum);
	TNode* equals = TNode::tokenToNode(tokens[start + 1], stmtNum);
	TNode* expr = parseExpr(tokens, start + 2, end - 1);
	equals->addChild(varName);
	equals->addChild(expr);
	stmtNum++;

	return equals;
}

TNode* Parser::parseRelFactor(const vector<Token>& tokens, int start, int end) {
	if (start >= end) {
		// rel_factor is var_name or const_value
		return TNode::tokenToNode(tokens[start], stmtNum);
	}

	// factor is expr
	return parseExpr(tokens, start, end);

}

TNode* Parser::parseRelExpr(const vector<Token>& tokens, int start, int end) {
	int opIndex = start;
	for (int i = start; i <= end; i++) {
		Token::Type type = tokens[i].getType();
		if (type == Token::Type::GREATER ||
			type == Token::Type::GREATER_EQ ||
			type == Token::Type::LESS ||
			type == Token::Type::LESS_EQ ||
			type == Token::Type::DBL_EQUAL ||
			type == Token::Type::NOT_EQUAL) {
			opIndex = i;
			break;
		}
	}

	TNode* op = TNode::tokenToNode(tokens[opIndex], stmtNum);
	TNode* relFactor1 = parseRelFactor(tokens, start, opIndex - 1);
	TNode* relFactor2 = parseRelFactor(tokens, opIndex + 1, end);
	op->addChild(relFactor1);
	op->addChild(relFactor2);

	return op;
}

TNode* Parser::parseCondExpr(const vector<Token>& tokens, int start, int end) {
	if (tokens[start].getType() == Token::Type::NOT) {
		TNode * not = TNode::tokenToNode(tokens[start], stmtNum);
		not->addChild(parseCondExpr(tokens, start + 2, end - 1));
		return not;
	}

	if (tokens[start].getType() != Token::Type::LEFT_PARENTHESIS) {
		return parseRelExpr(tokens, start, end);
	}

	int opIndex = -1;
	int parenthesisCount = 1;
	for (int i = start + 1; i <= end; i++) {
		if (tokens[i].getType() == Token::Type::LEFT_PARENTHESIS) {
			parenthesisCount++;
		} else if (tokens[i].getType() == Token::Type::RIGHT_PARENTHESIS) {
			parenthesisCount--;
		} else if ((tokens[i].getType() == Token::Type::AND || tokens[i].getType() == Token::Type::OR) && parenthesisCount == 0) {
			opIndex = i;
			break;
		}
	}

	if (opIndex == -1) {
		// no && or || found
		return parseRelExpr(tokens, start, end);
	}

	TNode* op = TNode::tokenToNode(tokens[opIndex], stmtNum);
	TNode* condExpr1 = parseCondExpr(tokens, start + 1, opIndex - 2);
	TNode* condExpr2 = parseCondExpr(tokens, opIndex + 2, end - 1);
	op->addChild(condExpr1);
	op->addChild(condExpr2);

	return op;
}

TNode* Parser::parseIf(const vector<Token>& tokens, int start, int end) {
	int endOfCond = start;
	int elseIndex = start;
	int parenthesisCount = 0;
	int curlyCount = 0;
	for (int i = start + 2; i <= end; i++) {
		if (tokens[i].getType() == Token::Type::LEFT_PARENTHESIS) {
			parenthesisCount++;
		} else if (tokens[i].getType() == Token::Type::RIGHT_PARENTHESIS) {
			if (parenthesisCount > 0) {
				parenthesisCount--;
			} else {
				endOfCond = i - 1;
			}
		} else if (tokens[i].getType() == Token::Type::LEFT_CURLY) {
			curlyCount++;
		} else if (tokens[i].getType() == Token::Type::RIGHT_CURLY) {
			curlyCount--;
		} else if (tokens[i].getType() == Token::Type::ELSE && curlyCount == 0) {
			elseIndex = i;
			break;
		}
	}
	TNode* ifNode = TNode::tokenToNode(tokens[start], stmtNum);
	TNode* condExpr = parseCondExpr(tokens, start + 2, endOfCond);
	stmtNum++;
	TNode* thenStmt = parseStmtLst(tokens, endOfCond + 4, elseIndex - 2);
	TNode* elseStmt = parseStmtLst(tokens, elseIndex + 2, end - 1);
	ifNode->addChild(condExpr);
	ifNode->addChild(thenStmt);
	ifNode->addChild(elseStmt);

	return ifNode;
}

TNode* Parser::parseWhile(const vector<Token>& tokens, int start, int end) {
	int endOfCond = start + 2;
	int startOfStmtLst = start + 2;
	int parenthesisCount = 0;
	for (int i = start + 2; i <= end; i++) {
		if (tokens[i].getType() == Token::Type::LEFT_PARENTHESIS) {
			parenthesisCount++;
		}

		if (tokens[i].getType() == Token::Type::RIGHT_PARENTHESIS) {
			if (parenthesisCount > 0) {
				parenthesisCount--;
			} else {
				endOfCond = i - 1;
			}
		}

		if (tokens[i].getType() == Token::Type::LEFT_CURLY) {
			startOfStmtLst = i + 1;
			break;
		}
	}
	TNode* whileNode = TNode::tokenToNode(tokens[start], stmtNum);
	TNode* condExpr = parseCondExpr(tokens, start + 2, endOfCond);
	stmtNum++;
	TNode* stmtLst = parseStmtLst(tokens, startOfStmtLst, end - 1);
	whileNode->addChild(condExpr);
	whileNode->addChild(stmtLst);

	return whileNode;
}

TNode* Parser::parseSimpleStmt(const vector<Token>& tokens, int start) {
	// parse read/print statements
	TNode* stmt = TNode::tokenToNode(tokens[start], stmtNum);
	stmt->addChild(TNode::tokenToNode(tokens[start + 1], stmtNum));
	stmtNum++;

	return stmt;
}

TNode* Parser::parseCall(const vector<Token>& tokens, int start) {
	// parse read/print statements
	Token procNameToken = tokens[start + 1];
	return TNode::createCallNode(procNameToken.getValue(), stmtNum++);
}

vector<StmtNode*> Parser::createCFGs(TNode* AST) {
	vector<StmtNode*> result;
	vector<TNode*> procs = AST->getChildren();
	for (std::vector<TNode*>::size_type i = 0; i != procs.size(); ++i) {
		TNode* proc = procs[i];
		TNode* stmtLst = proc->getChildren()[0];
		result.push_back(stmtLstToCFG(stmtLst));
	}

	return result;
}

StmtNode* Parser::stmtLstToCFG(TNode* stmtLst) {
	StmtNode* root = new StmtNode(StmtData(-1, NodeType::EMPTY, ""));
	vector<TNode*> stmts = stmtLst->getChildren();
	if (stmts.size() <= 0) {
		return root;
	}

	StmtNode* curr = stmtToCFG(stmts[0], root);

	for (std::vector<TNode*>::size_type i = 1; i != stmts.size(); ++i) {
		StmtNode* next = stmtToCFG(stmts[i], curr);
		curr = next;
	}

	if (curr->getData().getStmtNo() == -1) {
		// remove dummy node at the end
		for (StmtNode* prev : curr->getParents()) {
			prev->removeChild(curr);
		}
	}

	return root;
}

StmtNode* Parser::stmtToCFG(TNode* stmt, StmtNode* curr) {
	switch (stmt->getType()) {
		case NodeType::IF:
			return ifToCFG(stmt, curr);
		case NodeType::WHILE:
			return whileToCFG(stmt, curr);
		default:
			StmtNode* node = new StmtNode(StmtData(stmt->getStmtNum(), stmt->getType(), ""));
			return mergeCFGNodes(curr, node);
	}
}

StmtNode* Parser::whileToCFG(TNode* whileNode, StmtNode* curr) {
	StmtNode* start = new StmtNode(StmtData(whileNode->getStmtNum(), NodeType::WHILE, ""));
	start = mergeCFGNodes(curr, start);
	StmtNode* end = start;

	TNode* stmtLst = whileNode->getChildren()[1];
	vector<TNode*> stmts = stmtLst->getChildren();
	for (std::vector<TNode*>::size_type i = 0; i != stmts.size(); ++i) {
		StmtNode* next = stmtToCFG(stmts[i], end);
		end = next;
	}

	// while node loops back to start
	mergeCFGNodes(end, start);

	return start;
}

StmtNode* Parser::ifToCFG(TNode* ifNode, StmtNode* curr) {
	StmtNode* dummyNode = new StmtNode(StmtData(-1, NodeType::EMPTY, ""));
	StmtNode* start = new StmtNode(StmtData(ifNode->getStmtNum(), NodeType::IF, ""));
	start = mergeCFGNodes(curr, start);
	StmtNode* end = start;

	TNode* thenStmtLst = ifNode->getChildren()[1];
	TNode* elseStmtLst = ifNode->getChildren()[2];
	vector<TNode*> thenStmts = thenStmtLst->getChildren();
	vector<TNode*> elseStmts = elseStmtLst->getChildren();

	// create "then" branch
	for (std::vector<TNode*>::size_type i = 0; i != thenStmts.size(); ++i) {
		StmtNode* next = stmtToCFG(thenStmts[i], end);
		end = next;
	}
	StmtNode* thenEnd = end;

	// create "else" branch
	end = start; // "else" branches from start node
	for (std::vector<TNode*>::size_type i = 0; i != elseStmts.size(); ++i) {
		StmtNode* next = stmtToCFG(elseStmts[i], end);
		end = next;
	}
	StmtNode* elseEnd = end;

	// branches end at dummyNode
	mergeCFGNodes(thenEnd, dummyNode);
	mergeCFGNodes(elseEnd, dummyNode);

	return dummyNode;
}

StmtNode* Parser::mergeCFGNodes(StmtNode* curr, StmtNode* next) {
	if (curr->getData().getStmtNo() > 0) {
		curr->addChild(next);
	} else if (curr->getData().getStmtNo() == -1 && curr->getParents().size() == 0) {
		// curr has not been initialised
		*curr = *next;
		return curr;
	} else {
		// curr is a dummy node
		for (std::vector<StmtNode*>::size_type i = 0; i != curr->getParents().size(); ++i) {
			StmtNode* parent = curr->getParents()[i];
			parent->replaceChild(curr, next);
		}
		free(curr);
	}

	return next;
}
