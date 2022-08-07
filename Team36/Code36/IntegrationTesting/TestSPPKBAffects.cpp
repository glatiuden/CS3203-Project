#include <algorithm>
#include "stdafx.h"
#include "CppUnitTest.h"

#include "SP/SourceProcessor.h"
#include "PKB/PKB.h"
#include "PKB/ASTProcessor.h"
#include "PKB/CFGProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting {
	TEST_CLASS(IntegrationTestSPPKBAffects) {
public:
	TEST_METHOD_INITIALIZE(setUp) {
		SourceProcessor sp;
		TNode* tree;
		vector<StmtNode*> cfgs;
		sp.processSource("procedure p { a = 2; b = 1; if (a > c) then { c = a - b; } else { b = a - 1; c = a + b + c;  } while (d == 1) { d = d - 1; c = c + 1; e = e + f; call q; k = c + 1; e = e + 1; if (f > 0) then { g = h + 1; } else { h = g - 1; } } i = a * 2; } procedure q { if (c > 0) then { c = 0; } else { print j; } }", tree, cfgs);
		/*
		procedure p {
		1	a = 2;
		2	b = 1;
		3	if (a > c) then {
		4		c = a - b;
			} else {
		5		b = a - 1;
		6		c = a + b + c;
			}

		7	while (d == 1) {
		8		d = d - 1;
		9		c = c + 1;
		10		e = e + f;
		11		call q;
		12		k = c + 1;
		13		e = e + 1;
		14		if (f > 0) then {
		15			g = h + 1;
				} else {
		16			h = g - 1;
				}
			}

		17	i = a * 2;
		}

		procedure q {
		18	if (c > 0) then {
		19		c = 0;
			} else {
		20		print j;
			}
		}
		*/
		ASTProcessor::processAST(tree);
		CFGProcessor::processCFGs(cfgs);
	}

	TEST_METHOD_CLEANUP(tearDown) {
		PKB::clear();
	}

	TEST_METHOD(testCheckAffects_shouldBeTrue) {
		bool actualData1 = PKB::checkAffects(1, 4); // stmt in then clause
		bool actualData2 = PKB::checkAffects(1, 6); // stmt in else clause
		bool actualData3 = PKB::checkAffects(1, 17); // last stmt in procedure
		bool actualData4 = PKB::checkAffects(5, 6); // within else clause
		bool actualData5 = PKB::checkAffects(8, 8); // affects itself
		bool actualData6 = PKB::checkAffects(4, 9); // afterStmt affected by stmt in then clause
		bool actualData7 = PKB::checkAffects(6, 9); // afterStmt affected by stmt in else clause
		bool actualData8 = PKB::checkAffects(15, 16); // affects stmt in other clause
		bool actualData9 = PKB::checkAffects(16, 15); // affects stmt in other clause

		Assert::IsTrue(actualData1);
		Assert::IsTrue(actualData2);
		Assert::IsTrue(actualData3);
		Assert::IsTrue(actualData4);
		Assert::IsTrue(actualData5);
		Assert::IsTrue(actualData6);
		Assert::IsTrue(actualData7);
		Assert::IsTrue(actualData8);
		Assert::IsTrue(actualData9);
	}

	TEST_METHOD(testCheckAffects_shouldBeFalse) {
		bool actualData1 = PKB::checkAffects(1, 2); // no common variable
		bool actualData2 = PKB::checkAffects(1, 19); // stmt in another procedure
		bool actualData3 = PKB::checkAffects(2, 1); // afterStmt comes before beforeStmt
		bool actualData4 = PKB::checkAffects(2, 6); // have path, have var, but var is modified by stmt 5
		bool actualData5 = PKB::checkAffects(4, 6); // stmt in then and stmt in else
		bool actualData6 = PKB::checkAffects(6, 6); // no loop for stmt to affect itself
		bool actualData7 = PKB::checkAffects(4, 12); // have path, have var, but var is modified by calls q

		Assert::IsFalse(actualData1);
		Assert::IsFalse(actualData2);
		Assert::IsFalse(actualData3);
		Assert::IsFalse(actualData4);
		Assert::IsFalse(actualData5);
		Assert::IsFalse(actualData6);
		Assert::IsFalse(actualData7);
	}

	TEST_METHOD(testCheckAffectsTransOnNonTrans_shouldBeTrue) {
		// repeated data from checkAffects
		bool actualData1 = PKB::checkAffectsTrans(1, 4); // stmt in then clause
		bool actualData2 = PKB::checkAffectsTrans(1, 6); // stmt in else clause
		bool actualData3 = PKB::checkAffectsTrans(1, 17); // last stmt in procedure
		bool actualData4 = PKB::checkAffectsTrans(5, 6); // within else clause
		bool actualData5 = PKB::checkAffectsTrans(8, 8); // affects itself
		bool actualData6 = PKB::checkAffectsTrans(4, 9); // afterStmt affected by stmt in then clause
		bool actualData7 = PKB::checkAffectsTrans(6, 9); // afterStmt affected by stmt in else clause
		bool actualData8 = PKB::checkAffectsTrans(15, 16); // affects stmt in other clause
		bool actualData9 = PKB::checkAffectsTrans(16, 15); // affects stmt in other clause

		Assert::IsTrue(actualData1);
		Assert::IsTrue(actualData2);
		Assert::IsTrue(actualData3);
		Assert::IsTrue(actualData4);
		Assert::IsTrue(actualData5);
		Assert::IsTrue(actualData6);
		Assert::IsTrue(actualData7);
		Assert::IsTrue(actualData8);
		Assert::IsTrue(actualData9);
	}

	TEST_METHOD(testCheckAffectsTrans_shouldBeTrue) {
		bool actualData1 = PKB::checkAffectsTrans(1, 9); // (1, 4) (4, 9) or (1, 6) (6, 9)
		bool actualData2 = PKB::checkAffectsTrans(5, 9); // (5, 6) (6, 9)
		bool actualData3 = PKB::checkAffectsTrans(15, 15); // (15, 16) (16, 15)
		bool actualData4 = PKB::checkAffectsTrans(16, 16); // (16, 15) (15, 16) 
		bool actualData5 = PKB::checkAffectsTrans(2, 9); // (2, 4) (4, 9)

		Assert::IsTrue(actualData1);
		Assert::IsTrue(actualData2);
		Assert::IsTrue(actualData3);
		Assert::IsTrue(actualData4);
		Assert::IsTrue(actualData5);
	}

	TEST_METHOD(testCheckAffectsTransOnNonTrans_shouldBeFalse) {
		// repeated data from checkAffects
		bool actualData1 = PKB::checkAffectsTrans(1, 2); // no common variable
		bool actualData2 = PKB::checkAffectsTrans(1, 19); // stmt in another procedure
		bool actualData3 = PKB::checkAffectsTrans(2, 1); // afterStmt comes before beforeStmt
		bool actualData4 = PKB::checkAffectsTrans(2, 6); // have path, have var, but var is modified by stmt 5
		bool actualData5 = PKB::checkAffectsTrans(4, 6); // stmt in then and stmt in else
		bool actualData6 = PKB::checkAffectsTrans(6, 6); // no loop for stmt to affect itself
		bool actualData7 = PKB::checkAffectsTrans(4, 12); // have path, have var, but var is modified by calls q

		Assert::IsFalse(actualData1);
		Assert::IsFalse(actualData2);
		Assert::IsFalse(actualData3);
		Assert::IsFalse(actualData4);
		Assert::IsFalse(actualData5);
		Assert::IsFalse(actualData6);
		Assert::IsFalse(actualData7);
	}

	TEST_METHOD(testCheckAffectsTrans_shouldBeFalse) {
		bool actualData1 = PKB::checkAffectsTrans(2, 19);
		bool actualData2 = PKB::checkAffectsTrans(4, 5);
		bool actualData3 = PKB::checkAffectsTrans(1, 1);

		Assert::IsFalse(actualData1);
		Assert::IsFalse(actualData2);
		Assert::IsFalse(actualData3);
	}
	TEST_METHOD(testCheckIfAffectsExists_shouldBeTrue) {
		Assert::IsTrue(PKB::checkIfAffectsExists());
	}

	TEST_METHOD(testGetAffectsBefore_lastStmtInProc) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBefore(17); // last stmt in proc uses "a" that is modified by first stmt in proc
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(17, NodeType::ASSIGN, "")}, // stmt modifies "a"
		};

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsBefore_stmtWithMultipleVars) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBefore(4); // stmt in then clause that uses  "a" and "b"
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(4, NodeType::ASSIGN, "")}, // stmt modifies "a"
			pair<StmtData, StmtData>{StmtData(2, NodeType::ASSIGN, ""), StmtData(4, NodeType::ASSIGN, "")}, // stmt modifies "b"
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

	TEST_METHOD(testGetAffectsBefore_stmtWithSingleVarButAffectedByBranches) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBefore(9); // stmt uses "c" which is modified in both clauses of if stmt 3
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(4, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")}, // stmt in then clause
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")}, // stmt else clause
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

	TEST_METHOD(testGetAffectsBefore_stmtInLoop) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBefore(10);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(13, NodeType::ASSIGN, ""), StmtData(10, NodeType::ASSIGN, "")}, // stmt after it can affect due to loop
		};

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsBefore_stmtThatUsesAndModifiesSameVarNotInLoop) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBefore(6); // stmt uses and modifies "c" but is not in a loop
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(6, NodeType::ASSIGN, "")}, // stmt in then clause
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(6, NodeType::ASSIGN, "")}, // stmt else clause
		};
		// stmt 6 does not affect itself

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

	TEST_METHOD(testGetAffectsBefore_stmtThatUsesAndModifiesSameVarInLoop) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBefore(8); // stmt uses and modifies "d" and is in a loop
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(8, NodeType::ASSIGN, ""), StmtData(8, NodeType::ASSIGN, "")}, // stmt affects itself
		};

		Assert::IsTrue(actualData == expectedData);

		actualData = PKB::getAffectsBefore(13); // stmt uses and modifies "e" and is in a loop
		expectedData = {
			pair<StmtData, StmtData>{StmtData(10, NodeType::ASSIGN, ""), StmtData(13, NodeType::ASSIGN, "")}
		}; // stmt does not affect itself as "e" is modified by stmt 10 along the loop

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsBefore_firstStmtInProc) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBefore(1);
		vector<pair<StmtData, StmtData>> expectedData = {}; // no stmt before it to affect it

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsBefore_onlyAssignStmtInProc) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBefore(19);
		vector<pair<StmtData, StmtData>> expectedData = {}; // no other assign stmt to affect it

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsAfter_firstStmtInProc) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsAfter(1);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(4, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(17, NodeType::ASSIGN, "")},
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

	TEST_METHOD(testGetAffectsAfter_stmtInLoop) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsAfter(13);
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(13, NodeType::ASSIGN, ""), StmtData(10, NodeType::ASSIGN, "")}, // it can affect a stmt before it due to loop
		};

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsAfter_stmtThatUsesAndModifiesSameVarNotInLoop) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsAfter(6); // stmt uses and modifies "c" but is not in a loop
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")}, // stmt in then clause
		};
		// stmt 6 does not affect itself

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsAfter_stmtThatUsesAndModifiesSameVarInLoop) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsAfter(8); // stmt uses and modifies "d" and is in a loop
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(8, NodeType::ASSIGN, ""), StmtData(8, NodeType::ASSIGN, "")}, // stmt affects itself
		};

		Assert::IsTrue(actualData == expectedData);

		actualData = PKB::getAffectsAfter(13); // stmt uses and modifies "e" and is in a loop
		expectedData = {
			pair<StmtData, StmtData>{StmtData(13, NodeType::ASSIGN, ""), StmtData(10, NodeType::ASSIGN, "")}
		}; // stmt does not affect itself as "e" is modified by stmt 10 along the loop

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsAfter_lastStmtInProc) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsAfter(17);
		vector<pair<StmtData, StmtData>> expectedData = {}; // no stmt after it to affect it

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsAfter_onlyAssignStmtInProc) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsAfter(19);
		vector<pair<StmtData, StmtData>> expectedData = {}; // no other assign stmt to affect it

		Assert::IsTrue(actualData == expectedData);
	}

	TEST_METHOD(testGetAffectsBeforeTrans) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsBeforeTrans(9); // stmt in else clause that uses  "a" and "b"
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(4, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
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

	TEST_METHOD(testGetAffectsAfterTrans) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAffectsAfterTrans(1); // stmt in else clause that uses  "a" and "b"
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(4, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(17, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
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

	TEST_METHOD(testGetAllAffects) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAllAffects(); // stmt in else clause that uses  "a" and "b"
		vector<pair<StmtData, StmtData>> expectedData = {
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(4, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(17, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::ASSIGN, ""), StmtData(4, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(4, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(8, NodeType::ASSIGN, ""), StmtData(8, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(10, NodeType::ASSIGN, ""), StmtData(13, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(13, NodeType::ASSIGN, ""), StmtData(10, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(15, NodeType::ASSIGN, ""), StmtData(16, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(16, NodeType::ASSIGN, ""), StmtData(15, NodeType::ASSIGN, "")},
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

	TEST_METHOD(testGetAllAffectsTrans) {
		vector<pair<StmtData, StmtData>> actualData = PKB::getAllAffectsTrans(); // stmt in else clause that uses  "a" and "b"
		vector<pair<StmtData, StmtData>> expectedData = {
			// Original
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(4, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(5, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(17, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::ASSIGN, ""), StmtData(4, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(4, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(6, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(6, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(8, NodeType::ASSIGN, ""), StmtData(8, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(10, NodeType::ASSIGN, ""), StmtData(13, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(13, NodeType::ASSIGN, ""), StmtData(10, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(15, NodeType::ASSIGN, ""), StmtData(16, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(16, NodeType::ASSIGN, ""), StmtData(15, NodeType::ASSIGN, "")},

			// Trans
			pair<StmtData, StmtData>{StmtData(1, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(2, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(5, NodeType::ASSIGN, ""), StmtData(9, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(10, NodeType::ASSIGN, ""), StmtData(10, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(13, NodeType::ASSIGN, ""), StmtData(13, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(15, NodeType::ASSIGN, ""), StmtData(15, NodeType::ASSIGN, "")},
			pair<StmtData, StmtData>{StmtData(16, NodeType::ASSIGN, ""), StmtData(16, NodeType::ASSIGN, "")},
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