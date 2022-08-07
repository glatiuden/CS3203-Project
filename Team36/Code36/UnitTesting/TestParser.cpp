#include <string>
#include <vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "SP/Parser.h"
#include "Token.h"
#include "TNode.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestParser) {
public:

	TEST_METHOD(parseAssign) {
		Parser parser;
		std::vector<Token> tokens;
		TNode* program;
		TNode* proc;

		// Parse an assignment statement where RHS is a factor
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root1 = proc->getChildren()[0];
		Assert::IsTrue(root1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root1->getChildren().size());
		TNode* assign1 = root1->getChildren()[0];
		Assert::IsTrue(assign1->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign1->getChildren().size());
		Assert::AreEqual(1, assign1->getStmtNum());
		TNode* x1 = assign1->getChildren()[0];
		TNode* one1 = assign1->getChildren()[1];
		Assert::AreEqual(std::string("x"), x1->getValue());
		Assert::AreEqual(std::string("1"), one1->getValue());
		Assert::AreEqual((size_t)0, x1->getChildren().size());
		Assert::AreEqual((size_t)0, one1->getChildren().size());

		// Parse an assignment statement where RHS is an expression with only 1 operation
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::PLUS, "+", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root2 = proc->getChildren()[0];
		Assert::IsTrue(root2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root2->getChildren().size());
		TNode* assign2 = root2->getChildren()[0];
		Assert::IsTrue(assign2->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign2->getChildren().size());
		Assert::AreEqual(1, assign2->getStmtNum());
		TNode* x2 = assign2->getChildren()[0];
		TNode* plus2 = assign2->getChildren()[1];
		Assert::AreEqual(std::string("x"), x2->getValue());
		Assert::AreEqual(std::string("+"), plus2->getValue());
		Assert::AreEqual((size_t)0, x2->getChildren().size());
		Assert::AreEqual((size_t)2, plus2->getChildren().size());
		TNode* one2 = plus2->getChildren()[0];
		TNode* two2 = plus2->getChildren()[1];
		Assert::AreEqual(std::string("1"), one2->getValue());
		Assert::AreEqual(std::string("2"), two2->getValue());
		Assert::AreEqual((size_t)0, one2->getChildren().size());
		Assert::AreEqual((size_t)0, two2->getChildren().size());

		// Parse an assignment statement where RHS is a term with only 1 operation
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::MODULO, "%", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root3 = proc->getChildren()[0];
		Assert::IsTrue(root3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root3->getChildren().size());
		TNode* assign3 = root3->getChildren()[0];
		Assert::IsTrue(assign3->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign3->getChildren().size());
		Assert::AreEqual(1, assign3->getStmtNum());
		TNode* x3 = assign3->getChildren()[0];
		TNode* mod3 = assign3->getChildren()[1];
		Assert::AreEqual(std::string("x"), x3->getValue());
		Assert::AreEqual(std::string("%"), mod3->getValue());
		Assert::AreEqual((size_t)0, x3->getChildren().size());
		Assert::AreEqual((size_t)2, mod3->getChildren().size());
		TNode* one3 = mod3->getChildren()[0];
		TNode* two3 = mod3->getChildren()[1];
		Assert::AreEqual(std::string("1"), one3->getValue());
		Assert::AreEqual(std::string("2"), two3->getValue());
		Assert::AreEqual((size_t)0, one3->getChildren().size());
		Assert::AreEqual((size_t)0, two3->getChildren().size());

		// Parse an assignment statement where expression has an addition and a multiplication
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::PLUS, "+", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::MULTIPLY, "*", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root4 = proc->getChildren()[0];
		Assert::IsTrue(root4->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root4->getChildren().size());
		TNode* assign4 = root4->getChildren()[0];
		Assert::IsTrue(assign4->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign4->getChildren().size());
		Assert::AreEqual(1, assign4->getStmtNum());
		TNode* x4 = assign4->getChildren()[0];
		TNode* plus4 = assign4->getChildren()[1];
		Assert::AreEqual(std::string("x"), x4->getValue());
		Assert::AreEqual(std::string("+"), plus4->getValue());
		Assert::AreEqual((size_t)0, x4->getChildren().size());
		Assert::AreEqual((size_t)2, plus4->getChildren().size());
		TNode* one4 = plus4->getChildren()[0];
		TNode* multiply4 = plus4->getChildren()[1];
		Assert::AreEqual(std::string("1"), one4->getValue());
		Assert::AreEqual(std::string("*"), multiply4->getValue());
		Assert::AreEqual((size_t)0, one4->getChildren().size());
		Assert::AreEqual((size_t)2, multiply4->getChildren().size());
		TNode* two4 = multiply4->getChildren()[0];
		TNode* three4 = multiply4->getChildren()[1];
		Assert::AreEqual(std::string("2"), two4->getValue());
		Assert::AreEqual(std::string("3"), three4->getValue());
		Assert::AreEqual((size_t)0, two4->getChildren().size());
		Assert::AreEqual((size_t)0, three4->getChildren().size());

		// Parse an assignment statement where expression has a parenthesis
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::PLUS, "+", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::MULTIPLY, "*", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root5 = proc->getChildren()[0];
		Assert::IsTrue(root5->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root5->getChildren().size());
		TNode* assign5 = root5->getChildren()[0];
		Assert::IsTrue(assign5->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign5->getChildren().size());
		Assert::AreEqual(1, assign5->getStmtNum());
		TNode* y5 = assign5->getChildren()[0];
		TNode* multiply5 = assign5->getChildren()[1];
		Assert::AreEqual(std::string("y"), y5->getValue());
		Assert::AreEqual(std::string("*"), multiply5->getValue());
		Assert::AreEqual((size_t)0, y5->getChildren().size());
		Assert::AreEqual((size_t)2, multiply5->getChildren().size());
		TNode* plus5 = multiply5->getChildren()[0];
		TNode* three5 = multiply5->getChildren()[1];
		Assert::AreEqual(std::string("+"), plus5->getValue());
		Assert::AreEqual(std::string("3"), three5->getValue());
		Assert::AreEqual((size_t)2, plus5->getChildren().size());
		Assert::AreEqual((size_t)0, three5->getChildren().size());
		TNode* one5 = plus5->getChildren()[0];
		TNode* two5 = plus5->getChildren()[1];
		Assert::AreEqual(std::string("1"), one5->getValue());
		Assert::AreEqual(std::string("2"), two5->getValue());
		Assert::AreEqual((size_t)0, one5->getChildren().size());
		Assert::AreEqual((size_t)0, two5->getChildren().size());

		// Parse multiple assignment statements
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::MULTIPLY, "*", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root6 = proc->getChildren()[0];
		Assert::IsTrue(root6->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)2, root6->getChildren().size());
		TNode* assign61 = root6->getChildren()[0];
		Assert::IsTrue(assign61->getType() == NodeType::ASSIGN);
		Assert::AreEqual(1, assign61->getStmtNum());
		TNode* assign62 = root6->getChildren()[1];
		Assert::IsTrue(assign62->getType() == NodeType::ASSIGN);
		Assert::AreEqual(2, assign62->getStmtNum());
	}

	TEST_METHOD(parseCondExpr) {
		Parser parser;
		std::vector<Token> tokens;
		TNode* program;
		TNode* proc;

		// Parse a cond_expr that is a single rel_expr
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root1 = proc->getChildren()[0];
		Assert::IsTrue(root1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root1->getChildren().size());
		TNode* while1 = root1->getChildren()[0];
		Assert::IsTrue(while1->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while1->getChildren().size());
		TNode* greaterThan1 = while1->getChildren()[0];
		TNode* stmtLst1 = while1->getChildren()[1];
		Assert::IsTrue(greaterThan1->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan1->getChildren().size());
		Assert::IsTrue(stmtLst1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst1->getChildren().size());

		// Parse a cond_expr that is !(rel_expr)
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NOT, "!", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root2 = proc->getChildren()[0];
		Assert::IsTrue(root2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root2->getChildren().size());
		TNode* while2 = root2->getChildren()[0];
		Assert::IsTrue(while2->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while2->getChildren().size());
		TNode* not2 = while2->getChildren()[0];
		TNode* stmtLst2 = while2->getChildren()[1];
		Assert::IsTrue(not2->getType() == NodeType::NOT);
		Assert::AreEqual((size_t)1, not2->getChildren().size());
		Assert::IsTrue(stmtLst2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst2->getChildren().size());
		TNode* greaterThan2 = not2->getChildren()[0];
		Assert::IsTrue(greaterThan2->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan2->getChildren().size());

		// Parse a cond_expr that is (rel_expr) && (rel_expr)
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::AND, "&&", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root3 = proc->getChildren()[0];
		Assert::IsTrue(root3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root3->getChildren().size());
		TNode* while3 = root3->getChildren()[0];
		Assert::IsTrue(while3->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while3->getChildren().size());
		TNode* and3 = while3->getChildren()[0];
		TNode* stmtLst3 = while3->getChildren()[1];
		Assert::IsTrue(and3->getType() == NodeType::AND);
		Assert::AreEqual((size_t)2, and3->getChildren().size());
		Assert::IsTrue(stmtLst3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst3->getChildren().size());
		TNode* greaterThan31 = and3->getChildren()[0];
		TNode* greaterThan32 = and3->getChildren()[1];
		Assert::IsTrue(greaterThan31->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan31->getChildren().size());
		Assert::IsTrue(greaterThan32->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan32->getChildren().size());

		// Parse a cond_expr that is (rel_expr) || (rel_expr)
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::OR, "||", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root4 = proc->getChildren()[0];
		Assert::IsTrue(root4->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root4->getChildren().size());
		TNode* while4 = root4->getChildren()[0];
		Assert::IsTrue(while4->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while4->getChildren().size());
		TNode* or4 = while4->getChildren()[0];
		TNode* stmtLst4 = while4->getChildren()[1];
		Assert::IsTrue(or4->getType() == NodeType::OR);
		Assert::AreEqual((size_t)2, or4->getChildren().size());
		Assert::IsTrue(stmtLst4->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst4->getChildren().size());
		TNode* greaterThan41 = or4->getChildren()[0];
		TNode* greaterThan42 = or4->getChildren()[1];
		Assert::IsTrue(greaterThan41->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan41->getChildren().size());
		Assert::IsTrue(greaterThan42->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan42->getChildren().size());

		// Parse a cond_expr that is ((rel_expr) && (rel_expr)) || (rel_expr)
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::AND, "&&", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::OR, "||", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root5 = proc->getChildren()[0];
		Assert::IsTrue(root5->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root5->getChildren().size());
		TNode* while5 = root5->getChildren()[0];
		Assert::IsTrue(while5->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while5->getChildren().size());
		TNode* or5 = while5->getChildren()[0];
		TNode* stmtLst5 = while5->getChildren()[1];
		Assert::IsTrue(or5->getType() == NodeType::OR);
		Assert::AreEqual((size_t)2, or5->getChildren().size());
		Assert::IsTrue(stmtLst5->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst5->getChildren().size());
		TNode* and5 = or5->getChildren()[0];
		TNode* greaterThan5 = or5->getChildren()[1];
		Assert::IsTrue(and5->getType() == NodeType::AND);
		Assert::AreEqual((size_t)2, and5->getChildren().size());
		Assert::IsTrue(greaterThan5->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan5->getChildren().size());

		// Parse a cond_expr that is ((rel_expr) || (rel_expr)) && (rel_expr)
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::OR, "||", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::AND, "&&", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root6 = proc->getChildren()[0];
		Assert::IsTrue(root6->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root6->getChildren().size());
		TNode* while6 = root6->getChildren()[0];
		Assert::IsTrue(while6->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while6->getChildren().size());
		TNode* and6 = while6->getChildren()[0];
		TNode* stmtLst6 = while6->getChildren()[1];
		Assert::IsTrue(and6->getType() == NodeType::AND);
		Assert::AreEqual((size_t)2, and6->getChildren().size());
		Assert::IsTrue(stmtLst6->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst6->getChildren().size());
		TNode* or6 = and6->getChildren()[0];
		TNode* greaterThan6 = and6->getChildren()[1];
		Assert::IsTrue(or6->getType() == NodeType::OR);
		Assert::AreEqual((size_t)2, or6->getChildren().size());
		Assert::IsTrue(greaterThan6->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan6->getChildren().size());

		// Parse a cond_expr that is (rel_expr) || ((rel_expr) && (rel_expr))
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::OR, "||", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::AND, "&&", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root7 = proc->getChildren()[0];
		Assert::IsTrue(root7->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root7->getChildren().size());
		TNode* while7 = root7->getChildren()[0];
		Assert::IsTrue(while7->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while7->getChildren().size());
		TNode* or7 = while7->getChildren()[0];
		TNode* stmtLst7 = while7->getChildren()[1];
		Assert::IsTrue(or7->getType() == NodeType::OR);
		Assert::AreEqual((size_t)2, or7->getChildren().size());
		Assert::IsTrue(stmtLst7->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst7->getChildren().size());
		TNode* greaterThan7 = or7->getChildren()[0];
		TNode* and7 = or7->getChildren()[1];
		Assert::IsTrue(greaterThan7->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan7->getChildren().size());
		Assert::IsTrue(and7->getType() == NodeType::AND);
		Assert::AreEqual((size_t)2, and7->getChildren().size());

		// Parse a cond_expr that is (rel_expr) && ((rel_expr) || (rel_expr))
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::AND, "&&", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::OR, "||", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root8 = proc->getChildren()[0];
		Assert::IsTrue(root8->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root8->getChildren().size());
		TNode* while8 = root8->getChildren()[0];
		Assert::IsTrue(while8->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while8->getChildren().size());
		TNode* and8 = while8->getChildren()[0];
		TNode* stmtLst8 = while8->getChildren()[1];
		Assert::IsTrue(and8->getType() == NodeType::AND);
		Assert::AreEqual((size_t)2, and8->getChildren().size());
		Assert::IsTrue(stmtLst8->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst8->getChildren().size());
		TNode* greaterThan8 = and8->getChildren()[0];
		TNode* or8 = and8->getChildren()[1];
		Assert::IsTrue(greaterThan8->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan8->getChildren().size());
		Assert::IsTrue(or8->getType() == NodeType::OR);
		Assert::AreEqual((size_t)2, or8->getChildren().size());

		// Parse a cond_expr that is ((expr) > digit))
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::PLUS, "+", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root9 = proc->getChildren()[0];
		Assert::IsTrue(root9->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root9->getChildren().size());
		TNode* while9 = root9->getChildren()[0];
		Assert::IsTrue(while9->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while9->getChildren().size());
		TNode* greaterThan9 = while9->getChildren()[0];
		TNode* stmtLst9 = while9->getChildren()[1];
		Assert::IsTrue(greaterThan9->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan9->getChildren().size());
		Assert::IsTrue(stmtLst9->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst9->getChildren().size());
		TNode* plus9 = greaterThan9->getChildren()[0];
		Assert::IsTrue(plus9->getType() == NodeType::PLUS);
		Assert::AreEqual((size_t)2, plus9->getChildren().size());
	}

	TEST_METHOD(parseWhile) {
		Parser parser;
		std::vector<Token> tokens;
		TNode* program;
		TNode* proc;

		// Parse a while statement with > cond and 1 statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root1 = proc->getChildren()[0];
		Assert::IsTrue(root1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root1->getChildren().size());
		TNode* while1 = root1->getChildren()[0];
		Assert::IsTrue(while1->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while1->getChildren().size());
		Assert::AreEqual(1, while1->getStmtNum());
		TNode* greaterThan1 = while1->getChildren()[0];
		TNode* stmtLst1 = while1->getChildren()[1];
		Assert::IsTrue(greaterThan1->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan1->getChildren().size());
		Assert::AreEqual(1, greaterThan1->getStmtNum());
		Assert::IsTrue(stmtLst1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst1->getChildren().size());
		TNode* x1 = greaterThan1->getChildren()[0];
		TNode* one1 = greaterThan1->getChildren()[1];
		Assert::AreEqual(std::string("x"), x1->getValue());
		Assert::AreEqual((size_t)0, x1->getChildren().size());
		Assert::AreEqual(std::string("1"), one1->getValue());
		Assert::AreEqual((size_t)0, one1->getChildren().size());
		TNode* assign1 = stmtLst1->getChildren()[0];
		Assert::IsTrue(assign1->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign1->getChildren().size());
		Assert::AreEqual(2, assign1->getStmtNum());

		// Parse a while statement with < cond and multiple assign statements
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::LESS , "<", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::MULTIPLY, "*", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root2 = proc->getChildren()[0];
		Assert::IsTrue(root2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root2->getChildren().size());
		TNode* while2 = root2->getChildren()[0];
		Assert::IsTrue(while2->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while2->getChildren().size());
		TNode* lessThan2 = while2->getChildren()[0];
		TNode* stmtLst2 = while2->getChildren()[1];
		Assert::IsTrue(lessThan2->getType() == NodeType::LESS);
		Assert::AreEqual((size_t)2, lessThan2->getChildren().size());
		Assert::IsTrue(stmtLst2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)2, stmtLst2->getChildren().size());
		TNode* x2 = lessThan2->getChildren()[0];
		TNode* one2 = lessThan2->getChildren()[1];
		Assert::AreEqual(std::string("x"), x2->getValue());
		Assert::AreEqual((size_t)0, x2->getChildren().size());
		Assert::AreEqual(std::string("1"), one2->getValue());
		Assert::AreEqual((size_t)0, one2->getChildren().size());
		TNode* assign21 = stmtLst2->getChildren()[0];
		TNode* assign22 = stmtLst2->getChildren()[1];
		Assert::IsTrue(assign21->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign21->getChildren().size());
		Assert::IsTrue(assign22->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign22->getChildren().size());

		// Parse a while statement with != cond and if statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::NOT_EQUAL, "!=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root3 = proc->getChildren()[0];
		Assert::IsTrue(root3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root3->getChildren().size());
		TNode* while3 = root3->getChildren()[0];
		Assert::IsTrue(while3->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while3->getChildren().size());
		TNode* notEqual3 = while3->getChildren()[0];
		TNode* stmtLst3 = while3->getChildren()[1];
		Assert::IsTrue(notEqual3->getType() == NodeType::NOT_EQUAL);
		Assert::AreEqual((size_t)2, notEqual3->getChildren().size());
		Assert::IsTrue(stmtLst3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst3->getChildren().size());
		TNode* x3 = notEqual3->getChildren()[0];
		TNode* one3 = notEqual3->getChildren()[1];
		Assert::AreEqual(std::string("x"), x3->getValue());
		Assert::AreEqual((size_t)0, x3->getChildren().size());
		Assert::AreEqual(std::string("1"), one3->getValue());
		Assert::AreEqual((size_t)0, one3->getChildren().size());
		TNode* if3 = stmtLst3->getChildren()[0];
		Assert::IsTrue(if3->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if3->getChildren().size());

		// Parse a while statement with >= cond and 1 if statement then 1 assign statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER_EQ, ">=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::NAME, "v", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "5", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root4 = proc->getChildren()[0];
		Assert::IsTrue(root4->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root4->getChildren().size());
		TNode* while4 = root4->getChildren()[0];
		Assert::IsTrue(while4->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while4->getChildren().size());
		TNode* greaterEq4 = while4->getChildren()[0];
		TNode* stmtLst4 = while4->getChildren()[1];
		Assert::IsTrue(greaterEq4->getType() == NodeType::GREATER_EQ);
		Assert::AreEqual((size_t)2, greaterEq4->getChildren().size());
		Assert::IsTrue(stmtLst4->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)2, stmtLst4->getChildren().size());
		TNode* x4 = greaterEq4->getChildren()[0];
		TNode* one4 = greaterEq4->getChildren()[1];
		Assert::AreEqual(std::string("x"), x4->getValue());
		Assert::AreEqual((size_t)0, x4->getChildren().size());
		Assert::AreEqual(std::string("1"), one4->getValue());
		Assert::AreEqual((size_t)0, one4->getChildren().size());
		TNode* if4 = stmtLst4->getChildren()[0];
		TNode* assign4 = stmtLst4->getChildren()[1];
		Assert::IsTrue(if4->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if4->getChildren().size());
		Assert::IsTrue(assign4->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign4->getChildren().size());

		// Parse a while statement with <= cond and 1 assign statement then 1 if statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::LESS_EQ, "<=", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "v", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "5", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root5 = proc->getChildren()[0];
		Assert::IsTrue(root5->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root5->getChildren().size());
		TNode* while5 = root5->getChildren()[0];
		Assert::IsTrue(while5->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while5->getChildren().size());
		TNode* lessEq5 = while5->getChildren()[0];
		TNode* stmtLst5 = while5->getChildren()[1];
		Assert::IsTrue(lessEq5->getType() == NodeType::LESS_EQ);
		Assert::AreEqual((size_t)2, lessEq5->getChildren().size());
		Assert::IsTrue(stmtLst5->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)2, stmtLst5->getChildren().size());
		TNode* x5 = lessEq5->getChildren()[0];
		TNode* one5 = lessEq5->getChildren()[1];
		Assert::AreEqual(std::string("x"), x5->getValue());
		Assert::AreEqual((size_t)0, x5->getChildren().size());
		Assert::AreEqual(std::string("1"), one5->getValue());
		Assert::AreEqual((size_t)0, one5->getChildren().size());
		TNode* assign5 = stmtLst5->getChildren()[0];
		TNode* if5 = stmtLst5->getChildren()[1];
		Assert::IsTrue(assign5->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign5->getChildren().size());
		Assert::IsTrue(if5->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if5->getChildren().size());

		// Parse a while statement with == cond and 1 if statement between 2 assign statements
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::DBL_EQUAL, "==", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "v", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "5", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::NAME, "u", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "6", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root6 = proc->getChildren()[0];
		Assert::IsTrue(root6->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root6->getChildren().size());
		TNode* while6 = root6->getChildren()[0];
		Assert::IsTrue(while6->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while6->getChildren().size());
		TNode* dblEqual6 = while6->getChildren()[0];
		TNode* stmtLst6 = while6->getChildren()[1];
		Assert::IsTrue(dblEqual6->getType() == NodeType::DBL_EQUAL);
		Assert::AreEqual((size_t)2, dblEqual6->getChildren().size());
		Assert::IsTrue(stmtLst6->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)3, stmtLst6->getChildren().size());
		TNode* x6 = dblEqual6->getChildren()[0];
		TNode* one6 = dblEqual6->getChildren()[1];
		Assert::AreEqual(std::string("x"), x6->getValue());
		Assert::AreEqual((size_t)0, x6->getChildren().size());
		Assert::AreEqual(std::string("1"), one6->getValue());
		Assert::AreEqual((size_t)0, one6->getChildren().size());
		TNode* assign61 = stmtLst6->getChildren()[0];
		TNode* if6 = stmtLst6->getChildren()[1];
		TNode* assign62 = stmtLst6->getChildren()[2];
		Assert::IsTrue(assign61->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign61->getChildren().size());
		Assert::IsTrue(if6->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if6->getChildren().size());
		Assert::IsTrue(assign62->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign62->getChildren().size());

		// Parse a while statement with ! cond and 1 while statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NOT, "!", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::DBL_EQUAL, "==", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root7 = proc->getChildren()[0];
		Assert::IsTrue(root7->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root7->getChildren().size());
		TNode* while71 = root7->getChildren()[0];
		Assert::IsTrue(while71->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while71->getChildren().size());
		TNode* not7 = while71->getChildren()[0];
		TNode* stmtLst7 = while71->getChildren()[1];
		Assert::IsTrue(not7->getType() == NodeType::NOT);
		Assert::AreEqual((size_t)1, not7->getChildren().size());
		TNode* dblEqual7 = not7->getChildren()[0];
		Assert::IsTrue(dblEqual7->getType() == NodeType::DBL_EQUAL);
		Assert::AreEqual((size_t)2, dblEqual7->getChildren().size());
		Assert::IsTrue(stmtLst7->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst7->getChildren().size());
		TNode* x7 = dblEqual7->getChildren()[0];
		TNode* one7 = dblEqual7->getChildren()[1];
		Assert::AreEqual(std::string("x"), x7->getValue());
		Assert::AreEqual((size_t)0, x7->getChildren().size());
		Assert::AreEqual(std::string("1"), one7->getValue());
		Assert::AreEqual((size_t)0, one7->getChildren().size());
		TNode* while72 = stmtLst7->getChildren()[0];
		Assert::IsTrue(while72->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while72->getChildren().size());

		// Parse a while statement with && cond and 1 statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::DBL_EQUAL, "==", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::AND, "&&", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::DBL_EQUAL, "==", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root8 = proc->getChildren()[0];
		Assert::IsTrue(root8->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root8->getChildren().size());
		TNode* while8 = root8->getChildren()[0];
		Assert::IsTrue(while8->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while8->getChildren().size());
		TNode* and8 = while8->getChildren()[0];
		TNode* stmtLst8 = while8->getChildren()[1];
		Assert::IsTrue(and8->getType() == NodeType::AND);
		Assert::AreEqual((size_t)2, and8->getChildren().size());
		Assert::IsTrue(stmtLst8->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst8->getChildren().size());
		TNode* dblEqual81 = and8->getChildren()[0];
		TNode* dblEqual82 = and8->getChildren()[0];
		Assert::IsTrue(dblEqual81->getType() == NodeType::DBL_EQUAL);
		Assert::AreEqual((size_t)2, dblEqual81->getChildren().size());
		Assert::IsTrue(dblEqual82->getType() == NodeType::DBL_EQUAL);
		Assert::AreEqual((size_t)2, dblEqual82->getChildren().size());
		TNode* assign8 = stmtLst8->getChildren()[0];
		Assert::IsTrue(assign8->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign8->getChildren().size());

		// Parse a while statement with || cond and 1 statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::DBL_EQUAL, "==", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::OR, "||", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::DBL_EQUAL, "==", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root9 = proc->getChildren()[0];
		Assert::IsTrue(root9->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root9->getChildren().size());
		TNode* while9 = root9->getChildren()[0];
		Assert::IsTrue(while9->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while9->getChildren().size());
		TNode* or9 = while9->getChildren()[0];
		TNode* stmtLst9 = while9->getChildren()[1];
		Assert::IsTrue(or9->getType() == NodeType::OR);
		Assert::AreEqual((size_t)2, or9->getChildren().size());
		Assert::IsTrue(stmtLst9->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst9->getChildren().size());
		TNode* dblEqual91 = or9->getChildren()[0];
		TNode* dblEqual92 = or9->getChildren()[0];
		Assert::IsTrue(dblEqual91->getType() == NodeType::DBL_EQUAL);
		Assert::AreEqual((size_t)2, dblEqual91->getChildren().size());
		Assert::IsTrue(dblEqual92->getType() == NodeType::DBL_EQUAL);
		Assert::AreEqual((size_t)2, dblEqual92->getChildren().size());
		TNode* assign9 = stmtLst9->getChildren()[0];
		Assert::IsTrue(assign9->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign9->getChildren().size());
	}

	TEST_METHOD(parseIf) {
		Parser parser;
		std::vector<Token> tokens;
		TNode* program;
		TNode* proc;

		// Parse an if statement with 1 statement in then and else clauses
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root1 = proc->getChildren()[0];
		Assert::IsTrue(root1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root1->getChildren().size());
		TNode* if1 = root1->getChildren()[0];
		Assert::IsTrue(if1->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if1->getChildren().size());
		Assert::AreEqual(1, if1->getStmtNum());
		TNode* greaterThan1 = if1->getChildren()[0];
		TNode* thenStmt1 = if1->getChildren()[1];
		TNode* elseStmt1 = if1->getChildren()[2];
		Assert::IsTrue(greaterThan1->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan1->getChildren().size());
		Assert::AreEqual(1, greaterThan1->getStmtNum());
		Assert::IsTrue(thenStmt1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, thenStmt1->getChildren().size());
		Assert::IsTrue(elseStmt1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, elseStmt1->getChildren().size());
		TNode* x1 = greaterThan1->getChildren()[0];
		TNode* one1 = greaterThan1->getChildren()[1];
		Assert::AreEqual(std::string("x"), x1->getValue());
		Assert::AreEqual((size_t)0, x1->getChildren().size());
		Assert::AreEqual(std::string("1"), one1->getValue());
		Assert::AreEqual((size_t)0, one1->getChildren().size());
		TNode* assign11 = thenStmt1->getChildren()[0];
		Assert::IsTrue(assign11->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign11->getChildren().size());
		Assert::AreEqual(2, assign11->getStmtNum());
		TNode* assign12 = elseStmt1->getChildren()[0];
		Assert::IsTrue(assign12->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign12->getChildren().size());
		Assert::AreEqual(3, assign12->getStmtNum());

		// Parse an if statement with 1 if statement in then clause and 1 while statement in else clause
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "v", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "5", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "u", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "6", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root2 = proc->getChildren()[0];
		Assert::IsTrue(root2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root2->getChildren().size());
		TNode* if21 = root2->getChildren()[0];
		Assert::IsTrue(if21->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if21->getChildren().size());
		TNode* greaterThan2 = if21->getChildren()[0];
		TNode* thenStmt2 = if21->getChildren()[1];
		TNode* elseStmt2 = if21->getChildren()[2];
		Assert::IsTrue(greaterThan2->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan2->getChildren().size());
		Assert::IsTrue(thenStmt2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, thenStmt2->getChildren().size());
		Assert::IsTrue(elseStmt2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, elseStmt2->getChildren().size());
		TNode* if22 = thenStmt2->getChildren()[0];
		Assert::IsTrue(if22->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if22->getChildren().size());
		TNode* while2 = elseStmt2->getChildren()[0];
		Assert::IsTrue(while2->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while2->getChildren().size());

		// Parse an if statement with 
		// 1 while statement between 2 assignment statements in then clause 
		// and 1 if statement between 2 assignment statements in else clause
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "1", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "t", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "7", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::WHILE, "while", 5),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "v", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "5", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "u", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "6", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::NAME, "s", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "8", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "r", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "9", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::IF, "if", 2),
			Token(Token::Type::LEFT_PARENTHESIS, "(", 1),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::GREATER, ">", 1),
			Token(Token::Type::DIGIT, "2", 1),
			Token(Token::Type::RIGHT_PARENTHESIS, ")", 1),
			Token(Token::Type::THEN, "then", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "3", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::ELSE, "else", 4),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::NAME, "w", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "4", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::NAME, "q", 1),
			Token(Token::Type::EQUAL, "=", 1),
			Token(Token::Type::DIGIT, "0", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root3 = proc->getChildren()[0];
		Assert::IsTrue(root3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root3->getChildren().size());
		TNode* if31 = root3->getChildren()[0];
		Assert::IsTrue(if31->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if31->getChildren().size());
		TNode* greaterThan3 = if31->getChildren()[0];
		TNode* thenStmt3 = if31->getChildren()[1];
		TNode* elseStmt3 = if31->getChildren()[2];
		Assert::IsTrue(greaterThan3->getType() == NodeType::GREATER);
		Assert::AreEqual((size_t)2, greaterThan3->getChildren().size());
		Assert::IsTrue(thenStmt3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)3, thenStmt3->getChildren().size());
		Assert::IsTrue(elseStmt3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)3, elseStmt3->getChildren().size());
		TNode* assign31 = thenStmt3->getChildren()[0];
		TNode* while3 = thenStmt3->getChildren()[1];
		TNode* assign32 = thenStmt3->getChildren()[2];
		Assert::IsTrue(assign31->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign31->getChildren().size());
		Assert::IsTrue(while3->getType() == NodeType::WHILE);
		Assert::AreEqual((size_t)2, while3->getChildren().size());
		Assert::IsTrue(assign32->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign32->getChildren().size());
		TNode* assign33 = elseStmt3->getChildren()[0];
		TNode* if32 = elseStmt3->getChildren()[1];
		TNode* assign34 = elseStmt3->getChildren()[2];
		Assert::IsTrue(assign33->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign33->getChildren().size());
		Assert::IsTrue(if32->getType() == NodeType::IF);
		Assert::AreEqual((size_t)3, if32->getChildren().size());
		Assert::IsTrue(assign34->getType() == NodeType::ASSIGN);
		Assert::AreEqual((size_t)2, assign34->getChildren().size());
	}

	TEST_METHOD(parseSimpleStmt) {
		Parser parser;
		std::vector<Token> tokens;
		TNode* program;
		TNode* proc;

		// Parse read statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::READ, "read", 4),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root1 = proc->getChildren()[0];
		Assert::IsTrue(root1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root1->getChildren().size());
		TNode* read = root1->getChildren()[0];
		Assert::IsTrue(read->getType() == NodeType::READ);
		Assert::AreEqual(1, read->getStmtNum());
		Assert::AreEqual((size_t)1, read->getChildren().size());
		TNode* x1 = read->getChildren()[0];
		Assert::AreEqual(std::string("x"), x1->getValue());
		Assert::AreEqual((size_t)0, x1->getChildren().size());

		// Parse print statement
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::PRINT, "print", 5),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root2 = proc->getChildren()[0];
		Assert::IsTrue(root2->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, root2->getChildren().size());
		TNode* print = root2->getChildren()[0];
		Assert::IsTrue(print->getType() == NodeType::PRINT);
		Assert::AreEqual(1, print->getStmtNum());
		Assert::AreEqual((size_t)1, print->getChildren().size());
		TNode* x2 = print->getChildren()[0];
		Assert::AreEqual(std::string("x"), x2->getValue());
		Assert::AreEqual((size_t)0, x2->getChildren().size());

		// Parse multiple read and print statements
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::READ, "read", 4),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::PRINT, "print", 5),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::READ, "read", 4),
			Token(Token::Type::NAME, "z", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		proc = program->getChildren()[0];
		Assert::AreEqual((size_t)1, proc->getChildren().size());
		TNode* root3 = proc->getChildren()[0];
		Assert::IsTrue(root3->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)3, root3->getChildren().size());
		TNode* read31 = root3->getChildren()[0];
		TNode* print3 = root3->getChildren()[1];
		TNode* read32 = root3->getChildren()[2];
		Assert::IsTrue(read31->getType() == NodeType::READ);
		Assert::AreEqual(1, read31->getStmtNum());
		Assert::AreEqual((size_t)1, read31->getChildren().size());
		Assert::IsTrue(print3->getType() == NodeType::PRINT);
		Assert::AreEqual(2, print3->getStmtNum());
		Assert::AreEqual((size_t)1, print3->getChildren().size());
		Assert::IsTrue(read32->getType() == NodeType::READ);
		Assert::AreEqual(3, read32->getStmtNum());
		Assert::AreEqual((size_t)1, read32->getChildren().size());
		TNode* x3 = read31->getChildren()[0];
		Assert::AreEqual(std::string("x"), x3->getValue());
		Assert::AreEqual((size_t)0, x3->getChildren().size());
		TNode* y3 = print3->getChildren()[0];
		Assert::AreEqual(std::string("y"), y3->getValue());
		Assert::AreEqual((size_t)0, y3->getChildren().size());
		TNode* z3 = read32->getChildren()[0];
		Assert::AreEqual(std::string("z"), z3->getValue());
		Assert::AreEqual((size_t)0, z3->getChildren().size());
	}

	TEST_METHOD(parseProc) {
		Parser parser;
		std::vector<Token> tokens;
		TNode* program;

		// Parse program with 1 procedure
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::READ, "read", 4),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::IsTrue(program->getType() == NodeType::PROGRAM);
		Assert::AreEqual((size_t)1, program->getChildren().size());
		TNode* proc1 = program->getChildren()[0];
		Assert::IsTrue(proc1->getType() == NodeType::PROCEDURE);
		Assert::AreEqual(std::string("p"), proc1->getValue());
		Assert::AreEqual((size_t)1, proc1->getChildren().size());

		// Parse program with more than 1 procedures
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::READ, "read", 4),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "q", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::READ, "read", 4),
			Token(Token::Type::NAME, "y", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::IsTrue(program->getType() == NodeType::PROGRAM);
		Assert::AreEqual((size_t)2, program->getChildren().size());
		TNode* proc21 = program->getChildren()[0];
		TNode* proc22 = program->getChildren()[1];
		Assert::IsTrue(proc21->getType() == NodeType::PROCEDURE);
		Assert::AreEqual((size_t)1, proc21->getChildren().size());
		Assert::AreEqual(std::string("p"), proc21->getValue());
		Assert::IsTrue(proc22->getType() == NodeType::PROCEDURE);
		Assert::AreEqual((size_t)1, proc22->getChildren().size());
		Assert::AreEqual(std::string("q"), proc22->getValue());
		TNode* stmtLst21 = proc21->getChildren()[0];
		TNode* stmtLst22 = proc22->getChildren()[0];
		Assert::AreEqual((size_t)1, stmtLst21->getChildren().size());
		Assert::AreEqual((size_t)1, stmtLst22->getChildren().size());
		TNode* read21 = stmtLst21->getChildren()[0];
		TNode* read22 = stmtLst22->getChildren()[0];
		Assert::AreEqual(1, read21->getStmtNum());
		Assert::AreEqual(2, read22->getStmtNum());
	}

	TEST_METHOD(parseCall) {
		Parser parser;
		std::vector<Token> tokens;
		TNode* program;

		// Parse program with 1 call stmt
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::READ, "read", 4),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "q", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::CALL, "call", 4),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		program = parser.Parse(tokens);
		Assert::IsTrue(program->getType() == NodeType::PROGRAM);
		Assert::AreEqual((size_t)2, program->getChildren().size());
		TNode* q1 = program->getChildren()[1];
		Assert::IsTrue(q1->getType() == NodeType::PROCEDURE);
		Assert::AreEqual((size_t)1, q1->getChildren().size());
		TNode* stmtLst1 = q1->getChildren()[0];
		Assert::IsTrue(stmtLst1->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst1->getChildren().size());
		TNode* call1 = stmtLst1->getChildren()[0];
		Assert::IsTrue(call1->getType() == NodeType::CALL);
		Assert::AreEqual(string("p"), call1->getValue());

		// Parse program with 2 call stmts
		tokens = std::vector<Token>{
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::READ, "read", 4),
			Token(Token::Type::NAME, "x", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "q", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::CALL, "call", 4),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
			Token(Token::Type::PROCEDURE, "procedure", 9),
			Token(Token::Type::NAME, "r", 1),
			Token(Token::Type::LEFT_CURLY, "{", 1),
			Token(Token::Type::CALL, "call", 4),
			Token(Token::Type::NAME, "p", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::CALL, "call", 4),
			Token(Token::Type::NAME, "q", 1),
			Token(Token::Type::SEMICOLON, ";", 1),
			Token(Token::Type::RIGHT_CURLY, "}", 1),
		};

		parser.resetStmtNum();

		program = parser.Parse(tokens);
		Assert::IsTrue(program->getType() == NodeType::PROGRAM);
		Assert::AreEqual((size_t)3, program->getChildren().size());
		TNode* q2 = program->getChildren()[1];
		TNode* r2 = program->getChildren()[2];
		Assert::IsTrue(q2->getType() == NodeType::PROCEDURE);
		Assert::AreEqual((size_t)1, q2->getChildren().size());
		Assert::IsTrue(r2->getType() == NodeType::PROCEDURE);
		Assert::AreEqual((size_t)1, r2->getChildren().size());
		TNode* stmtLst21 = q2->getChildren()[0];
		TNode* stmtLst22 = r2->getChildren()[0];
		Assert::IsTrue(stmtLst21->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)1, stmtLst21->getChildren().size());
		Assert::IsTrue(stmtLst22->getType() == NodeType::STMT_LST);
		Assert::AreEqual((size_t)2, stmtLst22->getChildren().size());
		TNode* call21 = stmtLst21->getChildren()[0];
		TNode* call22 = stmtLst22->getChildren()[0];
		TNode* call23 = stmtLst22->getChildren()[1];
		Assert::IsTrue(call21->getType() == NodeType::CALL);
		Assert::AreEqual(string("p"), call21->getValue());
		Assert::AreEqual(2, call21->getStmtNum());
		Assert::IsTrue(call22->getType() == NodeType::CALL);
		Assert::AreEqual(string("p"), call22->getValue());
		Assert::AreEqual(3, call22->getStmtNum());
		Assert::IsTrue(call23->getType() == NodeType::CALL);
		Assert::AreEqual(string("q"), call23->getValue());
		Assert::AreEqual(4, call23->getStmtNum());
	}

	TEST_METHOD(createCFGs_singleNonContainerStmt) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("p");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token readToken(Token::Type::READ, "read", 4);
		procStmtLst->addChild(TNode::tokenToNode(readToken, 1));
		// tree is equivalent to:
		// procedure p {
		//   read _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* readNode = CFGs[0];
		Assert::AreEqual(1, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)0, readNode->getChildren().size());
	}

	TEST_METHOD(createCFGs_multipleNonContainerStmts) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		Token printToken(Token::Type::PRINT, "print", 5);
		procStmtLst->addChild(TNode::tokenToNode(readToken, 1));
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 2));
		procStmtLst->addChild(TNode::tokenToNode(printToken, 3));
		procStmtLst->addChild(TNode::createCallNode("p", 4));
		// tree is equivalent to:
		// procedure proc {
		//   read _;
		//   _ = _;
		//   print _;
		//   call p;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* curr = CFGs[0];
		for (int i = 1; i <= 3; i++) {
			Assert::AreEqual(i, curr->getData().getStmtNo());
			Assert::AreEqual((size_t)1, curr->getChildren().size());
			if (i != 1) {
				Assert::AreEqual((size_t)1, curr->getParents().size());
			} else {
				Assert::AreEqual((size_t)0, curr->getParents().size());
			}
			curr = curr->getChildren()[0];
		}

		// curr is end dummy node
		Assert::AreEqual(4, curr->getData().getStmtNo());
		Assert::AreEqual((size_t)0, curr->getChildren().size());
		Assert::AreEqual((size_t)1, curr->getParents().size());
	}

	TEST_METHOD(createCFGs_startWithWhileStmt) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token whileToken(Token::Type::WHILE, "while", 5);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		TNode* whileNode = TNode::tokenToNode(whileToken, 1);
		whileNode->addChild(TNode::tokenToNode(lessToken, 1));
		TNode* whileStmtLst = TNode::createStmtLstNode();
		whileNode->addChild(whileStmtLst);
		whileStmtLst->addChild(TNode::tokenToNode(readToken, 2));
		procStmtLst->addChild(whileNode);
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 3));
		// tree is equivalent to:
		// procedure proc {
		//   while (_ < _) {
		//     read _;
		//   }
		//   _ = _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* whileCFGNode = CFGs[0];
		Assert::AreEqual(1, whileCFGNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, whileCFGNode->getChildren().size());
		Assert::AreEqual((size_t)1, whileCFGNode->getParents().size());
		StmtNode* readNode = whileCFGNode->getChildren()[0];
		StmtNode* assignNode = whileCFGNode->getChildren()[1];
		Assert::AreEqual(2, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode->getChildren().size());
		Assert::AreEqual(3, assignNode->getData().getStmtNo());
		Assert::AreEqual((size_t)0, assignNode->getChildren().size());
		Assert::IsTrue(whileCFGNode == readNode->getChildren()[0]);
		Assert::IsTrue(readNode == whileCFGNode->getParents()[0]);
	}

	TEST_METHOD(createCFGs_whileStmtInMiddle) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token whileToken(Token::Type::WHILE, "while", 5);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 1));
		TNode* whileNode = TNode::tokenToNode(whileToken, 2);
		whileNode->addChild(TNode::tokenToNode(lessToken, 2));
		TNode* whileStmtLst = TNode::createStmtLstNode();
		whileNode->addChild(whileStmtLst);
		whileStmtLst->addChild(TNode::tokenToNode(readToken, 3));
		procStmtLst->addChild(whileNode);
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 4));
		// tree is equivalent to:
		// procedure proc {
		//   _ = _;
		//   while (_ < _) {
		//     read _;
		//   }
		//   _ = _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* assignNode1 = CFGs[0];
		Assert::AreEqual(1, assignNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode1->getChildren().size());
		StmtNode* whileCFGNode = assignNode1->getChildren()[0];
		Assert::AreEqual(2, whileCFGNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, whileCFGNode->getChildren().size());
		Assert::AreEqual((size_t)2, whileCFGNode->getParents().size());
		Assert::IsTrue(assignNode1 == whileCFGNode->getParents()[0]);
		StmtNode* readNode = whileCFGNode->getChildren()[0];
		StmtNode* assignNode2 = whileCFGNode->getChildren()[1];
		Assert::AreEqual(3, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode->getChildren().size());
		Assert::AreEqual(4, assignNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)0, assignNode2->getChildren().size());
		Assert::IsTrue(whileCFGNode == readNode->getChildren()[0]);
		Assert::IsTrue(readNode == whileCFGNode->getParents()[1]);
	}

	TEST_METHOD(createCFGs_endWithWhileStmt) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token whileToken(Token::Type::WHILE, "while", 5);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 1));
		TNode* whileNode = TNode::tokenToNode(whileToken, 2);
		whileNode->addChild(TNode::tokenToNode(lessToken, 2));
		TNode* whileStmtLst = TNode::createStmtLstNode();
		whileNode->addChild(whileStmtLst);
		whileStmtLst->addChild(TNode::tokenToNode(readToken, 3));
		procStmtLst->addChild(whileNode);
		// tree is equivalent to:
		// procedure proc {
		//   _ = _;
		//   while (_ < _) {
		//     read _;
		//   }
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* assignNode = CFGs[0];
		Assert::AreEqual(1, assignNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode->getChildren().size());
		StmtNode* whileCFGNode = assignNode->getChildren()[0];
		Assert::AreEqual(2, whileCFGNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, whileCFGNode->getChildren().size());
		StmtNode* readNode = whileCFGNode->getChildren()[0];
		Assert::AreEqual(3, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode->getChildren().size());
		Assert::IsTrue(whileCFGNode == readNode->getChildren()[0]);
	}

	TEST_METHOD(createCFGs_startWithIfStmt) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token ifToken(Token::Type::IF, "if", 2);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		Token printToken(Token::Type::PRINT, "print", 5);
		TNode* ifNode = TNode::tokenToNode(ifToken, 1);
		ifNode->addChild(TNode::tokenToNode(lessToken, 1));
		TNode* ifThenStmtLst = TNode::createStmtLstNode();
		TNode* ifElseStmtLst = TNode::createStmtLstNode();
		ifNode->addChild(ifThenStmtLst);
		ifNode->addChild(ifElseStmtLst);
		ifThenStmtLst->addChild(TNode::tokenToNode(readToken, 2));
		ifElseStmtLst->addChild(TNode::tokenToNode(equalToken, 3));
		procStmtLst->addChild(ifNode);
		procStmtLst->addChild(TNode::tokenToNode(printToken, 4));
		// tree is equivalent to:
		// procedure proc {
		//   if (_ < _) then {
		//     read _;
		//   } else {
		//     _ = _;
		//   }
		//   print _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* ifCFGNode = CFGs[0];
		Assert::AreEqual(1, ifCFGNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, ifCFGNode->getChildren().size());
		StmtNode* readNode = ifCFGNode->getChildren()[0];
		StmtNode* assignNode = ifCFGNode->getChildren()[1];
		Assert::AreEqual(2, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode->getChildren().size());
		Assert::AreEqual(3, assignNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode->getChildren().size());
		StmtNode* printNode = readNode->getChildren()[0];
		Assert::AreEqual(4, printNode->getData().getStmtNo());
		Assert::AreEqual((size_t)0, printNode->getChildren().size());
		Assert::AreEqual((size_t)2, printNode->getParents().size());
		Assert::IsTrue(readNode == printNode->getParents()[0]);
		Assert::IsTrue(assignNode == printNode->getParents()[1]);
		Assert::IsTrue(printNode == assignNode->getChildren()[0]); // printNode should also be next node of assignNode
	}

	TEST_METHOD(createCFGs_ifStmtInMiddle) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token ifToken(Token::Type::IF, "if", 2);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		Token printToken(Token::Type::PRINT, "print", 5);
		procStmtLst->addChild(TNode::tokenToNode(printToken, 1));
		TNode* ifNode = TNode::tokenToNode(ifToken, 2);
		ifNode->addChild(TNode::tokenToNode(lessToken, 2));
		TNode* ifThenStmtLst = TNode::createStmtLstNode();
		TNode* ifElseStmtLst = TNode::createStmtLstNode();
		ifNode->addChild(ifThenStmtLst);
		ifNode->addChild(ifElseStmtLst);
		ifThenStmtLst->addChild(TNode::tokenToNode(readToken, 3));
		ifElseStmtLst->addChild(TNode::tokenToNode(equalToken, 4));
		procStmtLst->addChild(ifNode);
		procStmtLst->addChild(TNode::tokenToNode(printToken, 5));
		// tree is equivalent to:
		// procedure proc {
		//   print _;
		//   if (_ < _) then {
		//     read _;
		//   } else {
		//     _ = _;
		//   }
		//   print _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* printNode1 = CFGs[0];
		Assert::AreEqual(1, printNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, printNode1->getChildren().size());
		StmtNode* ifCFGNode = printNode1->getChildren()[0];
		Assert::AreEqual(2, ifCFGNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, ifCFGNode->getChildren().size());
		StmtNode* readNode = ifCFGNode->getChildren()[0];
		StmtNode* assignNode = ifCFGNode->getChildren()[1];
		Assert::AreEqual(3, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode->getChildren().size());
		Assert::AreEqual(4, assignNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode->getChildren().size());
		StmtNode* printNode2 = assignNode->getChildren()[0];
		Assert::AreEqual(5, printNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)0, printNode2->getChildren().size());
		Assert::IsTrue(printNode2 == readNode->getChildren()[0]); // printNode2 should also be next node of readNode
	}

	TEST_METHOD(createCFGs_endWithIfStmt) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token ifToken(Token::Type::IF, "if", 2);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		Token printToken(Token::Type::PRINT, "print", 5);
		procStmtLst->addChild(TNode::tokenToNode(printToken, 1));
		TNode* ifNode = TNode::tokenToNode(ifToken, 2);
		ifNode->addChild(TNode::tokenToNode(lessToken, 2));
		TNode* ifThenStmtLst = TNode::createStmtLstNode();
		TNode* ifElseStmtLst = TNode::createStmtLstNode();
		ifNode->addChild(ifThenStmtLst);
		ifNode->addChild(ifElseStmtLst);
		ifThenStmtLst->addChild(TNode::tokenToNode(readToken, 3));
		ifElseStmtLst->addChild(TNode::tokenToNode(equalToken, 4));
		procStmtLst->addChild(ifNode);
		// tree is equivalent to:
		// procedure proc {
		//   print _;
		//   if (_ < _) then {
		//     read _;
		//   } else {
		//     _ = _;
		//   }
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* printNode = CFGs[0];
		Assert::AreEqual(1, printNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, printNode->getChildren().size());
		StmtNode* ifCFGNode = printNode->getChildren()[0];
		Assert::AreEqual(2, ifCFGNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, ifCFGNode->getChildren().size());
		StmtNode* readNode = ifCFGNode->getChildren()[0];
		StmtNode* assignNode = ifCFGNode->getChildren()[1];
		Assert::AreEqual(3, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)0, readNode->getChildren().size());
		Assert::AreEqual(4, assignNode->getData().getStmtNo());
		Assert::AreEqual((size_t)0, assignNode->getChildren().size());
	}

	TEST_METHOD(createCFGs_whileStmtWithNestedWhile) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token whileToken(Token::Type::WHILE, "while", 5);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 1));
		TNode* parentWhile = TNode::tokenToNode(whileToken, 2);
		parentWhile->addChild(TNode::tokenToNode(lessToken, 2));
		TNode* parentWhileStmtLst = TNode::createStmtLstNode();
		parentWhile->addChild(parentWhileStmtLst);
		TNode* nestedWhile = TNode::tokenToNode(whileToken, 3);
		nestedWhile->addChild(TNode::tokenToNode(lessToken, 3));
		TNode* nestedWhileStmtLst = TNode::createStmtLstNode();
		nestedWhile->addChild(nestedWhileStmtLst);
		nestedWhileStmtLst->addChild(TNode::tokenToNode(readToken, 4));
		parentWhileStmtLst->addChild(nestedWhile);
		parentWhileStmtLst->addChild(TNode::tokenToNode(readToken, 5));
		procStmtLst->addChild(parentWhile);
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 6));
		// tree is equivalent to:
		// procedure proc {
		//   _ = _;
		//   while (_ < _) {
		//     while (_ < _) {
		//        read _;
		//     }
		//     read _;
		//   }
		//   _ = _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* assignNode1 = CFGs[0];
		Assert::AreEqual(1, assignNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode1->getChildren().size());
		StmtNode* parentWhileNode = assignNode1->getChildren()[0];
		Assert::AreEqual(2, parentWhileNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, parentWhileNode->getChildren().size());
		StmtNode* nestedWhileNode = parentWhileNode->getChildren()[0];
		StmtNode* assignNode2 = parentWhileNode->getChildren()[1];
		Assert::AreEqual(3, nestedWhileNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, nestedWhileNode->getChildren().size());
		Assert::AreEqual(6, assignNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)0, assignNode2->getChildren().size());
		StmtNode* readNode1 = nestedWhileNode->getChildren()[0];
		StmtNode* readNode2 = nestedWhileNode->getChildren()[1];
		Assert::AreEqual(4, readNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode1->getChildren().size());
		Assert::AreEqual(5, readNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode2->getChildren().size());
		Assert::IsTrue(nestedWhileNode == readNode1->getChildren()[0]);
		Assert::IsTrue(parentWhileNode == readNode2->getChildren()[0]);
	}

	TEST_METHOD(createCFGs_whileStmtWithNestedIf) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token whileToken(Token::Type::WHILE, "while", 5);
		Token ifToken(Token::Type::IF, "if", 2);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		Token printToken(Token::Type::PRINT, "print", 5);
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 1));
		TNode* parentWhile = TNode::tokenToNode(whileToken, 2);
		parentWhile->addChild(TNode::tokenToNode(lessToken, 2));
		TNode* parentWhileStmtLst = TNode::createStmtLstNode();
		parentWhile->addChild(parentWhileStmtLst);
		parentWhileStmtLst->addChild(TNode::createCallNode("p", 3));
		TNode* nestedIf = TNode::tokenToNode(ifToken, 4);
		nestedIf->addChild(TNode::tokenToNode(lessToken, 4));
		TNode* ifThenStmtLst = TNode::createStmtLstNode();
		TNode* ifElseStmtLst = TNode::createStmtLstNode();
		nestedIf->addChild(ifThenStmtLst);
		nestedIf->addChild(ifElseStmtLst);
		ifThenStmtLst->addChild(TNode::tokenToNode(readToken, 5));
		ifElseStmtLst->addChild(TNode::tokenToNode(printToken, 6));
		parentWhileStmtLst->addChild(nestedIf);
		procStmtLst->addChild(parentWhile);
		procStmtLst->addChild(TNode::tokenToNode(equalToken, 7));
		// tree is equivalent to:
		// procedure proc {
		//   _ = _;
		//   while (_ < _) {
		//     call _;
		//     if (_ < _) then {
		//        read _;
		//     } else {
		//        print _;
		//     }
		//   }
		//   _ = _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* assignNode1 = CFGs[0];
		Assert::AreEqual(1, assignNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode1->getChildren().size());
		StmtNode* parentWhileNode = assignNode1->getChildren()[0];
		Assert::AreEqual(2, parentWhileNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, parentWhileNode->getChildren().size());
		StmtNode* callNode = parentWhileNode->getChildren()[0];
		StmtNode* assignNode2 = parentWhileNode->getChildren()[1];
		Assert::AreEqual(3, callNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, callNode->getChildren().size());
		Assert::AreEqual(7, assignNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)0, assignNode2->getChildren().size());
		StmtNode* nestedIfNode = callNode->getChildren()[0];
		Assert::AreEqual(4, nestedIfNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, nestedIfNode->getChildren().size());
		StmtNode* readNode = nestedIfNode->getChildren()[0];
		StmtNode* printNode = nestedIfNode->getChildren()[1];
		Assert::AreEqual(5, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode->getChildren().size());
		Assert::AreEqual(6, printNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, printNode->getChildren().size());
		Assert::IsTrue(parentWhileNode == readNode->getChildren()[0]);
		Assert::IsTrue(readNode->getChildren()[0] == printNode->getChildren()[0]);
	}

	TEST_METHOD(createCFGs_ifStmtWithNestedWhileInThenAndNestedIfInElse) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token whileToken(Token::Type::WHILE, "while", 5);
		Token ifToken(Token::Type::IF, "if", 2);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		Token printToken(Token::Type::PRINT, "print", 5);
		procStmtLst->addChild(TNode::tokenToNode(printToken, 1));
		TNode* parentIf = TNode::tokenToNode(ifToken, 2);
		parentIf->addChild(TNode::tokenToNode(lessToken, 2));
		TNode* parentThenStmtLst = TNode::createStmtLstNode();
		TNode* parentElseStmtLst = TNode::createStmtLstNode();
		parentIf->addChild(parentThenStmtLst);
		parentIf->addChild(parentElseStmtLst);

		parentThenStmtLst->addChild(TNode::tokenToNode(readToken, 3));
		TNode* nestedWhile = TNode::tokenToNode(whileToken, 4);
		parentThenStmtLst->addChild(nestedWhile);
		nestedWhile->addChild(TNode::tokenToNode(lessToken, 4));
		TNode* whileStmtLst = TNode::createStmtLstNode();
		nestedWhile->addChild(whileStmtLst);
		whileStmtLst->addChild(TNode::createCallNode("p", 5));

		TNode* nestedIf = TNode::tokenToNode(ifToken, 6);
		parentElseStmtLst->addChild(nestedIf);
		nestedIf->addChild(TNode::tokenToNode(lessToken, 6));
		TNode* nestedThenStmtLst = TNode::createStmtLstNode();
		TNode* nestedElseStmtLst = TNode::createStmtLstNode();
		nestedIf->addChild(nestedThenStmtLst);
		nestedIf->addChild(nestedElseStmtLst);
		nestedThenStmtLst->addChild(TNode::tokenToNode(equalToken, 7));
		nestedElseStmtLst->addChild(TNode::tokenToNode(equalToken, 8));

		parentElseStmtLst->addChild(TNode::tokenToNode(equalToken, 9));
		procStmtLst->addChild(parentIf);
		procStmtLst->addChild(TNode::tokenToNode(printToken, 10));
		// tree is equivalent to:
		// procedure proc {
		//   print _;
		//   if (_ < _) then {
		//     read _;
		//     while (_ < _) {
		//       call _;
		//     }
		//   } else {
		//     if (_ < _) then {
		//       _ = _;
		//     } else {
		//       _ = _;
		//     }
		//     _ = _;
		//   }
		//   print _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* printNode1 = CFGs[0];
		Assert::AreEqual(1, printNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, printNode1->getChildren().size());
		StmtNode* parentIfNode = printNode1->getChildren()[0];
		Assert::AreEqual(2, parentIfNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, parentIfNode->getChildren().size());
		StmtNode* readNode = parentIfNode->getChildren()[0];
		StmtNode* nestedIfNode = parentIfNode->getChildren()[1];
		Assert::AreEqual(3, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode->getChildren().size());
		Assert::AreEqual(6, nestedIfNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, nestedIfNode->getChildren().size());
		StmtNode* whileNode = readNode->getChildren()[0];
		StmtNode* assignNode1 = nestedIfNode->getChildren()[0];
		StmtNode* assignNode2 = nestedIfNode->getChildren()[1];
		Assert::AreEqual(4, whileNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, whileNode->getChildren().size());
		Assert::AreEqual(7, assignNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode1->getChildren().size());
		Assert::AreEqual(8, assignNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode2->getChildren().size());
		StmtNode* callNode = whileNode->getChildren()[0];
		StmtNode* printNode2 = whileNode->getChildren()[1];
		StmtNode* assignNode3 = assignNode1->getChildren()[0];
		Assert::AreEqual(5, callNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, callNode->getChildren().size());
		Assert::AreEqual(10, printNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)0, printNode2->getChildren().size());
		Assert::AreEqual(9, assignNode3->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode3->getChildren().size());
		Assert::IsTrue(whileNode == callNode->getChildren()[0]);
		Assert::IsTrue(printNode2 == assignNode3->getChildren()[0]); // printNode2 should also be the next node of assignNode3
		Assert::IsTrue(assignNode3 == assignNode2->getChildren()[0]); // assignNode3 should also be the next node of assignNode2
	}

	TEST_METHOD(createCFGs_ifStmtWithNestedIfInThenAndNestedWhileInElse) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* proc = TNode::createProcNode("proc");
		tree->addChild(proc);
		TNode* procStmtLst = TNode::createStmtLstNode();
		proc->addChild(procStmtLst);
		Token whileToken(Token::Type::WHILE, "while", 5);
		Token ifToken(Token::Type::IF, "if", 2);
		Token lessToken(Token::Type::LESS, "<", 1);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		Token printToken(Token::Type::PRINT, "print", 5);
		procStmtLst->addChild(TNode::tokenToNode(printToken, 1));
		TNode* parentIf = TNode::tokenToNode(ifToken, 2);
		parentIf->addChild(TNode::tokenToNode(lessToken, 2));
		TNode* parentThenStmtLst = TNode::createStmtLstNode();
		TNode* parentElseStmtLst = TNode::createStmtLstNode();
		parentIf->addChild(parentThenStmtLst);
		parentIf->addChild(parentElseStmtLst);

		parentThenStmtLst->addChild(TNode::tokenToNode(readToken, 3));
		TNode* nestedIf = TNode::tokenToNode(ifToken, 4);
		parentThenStmtLst->addChild(nestedIf);
		nestedIf->addChild(TNode::tokenToNode(lessToken, 4));
		TNode* nestedThenStmtLst = TNode::createStmtLstNode();
		TNode* nestedElseStmtLst = TNode::createStmtLstNode();
		nestedIf->addChild(nestedThenStmtLst);
		nestedIf->addChild(nestedElseStmtLst);
		nestedThenStmtLst->addChild(TNode::tokenToNode(equalToken, 5));
		nestedElseStmtLst->addChild(TNode::tokenToNode(equalToken, 6));


		TNode* nestedWhile = TNode::tokenToNode(whileToken, 7);
		parentElseStmtLst->addChild(nestedWhile);
		nestedWhile->addChild(TNode::tokenToNode(lessToken, 7));
		TNode* whileStmtLst = TNode::createStmtLstNode();
		nestedWhile->addChild(whileStmtLst);
		whileStmtLst->addChild(TNode::createCallNode("p", 8));

		parentElseStmtLst->addChild(TNode::tokenToNode(equalToken, 9));
		procStmtLst->addChild(parentIf);
		procStmtLst->addChild(TNode::tokenToNode(printToken, 10));
		// tree is equivalent to:
		// procedure proc {
		//   print _;
		//   if (_ < _) then {
		//     read _;
		//     if (_ < _) then {
		//       _ = _;
		//     } else {
		//       _ = _;
		//     }
		//   } else {
		//     while (_ < _) {
		//       call _;
		//     }
		//     _ = _;
		//   }
		//   print _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)1, CFGs.size());
		StmtNode* printNode1 = CFGs[0];
		Assert::AreEqual(1, printNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, printNode1->getChildren().size());
		StmtNode* parentIfNode = printNode1->getChildren()[0];
		Assert::AreEqual(2, parentIfNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, parentIfNode->getChildren().size());
		StmtNode* readNode = parentIfNode->getChildren()[0];
		StmtNode* whileNode = parentIfNode->getChildren()[1];
		Assert::AreEqual(3, readNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, readNode->getChildren().size());
		Assert::AreEqual(7, whileNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, whileNode->getChildren().size());
		StmtNode* nestedIfNode = readNode->getChildren()[0];
		StmtNode* callNode = whileNode->getChildren()[0];
		StmtNode* assignNode1 = whileNode->getChildren()[1];
		Assert::AreEqual(4, nestedIfNode->getData().getStmtNo());
		Assert::AreEqual((size_t)2, nestedIfNode->getChildren().size());
		Assert::AreEqual(8, callNode->getData().getStmtNo());
		Assert::AreEqual((size_t)1, callNode->getChildren().size());
		Assert::AreEqual(9, assignNode1->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode1->getChildren().size());
		Assert::IsTrue(whileNode == callNode->getChildren()[0]);
		StmtNode* assignNode2 = nestedIfNode->getChildren()[0];
		StmtNode* assignNode3 = nestedIfNode->getChildren()[1];
		StmtNode* printNode2 = assignNode1->getChildren()[0];
		Assert::AreEqual(5, assignNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode2->getChildren().size());
		Assert::AreEqual(6, assignNode3->getData().getStmtNo());
		Assert::AreEqual((size_t)1, assignNode3->getChildren().size());
		Assert::AreEqual(10, printNode2->getData().getStmtNo());
		Assert::AreEqual((size_t)0, printNode2->getChildren().size());
		Assert::AreEqual((size_t)3, printNode2->getParents().size());
		Assert::IsTrue(assignNode2 == printNode2->getParents()[0]);
		Assert::IsTrue(assignNode3 == printNode2->getParents()[1]);
		Assert::IsTrue(assignNode1 == printNode2->getParents()[2]);
		Assert::IsTrue(printNode2 == assignNode2->getChildren()[0]); // printNode2 should also be the next node of assignNode2
		Assert::IsTrue(printNode2 == assignNode3->getChildren()[0]); // printNode2 should also be the next node of assignNode3
	}

	TEST_METHOD(createCFGs_multipleProcs) {
		Parser parser;
		TNode* tree = new TNode();
		TNode* procP = TNode::createProcNode("p");
		TNode* procQ = TNode::createProcNode("q");
		tree->addChild(procP);
		tree->addChild(procQ);
		TNode* pStmtLst = TNode::createStmtLstNode();
		TNode* qStmtLst = TNode::createStmtLstNode();
		procP->addChild(pStmtLst);
		procQ->addChild(qStmtLst);
		Token readToken(Token::Type::READ, "read", 4);
		Token equalToken(Token::Type::EQUAL, "=", 1);
		pStmtLst->addChild(TNode::tokenToNode(readToken, 1));
		qStmtLst->addChild(TNode::tokenToNode(equalToken, 2));
		// tree is equivalent to:
		// procedure p {
		//   read _;
		// }
		// procedure q {
		//   _ = _;
		// }

		vector<StmtNode*> CFGs = parser.createCFGs(tree);
		Assert::AreEqual((size_t)2, CFGs.size());
		StmtNode* pNode = CFGs[0];
		StmtNode* qNode = CFGs[1];
		Assert::AreEqual(1, pNode->getData().getStmtNo());
		Assert::AreEqual((size_t)0, pNode->getChildren().size());
		Assert::AreEqual(2, qNode->getData().getStmtNo());
		Assert::AreEqual((size_t)0, qNode->getChildren().size());
	}
	};
}
