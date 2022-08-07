#include <unordered_set>
#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB/StmtRsTable.h"
#include "PKB/StmtData.h"
#include "NodeType.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestStmtRsTable) {
public:
	StmtRsTable table;
	TEST_METHOD_INITIALIZE(setup) {
		vector<StmtData> testRs{
			StmtData(5, NodeType::ASSIGN, ""),
			StmtData(9, NodeType::WHILE, ""),
			StmtData(12, NodeType::IF, "")
		};
		table.insertStmtRs(testRs);
	}

	TEST_METHOD(TestGetStmtBefore) {
		pair<StmtData, StmtData> p{
			StmtData(5, NodeType::ASSIGN, ""),
			StmtData(9, NodeType::WHILE, "")
		};
		Assert::IsTrue(table.getStmtBefore(9)[0] == p);
	}

	TEST_METHOD(TestGetStmtBeforeTrans) {
		vector<pair<StmtData, StmtData>> testList{
			pair<StmtData, StmtData> {
				StmtData(5, NodeType::ASSIGN, ""),
				StmtData(12, NodeType::IF, "")
			},
			pair<StmtData, StmtData> {
				StmtData(9, NodeType::WHILE, ""),
				StmtData(12, NodeType::IF, "")
			},
		};
		vector<pair<StmtData, StmtData>> list = table.getStmtBeforeTrans(12);
		Assert::IsTrue(list == testList);
	}

	TEST_METHOD(TestGetStmtAfter) {
		pair<StmtData, StmtData> p{
			StmtData(5, NodeType::ASSIGN, ""),
			StmtData(9, NodeType::WHILE, "")
		};
		Assert::IsTrue(table.getStmtAfter(5)[0] == p);
	}

	TEST_METHOD(TestGetStmtAfterTrans) {
		vector<pair<StmtData, StmtData>> testList{
			pair<StmtData, StmtData> {
				StmtData(5, NodeType::ASSIGN, ""),
				StmtData(9, NodeType::WHILE, "")
			},
			pair<StmtData, StmtData> {
				StmtData(5, NodeType::ASSIGN, ""),
				StmtData(12, NodeType::IF, "")
			},
		};
		vector<pair<StmtData, StmtData>> list = table.getStmtAfterTrans(5);
		Assert::IsTrue(list == testList);
	}
	};
}
