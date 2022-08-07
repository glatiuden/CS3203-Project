#include <algorithm>
#include "stdafx.h"
#include "CppUnitTest.h"

#include "SP/SourceProcessor.h"
#include "PKB/ASTProcessor.h"
#include "PKB/PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	TEST_CLASS(TestSPPKBPattern) {
public:
	TEST_METHOD_INITIALIZE(setUp) {
		SourceProcessor sp;
		TNode* tree;
		vector<StmtNode*> cfgs;
		sp.processSource("procedure p { x = 2; z = z + x + i; i = 1 + 2 * 3 + 4; x = y; a = 1 + (2 + 3); b = (1 + 2) + 3;}", tree, cfgs);
		ASTProcessor::processASTForPattern(tree);
	}

	TEST_METHOD_CLEANUP(tearDown) {
		PKB::clear();
	}

	TEST_METHOD(testGetPatternAssignExactMatch_rhsIsSingleNode) {
		// lhs is wildcard/synonym and rhs is constant node
		Token digitToken(Token::Type::DIGIT, "2", 1);
		vector<StmtData> actual1 = PKB::getPatternAssignExactMatch("_", TNode::tokenToNode(digitToken, 1));
		vector<StmtData> expectedData1 = {
			StmtData(1, NodeType::VARIABLE, "x"),
		};
		Assert::IsTrue(expectedData1 == actual1);

		// lhs is var name and rhs is name node
		Token nameToken(Token::Type::NAME, "y", 1);
		vector<StmtData> actual2 = PKB::getPatternAssignExactMatch("x", TNode::tokenToNode(nameToken, 1));
		vector<StmtData> expectedData2 = {
			StmtData(4, NodeType::VARIABLE, "x"),
		};
		Assert::IsTrue(expectedData2 == actual2);

		// no results
		vector<StmtData> actual3 = PKB::getPatternAssignExactMatch("i", TNode::tokenToNode(nameToken, 1));
		vector<StmtData> expectedData3 = {};
		Assert::IsTrue(expectedData3 == actual3);
	}

	TEST_METHOD(testGetPatternAssignExactMatch_rhsIsTree) {
		Token oneToken(Token::Type::DIGIT, "1", 1);
		Token twoToken(Token::Type::DIGIT, "2", 1);
		Token threeToken(Token::Type::DIGIT, "3", 1);
		Token plusToken(Token::Type::PLUS, "+", 1);
		Token iToken(Token::Type::NAME, "i", 1);
		Token xToken(Token::Type::NAME, "x", 1);
		Token zToken(Token::Type::NAME, "z", 1);

		// lhs is wildcard/synonym and rhs is subtree equal to "1 + 2 + 3"
		TNode* one1 = TNode::tokenToNode(oneToken, 1);
		TNode* two1 = TNode::tokenToNode(twoToken, 1);
		TNode* three1 = TNode::tokenToNode(threeToken, 1);
		TNode* plus1 = TNode::tokenToNode(plusToken, 1);
		TNode* tree1 = TNode::tokenToNode(plusToken, 1);
		tree1->addChild(plus1);
		tree1->addChild(three1);
		plus1->addChild(one1);
		plus1->addChild(two1);

		vector<StmtData> actual1 = PKB::getPatternAssignExactMatch("_", tree1);
		vector<StmtData> expectedData1 = {
			StmtData(6, NodeType::VARIABLE, "b"),
		};
		Assert::IsTrue(expectedData1 == actual1);

		// lhs is var name and rhs is subtree equal to "z + x + i"
		TNode* z2 = TNode::tokenToNode(zToken, 1);
		TNode* x2 = TNode::tokenToNode(xToken, 1);
		TNode* i2 = TNode::tokenToNode(iToken, 1);
		TNode* plus2 = TNode::tokenToNode(plusToken, 1);
		TNode* tree2 = TNode::tokenToNode(plusToken, 1);
		tree2->addChild(plus2);
		tree2->addChild(i2);
		plus2->addChild(z2);
		plus2->addChild(x2);
		vector<StmtData> actual2 = PKB::getPatternAssignExactMatch("z", tree2);
		vector<StmtData> expectedData2 = {
			StmtData(2, NodeType::VARIABLE, "z"),
		};

		// lhs is wildcard/synonym and rhs is subtree equal to "1 + (2 + 3)"
		TNode* one3 = TNode::tokenToNode(oneToken, 1);
		TNode* two3 = TNode::tokenToNode(twoToken, 1);
		TNode* three3 = TNode::tokenToNode(threeToken, 1);
		TNode* plus3 = TNode::tokenToNode(plusToken, 1);
		TNode* tree3 = TNode::tokenToNode(plusToken, 1);
		tree3->addChild(one3);
		tree3->addChild(plus3);
		plus3->addChild(two3);
		plus3->addChild(three3);
		vector<StmtData> actual3 = PKB::getPatternAssignExactMatch("_", tree3);
		vector<StmtData> expectedData3 = {
			StmtData(5, NodeType::VARIABLE, "a"), };
		Assert::IsTrue(expectedData3 == actual3);

		// lhs is var name and rhs is subtree equal to "1 + 2"
		TNode* one4 = TNode::tokenToNode(oneToken, 1);
		TNode* two4 = TNode::tokenToNode(twoToken, 1);
		TNode* tree4 = TNode::tokenToNode(plusToken, 1);
		tree4->addChild(one4);
		tree4->addChild(two4);
		vector<StmtData> actual4 = PKB::getPatternAssignExactMatch("i", tree4);
		vector<StmtData> expectedData4 = {};
		Assert::IsTrue(expectedData4 == actual4);
	}

	TEST_METHOD(testGetPatternAssignExactMatch_rhsIsNull) {
		// lhs is wildcard/synonym
		vector<StmtData> actual1 = PKB::getPatternAssignExactMatch("_", nullptr);
		vector<StmtData> expectedData1 = {
			StmtData(1, NodeType::VARIABLE, "x"),
			StmtData(2, NodeType::VARIABLE, "z"),
			StmtData(3, NodeType::VARIABLE, "i"),
			StmtData(4, NodeType::VARIABLE, "x"),
			StmtData(5, NodeType::VARIABLE, "a"),
			StmtData(6, NodeType::VARIABLE, "b"),
		};

		Assert::AreEqual(expectedData1.size(), actual1.size());
		vector<StmtData> matchingData1;
		for (StmtData expectedD : expectedData1) {
			for (StmtData actualD : actual1) {
				if (actualD == expectedD) {
					matchingData1.push_back(actualD);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData1 == matchingData1);

		// lhs is var name that exists
		vector<StmtData> actual2 = PKB::getPatternAssignExactMatch("i", nullptr);
		vector<StmtData> expectedData2 = {
			StmtData(3, NodeType::VARIABLE, "i"),
		};
		Assert::IsTrue(expectedData2 == actual2);

		// lhs is var name that does not exist
		vector<StmtData> actual3 = PKB::getPatternAssignExactMatch("c", nullptr);
		vector<StmtData> expectedData3 = {
		};
		Assert::IsTrue(expectedData3 == actual3);
	}

	TEST_METHOD(testGetPatternAssignPartialMatch_rhsIsSingleNode) {
		// lhs is wildcard/synonym and rhs is constant node
		Token digitToken(Token::Type::DIGIT, "2", 1);
		vector<StmtData> actual1 = PKB::getPatternAssignPartialMatch("_", TNode::tokenToNode(digitToken, 1));
		vector<StmtData> expectedData1 = {
			StmtData(1, NodeType::VARIABLE, "x"),
			StmtData(3, NodeType::VARIABLE, "i"),
			StmtData(5, NodeType::VARIABLE, "a"),
			StmtData(6, NodeType::VARIABLE, "b"),
		};

		Assert::AreEqual(expectedData1.size(), actual1.size());
		vector<StmtData> matchingData1;
		for (StmtData expectedD : expectedData1) {
			for (StmtData actualD : actual1) {
				if (actualD == expectedD) {
					matchingData1.push_back(actualD);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData1 == matchingData1);

		// lhs is var name and rhs is name node
		Token nameToken(Token::Type::NAME, "y", 1);
		vector<StmtData> actual2 = PKB::getPatternAssignPartialMatch("x", TNode::tokenToNode(nameToken, 1));
		vector<StmtData> expectedData2 = {
			StmtData(4, NodeType::VARIABLE, "x"),
		};
		Assert::IsTrue(expectedData2 == actual2);

		// no results
		vector<StmtData> actual3 = PKB::getPatternAssignPartialMatch("i", TNode::tokenToNode(nameToken, 1));
		vector<StmtData> expectedData3 = {};
		Assert::IsTrue(expectedData3 == actual3);
	}

	TEST_METHOD(testGetPatternAssignPartialMatch_rhsIsTree) {
		Token oneToken(Token::Type::DIGIT, "1", 1);
		Token twoToken(Token::Type::DIGIT, "2", 1);
		Token threeToken(Token::Type::DIGIT, "3", 1);
		Token fourToken(Token::Type::DIGIT, "4", 1);
		Token plusToken(Token::Type::PLUS, "+", 1);
		Token multiplyToken(Token::Type::MULTIPLY, "*", 1);
		TNode* one = TNode::tokenToNode(oneToken, 1);
		TNode* two = TNode::tokenToNode(twoToken, 1);
		TNode* three = TNode::tokenToNode(threeToken, 1);
		TNode* four = TNode::tokenToNode(fourToken, 1);

		// lhs is wildcard/synonym and rhs is subtree equal to "1 + 2"/"(1 + 2)"
		TNode* tree1 = TNode::tokenToNode(plusToken, 1);
		tree1->addChild(one);
		tree1->addChild(two);
		vector<StmtData> actual1 = PKB::getPatternAssignPartialMatch("_", tree1);
		vector<StmtData> expectedData1 = {
			StmtData(6, NodeType::VARIABLE, "b"),
		};

		// lhs is wildcard/synonym and rhs is subtree equal to "2 + 3"/"(2 + 3)"
		TNode* tree2 = TNode::tokenToNode(plusToken, 1);
		tree2->addChild(two);
		tree2->addChild(three);
		vector<StmtData> actual2 = PKB::getPatternAssignPartialMatch("_", tree2);
		vector<StmtData> expectedData2 = {
			StmtData(5, NodeType::VARIABLE, "a"),
		};
		Assert::IsTrue(expectedData2 == actual2);

		// lhs is "i" and rhs is subtree equal to "2 * 3"/"(2 * 3)"
		TNode* tree3 = TNode::tokenToNode(multiplyToken, 1);
		tree3->addChild(two);
		tree3->addChild(three);
		vector<StmtData> actual3 = PKB::getPatternAssignPartialMatch("i", tree3);
		vector<StmtData> expectedData3 = {
			StmtData(3, NodeType::VARIABLE, "i"),
		};
		Assert::IsTrue(expectedData3 == actual3);

		// lhs is "i" and rhs is subtree equal to "3 + 4"
		TNode* tree4 = TNode::tokenToNode(plusToken, 1);
		tree4->addChild(three);
		tree4->addChild(four);
		vector<StmtData> actual4 = PKB::getPatternAssignPartialMatch("i", tree4);
		vector<StmtData> expectedData4 = {};
		Assert::IsTrue(expectedData4 == actual4);
	}

	TEST_METHOD(testGetPatternAssignPartialMatch_rhsIsNull) {
		// lhs is wildcard/synonym
		vector<StmtData> actual1 = PKB::getPatternAssignPartialMatch("_", nullptr);
		vector<StmtData> expectedData1 = {
			StmtData(1, NodeType::VARIABLE, "x"),
			StmtData(2, NodeType::VARIABLE, "z"),
			StmtData(3, NodeType::VARIABLE, "i"),
			StmtData(4, NodeType::VARIABLE, "x"),
			StmtData(5, NodeType::VARIABLE, "a"),
			StmtData(6, NodeType::VARIABLE, "b"),
		};

		Assert::AreEqual(expectedData1.size(), actual1.size());
		vector<StmtData> matchingData1;
		for (StmtData expectedD : expectedData1) {
			for (StmtData actualD : actual1) {
				if (actualD == expectedD) {
					matchingData1.push_back(actualD);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData1 == matchingData1);

		// lhs is var name that exists
		vector<StmtData> actual2 = PKB::getPatternAssignPartialMatch("i", nullptr);
		vector<StmtData> expectedData2 = {
			StmtData(3, NodeType::VARIABLE, "i"),
		};
		Assert::IsTrue(expectedData2 == actual2);

		// lhs is var name that does not exist
		vector<StmtData> actual3 = PKB::getPatternAssignPartialMatch("c", nullptr);
		vector<StmtData> expectedData3 = {
		};
		Assert::IsTrue(expectedData3 == actual3);
	}
	};
}
