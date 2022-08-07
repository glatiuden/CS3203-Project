#include "QPSFilter.h"

void QPSFilter::setToVector(vector<string>& results, unordered_set<string>& setResults) {
	results.reserve(setResults.size());
	for (auto& it = setResults.begin(); it != setResults.end(); ) {
		results.push_back(move(setResults.extract(it++).value()));
	}
}

vector<StmtData> QPSFilter::filterRawResults(const vector<StmtData>& rawResults, const NodeType& filterNodeType) {
	vector<StmtData> filteredResults{};
	if (!isFilterRequired(filterNodeType)) {
		return rawResults;
	}

	for (const StmtData& sd : rawResults) {
		if (sd.getType() == filterNodeType) {
			filteredResults.push_back(sd);
		}
	}
	return filteredResults;
}

vector<StmtData> QPSFilter::filterRawResults(const vector<StmtData>& rawResults, const string& varName) {
	vector<StmtData> filteredResults{};
	for (const StmtData& sd : rawResults) {
		if (sd.getValue() == varName) {
			filteredResults.push_back(sd);
		}
	}
	return filteredResults;
}

vector<string> QPSFilter::getDisplayValues(const vector<StmtData>& stmtDataResults, const NodeType& projectionNodeType) {
	vector<string> displayValues{};
	getDisplayValues(displayValues, stmtDataResults, projectionNodeType);
	return displayValues;
}

void QPSFilter::getDisplayValues(vector<string>& results, const vector<StmtData>& stmtDataResults, const NodeType& projectionNodeType) {
	if (projectionNodeType == NodeType::BOOLEAN) {
		if (stmtDataResults.empty()) {
			results.push_back(BOOLEAN_FALSE);
		} else {
			results.push_back(BOOLEAN_TRUE);
		}
		return;
	}

	unordered_set<string> resultSet{};
	resultSet.reserve(results.size());
	for (const StmtData& sd : stmtDataResults) {
		switch (projectionNodeType) {
			case NodeType::STMT:
			{
				if (sd.getStmtNo() > 0) {
					resultSet.insert(to_string(sd.getStmtNo()));
				}
				break;
			}
			case NodeType::PROCEDURE:
			{
				if (!sd.getProcName().empty()) {
					resultSet.insert(sd.getProcName());
				} else {
					resultSet.insert(sd.getValue());
				}
				break;
			}
			default:
			{
				if (!sd.getValue().empty()) {
					resultSet.insert(sd.getValue());
				}
				break;
			}
		}
	}

	setToVector(results, resultSet);
}

vector<StmtData> QPSFilter::getVectorFromPairVector(const vector<pair<StmtData, StmtData>>& pairResults, const int& pairNo) {
	vector<StmtData> results(pairResults.size());
	int i = 0;
	for (const pair<StmtData, StmtData>& pairSd : pairResults) {
		if (pairNo == FIRST) {
			results[i] = pairSd.first;
		} else {
			results[i] = pairSd.second;
		}
		i++;
	}
	return results;
}

vector<pair<StmtData, StmtData>> QPSFilter::filterRawResults(const vector<pair<StmtData, StmtData>>& rawResults, const int& pairNo, const NodeType& nodeType) {
	if (!isFilterRequired(nodeType)) {
		return rawResults;
	}

	vector<pair<StmtData, StmtData>> results{};
	for (const pair<StmtData, StmtData>& pairSd : rawResults) {
		StmtData sd{};
		if (pairNo == FIRST) {
			sd = pairSd.first;
		} else {
			sd = pairSd.second;
		}

		if (sd.getType() != nodeType) {
			continue;
		}

		results.push_back(pairSd);
	}
	return results;
}

vector<pair<StmtData, StmtData>> QPSFilter::getSameValuePairs(const vector<pair<StmtData, StmtData>>& rawResults) {
	vector<pair<StmtData, StmtData>> results{};
	for (const pair<StmtData, StmtData>& pairSd : rawResults) {
		if (pairSd.first == pairSd.second) {
			results.push_back(pairSd);
		}
	}
	return results;
}

NodeType QPSFilter::getProjectionNodeType(const NodeType& resultArgType) {
	switch (resultArgType) {
		case NodeType::CALL:
		case NodeType::READ:
		case NodeType::PRINT:
		case NodeType::STMT:
		case NodeType::WHILE:
		case NodeType::IF:
		case NodeType::ASSIGN:
			return NodeType::STMT;
		case NodeType::PROCEDURE:
			return NodeType::PROCEDURE;
		case NodeType::BOOLEAN:
			return NodeType::BOOLEAN;
		case NodeType::VARIABLE:
		case NodeType::CONSTANT:
		default:
			return NodeType::VARIABLE;
	}
}

NodeType QPSFilter::getProjectionNodeType(const NodeType& resultArgType, const NodeType& attrName) {
	if (resultArgType == NodeType::READ || resultArgType == NodeType::PRINT) {
		if (attrName == NodeType::VARNAME) {
			return NodeType::VARIABLE;
		}
	} else if (resultArgType == NodeType::CALL) {
		if (attrName == NodeType::PROCNAME) {
			return NodeType::PROCEDURE;
		}
	}
	return getProjectionNodeType(resultArgType);
}
