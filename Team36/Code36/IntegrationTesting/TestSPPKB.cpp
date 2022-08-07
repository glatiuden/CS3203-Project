#include <algorithm>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "SP/SourceProcessor.h"
#include "PKB/PKB.h"
#include "PKB/ASTProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	TEST_CLASS(IntegrationTestSPPKB) {
public:
	TEST_METHOD_INITIALIZE(setUp) {
		SourceProcessor sp;
		TNode* tree;
		vector<StmtNode*> cfgs;
		sp.processSource("procedure p { x = 2; z = z + x + i; i = 5; x = y; }", tree, cfgs);
		ASTProcessor::processASTForUsesModifies(tree);
		ASTProcessor::processASTForStatements(tree);
	}

	TEST_METHOD_CLEANUP(tearDown) {
		PKB::clear();
	}

	TEST_METHOD(testCheckModifies_shouldBeEqual) {
		bool actualBool = PKB::checkModifies(1, "x");
		Assert::IsTrue(actualBool);

		actualBool = PKB::checkModifies(3, "i");
		Assert::IsTrue(actualBool);
	}

	TEST_METHOD(testGetModifiesByStmtNo_shouldBeEqual) {
		vector<StmtData> actualData = PKB::getModifiesByStmtNo(1);
		vector<StmtData> expectedData = {
			StmtData(1, NodeType::ASSIGN, "x")
		};
		Assert::IsTrue(actualData == expectedData);
	}

	static bool compareData(StmtData sd1, StmtData sd2) {
		return sd1.getStmtNo() < sd2.getStmtNo();
	}
	};
}
