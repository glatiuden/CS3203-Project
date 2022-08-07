#include "stdafx.h"
#include "CppUnitTest.h"
#include "QPSNodeStub.cpp"
#include "QPS/QPSTree/QPSTree.h"
#include <CustomException.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
	static QPSTree qpsTree;
	static QPSNodeStub qpsNode;
	static ArgNode argNode;
	static ArgNode argNode2;

	TEST_CLASS(TestTestQPSTree) {
public:
	TEST_CLASS_INITIALIZE(setUp) {
		qpsTree = QPSTree();
		qpsNode = QPSNodeStub(QPSNodeType::SELECTION);
		argNode = ArgNode(NodeType::EMPTY, "");
		argNode2 = ArgNode(NodeType::EMPTY, "");
	}

	TEST_METHOD_CLEANUP(tearDown) {
		qpsTree.clearTree();
		qpsNode.clear();
		qpsNode = QPSNodeStub(QPSNodeType::SELECTION);
		argNode = ArgNode(NodeType::EMPTY, "");
		argNode2 = ArgNode(NodeType::EMPTY, "");
	}

	TEST_METHOD(testAddResultNode_stmtNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::STMT, "s");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_whileNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::WHILE, "w");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_ifNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::IF, "if");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_assignNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::ASSIGN, "a");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_variableNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::VARIABLE, "v");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_constantNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::CONSTANT, "c");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_procedureNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::PROCEDURE, "p");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_readNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::READ, "r");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_printNodeType_shouldBeEqual) {
		argNode = ArgNode(NodeType::PRINT, "pr");
		qpsNode.setLeftArg(argNode);
		qpsTree.addResultNode(&qpsNode);
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddResultNode_rootQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::ROOT, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddResultNode_resultQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::RESULT, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddResultNode_clauseQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::CLAUSE, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddResultNode_followsQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::FOLLOWS, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddResultNode_followsTQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::FOLLOWS_T, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddResultNode_parentQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::PARENT, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddResultNode_parentTQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::PARENT_T, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddResultNode_useSQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::USES, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddResultNode_modifiesSQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::MODIFIES, argNode);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addResultNode(&qpsNode); });
	}

	TEST_METHOD(testAddClauseNode_followsQPSNodeType_shouldBeEqual) {
		qpsNode = QPSNodeStub(QPSNodeType::FOLLOWS, argNode, argNode2);
		qpsTree.addClauseNode(&qpsNode);
		Assert::IsTrue(qpsTree.getClauseNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddClauseNode_followsTQPSNodeType_shouldBeEqual) {
		qpsNode = QPSNodeStub(QPSNodeType::FOLLOWS_T, argNode, argNode2);
		qpsTree.addClauseNode(&qpsNode);
		Assert::IsTrue(qpsTree.getClauseNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddClauseNode_useSQPSNodeType_shouldBeEqual) {
		qpsNode = QPSNodeStub(QPSNodeType::USES, argNode, argNode2);
		qpsTree.addClauseNode(&qpsNode);
		Assert::IsTrue(qpsTree.getClauseNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddClauseNode_modifiesSQPSNodeType_shouldBeEqual) {
		qpsNode = QPSNodeStub(QPSNodeType::MODIFIES, argNode, argNode2);
		qpsTree.addClauseNode(&qpsNode);
		Assert::IsTrue(qpsTree.getClauseNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddClauseNode_parentSQPSNodeType_shouldBeEqual) {
		qpsNode = QPSNodeStub(QPSNodeType::PARENT, argNode, argNode2);
		qpsTree.addClauseNode(&qpsNode);
		Assert::IsTrue(qpsTree.getClauseNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddClauseNode_parentTSQPSNodeType_shouldBeEqual) {
		qpsNode = QPSNodeStub(QPSNodeType::PARENT_T, argNode, argNode2);
		qpsTree.addClauseNode(&qpsNode);
		Assert::IsTrue(qpsTree.getClauseNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testAddClauseNode_rootQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::ROOT, argNode, argNode2);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addClauseNode(&qpsNode); });
	}

	TEST_METHOD(testAddClauseNode_resultQPSNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::RESULT, argNode, argNode2);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addClauseNode(&qpsNode); });
	}

	TEST_METHOD(testAddClauseNode_clauseNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::CLAUSE, argNode, argNode2);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addClauseNode(&qpsNode); });
	}

	TEST_METHOD(testAddClauseNode_selectionNodeType_shouldThrowInvalidArgumentException) {
		qpsNode = QPSNodeStub(QPSNodeType::SELECTION, argNode, argNode2);
		Assert::ExpectException<QPSTreeException>([&] { qpsTree.addClauseNode(&qpsNode); });
	}
	};
}
