#include "TestWrapper.h"
#include "AbstractWrapper.h"
#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "SP/Parser.h"
#include "Token.h"
#include "CustomException.h"
#include "SP/SourceValidator.h"
#include "SP/SourceProcessor.h"
#include "PKB/ASTProcessor.h"
#include "PKB/CFGProcessor.h"
#include "QPS/QPS.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
	if (wrapper == 0) wrapper = new TestWrapper;
	return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;
AbstractWrapper::~AbstractWrapper() = default;

// a default constructor
TestWrapper::TestWrapper() {
	// create any objects here as instance variables of this class
	// as well as any initialization required for your spa program
}

TestWrapper::~TestWrapper() = default;

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...

		// read source file
	std::ifstream sourceFile(filename);
	std::string source = string((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());;


	SourceProcessor sp;
	TNode* tree;
	vector<StmtNode*> cfgs;

	try {
		sp.processSource(source.c_str(), tree, cfgs);
	} catch (LexerException e) {
		cerr << endl << endl << endl << "--- Lexer Error ---" << endl << endl;
		cerr << "[Lexer] " << e.what() << endl << endl;
		exit(1);
	} catch (SourceValidatorException e) {
		cerr << endl << endl << endl << "--- Validation Error ---" << endl << endl;
		cerr << "[SourceValidator] " << e.what() << endl << endl;
		exit(1);
	} catch (exception& e) {
		cerr << endl << endl << endl << "--- Parsing Error ---" << endl << endl;
		cerr << "[Parser] " << e.what() << endl << endl;
		exit(1);
	}

	ASTProcessor::processAST(tree);
	CFGProcessor::processCFGs(cfgs);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>&results) {
	// call your evaluator to evaluate the query here
	// ...code to evaluate query...
	PKB::startQuery(); // Clears the PKB cache at every start of the query
	QPS::processQuery(query, results);
	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
}
