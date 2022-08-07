#include "stdafx.h"
#include "CppUnitTest.h"
#include "Lexer.h"
#include "Token.h"
#include <map>

#include "CustomException.h"


using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestLexer) {
public:

	TEST_METHOD(TestLex_valid) {
		map<string, vector<Token::Type>> testCases = {
			{"x = 0", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"x = 1", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"x=2", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"x =a", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::NAME }},
			{"x =  12", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"x = \n 12", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"if ( )", { Token::Type::NAME, Token::Type::LEFT_PARENTHESIS, Token::Type::RIGHT_PARENTHESIS }},
			{"if = 1", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"then { }", { Token::Type::NAME, Token::Type::LEFT_CURLY, Token::Type::RIGHT_CURLY }},
			{"then = 1", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"else { }", { Token::Type::NAME, Token::Type::LEFT_CURLY, Token::Type::RIGHT_CURLY }},
			{"else = 1", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"while ( )", { Token::Type::NAME, Token::Type::LEFT_PARENTHESIS, Token::Type::RIGHT_PARENTHESIS }},
			{"while = 1", { Token::Type::NAME, Token::Type::EQUAL, Token::Type::DIGIT }},
			{"!()", { Token::Type::NOT, Token::Type::LEFT_PARENTHESIS, Token::Type::RIGHT_PARENTHESIS }},
			{"!= 1", { Token::Type::NOT_EQUAL, Token::Type::DIGIT }},
			{"&&", { Token::Type::AND }},
			{"||", { Token::Type::OR }},
			{"> 1", { Token::Type::GREATER, Token::Type::DIGIT  }},
			{">= 1", { Token::Type::GREATER_EQ, Token::Type::DIGIT  }},
			{">x", { Token::Type::GREATER, Token::Type::NAME }},
			{"< 1", { Token::Type::LESS, Token::Type::DIGIT  }},
			{"<= 1", { Token::Type::LESS_EQ, Token::Type::DIGIT  }},
			{"<x", { Token::Type::LESS, Token::Type::NAME }},
			{"== 1", { Token::Type::DBL_EQUAL, Token::Type::DIGIT }},
			{"=x", { Token::Type::EQUAL, Token::Type::NAME }},
		};

		for (auto [source, expected] : testCases) {
			vector<Token> actual_ = Lexer(source.c_str()).getTokens();
			vector<Token::Type> actual;
			for (auto t : actual_) actual.push_back(t.getType());
			Assert::AreEqual(expected.size(), actual.size());
			for (int i = 0; i < int(expected.size()); i++) {
				Assert::IsTrue(expected[i] == actual[i]);
			}
		}
	}
	TEST_METHOD(TestLex_invalid) {
		vector<string> testCases = {
			"|=",
			"&=",
			"x = 0123",
		};

		for (auto str : testCases) {
			auto func = [&str] { Lexer(str.c_str()).getTokens(); };
			Assert::ExpectException<LexerException>(func);
		}
	}
	};
}
