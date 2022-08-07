#pragma once
#include <string>
#include <vector>
#include <unordered_set>

#include "../PKB/StmtData.h"
#include "QPSTree/ArgNode.h"

using namespace std;

constexpr auto FIRST = 1;
constexpr auto SECOND = 2;

class QPSFilter {
public:
	static void setToVector(vector<string>& results, unordered_set<string>& setResults);
	static vector<StmtData> filterRawResults(const vector<StmtData>& rawResults, const NodeType& filterNodeType);
	static vector<StmtData> filterRawResults(const vector<StmtData>& rawResults, const string& varName);
	static vector<pair<StmtData, StmtData>> QPSFilter::filterRawResults(const vector<pair<StmtData, StmtData>>& rawResults, const int& pairNo, const NodeType& nodeType);
	static vector<string> getDisplayValues(const vector<StmtData>& stmtDataResults, const NodeType& projectionNodeType);
	static void getDisplayValues(vector<string>& results, const vector<StmtData>& stmtDataResults, const NodeType& projectionNodeType);
	static vector<StmtData> getVectorFromPairVector(const vector<pair<StmtData, StmtData>>& pairResults, const int& pairNo);
	static NodeType getProjectionNodeType(const NodeType& resultArgType);
	static NodeType getProjectionNodeType(const NodeType& resultArgType, const NodeType& attrName);
	static vector<pair<StmtData, StmtData>> getSameValuePairs(const vector<pair<StmtData, StmtData>>& rawResults);
private:
	static inline bool isFilterRequired(const NodeType& filterNodeType) {
		return filterNodeType != NodeType::EMPTY && filterNodeType != NodeType::WILDCARD
			&& filterNodeType != NodeType::STMT && filterNodeType != NodeType::VARIABLE
			&& filterNodeType != NodeType::INTEGER;
	}
};
