#include <vector>
#include <iostream>

#include "stdafx.h"
#include "CppUnitTest.h"
#include <algorithm>
#include "PKB/StmtVarMapTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestStmtVarMapTable) {
public:
	StmtVarMapTable table;

	TEST_METHOD_INITIALIZE(setup) {
		table.insertEntry(StmtData(1, NodeType::VARIABLE, "world"));
		table.insertEntry(StmtData(1, NodeType::VARIABLE, "hello"));
		table.insertEntry(StmtData(2, NodeType::VARIABLE, "bye"));
		table.insertEntry(StmtData(2, NodeType::VARIABLE, "universe"));
		table.insertEntry(StmtData(5, NodeType::VARIABLE, "greg"));
		table.insertEntry(StmtData(6, NodeType::VARIABLE, "steve"));
	}

	TEST_METHOD(TestCheckEntry) {
		Assert::AreEqual(table.checkEntry(1, "hello"), true);
		Assert::AreEqual(table.checkEntry(2, "universe"), true);
		Assert::AreEqual(table.checkEntry(3, "hello"), false);
	}

	static bool compareData(StmtData sd1, StmtData sd2) {
		return sd1.getStmtNo() < sd2.getStmtNo();
	}

	TEST_METHOD(TestGetModifiesByStmtNo) {
		vector<StmtData> list{
			StmtData(5, NodeType::VARIABLE, "greg")
		};
		Assert::IsTrue(table.getEntryByStmtNo(5) == list);
	}
	};
}
