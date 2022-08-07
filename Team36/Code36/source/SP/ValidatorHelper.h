#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "../Token.h"

using namespace std;

class ValidatorHelper {
public:
	static bool isExpr(vector<Token> expr);
private:
	static bool isTerm(vector<Token> term);
	static bool isFactor(vector<Token> factor);
};


// Graph Utility Functions for Call Chain Validation
namespace CallChainValidation {
	typedef unordered_map<string, vector<string>> Graph;
	typedef pair<string, string> Edge;
	void addEdge(Graph& graph, Edge edge);
	bool hasCycle(Graph& graph);
}