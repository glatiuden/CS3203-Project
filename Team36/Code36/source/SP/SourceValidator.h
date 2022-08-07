#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../Token.h"
#include "ValidatorHelper.h"

using namespace std;
using namespace CallChainValidation;

class SourceValidator {
public:
	SourceValidator(vector<Token>& t);
	bool validate();

private:
	vector<Token>& tokens;
	int currIndex;
	string currentProc;

	//Mainly used to ensure proc names are unique
	unordered_set<string> existingProcNames;
	// Used to ensure call refers to valid procname
	vector<string> calledProcs;
	Graph callGraph;

	bool hasNext(int offset = 0);
	Token& get();
	Token& peek(int offset = 0);

	// Basic Validation
	void validateProgram();
	void validateProcedure();
	void validateStmtLst();
	void validateStmt();
	void validateReadPrintCall();
	void validateWhileIf();
	void validateWhile();
	void validateIf();
	void validateCondExpr();
	void validateAssign();
	
	// Advanced Validation (requirements not captured by CSG)
	void validateCallChains();

	bool isCondExpr(vector<Token> condExpr);
	bool isRelExpr(vector<Token> relExp);
	bool isRelFactor(vector<Token> relFactor);

	// Utility Functions
	string stringify(vector<Token> tokens);

};