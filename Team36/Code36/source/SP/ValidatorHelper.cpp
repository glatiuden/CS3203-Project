#include "ValidatorHelper.h"
#include <stack>
#include <unordered_set>


bool ValidatorHelper::isExpr(vector<Token> expr) {
	// REFERENCE :
	// expr: expr '+' term | expr '-' term | term


	auto isPlusOrMinus = [](Token t) -> bool { return t.isPlus() || t.isMinus(); };
	auto isNameOrDigit = [](Token t) -> bool { return t.isName() || t.isDigit(); };

	if (expr.size() == 0) return false;
	if (expr.size() == 1 && isNameOrDigit(expr[0])) return true;


	// Find outer most +/- from the right
	int unclosed = 0;
	auto op = expr.rbegin();
	for (; op < expr.rend(); op++) {
		if (op->isRParam()) unclosed += 1;
		if (op->isLParam()) unclosed -= 1;

		if (isPlusOrMinus(*op) && unclosed == 0) break;
	}
	// +/- not found
	if (op == expr.rend()) return isTerm(expr);


	vector<Token> right(expr.rbegin(), op);
	vector<Token> left(op + 1, expr.rend());

	reverse(right.begin(), right.end());
	reverse(left.begin(), left.end());


	return (isExpr(left) && isTerm(right))
		|| isTerm(expr); // This means that the operator is surrounded by Parenthesis
}

bool ValidatorHelper::isTerm(vector<Token> term) {
	// REFERENCE :
	// term: term '*' factor | term '/' factor | term '%' factor | factor

	auto isTermOp = [](Token t) -> bool { return t.isMultiply() || t.isDivide() || t.isModulo(); };

	// Find outer most ( * | / | % ) from the right
	int unclosed = 0;
	auto op = term.rbegin();
	for (; op < term.rend(); op++) {
		if (op->isLParam()) unclosed += 1;
		if (op->isRParam()) unclosed -= 1;

		if (isTermOp(*op) && unclosed == 0) break;
	}
	// ( * | / | % ) not found
	if (op == term.rend()) return isFactor(term);

	vector<Token> right(term.rbegin(), op);
	vector<Token> left(op + 1, term.rend());

	reverse(right.begin(), right.end());
	reverse(left.begin(), left.end());

	return isTerm(left) && isFactor(right);

}

bool ValidatorHelper::isFactor(vector<Token> factor) {
	// REFERENCE :
	// factor : var_name | const_value | '(' expr ')'

	if (factor.size() == 0) return false;

	if (factor.size() == 1 && (factor[0].isName() || factor[0].isDigit())) return true;

	if (factor.size() >= 3 &&
		factor.front().isLParam() && factor.back().isRParam())
		return isExpr(vector<Token>(factor.begin() + 1, factor.end() - 1));

	return false;
}


// Graph Utility Functions for Call Chain
void CallChainValidation::addEdge(Graph& graph, Edge edge) {
	auto& [caller, callee] = edge;
	if (graph.count(caller) == 0) graph.insert({ caller, {} });
	graph[caller].push_back(callee);
}
bool CallChainValidation::hasCycle(Graph& graph) {
	// Hashmap of call stack
	typedef unordered_set<string> CallChain;

	unordered_set<string> visited;
	stack<pair<string, CallChain>> stck;

	for (auto [caller, _] : graph) {
		if (!visited.count(caller)) stck.push({ caller, {} });

		while (!stck.empty()) {
			auto [e, chain] = stck.top();
			chain.insert(e);
			stck.pop();

			// No need do anything for visited nodes.
			if (visited.count(e)) continue;
			if (!graph.count(e)) continue; // No need to check procedure who are solely callees since they dont call.


			for (auto i : graph[e]) {
				// If node was on call chain before, there is a cycle.
				if (chain.count(i)) return true;
				// add edges to stack
				stck.push({ i, chain });
			}

			// Mark as visited
			visited.insert(e);
		}

	}
	return false;
}