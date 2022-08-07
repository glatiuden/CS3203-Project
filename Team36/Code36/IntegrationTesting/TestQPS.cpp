#include "stdafx.h"
#include "CppUnitTest.h"
#include "QPS/QPSPreprocessor.h"
#include "QPS/QPSTree/QPSTree.h"
#include "QPS/QPSTree/QPSNode.h"
#include "QPS/QPSEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	static string queryStr{};
	static QPSPreprocessor qpsPreprocessor{};
	static QPSTree qpsTree{};
	static QPSNode qpsNode{};
	static QPSClauseParser qpsClauseParser{};

	TEST_CLASS(IntegrationTestQPS) {
public:
	TEST_CLASS_INITIALIZE(setUp) {
		qpsNode = QPSNode(QPSNodeType::SELECTION);
	}

	TEST_METHOD_CLEANUP(tearDown) {
		qpsClauseParser = QPSClauseParser();
		qpsTree.clearTree();
		qpsNode.clear();
		qpsNode = QPSNode(QPSNodeType::SELECTION);
	}

	TEST_METHOD(testQPSResultNode_single_elem) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_single_elem_with_attrName) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a.stmt# such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a", NodeType::STMTNUM));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_single_elem_as_tuple) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select <a> such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_single_elem_tuple_attrName) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select <a.stmt#> such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a", NodeType::STMTNUM));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_pair) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select <a, v> such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		Assert::IsTrue(qpsTree.getResultNode().getSize() == 2);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::VARIABLE, "v"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(1) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_pair_with_attrName) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select <a.stmt#, v.varName> such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		Assert::IsTrue(qpsTree.getResultNode().getSize() == 2);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a", NodeType::STMTNUM));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::VARIABLE, "v", NodeType::VARNAME));
		Assert::IsTrue(qpsTree.getResultNode().getChild(1) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_tuple) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "procedure p; assign a; variable v; constant c; Select <p, a, v, c> such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		Assert::IsTrue(qpsTree.getResultNode().getSize() == 4);

		qpsNode.setLeftArg(ArgNode(NodeType::PROCEDURE, "p"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(1) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::VARIABLE, "v"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(2) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::CONSTANT, "c"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(3) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_tuple_with_attrName) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "procedure p; assign a; variable v; constant c; Select <p.procName, a.stmt#, v.varName, c.value> such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		Assert::IsTrue(qpsTree.getResultNode().getSize() == 4);

		qpsNode.setLeftArg(ArgNode(NodeType::PROCEDURE, "p", NodeType::PROCNAME));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a", NodeType::STMTNUM));
		Assert::IsTrue(qpsTree.getResultNode().getChild(1) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::VARIABLE, "v", NodeType::VARNAME));
		Assert::IsTrue(qpsTree.getResultNode().getChild(2) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::CONSTANT, "c", NodeType::VALUE));
		Assert::IsTrue(qpsTree.getResultNode().getChild(3) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_BOOLEAN) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select BOOLEAN such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		qpsNode.setLeftArg(ArgNode(NodeType::BOOLEAN, "BOOLEAN"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_BOOLEAN_as_synonym) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign BOOLEAN; variable v; Select BOOLEAN such that Modifies (BOOLEAN, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "BOOLEAN"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_BOOLEAN_as_synonym_pair) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign BOOLEAN; variable v; Select <BOOLEAN, v> such that Modifies (BOOLEAN, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		Assert::IsTrue(qpsTree.getResultNode().getSize() == 2);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "BOOLEAN"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::VARIABLE, "v"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(1) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_BOOLEAN_as_synonym_pair_with_attrName) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign BOOLEAN; variable v; Select <BOOLEAN.stmt#, v.varName> such that Modifies (BOOLEAN, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		Assert::IsTrue(qpsTree.getResultNode().getSize() == 2);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "BOOLEAN", NodeType::STMTNUM));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::VARIABLE, "v", NodeType::VARNAME));
		Assert::IsTrue(qpsTree.getResultNode().getChild(1) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_BOOLEAN_as_synonym_tuple) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "procedure BOOLEAN; assign a; variable v; constant c; Select <BOOLEAN, a, v, c> such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		Assert::IsTrue(qpsTree.getResultNode().getSize() == 4);

		qpsNode.setLeftArg(ArgNode(NodeType::PROCEDURE, "BOOLEAN"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(1) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::VARIABLE, "v"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(2) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::CONSTANT, "c"));
		Assert::IsTrue(qpsTree.getResultNode().getChild(3) == qpsNode);
	}

	TEST_METHOD(testQPSResultNode_BOOLEAN_as_synonym_tuple_with_attrName) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "procedure BOOLEAN; assign a; variable v; constant c; Select <BOOLEAN.procName, a.stmt#, v.varName, c.value> such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		Assert::IsTrue(qpsTree.getResultNode().getSize() == 4);

		qpsNode.setLeftArg(ArgNode(NodeType::PROCEDURE, "BOOLEAN", NodeType::PROCNAME));
		Assert::IsTrue(qpsTree.getResultNode().getChild(0) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::ASSIGN, "a", NodeType::STMTNUM));
		Assert::IsTrue(qpsTree.getResultNode().getChild(1) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::VARIABLE, "v", NodeType::VARNAME));
		Assert::IsTrue(qpsTree.getResultNode().getChild(2) == qpsNode);

		qpsNode.setLeftArg(ArgNode(NodeType::CONSTANT, "c", NodeType::VALUE));
		Assert::IsTrue(qpsTree.getResultNode().getChild(3) == qpsNode);
	}

	TEST_METHOD(testQPSClauseParser_missingDeclaration_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select s such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_duplicateDeclaration_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; variable v; Select s such that Modifies (a, v)";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_invalidPattern_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select s pattern s(v, _)";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_affects_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a such that Affects(v, v)";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_calls_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a such that Calls(v, v)";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_modifies_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a such that Modifies(v, a)";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_uses_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a such that Uses(v, a)";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_withValue_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a with a.value = 1";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_withStmtNo_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a with v.stmt# = 1";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_withProcName_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a with v.procName = 1";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}

	TEST_METHOD(testQPSClauseParser_withVarName_semanticInvalid) {
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		queryStr = "assign a; variable v; Select a with a.varName = 1";
		qpsPreprocessor.validateQuery(queryStr);
		Assert::ExpectException<QPSClauseParserException>([&] { qpsClauseParser.validateSemanticsAndParseTree(qpsTree); });
	}
	};
}
