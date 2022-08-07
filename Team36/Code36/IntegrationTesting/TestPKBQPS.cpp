#include "stdafx.h"
#include <string>
#include <list>

#include "CppUnitTest.h"
#include "SP/SourceProcessor.h"
#include "PKB/PKB.h"
#include "PKB/ASTProcessor.h"
#include "QPS/QPS.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	static list<string> actualResults;

	TEST_CLASS(IntegrationTestPKBQPS) {
public:
	TEST_CLASS_INITIALIZE(setUp) {
		SourceProcessor sp;
		TNode* tree;
		vector<StmtNode*> cfgs;
		sp.processSource("procedure p12 { a12 = 2; b12 = 3; c12 = 5; while (c12 != 0) { a12 = a12 - 1; if (a12 == 1) then{ b12 = a12 + 1; } else { d12 = b12 + a12; } b12 = b12 + a12 + c12; c12 = c12 - 1; } }", tree, cfgs);
		ASTProcessor::processAST(tree);
	}

	TEST_CLASS_CLEANUP(tearDown) {
		PKB::clear();
	}

	TEST_METHOD_CLEANUP(clear) {
		actualResults.clear();
	}

	TEST_METHOD(testEvaluateModifiesS_shouldReturnResults) {
		QPS::processQuery("assign a; stmt s; variable v;\nSelect a such that Modifies(a, v)", actualResults);
		list<string> expectedResults = { "1", "10", "2", "3", "5", "7", "8", "9" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateModifiesP_shouldReturnResults) {
		QPS::processQuery("procedure p;\nSelect p such that Modifies(p, \"a12\")", actualResults);
		list<string> expectedResults = { "p12" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateUsesS_shouldReturnResults) {
		QPS::processQuery("Select BOOLEAN such that Uses(6, \"a12\")", actualResults);
		list<string> expectedResults = { "TRUE" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateUsesSPattern_shouldReturnResults) {
		QPS::processQuery("assign a;\nSelect BOOLEAN such that Uses(6, \"a12\") pattern a (_, \"a12\")", actualResults);
		list<string> expectedResults = { "FALSE" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateUsesP_shouldReturnResults) {
		QPS::processQuery("procedure p; variable v;\nSelect p such that Uses(p, v)", actualResults);
		list<string> expectedResults = { "p12" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateUsesPPattern_shouldReturnResults) {
		QPS::processQuery("procedure p; variable v; assign a;\nSelect v such that Modifies(p, \"a12\") pattern a (v, _\"a12\"_)", actualResults);
		list<string> expectedResults = { "a12", "b12", "d12" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateFollows_shouldReturnResults) {
		QPS::processQuery("assign a; stmt s; variable v;\nSelect a such that Follows(a, s)", actualResults);
		list<string> expectedResults = { "1", "2", "3", "5", "9" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateFollowsT_shouldReturnResults) {
		QPS::processQuery("assign a; stmt s; variable v;\nSelect a such that Follows*(a, s)", actualResults);
		list<string> expectedResults = { "1", "2", "3", "5", "9" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateFollowsPattern_shouldReturnResults) {
		QPS::processQuery("assign a; stmt s; variable v;\nSelect a such that Follows(a, s) pattern a (v, _)", actualResults);
		list<string> expectedResults = { "1", "2", "3", "5", "9" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateParent_shouldReturnResults) {
		QPS::processQuery("assign a; stmt s; while w;\nSelect w such that Parent(w, a)", actualResults);
		list<string> expectedResults = { "4" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateParentT_shouldReturnResults) {
		QPS::processQuery("assign a; stmt s; while w;\nSelect w such that Parent*(w, a)", actualResults);
		list<string> expectedResults = { "4" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateParentPattern_shouldReturnResults) {
		QPS::processQuery("assign a; stmt s; while w;\nSelect w such that Parent(w, a) pattern a (_, _\"c12\"_)", actualResults);
		list<string> expectedResults = { "4" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}

	TEST_METHOD(testEvaluateNext_shouldReturnResults) {
		QPS::processQuery("assign a; stmt s; while w;\nSelect w pattern a(_, \"a12 - 1\")", actualResults);
		list<string> expectedResults = { "4" };
		actualResults.sort();
		expectedResults.sort();
		Assert::IsTrue(actualResults == expectedResults);
	}
	};
}
