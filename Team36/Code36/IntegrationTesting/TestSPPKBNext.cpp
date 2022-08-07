#include <algorithm>
#include "stdafx.h"
#include "CppUnitTest.h"

#include "SP/SourceProcessor.h"
#include "PKB/PKB.h"
#include "PKB/CFGProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	TEST_CLASS(IntegrationTestSPPKBNext) {
public:
	TEST_METHOD_INITIALIZE(setUp) {
		SourceProcessor sp;
		TNode* tree;
		vector<StmtNode*> cfgs;
		sp.processSource("procedure p { x = 2; if (a > 0) then { while (b == 0) { print c; } d = 1; } else { e = 0; } read f; }", tree, cfgs);
		CFGProcessor::processCFGs(cfgs);
	}

	TEST_METHOD_CLEANUP(tearDown) {
		PKB::clear();
	}

	TEST_METHOD(testCheckNext_shouldBeTrue) {
		bool actualData1 = PKB::checkNext(1, 2); // first and second stmt in program
		bool actualData2 = PKB::checkNext(2, 3); // if stmt and first stmt in then
		bool actualData3 = PKB::checkNext(2, 6); // if stmt and first stmt in else
		bool actualData4 = PKB::checkNext(4, 3); // last stmt in while and while stmt
		bool actualData5 = PKB::checkNext(5, 7); // last stmt in then and last stmt
		bool actualData6 = PKB::checkNext(6, 7); // last stmt in else and last stmt

		Assert::IsTrue(actualData1);
		Assert::IsTrue(actualData2);
		Assert::IsTrue(actualData3);
		Assert::IsTrue(actualData4);
		Assert::IsTrue(actualData5);
		Assert::IsTrue(actualData6);
	}

	TEST_METHOD(testCheckNext_shouldBeFalse) {
		bool actualData1 = PKB::checkNext(2, 1); // second and first stmt in program
		bool actualData2 = PKB::checkNext(4, 5); // last stmt in while and last stmt in then
		bool actualData3 = PKB::checkNext(5, 6); // last stmt in then and first stmt in else
		bool actualData4 = PKB::checkNext(3, 3); // while stmt and while stmt

		Assert::IsFalse(actualData1);
		Assert::IsFalse(actualData2);
		Assert::IsFalse(actualData3);
		Assert::IsFalse(actualData4);
	}

	TEST_METHOD(testCheckNextTrans_shouldBeTrue) {
		bool actualData1 = PKB::checkNextTrans(1, 3); // first stmt and first stmt in then
		bool actualData2 = PKB::checkNextTrans(1, 6); // first stmt and first stmt in else
		bool actualData3 = PKB::checkNextTrans(1, 7); // first stmt and last stmt
		bool actualData4 = PKB::checkNextTrans(4, 5); // last stmt in while and last stmt in then
		bool actualData5 = PKB::checkNextTrans(2, 7); // if stmt and last stmt
		bool actualData6 = PKB::checkNextTrans(3, 3); // while stmt and itself
		bool actualData7 = PKB::checkNextTrans(1, 2); // first stmt and second stmt in program (next is subset of next*)

		Assert::IsTrue(actualData1);
		Assert::IsTrue(actualData2);
		Assert::IsTrue(actualData3);
		Assert::IsTrue(actualData4);
		Assert::IsTrue(actualData5);
		Assert::IsTrue(actualData6);
		Assert::IsTrue(actualData7);
	}

	TEST_METHOD(testCheckNextTrans_shouldBeFalse) {
		bool actualData1 = PKB::checkNext(2, 1); // second and first stmt in program
		bool actualData2 = PKB::checkNext(4, 5); // last stmt in while and last stmt in then
		bool actualData3 = PKB::checkNext(5, 6); // last stmt in then and first stmt in else
		bool actualData4 = PKB::checkNext(4, 6); // last stmt in while and first stmt in else
		bool actualData5 = PKB::checkNext(4, 2); // last stmt in while and if stmt

		Assert::IsFalse(actualData1);
		Assert::IsFalse(actualData2);
		Assert::IsFalse(actualData3);
	}

	TEST_METHOD(testGetNext_ifAndStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNext(NodeType::IF, NodeType::STMT);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")}, // first stmt in then
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(6, NodeType::ASSIGN, "")}, // first stmt in else
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNext_whileAndStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNext(NodeType::WHILE, NodeType::STMT);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")}, // first stmt in while loop
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(5, NodeType::ASSIGN, "")}, // first stmt outside while loop in then
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNext_stmtAndWhile) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNext(NodeType::STMT, NodeType::WHILE);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")},// if stmt
			pair<StmtData, StmtData>{StmtData(4, NodeType::PRINT, ""), StmtData(3, NodeType::WHILE, "")}, // last stmt in while loop
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNext_stmtAndStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNext(NodeType::STMT, NodeType::STMT);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(2, NodeType::IF, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(4, NodeType::PRINT, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")},
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextTrans_ifAndRead) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextTrans(NodeType::IF, NodeType::READ);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(7, NodeType::READ, "")},
		};
		Assert::IsTrue(expectedData == actualData);
	}

	TEST_METHOD(testGetNextTrans_stmtAndPrint) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextTrans(NodeType::STMT, NodeType::PRINT);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(4, NodeType::PRINT, ""), StmtData(4, NodeType::PRINT, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextTrans_whileAndStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextTrans(NodeType::WHILE, NodeType::STMT);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(7, NodeType::READ, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextBefore_onlyStmtBefore) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBefore(2); // second stmt in program
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(2, NodeType::IF, "")}
		};
		Assert::IsTrue(expectedData == actualData);
	}

	TEST_METHOD(testGetNextBefore_whileStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBefore(3); // while stmt
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")}, // if stmt before it
			pair<StmtData, StmtData>{StmtData(4, NodeType::PRINT, ""), StmtData(3, NodeType::WHILE, "")}, // last stmt of while loop
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextBefore_stmtFollowingIf) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBefore(7); // stmt that follows if stmt
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")}, // last stmt of then
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")}, // last stmt of else
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextBefore_firstStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBefore(1); // first stmt
		vector<pair<StmtData, StmtData>> expectedData = {}; // should be empty
		Assert::IsTrue(expectedData == actualData);
	}

	TEST_METHOD(testGetNextBefore_readStmts) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBefore(NodeType::READ); // read stmt type
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")}, // last stmt of then
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")}, // last stmt of else
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextBefore_assignStmts) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBefore(NodeType::ASSIGN); // assign stmt type
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(5, NodeType::ASSIGN, "")}, // last stmt of then
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(6, NodeType::ASSIGN, "")}, // last stmt of else
		}; // stmt 1 is an assign stmt but does not have any stmt before it			

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextBefore_allStmts) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBefore(NodeType::STMT); // stmt type
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(2, NodeType::IF, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(4, NodeType::PRINT, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")},
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextBeforeTrans_whileStmtNo) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBeforeTrans(3); // while stmt
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(4, NodeType::PRINT, ""), StmtData(3, NodeType::WHILE, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextBeforeTrans_printStmtType) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextBeforeTrans(NodeType::PRINT);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(4, NodeType::PRINT, ""), StmtData(4, NodeType::PRINT, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextAfter_firstStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfter(1); // first stmt
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(2, NodeType::IF, "")},
		};
		Assert::IsTrue(expectedData == actualData);
	}

	TEST_METHOD(testGetNextAfter_ifStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfter(2); // if stmt
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")}, // first stmt in then
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(6, NodeType::ASSIGN, "")}, // first stmt in else
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextAfter_whileStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfter(3); // while stmt
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")}, // first stmt in while loop
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(5, NodeType::ASSIGN, "")}, // stmt that follows while
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextAfter_lastStmt) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfter(7); // last stmt
		vector<pair<StmtData, StmtData>> expectedData = {}; // should be empty
		Assert::IsTrue(expectedData == actualData);
	}

	TEST_METHOD(testGetNextAfter_readStmts) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfter(NodeType::READ); // read stmt type
		vector<pair<StmtData, StmtData>> expectedData = {}; // no stmts after read stmt
		Assert::IsTrue(expectedData == actualData);
	}

	TEST_METHOD(testGetNextAfter_assignStmts) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfter(NodeType::ASSIGN); // assign stmt type
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(2, NodeType::IF, "")},
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")},
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextAfter_allStmts) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfter(NodeType::STMT); // stmt type
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(2, NodeType::IF, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(4, NodeType::PRINT, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")},
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(7, NodeType::READ, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextAfterTrans_ifStmtNo) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfterTrans(2);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::IF, ""), StmtData(7, NodeType::READ, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}

	TEST_METHOD(testGetNextAfterTrans_whileStmtType) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getNextAfterTrans(NodeType::WHILE);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(3, NodeType::WHILE, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(4, NodeType::PRINT, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(3, NodeType::WHILE, ""), StmtData(7, NodeType::READ, "")},
		};

		Assert::AreEqual(expectedData.size(), actualData.size());
		vector<pair<StmtData, StmtData>> matchingData;
		for (pair<StmtData, StmtData> expectedP : expectedData) {
			for (pair<StmtData, StmtData> actualP : actualData) {
				if (actualP == expectedP) {
					matchingData.push_back(actualP);
					break;
				}
			}
		}
		Assert::IsTrue(expectedData == matchingData);
	}
	};
}
