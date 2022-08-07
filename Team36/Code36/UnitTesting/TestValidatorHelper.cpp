#include "stdafx.h"
#include "CppUnitTest.h"
#include "Token.h"
#include "Lexer.h"
#include "SP/ValidatorHelper.h"

#define MSG(msg) [&]{ std::wstringstream _s; _s << "Case " << i+1 << " failed"; return _s.str(); }().c_str()

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestValidatorHelper) {
		vector<Token> stringToExprTokens(const string str) {
			Lexer lex(str.c_str());
			return lex.getTokens();
		}
public:
	TEST_METHOD(TestExpression_valid) {
		vector<Token> testcases[] = {
			stringToExprTokens("x + y"),
			stringToExprTokens("x + y - z"),
			stringToExprTokens("select*while+if-assign / stmt % stmtList"),
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++) {
			Assert::AreEqual(true, ValidatorHelper::isExpr(testcases[i]), MSG(i));
		}
	}

	TEST_METHOD(TestExpression_invalid) {
		vector<Token> testcases[] = {
			stringToExprTokens("x _ y"),
			stringToExprTokens("x + y # z"),
			stringToExprTokens("+ y + z"),
		};
		for (int i = 0; i < sizeof(testcases) / sizeof(testcases[0]); i++) {
			Assert::AreEqual(false, ValidatorHelper::isExpr(testcases[i]), MSG(i));
		}
	}
	};
}