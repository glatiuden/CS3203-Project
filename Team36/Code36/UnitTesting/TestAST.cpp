#include <string>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"
#include "Token.h"
#include "CustomException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestAST) {
public:

	TEST_METHOD(tokenToNode) {
		TNode T;
		Token token;

		// Test all possible node types
		char* digit = "1";
		token = Token(Token::Type::DIGIT, digit, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::CONSTANT);
		Assert::AreEqual(std::string(digit), T.getValue());

		char* name = "name";
		token = Token(Token::Type::NAME, name, 4);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::VARIABLE);
		Assert::AreEqual(std::string(name), T.getValue());

		char* plus = "+";
		token = Token(Token::Type::PLUS, plus, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::PLUS);
		Assert::AreEqual(std::string(plus), T.getValue());

		char* minus = "+";
		token = Token(Token::Type::MINUS, minus, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::MINUS);
		Assert::AreEqual(std::string(minus), T.getValue());

		char* multiply = "*";
		token = Token(Token::Type::MULTIPLY, multiply, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::TIMES);
		Assert::AreEqual(std::string(multiply), T.getValue());

		char* divide = "/";
		token = Token(Token::Type::DIVIDE, divide, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::DIV);
		Assert::AreEqual(std::string(divide), T.getValue());

		char* modulo = "%";
		token = Token(Token::Type::MODULO, modulo, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::MOD);
		Assert::AreEqual(std::string(modulo), T.getValue());

		char* equal = "=";
		token = Token(Token::Type::EQUAL, equal, 1);
		T = *TNode::tokenToNode(token, 2);
		Assert::IsTrue(T.getType() == NodeType::ASSIGN);
		Assert::AreEqual(std::string(equal), T.getValue());
		Assert::AreEqual(2, T.getStmtNum());

		token = Token(Token::Type::IF, "if", 2);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::IF);
		Assert::AreEqual(std::string("if"), T.getValue());

		token = Token(Token::Type::WHILE, "while", 5);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::WHILE);
		Assert::AreEqual(std::string("while"), T.getValue());

		char* not = "!";
		token = Token(Token::Type::NOT, not, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::NOT);
		Assert::AreEqual(std::string(not), T.getValue());

		char*and = "&&";
		token = Token(Token::Type::AND, and, 2);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::AND);
		Assert::AreEqual(std::string(and), T.getValue());

		char* or = "||";
		token = Token(Token::Type::OR, or , 2);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::OR);
		Assert::AreEqual(std::string(or ), T.getValue());

		char* greater = ">";
		token = Token(Token::Type::GREATER, greater, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::GREATER);
		Assert::AreEqual(std::string(greater), T.getValue());

		char* greaterEq = ">=";
		token = Token(Token::Type::GREATER_EQ, greaterEq, 2);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::GREATER_EQ);
		Assert::AreEqual(std::string(greaterEq), T.getValue());

		char* less = "<";
		token = Token(Token::Type::LESS, less, 1);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::LESS);
		Assert::AreEqual(std::string(less), T.getValue());

		char* lessEq = "<=";
		token = Token(Token::Type::LESS_EQ, lessEq, 2);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::LESS_EQ);
		Assert::AreEqual(std::string(lessEq), T.getValue());

		char* dblEqual = "==";
		token = Token(Token::Type::DBL_EQUAL, dblEqual, 2);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::DBL_EQUAL);
		Assert::AreEqual(std::string(dblEqual), T.getValue());

		char* notEqual = "!=";
		token = Token(Token::Type::NOT_EQUAL, notEqual, 2);
		T = *TNode::tokenToNode(token, 0);
		Assert::IsTrue(T.getType() == NodeType::NOT_EQUAL);
		Assert::AreEqual(std::string(notEqual), T.getValue());

		// Test invalid token
		char* invalid = "(";
		token = Token(Token::Type::LEFT_PARENTHESIS, invalid, 1);
		auto func1 = [&token] { TNode::tokenToNode(token, 0); };
		Assert::ExpectException<TNodeException>(func1);
	}

	TEST_METHOD(createProcNode) {
		TNode* T = TNode::createProcNode("p");
		Assert::IsTrue(T->getType() == NodeType::PROCEDURE);
		Assert::AreEqual(std::string("p"), T->getValue());
	}

	TEST_METHOD(createStmtLstNode) {
		TNode* T = TNode::createStmtLstNode();
		Assert::IsTrue(T->getType() == NodeType::STMT_LST);
		Assert::AreEqual(std::string(""), T->getValue());
	}

	TEST_METHOD(createCallNode) {
		TNode* T = TNode::createCallNode("proc", 1);
		Assert::IsTrue(T->getType() == NodeType::CALL);
		Assert::AreEqual(std::string("proc"), T->getValue());
		Assert::AreEqual(1, T->getStmtNum());
	}

	TEST_METHOD(addChild) {
		// New node should have no children
		char* equal = "=";
		Token eqToken(Token::Type::EQUAL, equal, 1);
		TNode* T = TNode::tokenToNode(eqToken, 2);
		Assert::AreEqual(T->getChildren().size(), (size_t)0);

		// Number of children should increase
		char* digit = "1";
		Token digitToken(Token::Type::DIGIT, digit, 1);
		TNode* child = TNode::tokenToNode(digitToken, 0);
		T->addChild(child);
		Assert::AreEqual(T->getChildren().size(), (size_t)1);

		// Number of children of T should remain the same even when added as a child to another node
		TNode* parent = new TNode();
		parent->addChild(T);
		Assert::AreEqual(parent->getChildren().size(), (size_t)1);
		Assert::AreEqual(T->getChildren().size(), (size_t)1);
	}

	TEST_METHOD(isEqualExclStmtNo_singleNodeShouldBeTrue) {
		Token digitToken(Token::Type::DIGIT, "1", 1);
		TNode* node1 = TNode::tokenToNode(digitToken, 1);
		TNode* node2 = TNode::tokenToNode(digitToken, 1);
		TNode* node3 = TNode::tokenToNode(digitToken, 3);

		// same object
		Assert::IsTrue(node1->isEqualExclStmtNo(node1));

		// diff objects but equal incl stmt no
		Assert::IsTrue(node1->isEqualExclStmtNo(node2));

		// diff objects but equal excl stmt no
		Assert::IsTrue(node1->isEqualExclStmtNo(node3));
	}

	TEST_METHOD(isEqualExclStmtNo_singleNodeShouldBeFalse) {
		Token digitToken1(Token::Type::DIGIT, "1", 1);
		Token digitToken2(Token::Type::DIGIT, "2", 1);
		Token nameToken(Token::Type::NAME, "x", 1);
		TNode* node1 = TNode::tokenToNode(digitToken1, 1);
		TNode* node2 = TNode::tokenToNode(digitToken2, 1);
		TNode* node3 = TNode::tokenToNode(nameToken, 1);

		// same type, diff value
		Assert::IsFalse(node1->isEqualExclStmtNo(node2));

		// diff type
		Assert::IsFalse(node1->isEqualExclStmtNo(node3));
	}

	TEST_METHOD(isEqualExclStmtNo_treeShouldBeTrue) {
		Token plusToken(Token::Type::PLUS, "+", 1);
		Token digitToken1(Token::Type::DIGIT, "1", 1);
		Token digitToken2(Token::Type::DIGIT, "2", 1);
		TNode* tree1 = TNode::tokenToNode(plusToken, 1);
		TNode* tree2 = TNode::tokenToNode(plusToken, 2);
		TNode* tree3 = TNode::tokenToNode(plusToken, 3);
		TNode* one1 = TNode::tokenToNode(digitToken1, 1);
		TNode* one2 = TNode::tokenToNode(digitToken1, 2);
		TNode* two1 = TNode::tokenToNode(digitToken2, 1);
		TNode* two2 = TNode::tokenToNode(digitToken2, 2);

		tree1->addChild(one1);
		tree1->addChild(two1);

		tree2->addChild(one1);
		tree2->addChild(two1);

		tree3->addChild(one2);
		tree3->addChild(two2);


		// same object
		Assert::IsTrue(tree1->isEqualExclStmtNo(tree1));

		// diff root objects but same children
		Assert::IsTrue(tree1->isEqualExclStmtNo(tree2));

		// diff root and children objects, but equal
		Assert::IsTrue(tree1->isEqualExclStmtNo(tree3));
	}

	TEST_METHOD(isEqualExclStmtNo_treeShouldBeFalse) {
		Token plusToken(Token::Type::PLUS, "+", 1);
		Token digitToken1(Token::Type::DIGIT, "1", 1);
		Token digitToken2(Token::Type::DIGIT, "2", 1);
		TNode* tree1 = TNode::tokenToNode(plusToken, 1);
		TNode* tree2 = TNode::tokenToNode(plusToken, 2);
		TNode* tree3 = TNode::tokenToNode(plusToken, 3);
		TNode* tree4 = TNode::tokenToNode(plusToken, 4);
		TNode* one1 = TNode::tokenToNode(digitToken1, 1);
		TNode* one2 = TNode::tokenToNode(digitToken1, 2);
		TNode* two1 = TNode::tokenToNode(digitToken2, 1);
		TNode* two2 = TNode::tokenToNode(digitToken2, 2);

		tree1->addChild(one1);
		tree1->addChild(two1);

		tree2->addChild(one2);
		tree2->addChild(two2);
		tree2->addChild(two1);

		tree3->addChild(two2);
		tree3->addChild(two1);

		tree4->addChild(two1);
		tree4->addChild(one1);

		// diff no of children
		Assert::IsFalse(tree1->isEqualExclStmtNo(tree2));

		// same no of children, but different children
		Assert::IsFalse(tree1->isEqualExclStmtNo(tree3));

		// same no of children, same children, but different order
		Assert::IsFalse(tree1->isEqualExclStmtNo(tree4));
	}

	TEST_METHOD(hasSubtree_leafNodeSubtree) {
		Token plusToken(Token::Type::PLUS, "+", 1);
		Token digitToken1(Token::Type::DIGIT, "1", 1);
		Token digitToken2(Token::Type::DIGIT, "2", 1);
		Token digitToken3(Token::Type::DIGIT, "3", 1);
		TNode* subtree1 = TNode::tokenToNode(digitToken1, 1);
		TNode* subtree2 = TNode::tokenToNode(plusToken, 1);
		TNode* tree1 = TNode::tokenToNode(plusToken, 1);
		TNode* tree2 = TNode::tokenToNode(plusToken, 1);
		TNode* tree3 = TNode::tokenToNode(plusToken, 1);
		TNode* plus = TNode::tokenToNode(plusToken, 1);
		TNode* one = TNode::tokenToNode(digitToken1, 1);
		TNode* two = TNode::tokenToNode(digitToken2, 1);
		TNode* three = TNode::tokenToNode(digitToken3, 1);

		// subtree1: 1
		// subtree2: +
		// tree1: 2 + 1 + 3
		tree1->addChild(plus);
		tree1->addChild(three);
		plus->addChild(two);
		plus->addChild(one);

		// tree2: 2 + 3
		tree2->addChild(two);
		tree2->addChild(three);

		// tree is equal to subtree
		Assert::IsTrue(one->hasSubtree(subtree1));

		// tree contains subtree as child
		Assert::IsTrue(tree1->hasSubtree(subtree1));

		// tree does not contain leaf node "1"
		Assert::IsFalse(tree2->hasSubtree(subtree1));

		// tree has "+" but not left node
		Assert::IsFalse(tree1->hasSubtree(subtree2));
	}

	TEST_METHOD(hasSubtree_sumSubtree) {
		Token plusToken(Token::Type::PLUS, "+", 1);
		Token digitToken1(Token::Type::DIGIT, "1", 1);
		Token digitToken2(Token::Type::DIGIT, "2", 1);
		Token digitToken3(Token::Type::DIGIT, "3", 1);
		TNode* subtree = TNode::tokenToNode(plusToken, 1);
		TNode* tree1 = TNode::tokenToNode(plusToken, 1);
		TNode* tree2 = TNode::tokenToNode(plusToken, 1);
		TNode* tree3 = TNode::tokenToNode(plusToken, 1);
		TNode* tree4 = TNode::tokenToNode(plusToken, 1);
		TNode* plus1 = TNode::tokenToNode(plusToken, 1);
		TNode* plus2 = TNode::tokenToNode(plusToken, 1);
		TNode* plus3 = TNode::tokenToNode(plusToken, 1);
		TNode* one = TNode::tokenToNode(digitToken1, 1);
		TNode* two = TNode::tokenToNode(digitToken2, 1);
		TNode* three = TNode::tokenToNode(digitToken3, 1);

		// subtree: 1 + 2
		subtree->addChild(one);
		subtree->addChild(two);

		// tree1: 1 + 2
		tree1->addChild(one);
		tree1->addChild(two);

		// tree2: 1 + 2 + 3
		tree2->addChild(plus1);
		tree2->addChild(three);
		plus1->addChild(one);
		plus1->addChild(two);

		// tree3: 2 + 1 + 3
		tree3->addChild(plus2);
		tree3->addChild(three);
		plus2->addChild(two);
		plus2->addChild(one);

		// tree4: 3 + 1 + 2
		tree4->addChild(plus3);
		tree4->addChild(two);
		plus3->addChild(three);
		plus3->addChild(one);

		// tree is equal to subtree
		Assert::IsTrue(tree1->hasSubtree(subtree));

		// tree contains subtree as child
		Assert::IsTrue(tree2->hasSubtree(subtree));

		// tree does not contain subtree
		Assert::IsFalse(tree3->hasSubtree(subtree));

		// expr exists in string but not subtree
		Assert::IsFalse(tree4->hasSubtree(subtree));
	}
	};
}
