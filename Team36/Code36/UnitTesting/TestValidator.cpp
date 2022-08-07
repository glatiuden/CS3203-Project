#include "stdafx.h"
#include "CppUnitTest.h"
#include "Token.h"
#include "Lexer.h"
#include "SP/ValidatorHelper.h"
#include <SP/SourceValidator.h>
#include <CustomException.h>
#include <map>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestValidator) {
public:

	// Basic SIMPLE requirements
	TEST_METHOD(validate_valid_assigns) {
		string testCases[] = {
			"x = 1;",
			"x=2;",
			"x =a;",
			"x =  12;",
			"x = \n 12;",
			"x = 12 * 1 + a;",
			"x = 1 + b / \n 12;",
			"x =a- 1  / \n 12;",
		};
		// Individual
		for (auto stmt : testCases) {
			string testStr = "procedure test {" + stmt + "}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}

		// Group
		string testStr = "procedure test {";
		for (auto stmt : testCases) testStr += stmt;
		testStr += "}";
		vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
		Assert::AreEqual(SourceValidator(tokens).validate(), true);

	}
	TEST_METHOD(validate_invalid_assigns) {
		string testCases[] = {
			"x = 1",
			"x k=2;",
			"x =a a;",
			"x = 12 12;",
			"x = \n 12",
			"x = 12 %;",
			"12 = a;",
		};
		for (auto stmt : testCases) {
			string testStr = "procedure test {" + stmt + "}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			auto func = [&tokens] { SourceValidator(tokens).validate(); };
			Assert::ExpectException<SourceValidatorException>(func);
		}

	}

	TEST_METHOD(validate_valid_expr) {
		string testCases[] = {
			"(1)",
			"1 * 1 + 1",
			"(1/0) - 3",
			"1 % ((0 - 1))",
			"1 % ((1))",
			"(1) * (2 / (1 + 2))",
			"1 % ((1)) + (1) * (2 / (1 + 2))",
			"1 % ((1)) * (1) * (2 / (1 + 2))",
			"1 + ((1)) + (1) + (2 / (1 + 2))",
			"1 + ((1)) + (1) + (2 / (1 + 1 + ((1)) + (1) + (2 / (1 + 2)))) + (1 + ((1)) + (1) + (2 / (1 + 1 + (((1) * (2 / (1 + 2)))) + (1) + (2 / (1 + 2)))))"
		};
		for (auto expr : testCases) {
			string testStr = "procedure test { x = " + expr + ";}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}
	}

	TEST_METHOD(validate_invalid_expr) {
		string testCases[] = {
			"",
			"()1 + 2",
			")1 + 2(",
			")1 + 2",
			"1 + (2",
		};
		// Individual
		for (auto expr : testCases) {
			string testStr = "procedure test { x = " + expr + ";}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			auto func = [&tokens] { SourceValidator(tokens).validate(); };
			Assert::ExpectException<SourceValidatorException>(func);
		}
	}
	TEST_METHOD(validate_valid_procedures) {
		string testCases[] = {
			"procedure abc123 { }",
			"procedure procedure { }",
			"procedure procedure{}",
			"procedure a{}",
		};
		for (auto stmt : testCases) {
			vector<Token> tokens = Lexer(stmt.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}


	}
	TEST_METHOD(validate_valid_multiple_procedures) {
		string testStr = " \
				procedure abc123 { } \
				procedure procedure { } \
				procedure procedure3{} \
				procedure a{} \
			";
		vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
		Assert::AreEqual(SourceValidator(tokens).validate(), true);
	}
	TEST_METHOD(validate_invalid_multiple_procedures) {
		string testStr = " \
				procedure abc123 { } \
				procedure procedure { } \
				procedure procedure{} \
			";
		vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
		auto func = [&tokens] { SourceValidator(tokens).validate(); };
		Assert::ExpectException<SourceValidatorException>(func);
	}

	TEST_METHOD(validate_invalid_procedures) {
		string testCases[] = {
			"procedure { }",
			"procedure 123abc { }",
			"procedure ;",
			"procedure a {",
		};
		for (auto stmt : testCases) {
			vector<Token> tokens = Lexer(stmt.c_str()).getTokens();
			auto func = [&tokens] { SourceValidator(tokens).validate(); };
			Assert::ExpectException<SourceValidatorException>(func);
		}
	}


	TEST_METHOD(validate_valid_call_print_read) {
		string testCases[] = {
			"call x;",
			"call x; call y ;",
			"call  \n x;",

			"print x;",
			"print x; print y ;",
			"print  \n x;",

			"read x;",
			"read x; read y ;",
			"read  \n x;",

			"read print;",
			"print call;",

		};
		for (auto stmt : testCases) {
			string testStr = "procedure x {} procedure y {} procedure test {" + stmt + "}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}
	}
	TEST_METHOD(validate_invalid_call_print_read) {
		string testCases[] = {
			"read 1;",
			"call 2;",
			"call a",
			"print 1s;"
		};
		for (auto stmt : testCases) {
			string testStr = "procedure test {" + stmt + "}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			auto func = [&tokens] { SourceValidator(tokens).validate(); };
			Assert::ExpectException<SourceValidatorException>(func);
		}
	}
	TEST_METHOD(validate_valid_while_if) {
		string testCases[] = {
			"while(x >= 1){}",
			"if(x == 1) then {} else {}",
			"if ( 1 == 1) then { x = 1; } else { y = 2; }",
		};
		for (auto stmt : testCases) {
			string testStr = "procedure test {" + stmt + "}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}
	}

	TEST_METHOD(validate_valid_nested_while_if) {
		string testStr = "\
		procedure test { \
			while(x > 1) { \
				x = 1; \
				if (x > 1) then { \
					x = 1;\
				} else {\
					x = 2; \
				} \
			} \
		}";
		vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
		Assert::AreEqual(SourceValidator(tokens).validate(), true);
	}
	TEST_METHOD(validate_invalid_while_if) {
		string testCases[] = {
			"while x =1 {};",
			"while(x =1 ){};",
			"while(){}",
			"if() then {} else {}",
			"if () then { x = 1; } else { y = 2; }",
		};
		for (auto stmt : testCases) {
			string testStr = "procedure test {" + stmt + "}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			auto func = [&tokens] { SourceValidator(tokens).validate(); };
			Assert::ExpectException<SourceValidatorException>(func);
		}
	}

	TEST_METHOD(validate_valid_rel_expr) {
		string testCases[] = {
			"x < 1",
			"x > 1",
			"x <= 1",
			"x >= 1",
			"x != 1",
			"x == 1",
			"(x+1)>=1",
			"x<=(x * 1 + a * s)",
			"1 == 1",
		};
		for (auto stmt : testCases) {
			string testStr = "procedure test { while(" + stmt + "){}}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}
	}

	TEST_METHOD(validate_valid_cond_expr) {
		string testCases[] = {
			"!(1 == 1)",
			"!(!(1 == 1))",
			"(!(1 == 1)) && (1 == 1)",
			"(1 == 1) && (1 == 1)",
			"(x > 1) && (x < 1)",
			"(x == 1) || (x != 1)",
			"1 >= 1 % ((0 - 1))",
			"!((1 == 0) && (1 == 0))",
			"1 >= 1 % ((1))"
		};
		for (auto stmt : testCases) {
			string testStr = "procedure test { while(" + stmt + "){}}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}
	}

	TEST_METHOD(validate_valid_nested_cond_expr) {
		string testCases[] = {
			"(!(1 == 1)) && (2 == 2)",
			"((!(1 == 1)) && (2 == 2)) || (x > 2)",
			"((!(1 == 1)) && (2 == 2)) || (((!(1 == 1)) && (2 == 2)) || (x > 2))",
		};
		for (auto stmt : testCases) {
			string testStr = "procedure test { while(" + stmt + "){}}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}
	}
	TEST_METHOD(validate_invalid_cond_expr) {
		string testCases[] = {
			"x+1",
			"(x < 1)",
			"x < 1 && x == 1",
			"!(1 == 1) && (1 == 1)",
		};
		for (auto stmt : testCases) {
			string testStr = "procedure test { while(" + stmt + "){}}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			auto func = [&tokens] { SourceValidator(tokens).validate(); };
			Assert::ExpectException<SourceValidatorException>(func);
		}
	}
	TEST_METHOD(validate_side_effects) {
		map<string, vector<Token::Type>> testCases = {
			{"read x;", { Token::Type::READ, Token::Type::NAME, Token::Type::SEMICOLON }},
			{"print x;", { Token::Type::PRINT, Token::Type::NAME, Token::Type::SEMICOLON }},
			{"call x;", { Token::Type::CALL, Token::Type::NAME, Token::Type::SEMICOLON }},

		};

		for (auto [stmt, expected] : testCases) {
			string testStr = "procedure test { " + stmt + "} procedure x {}";
			vector<Token> tokens = Lexer(testStr.c_str()).getTokens();
			SourceValidator(tokens).validate();

			vector<Token::Type> actual;
			for (auto t : tokens) actual.push_back(t.getType());
			actual = vector(actual.begin() + 3, actual.end() - 1);


			for (unsigned i = 0; i < expected.size(); i++) {
				Assert::IsTrue(expected[i] == actual[i]);
			}
		}

	}

	// Advance SIMPLE requirements

	TEST_METHOD(validate_valid_call_chain) {
		string testCases[] = {
			"procedure a { call b; } procedure b { call c; } procedure c {}",
		};
		for (auto stmt : testCases) {
			vector<Token> tokens = Lexer(stmt.c_str()).getTokens();
			Assert::AreEqual(SourceValidator(tokens).validate(), true);
		}
	}
	TEST_METHOD(validate_invalid_call_chain) {
		string testCases[] = {
			"procedure a { call b; } procedure b { call c; } procedure c { call a; }",
			"procedure a { call b; } procedure b { call c; } procedure c { call d; } procedure d { call a;}",
			"procedure a { call b; } procedure b { call c; } procedure c { call d; } procedure d { call a;} procedure e {}",
			"procedure a { call a; }",
		};
		for (auto stmt : testCases) {
			vector<Token> tokens = Lexer(stmt.c_str()).getTokens();
			auto func = [&tokens] { SourceValidator(tokens).validate(); };
			Assert::ExpectException<SourceValidatorException>(func);
		}
	}
	};
}
