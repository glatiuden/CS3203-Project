#include "SourceValidator.h"
#include "../CustomException.h"

SourceValidator::SourceValidator(vector<Token>& t) :
	tokens(t),
	currIndex(0),
	existingProcNames(unordered_set<string>()),
	calledProcs(vector<string>()),
	callGraph(Graph()) {
}

// Validate has side effect of converting NAME tokens to more specific token types.
bool SourceValidator::validate() {

	// Basic Requirements
	validateProgram();

	// Advanced Requirements
	validateCallChains();

	return true;
}

void SourceValidator::validateProgram() {
	while (hasNext()) {
		Token& curr = get();
		if (!curr.isName() || curr.getValue() != "procedure") throw SourceValidatorException("Unknown Statement");

		curr.setType(Token::Type::PROCEDURE);

		validateProcedure();
	}
}

void SourceValidator::validateProcedure() {
	// First Token guranteed to be procedure
	// Second token must be NAME

	if (!hasNext() || !peek().isName()) throw SourceValidatorException("Procedure statement must have a PROC_NAME.");

	// Ensure that proc name is unique
	string name = get().getValue();
	if (existingProcNames.count(name)) throw SourceValidatorException("Procedure names must be unique.");
	existingProcNames.insert(name);

	// Set current proc to provide context for CALL in validateReadPrintCall()
	currentProc = name;

	if (!hasNext() || !get().isLCurly()) throw SourceValidatorException("Procedure statement must have a LPARAM.");

	validateStmtLst();

	if (!hasNext() || !get().isRCurly()) throw SourceValidatorException("Procedure statement must have a RPARAM.");

}
void SourceValidator::validateStmtLst() {
	while (hasNext()) {
		if (peek().isRCurly()) return;

		if (!(peek().isName())) throw SourceValidatorException("Unknown Statement");

		validateStmt();
	}
}

// All statements begin with NAME
// Next Token Reference
// read, print, call : NAME
// while, if : LPARAM
void SourceValidator::validateStmt() {
	if (!hasNext(1)) throw SourceValidatorException("Incomplete statement at EOF");

	switch (peek(1).getType()) {
		case Token::Type::NAME:
			validateReadPrintCall();
			break;
		case Token::Type::LEFT_PARENTHESIS:
			validateWhileIf();
			break;
		case Token::Type::EQUAL:
			validateAssign();
			break;

		default:
			throw SourceValidatorException("Unrecognized Statement");
	}
}

// Parent call gurantees first two tokens are NAME NAME.
// Store data for deferred CALL validation.
void SourceValidator::validateReadPrintCall() {
	Token& curr = get();
	if (curr.getValue() == "read") {
		curr.setType(Token::Type::READ);
		get(); // NAME
	} else if (curr.getValue() == "print") {
		curr.setType(Token::Type::PRINT);
		get(); // NAME
	} else if (curr.getValue() == "call") {
		curr.setType(Token::Type::CALL);
		auto calledProc = get().getValue();
		calledProcs.push_back(calledProc); // Store all called procs for validation after all procs have been iterated. See validateCallCyles().
		addEdge(callGraph, make_pair(currentProc, calledProc));
	} else {
		throw SourceValidatorException("Unknown Statement");
	}

	// Check that stmt is closed with ;
	if (!hasNext() || !get().isSemicolon()) throw SourceValidatorException("Missing ; for read | print | call statement.");
}

void SourceValidator::validateWhileIf() {
	if (peek().getValue() == "if") return validateIf();
	if (peek().getValue() == "while") return validateWhile();

	throw SourceValidatorException("Unknown Statement");
}

void SourceValidator::validateWhile() {
	get().setType(Token::Type::WHILE);
	get(); // LParenthesis

	validateCondExpr();

	if (!hasNext() || !get().isRParam()) throw SourceValidatorException("Missing closing ')' for WHILE after COND_EXPR");

	if (!hasNext() || !get().isLCurly()) throw SourceValidatorException("Missing '{' for WHILE after COND_EXPR");

	validateStmtLst();

	if (!hasNext() || !get().isRCurly()) throw SourceValidatorException("Missing '}' for WHILE after STMTLST");
}

// REFERENCE
// if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
void SourceValidator::validateIf() {
	get().setType(Token::Type::IF);
	get(); // LParenthesis

	validateCondExpr();

	if (!hasNext() || !get().isRParam()) throw SourceValidatorException("Missing ')' for IF after COND_EXPR");

	if (!hasNext()
		|| !peek().isName()
		|| !(peek().getValue() == "then")
		) throw SourceValidatorException("Missing THEN for IF");

	get().setType(Token::Type::THEN);

	if (!hasNext() || !get().isLCurly()) throw SourceValidatorException("Missing '{' for THEN clause");

	validateStmtLst();

	if (!hasNext() || !get().isRCurly()) throw SourceValidatorException("Missing '}' for THEN clause");

	if (!hasNext()
		|| !peek().isName()
		|| !(peek().getValue() == "else")
		) throw SourceValidatorException("Missing THEN for IF");

	get().setType(Token::Type::ELSE);

	if (!hasNext() || !get().isLCurly()) throw SourceValidatorException("Missing '{' for ELSE clause");

	validateStmtLst();

	if (!hasNext() || !get().isRCurly()) throw SourceValidatorException("Missing '}' for ELSE clause");
}

void SourceValidator::validateCondExpr() {
	vector<Token> cond_expr;
	int unclosed = 0;
	while (
		hasNext() &&
		(unclosed != 0 || !peek().isRParam())
		) {
		if (peek().isRParam()) unclosed -= 1;
		if (peek().isLParam()) unclosed += 1;
		cond_expr.push_back(get());
	}

	if (!isCondExpr(cond_expr)) throw SourceValidatorException("\"" + stringify(cond_expr) + "\"  is not a valid cond_expr");
}

// NAME and EQUAL already validated.
// Get all tokens related to expr except semi colon
// validate assign
// Ensure assign has a closing semicolon
void SourceValidator::validateAssign() {
	get(); // NAME
	get(); // EQUAL

	vector<Token> expr;

	while (hasNext() && !peek().isSemicolon()) expr.push_back(get());

	if (!ValidatorHelper::isExpr(expr)) throw SourceValidatorException("\"" + stringify(expr) + "\" is an invalid expr for assign");
	if (!hasNext() || !get().isSemicolon()) throw SourceValidatorException("Assign stmt missing semicolon");
}

// Ensure CALLs call existing procedures
// Ensure no call cycles.
void SourceValidator::validateCallChains() {
	for (string proc : calledProcs)
		if (!existingProcNames.count(proc)) throw SourceValidatorException("Cannot call undefined proc: " + proc);

	if (hasCycle(callGraph)) throw SourceValidatorException("Cyclic SIMPLE source not allowed.");
}

/*
 *	cond_expr:
 *		rel_expr
 *		| '!' '(' cond_expr ')'
 *		| '(' cond_expr ')' '&&' '(' cond_expr ')'
 *		| '(' cond_expr ')' '||' '(' cond_expr ')'
 */
bool SourceValidator::isCondExpr(vector<Token> expr) {
	auto isNameOrDigit = [](Token t) -> bool { return t.isName() || t.isDigit(); };

	if (expr.size() == 0) return false;
	if (expr.size() == 1 && isNameOrDigit(expr[0])) return true;

	// CASE 1 : '!' '(' cond_expr ')'
	if (expr.size() >= 3
		&& expr[0].isNot()
		&& expr[1].isLParam()
		&& expr.back().isRParam()
		) return isCondExpr(vector<Token>(expr.begin() + 2, expr.end() - 1));


	// CASE 2 : | '(' cond_expr ')' '||&&' '(' cond_expr ')'
	if (!(
		expr.size() >= 5
		&& expr.front().isLParam()
		&& expr.back().isRParam()
		)) return isRelExpr(expr); // else CASE 3 : rel_expr


	// Find outermost ||&& 
	int unclosed = 0;
	auto op = expr.begin();
	for (; op < expr.end(); op++) {
		if (op->isLParam()) unclosed += 1;
		if (op->isRParam()) unclosed -= 1;

		if (
			(op->isAnd() || op->isOr())
			&& unclosed == 0
			) break;
	}
	if (op == expr.end()) return isRelExpr(expr);

	if (!(op + 1)->isLParam()) throw SourceValidatorException("&&|| Missing an opening '('");

	vector<Token> left(expr.begin() + 1, op - 1);
	vector<Token> right(op + 2, expr.end() - 1);

	return isCondExpr(left) && isCondExpr(right);
}

// rel_expr: rel_factor 'COND_OP' rel_factor
bool SourceValidator::isRelExpr(vector<Token> expr) {

	if (expr.size() < 3) return false;

	auto op = find_if(expr.begin(), expr.end(), [](Token t) -> bool { return t.isConditionalOp(); });
	if (op == expr.end()) return false;

	vector<Token> left(expr.begin(), op);
	vector<Token> right(op + 1, expr.end());

	return isRelFactor(left) && isRelFactor(right);
}

// check for var_name | const_value
bool SourceValidator::isRelFactor(vector<Token> relFactor) {
	if (relFactor.size() == 1) return relFactor[0].isName() || relFactor[0].isDigit();
	return ValidatorHelper::isExpr(relFactor);
}

string SourceValidator::stringify(vector<Token> tokens) {
	string s = "";
	for (auto i : tokens) s += i.getValue();
	return s;
}

bool SourceValidator::hasNext(int offset) {
	return currIndex + offset < int(tokens.size());
}

Token& SourceValidator::get() {
	return tokens[currIndex++];
}

Token& SourceValidator::peek(int offset) {
	return tokens[currIndex + offset];
}
