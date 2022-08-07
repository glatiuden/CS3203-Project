#include <string>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Lexer.h"
#include "CustomException.h"
#include "QPS/QPSPreprocessor.h"

#define MSG(msg) [&]{ std::wstringstream _s; _s << "Case " << i+1 << " failed"; return _s.str(); }().c_str()

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQPSPreprocessor) {
public:
	TEST_METHOD(TestDeclaration_valid) {

		std::string testcases[] = {
			// Test all possible DESIGN_ENTITIES as declaration.
			"stmt s;\nSelect s such that Modifies(s, \"i\")",
			"read re;\nSelect re such that Modifies(re, \"i\")",
			"print pn;\nSelect pn such that Modifies(pn, \"i\")",
			"while w;\nSelect w such that Modifies(w, \"i\")",
			"if ifs;\nSelect ifs such that Modifies(ifs, \"i\")",
			"assign a;\nSelect a such that Modifies(a, \"i\")",
			"variable v;\nSelect v such that Modifies(v, \"i\")",
			"constant c;\nSelect c such that Modifies(c, \"i\")",
			"procedure p;\nSelect p such that Modifies(p, \"i\")",
			// Test edge case synonym after DESIGN_ENTITIES
			"procedure Select;\nSelect Select such that Modifies(Select, \"i\")",
			"procedure procedure;\nSelect procedure such that Modifies(procedure, \"i\")",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++) {
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
		}
	}
	TEST_METHOD(TestDeclaration_invalid) {
		std::string testcases[] = {
			// Test invalid DESING_ENTITIES
			"error p;\nSelect s such that Modifies(s, \"i\")",
			// Test invalid synonyms after DESIGN_ENTITIES
			"stmt 12;\nSelect s such that Modifies(s, \"i\")",
			"stmt +-;\nSelect s such that Modifies(s, \"i\")",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestDeclaration_valid_multipleDeclaration) {
		// Test multiple declarations
		std::string testcases[] = {
			"stmt s; procedure p;\nSelect s such that Modifies(s, \"i\")",
			"stmt s; procedure p; constant c;\nSelect s such that Modifies(s, \"i\")",
			"stmt s, s1; \nSelect s such that Modifies(s, \"i\")",
			"stmt s, s1; procedure p;\nSelect s such that Modifies(s, \"i\")",
			"stmt s, s1; procedure p; constant c, c1;\nSelect s such that Modifies(s, \"i\")",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestDeclaration_invalid_multipleDeclaration) {
		std::string testcases[] = {
			// Test invalid multiple declarations
			"stmt , s1;\nSelect s such that Modifies(s, \"i\")",
			"stmt s, ;\nSelect s such that Modifies(s, \"i\")",
			"stmt s; procedure ;\nSelect s such that Modifies(s, \"i\")",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestSelectQuery_valid) {

		std::string testcases[] = {
			"stmt s;\nSelect s such that Modifies(s, \"i\")",
			"stmt s;\nSelect s such that Modifies(s, \"i\")            ",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestSelectQuery_invalid) {
		std::string testcases[] = {
			// Test invalid Select keyword
			"stmt s;\nselect s such that Modifies(s, \"i\")",
			"stmt s;\nprocedure s such that Modifies(s, \"i\")",

			// Test invalid synonym after Select
			"stmt s;\nSelect such that Modifies(s, \"i\")",
			"stmt s;\nSelect 123 such that Modifies(s, \"i\")",
			"stmt s;\nSelect +- such that Modifies(s, \"i\")",
			"stmt s;\nSelect s, p such that Modifies(s, \"i\")",

			// Test invalid suchthat-cl
			"stmt s;\nSelect s suchs that Modifies(s, \"i\")",
			"stmt s;\nSelect s suchthat Modifies(s, \"i\")",
			"stmt s;\nSelect s such-that Modifies(s, \"i\")",
			"stmt s;\nSelect s Modifies(s, \"i\")",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestResultClause_valid) {

		std::string testcases[] = {
			"stmt s; variable v;\nSelect BOOLEAN such that Uses(s, v)",
			"stmt BOOLEAN; variable v;\nSelect BOOLEAN such that Uses(BOOLEAN, v)",
			"stmt BOOLEAN; variable v;\nSelect BOOLEAN.stmt# such that Uses(BOOLEAN, v)",
			"stmt s; variable v;\nSelect s such that Uses(s, v)",
			"stmt s; variable v;\nSelect s.stmt# such that Uses(s, v)",
			"stmt s; variable v;\nSelect v.varName such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s, v> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s.stmt#, v> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s.stmt#, v.varName> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s,v,s,v,s> such that Uses(s, v)",
			"stmt s, BOOLEAN; variable v;\nSelect <BOOLEAN,s,v> such that Uses(s, v)",
			"stmt s, BOOLEAN; variable v;\nSelect <s,v,BOOLEAN,v,s> such that Uses(s, v)",
			"stmt s, boolean; variable v;\nSelect <s,v,boolean,v,s> such that Uses(s, v)",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestResultClause_invalid) {
		std::string testcases[] = {
			"stmt s; variable v;\nSelect s,s such that Uses(s, v)",
			"stmt s; variable v;\nSelect s.stmt such that Uses(s, v)",
			"stmt s; variable v;\nSelect v.name such that Uses(s, v)",
			"stmt s; variable v;\nSelect < such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s, such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s,> such that Uses(s, v)",
			"stmt s; variable v;\nSelect s,s> such that Uses(s, v)",
			"stmt s; variable v;\nSelect ,s> such that Uses(s, v)",
			"stmt s; variable v;\nSelect s> such that Uses(s, v)",
			"stmt s; variable v;\nSelect > such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s.stmt> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s.stmt#, v.name> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s, v.name> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <BOOLEAN> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <BOOLEAN, s, v> such that Uses(s, v)",
			"stmt s; variable v;\nSelect <s, v, BOOLEAN> such that Uses(s, v)",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestFollowsQuery_valid) {
		// Test valid Follows & Follows*
		std::string testcases[] = {
			"stmt s;\nSelect s such that Follows(s, s)",
			"stmt s;\nSelect s such that Follows(_, s)",
			"stmt s;\nSelect s such that Follows(1, s)",
			"stmt s;\nSelect s such that Follows*(s, s)",
			"stmt s;\nSelect s such that Follows*(_, s)",
			"stmt s;\nSelect s such that Follows*(1, s)",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestFollowsQuery_invalid) {
		std::string testcases[] = {
			// Test invalid Follows & Follows*
			"stmt s;\nSelect s such that Follow(s, s)",
			"stmt s;\nSelect s such that follows(s, s)",
			"stmt s;\nSelect s such that Follow*(s, s)",
			"stmt s;\nSelect s such that follows*(s, s)",
			// Test invalid Follows & Follows* stmtRef
			"stmt s;\nSelect s such that Follows(\"s\", s)",
			"stmt s;\nSelect s such that Follows(s, \"s\")",
			"stmt s;\nSelect s such that Follows*(\"s\", s)",
			"stmt s;\nSelect s such that Follows*(s, \"s\")",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestParentQuery_valid) {
		std::string testcases[] = {
			// Test valid Parent & Parent*
			"stmt s;\nSelect s such that Parent(s, s)",
			"stmt s;\nSelect s such that Parent(_, s)",
			"stmt s;\nSelect s such that Parent(1, s)",
			"stmt s;\nSelect s such that Parent*(s, s)",
			"stmt s;\nSelect s such that Parent*(_, s)",
			"stmt s;\nSelect s such that Parent*(1, s)",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestParentQuery_invalid) {

		std::string testcases[] = {
			// Test invalid Parent & Parent*
			"stmt s;\nSelect s such that Parents(s, s)",
			"stmt s;\nSelect s such that parent(s, s)",
			"stmt s;\nSelect s such that Parents*(s, s)",
			"stmt s;\nSelect s such that parent*(s, s)",

			// Test invalid Parent & Parent* stmtRef
			"stmt s;\nSelect s such that Parent(\"s\", s)",
			"stmt s;\nSelect s such that Parent(s, \"s\")",
			"stmt s;\nSelect s such that Parent*(\"s\", s)",
			"stmt s;\nSelect s such that Parent*(s, \"s\")"
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestUsesQuery_valid) {
		std::string testcases[] = {
			// Test valid Uses stmtRef & entRef
			"stmt s; variable v;\nSelect s such that Uses(s, _)",
			"stmt s; variable v;\nSelect s such that Uses(s, v)",
			"stmt s; variable v;\nSelect s such that Uses(s, \"i\")",
			"stmt s; variable v;\nSelect s such that Uses(_, v)",
			"stmt s; variable v;\nSelect s such that Uses(_, _)",
			"stmt s; variable v;\nSelect s such that Uses(_, \"i\")",
			"stmt s; variable v;\nSelect s such that Uses(123, v)",
			"stmt s; variable v;\nSelect s such that Uses(123, _)",
			"stmt s; variable v;\nSelect s such that Uses(123, \"i\")",
			"stmt s; variable v;\nSelect s such that Uses(\"i\", v)",
			"stmt s; variable v;\nSelect s such that Uses(\"i\", _)",
			"stmt s; variable v;\nSelect s such that Uses(\"i\", \"i\")",
			// syntactically valid but semantically invalid
			"stmt s; assign a;\nSelect s such that Uses(s, a)",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestUsesQuery_invalid) {

		std::string testcases[] = {
			// Test invalid Uses relRef
			"stmt s;\nSelect s such that uses(s, \"i\")",
			"stmt s;\nSelect s such that Use(s, \"i\")",
			"stmt s;\nSelect s such that Uses*(s, \"i\")",
			"stmt s;\nSelect s such that (s, \"i\")",
			"stmt s;\nSelect s such that Uses s, \"i\")",
			"stmt s;\nSelect s such that Uses(s \"i\")",
			"stmt s;\nSelect s such that Uses(s, i\")",
			"stmt s;\nSelect s such that Uses(s, __)",
			"stmt s;\nSelect s such that Uses(s, 123)",
			"stmt s;\nSelect s such that Uses(s, \"i\"",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestModifiesQuery_valid) {
		std::string testcases[] = {
			// Test valid Modifies stmtRef & entRef
			"stmt s; variable v;\nSelect s such that Modifies(s, _)",
			"stmt s; variable v;\nSelect s such that Modifies(s, v)",
			"stmt s; variable v;\nSelect s such that Modifies(s, \"i\")",
			"stmt s; variable v;\nSelect s such that Modifies(_, v)",
			"stmt s; variable v;\nSelect s such that Modifies(_, _)",
			"stmt s; variable v;\nSelect s such that Modifies(_, \"i\")",
			"stmt s; variable v;\nSelect s such that Modifies(123, v)",
			"stmt s; variable v;\nSelect s such that Modifies(123, _)",
			"stmt s; variable v;\nSelect s such that Modifies(123, \"i\")",
			"stmt s; variable v;\nSelect s such that Modifies(\"i\", v)",
			"stmt s; variable v;\nSelect s such that Modifies(\"i\", _)",
			"stmt s; variable v;\nSelect s such that Modifies(\"i\", \"i\")",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestModifiesQuery_invalid) {
		std::string testcases[] = {
			// Test invalid Modifies relRef
			"stmt s;\nSelect s such that modifies(s, \"i\")",
			"stmt s;\nSelect s such that Modify(s, \"i\")",
			"stmt s;\nSelect s such that Modifies*(s, \"i\")",
			"stmt s;\nSelect s such that (s, \"i\")",
			"stmt s;\nSelect s such that Modifies s, \"i\")",
			"stmt s;\nSelect s such that Modifies(s \"i\")",
			"stmt s;\nSelect s such that Modifies(s, i\")",
			"stmt s;\nSelect s such that Modifies(s, __)",
			"stmt s;\nSelect s such that Modifies(s, 123)",
			"stmt s;\nSelect s such that Modifies(s, \"i\"",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestPatternQuery_valid) {
		std::string testcases[] = {
			// Valid pattern clauses
			"assign a; variable v;\nSelect a pattern a (v, _)",
			"assign a; variable v;\nSelect a pattern a (_, _)",
			"assign a; variable v;\nSelect a pattern a (\"v\", _)",
			"assign a; variable v;\nSelect a pattern a (v, _\"x\"_)",
			"assign a; variable v;\nSelect a pattern a (_, _\"x\"_)",
			"assign a; variable v;\nSelect a pattern a (\"v\", _\"x\"_)",
			"assign a; variable v;\nSelect a pattern a (v, _\"1\"_)",
			"assign a; variable v;\nSelect a pattern a (_, _\"2\"_)",
			"assign a; variable v;\nSelect a pattern a (\"v\", _\"3\"_)",
			"assign a; variable v;\nSelect a pattern a (v, \"x + y\")",
			"assign a; variable v;\nSelect a pattern a (_, \"x + y\")",
			"assign a; variable v;\nSelect a pattern a (\"v\", \"x + y\")",
			"assign a; variable v;\nSelect a pattern a (v, _\"x + y\"_)",
			"assign a; variable v;\nSelect a pattern a (_, _\"x + y\"_)",
			"assign a; variable v;\nSelect a pattern a (\"v\", _\"x + y\"_)",
			"while w; variable v;\n Select w pattern w (v, _)",
			"while w; variable v;\n Select w pattern w (_, _)",
			"while w; variable v;\n Select w pattern w (\"v\", _)",
			"if ifs; variable v;\n Select ifs pattern ifs (v, _, _)",
			"if ifs; variable v;\n Select ifs pattern ifs (_, _, _)",
			"if ifs; variable v;\n Select ifs pattern ifs (\"v\", _, _)",
			// Syntax valid | Semantic Invalid
			"assign a; variable v;\nSelect a pattern a (v, _, _)",
			"while w; variable v;\n Select w pattern w (v, _, _)",
			"if ifs; variable v;\n Select ifs pattern ifs (v, _)",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestPatternQuery_invalid) {
		std::string testcases[] = {
			// Invalid pattern clause (QPSValidatorException)
			"assign a; variable v;\nSelect a patterns a (v, _)",
			"assign a; variable v;\nSelect a pattern a (123, _)",
			"assign a; variable v;\nSelect a pattern a (v, __)",
			"assign a; variable v;\nSelect a pattern a (v, _x\"_)",
			"assign a; variable v;\nSelect a pattern a (v, _\"\"_)",
			"assign a; variable v;\nSelect a pattern a (v, _\"x_)",
			"assign a; variable v;\nSelect a pattern a (v, _\"x\")",
			"assign a; variable v;\nSelect a pattern a (v, _",
			"assign a; variable v;\nSelect a pattern a (v, \"x _ y\")",
			"assign a; variable v;\nSelect a pattern a (v, _\"x _ y\"_)",
			"while w; variable v;\n Select w pattern w (123, _)",
			"while w; variable v;\n Select w pattern w (v, \"x\")",
			"while w; variable v;\n Select w pattern w (v, \"x + y\")",
			"while w; variable v;\n Select w pattern w (v, _\"x\"_)",
			"while w; variable v;\n Select w pattern w (v, _\"x + y\"_)",
			"if ifs; variable v;\n Select ifs pattern ifs (123, _, _)",
			"if ifs; variable v;\n Select ifs pattern ifs (v, _, \"x\")",
			"if ifs; variable v;\n Select ifs pattern ifs (v, _, \"x + y\")",
			"if ifs; variable v;\n Select ifs pattern ifs (v, _, _\"x\"_)",
			"if ifs; variable v;\n Select ifs pattern ifs (v, _, _\"x + y\"_)",
			"if ifs; variable v;\n Select ifs pattern ifs (v, \"x\", _)",
			"if ifs; variable v;\n Select ifs pattern ifs (v, \"x + y\", _)",
			"if ifs; variable v;\n Select ifs pattern ifs (v, _\"x\"_, _)",
			"if ifs; variable v;\n Select ifs pattern ifs (v, _\"x + y\", _)",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestCallsQuery_valid) {
		std::string testcases[] = {
			"procedure p, q;\nSelect p such that Calls(p, q)",
			"procedure p, q;\nSelect p such that Calls(p, \"name\")",
			"procedure p, q;\nSelect p such that Calls(\"name\", q)",
			"procedure p, q;\nSelect p such that Calls(\"name\", \"name\")",
			"procedure p, q;\nSelect p such that Calls*(p, q)",
			"procedure p, q;\nSelect p such that Calls*(p, \"name\")",
			"procedure p, q;\nSelect p such that Calls*(\"name\", q)",
			"procedure p, q;\nSelect p such that Calls*(\"name\", \"name\")",
			"stmt s; procedure p;\nSelect p such that Calls(s, p)",
			"stmt s; procedure p;\nSelect p such that Calls(p, s)",
			"stmt s; procedure p;\nSelect p such that Calls*(s, p)",
			"stmt s; procedure p;\nSelect p such that Calls*(p, s)",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestCallsQuery_invalid) {
		std::string testcases[] = {
			"procedure p, q;\nSelect p such that Call(p, q)",
			"procedure p, q;\nSelect p such that Calls(123, q)",
			"procedure p, q;\nSelect p such that Calls(p, 123)",
			"procedure p, q;\nSelect p such that Calls(123, \"name\")",
			"procedure p, q;\nSelect p such that Calls(\"name\", 123)",
			"procedure p, q;\nSelect p such that Call*(p, q)",
			"procedure p, q;\nSelect p such that Calls*(123, q)",
			"procedure p, q;\nSelect p such that Calls*(p, 123)",
			"procedure p, q;\nSelect p such that Calls*(123, \"name\")",
			"procedure p, q;\nSelect p such that Calls*(\"name\", 123)"
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestNextQuery_valid) {
		std::string testcases[] = {
			"stmt s;\nSelect s such that Next(s, s)",
			"stmt s;\nSelect s such that Next(s, _)",
			"stmt s;\nSelect s such that Next(s, 123)",
			"stmt s;\nSelect s such that Next(_, s)",
			"stmt s;\nSelect s such that Next(_, _)",
			"stmt s;\nSelect s such that Next(_, 123)",
			"stmt s;\nSelect s such that Next*(123, s)",
			"stmt s;\nSelect s such that Next*(123, _)",
			"stmt s;\nSelect s such that Next*(123, 123)",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestNextQuery_invalid) {
		std::string testcases[] = {
			"stmt s;\nSelect s such that Nexts(s, s)",
			"stmt s;\nSelect s such that next(s, s)",
			"stmt s;\nSelect s such that Nexts*(s, s)",
			"stmt s;\nSelect s such that next*(s, s)",
			"stmt s;\nSelect s such that Next(\"s\", s)",
			"stmt s;\nSelect s such that Next(s, \"s\")",
			"stmt s;\nSelect s such that Next*(\"s\", s)",
			"stmt s;\nSelect s such that Next*(s, \"s\")",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestAffectsQuery_valid) {
		// Test valid Follows & Follows*
		std::string testcases[] = {
			"stmt s;\nSelect s such that Affects(s, s)",
			"stmt s;\nSelect s such that Affects(s, _)",
			"stmt s;\nSelect s such that Affects(s, 123)",
			"stmt s;\nSelect s such that Affects(_, s)",
			"stmt s;\nSelect s such that Affects(_, _)",
			"stmt s;\nSelect s such that Affects(_, 123)",
			"stmt s;\nSelect s such that Affects(123, s)",
			"stmt s;\nSelect s such that Affects(123, _)",
			"stmt s;\nSelect s such that Affects(123, 123)",
			"stmt s;\nSelect s such that Affects*(s, s)",
			"stmt s;\nSelect s such that Affects*(s, _)",
			"stmt s;\nSelect s such that Affects*(s, 123)",
			"stmt s;\nSelect s such that Affects*(_, s)",
			"stmt s;\nSelect s such that Affects*(_, _)",
			"stmt s;\nSelect s such that Affects*(_, 123)",
			"stmt s;\nSelect s such that Affects*(123, s)",
			"stmt s;\nSelect s such that Affects*(123, _)",
			"stmt s;\nSelect s such that Affects*(123, 123)",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestAffectsQuery_invalid) {
		std::string testcases[] = {
			// Test invalid Follows & Follows*
			"stmt s;\nSelect s such that Affect(s, s)",
			"stmt s;\nSelect s such that affects(s, s)",
			"stmt s;\nSelect s such that Affect*(s, s)",
			"stmt s;\nSelect s such that affects*(s, s)",
			// Test invalid Follows & Follows* stmtRef
			"stmt s;\nSelect s such that Affects(\"s\", s)",
			"stmt s;\nSelect s such that Affects(s, \"s\")",
			"stmt s;\nSelect s such that Affects*(\"s\", s)",
			"stmt s;\nSelect s such that Affects*(s, \"s\")",
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestWithAttrNameQuery_valid) {
		std::string testcases[] = {
			// Valid attrName and synonym.attrName
			"procedure p; Select p with p.procName = p.procName",
			"call c;\nSelect c with c.procName = c.procName",
			"variable v;\nSelect v with v.varName = v.varName",
			"read re;\nSelect re with re.varName = re.varName",
			"print pn;\nSelect pn with pn.varName = pn.varName",
			"constant c;\nSelect c with c.value = c.value",
			"stmt s;\nSelect s with s.stmt# = s.stmt#",
			"read re;\nSelect re with re.stmt# = re.stmt#",
			"print pn;\nSelect pn with pn.stmt# = pn.stmt#",
			"call c;\nSelect c with c.stmt# = c.stmt#",
			"while w;\nSelect w with w.stmt# = w.stmt#",
			"if ifs;\nSelect ifs with ifs.stmt# = ifs.stmt#",
			"assign a;\nSelect a with a.stmt# = a.stmt#"
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestWithAttrNameQuery_invalid) {
		std::string testcases[] = {
			// Invalid attrName
			"procedure p; Select p with p.procNames = p.procName",
			"variable v;\nSelect v with v.varNames = v.varName",
			"constant c;\nSelect c with c.values = c.value",
			"stmt s;\nSelect s with s.stmt = s.stmt#",
			"stmt s;\nSelect s with s.test = s.stmt",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestWithQuery_valid) {
		std::string testcases[] = {
			// Valid With Clauses
			"variable v; Select v with \"x\" = \"x\"",
			"variable v; Select v with \"x\" = 123",
			"variable v; Select v with \"x\" = v.varName",
			"variable v; Select v with 123 = \"x\"",
			"variable v; Select v with 123 = 123",
			"variable v; Select v with 123 = v.varName",
			"variable v; Select v with v.varName = \"x\"",
			"variable v; Select v with v.varName = 123",
			"variable v; Select v with v.varName = v.varName"
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestWithQuery_invalid) {
		std::string testcases[] = {
			// Invalid With Clauses
			// mispelled "with"
			"variable v; Select v With v.varName = \"x\"",
			"variable v; Select v with v.varName + \"x\"",
			"variable v; Select v with v.varName = x",
			"variable v; Select v with v.varName = _"
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestMultipleClauses_valid) {
		std::string testcases[] = {
			// Test multiple suchthat-cl
			"stmt s;\nSelect s such that Modifies(s, \"i\") such that Modifies(s, \"i\")",
			"stmt s;\nSelect s such that Modifies(s, \"i\") such that Modifies(s, \"i\") such that Modifies(s, \"i\")",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestMultipleClauses_invalid) {
		std::string testcases[] = {
			// Test invalid mutiple suchthat-cl
			"stmt s;\nSelect s such that Modifies(s, \"i\") suchthat Modifies(s, \"i\")",
			"stmt s;\nSelect s such that Modifies(s, \"i\") suchthat-cl Modifies(s, \"i\")",
			"stmt s;\nSelect s such that Modifies(s, \"i\") Modifies(s, \"i\")",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}

	TEST_METHOD(TestAndClause_valid) {
		std::string testcases[] = {
			"stmt s1, s2, s3;\nSelect s1 such that Follows(s1, s2) and Follows(s2, s3)",
			"stmt s1, s2, s3, s4;\nSelect s1 such that Follows(s1, s2) and Follows(s2, s3) and Follows(s3, s4)",
			"assign a; variable v;\nSelect a pattern a(v,_) and a(_,_\"x\"_)",
			"assign a; variable v;\nSelect a pattern a(v,_) and a(_,_\"x\"_) and a(_,_\"y\"_)",
			"assign a; variable v;\nSelect a with a.stmt# = 123 and v.varName = \"x\"",
			"assign a; variable v;\nSelect a with a.stmt# = 123 and v.varName = \"x\" and v.varName = \"y\"",
			"assign a1, a2; variable v;\nSelect a1 such that Follows(a1, a2) pattern a1 (v,_) and a2 (_,_\"x\"_)",
			"assign a; variable v;\nSelect a pattern a(v,_) with a.stmt# = 123 and v.varName = \"x\"",
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(true, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	TEST_METHOD(TestAndClause_invalid) {
		std::string testcases[] = {
			"stmt s1, s2, s3;\nSelect s1 and Follows(s1, s2) such that Follows(s2, s3)",
			"stmt s1, s2, s3;\nSelect s1 such that Follows(s1, s2) AND Follows(s2, s3)"
		};

		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++)
			Assert::AreEqual(false, QPSPreprocessor().validateQuery(testcases[i]).isValid, MSG(i));
	}
	};
}
