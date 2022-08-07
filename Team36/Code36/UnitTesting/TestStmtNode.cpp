#include <string>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB/StmtNode.h"
#include "CustomException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestStmtNode) {
public:
	TEST_METHOD(replaceChild_oneOccurrence) {
		StmtNode* node = new StmtNode(StmtData(1, NodeType::ASSIGN, ""));
		StmtNode* child1 = new StmtNode(StmtData(2, NodeType::ASSIGN, ""));
		StmtNode* child2 = new StmtNode(StmtData(3, NodeType::ASSIGN, ""));
		StmtNode* replacement = new StmtNode(StmtData(4, NodeType::ASSIGN, ""));

		node->addChild(child1);
		node->addChild(child2);
		Assert::AreEqual((size_t)2, node->getChildren().size());
		Assert::IsTrue(child2 == node->getChildren()[1]);

		node->replaceChild(child2, replacement);
		Assert::AreEqual((size_t)2, node->getChildren().size());
		Assert::IsTrue(replacement == node->getChildren()[1]);
	}

	TEST_METHOD(replaceChild_multipleOccurrence) {
		StmtNode* node = new StmtNode(StmtData(1, NodeType::ASSIGN, ""));
		StmtNode* child1 = new StmtNode(StmtData(2, NodeType::ASSIGN, ""));
		StmtNode* child2 = new StmtNode(StmtData(3, NodeType::ASSIGN, ""));
		StmtNode* replacement = new StmtNode(StmtData(4, NodeType::ASSIGN, ""));

		node->addChild(child2);
		node->addChild(child1);
		node->addChild(child2);
		Assert::AreEqual((size_t)3, node->getChildren().size());
		Assert::IsTrue(child2 == node->getChildren()[0]);
		Assert::IsTrue(child2 == node->getChildren()[2]);

		node->replaceChild(child2, replacement);
		Assert::AreEqual((size_t)3, node->getChildren().size());
		Assert::IsTrue(replacement == node->getChildren()[0]);
		Assert::IsTrue(child2 == node->getChildren()[2]);
	}

	TEST_METHOD(replaceChild_noOccurrence) {
		StmtNode* node = new StmtNode(StmtData(1, NodeType::ASSIGN, ""));
		StmtNode* child1 = new StmtNode(StmtData(2, NodeType::ASSIGN, ""));
		StmtNode* child2 = new StmtNode(StmtData(3, NodeType::ASSIGN, ""));
		StmtNode* replacement = new StmtNode(StmtData(4, NodeType::ASSIGN, ""));

		node->addChild(child1);
		Assert::AreEqual((size_t)1, node->getChildren().size());
		Assert::IsTrue(child1 == node->getChildren()[0]);

		node->replaceChild(child2, replacement);
		Assert::AreEqual((size_t)1, node->getChildren().size());
		Assert::IsTrue(child1 == node->getChildren()[0]);
	}

	TEST_METHOD(removeChild_oneOccurrence) {
		StmtNode* node = new StmtNode(StmtData(1, NodeType::ASSIGN, ""));
		StmtNode* child1 = new StmtNode(StmtData(2, NodeType::ASSIGN, ""));
		StmtNode* child2 = new StmtNode(StmtData(3, NodeType::ASSIGN, ""));

		node->addChild(child1);
		node->addChild(child2);
		Assert::AreEqual((size_t)2, node->getChildren().size());
		Assert::IsTrue(child1 == node->getChildren()[0]);
		Assert::IsTrue(child2 == node->getChildren()[1]);

		node->removeChild(child2);
		Assert::AreEqual((size_t)1, node->getChildren().size());
		Assert::IsTrue(child1 == node->getChildren()[0]);
	}

	TEST_METHOD(removeChild_multipleOccurrence) {
		StmtNode* node = new StmtNode(StmtData(1, NodeType::ASSIGN, ""));
		StmtNode* child1 = new StmtNode(StmtData(2, NodeType::ASSIGN, ""));
		StmtNode* child2 = new StmtNode(StmtData(3, NodeType::ASSIGN, ""));

		node->addChild(child2);
		node->addChild(child1);
		node->addChild(child2);
		Assert::AreEqual((size_t)3, node->getChildren().size());
		Assert::IsTrue(child2 == node->getChildren()[0]);
		Assert::IsTrue(child1 == node->getChildren()[1]);
		Assert::IsTrue(child2 == node->getChildren()[2]);

		node->removeChild(child2);
		Assert::AreEqual((size_t)2, node->getChildren().size());
		Assert::IsTrue(child1 == node->getChildren()[0]);
		Assert::IsTrue(child2 == node->getChildren()[1]);
	}

	TEST_METHOD(removeChild_noOccurrence) {
		StmtNode* node = new StmtNode(StmtData(1, NodeType::ASSIGN, ""));
		StmtNode* child1 = new StmtNode(StmtData(2, NodeType::ASSIGN, ""));
		StmtNode* child2 = new StmtNode(StmtData(3, NodeType::ASSIGN, ""));

		node->addChild(child1);
		Assert::AreEqual((size_t)1, node->getChildren().size());
		Assert::IsTrue(child1 == node->getChildren()[0]);

		node->removeChild(child2);
		Assert::AreEqual((size_t)1, node->getChildren().size());
		Assert::IsTrue(child1 == node->getChildren()[0]);
	}
	};
}
