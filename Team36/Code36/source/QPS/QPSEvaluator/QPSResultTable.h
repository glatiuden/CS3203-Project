#pragma once
#include <queue>

#include "../../PKB/StmtData.h"
#include "../QPSTree/QPSNode.h"
#include "QPSResultTableEntities.h"

using namespace std;

constexpr auto INVALID_VECTINDEX = -1;

typedef pair<ResultTableRowData*, ResultTableRowData*> RowsPointer;
typedef vector<RowsPointer> RowsPointers;
typedef unordered_set<string> ValueTableMap;
typedef unordered_map<string, vector<RowIndex>> RowIndexTableMap;
typedef deque<pair<RowIndex, RowIndex>> RowIndexQueue;
typedef tuple<ArgNodeType, ArgAttrName, VectIndex> SelectionTuple;
typedef vector<SelectionTuple> SelectionTuples;

// Hash Function used for unordered_map for better time complexity (Access time O(1) instead of O(logn))
// The hash function calculation is adapted from the Boost library
struct QPSResultTableHash {
	template <class T1, class T2>
	size_t operator()(const pair<T1, T2>& p) const {
		size_t seed = 0;
		seed ^= hash<string>()(p.first) + hash<string>()(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

class QPSResultTable {
public:
	QPSResultTable();
	QPSResultTable(ArgNode argNodes[2], const vector<pair<StmtData, StmtData>>& rowsData); // Remaining Such That + WITH
	QPSResultTable(ArgNode argNodes[2], const vector<StmtData>& rowsData); // Pattern, Modifies, Use
	QPSResultTable(const ArgNode& argNode, const vector<StmtData>& rowsData); // Selection
	shared_ptr<QPSResultTable> processIntermediateTable(const shared_ptr<QPSResultTable>& anotherTable);
	bool checkIfColumnExists(const Column& column) const;
	void getResultsByColumns(const QPSNode& selectionRootNode, ResultTableColumnsVector& results);
	void print();
	bool isEmpty() const;
	int size() const;
	int columnSize() const;
	vector<Column> getColumns() const;
	void dropColumns(const ResultTableColumnsVector& columnNames);
	void removeDuplicateDataMultipleColumns();
	void removeDuplicateDataSingleColumn();
	bool operator < (const QPSResultTable& anotherTable) const {
		return noOfRows < anotherTable.noOfRows;
	}
private:
	ResultTableColumns columns{};
	ResultTableColumnsVector columnsVector{}; // Columns but stored in vector to retain the order
	ResultTableRows rows{};
	unordered_map<NodeType, unordered_map<string, string>> callReadPrintMap{};
	int noOfColumns = 0;
	int noOfRows = 0;

	shared_ptr<QPSResultTable> intersectAllColumns(const shared_ptr<QPSResultTable>& anotherTable);
	shared_ptr<QPSResultTable> intersectCommonColumns(const shared_ptr<QPSResultTable>& anotherTable, const unordered_set<Column>& commonColumns);
	shared_ptr<QPSResultTable> cartesianJoinTable(const shared_ptr<QPSResultTable>& anotherTable);
	pair<VectIndex, VectIndex> populateColumns(ArgNode argNodes[2], const int& rowSize);
	VectIndex insertColumn(const Column& column);
	VectIndex insertColumn(const Column& column, const int& rowSize);
	string getStringValue(const StmtData& stmtData, const NodeType& nodeType, const NodeType& attrName) const;
	void populateRows(unordered_set<pair<RowValue, RowValue>, QPSResultTableHash>& existMap, const pair<VectIndex, VectIndex>& vectorIndexes, const pair<RowValue, RowValue>& rowValues);
	void getRowString(string& rowString, const RowIndex& index, const vector<tuple<NodeType, NodeType, int>>& selectionTuples);
	void constructRowsPointers(RowsPointers& rowsPointers, QPSResultTable* resultTable, QPSResultTable* insertTable, const int& rowSize);
	void constructRowsPointers(RowsPointers& rowsPointers, QPSResultTable* resultTable, QPSResultTable* insertTable, const int& rowSize, const unordered_set<Column>& commonColumns);
	void buildTableMap(ValueTableMap& tableMap, QPSResultTable* outerTable);
	void buildTableMap(RowIndexTableMap& tableMap, QPSResultTable* outerTable, const ResultTableColumnsVector& commonColumnsVector);
	void buildPairRowIndexes(RowIndexQueue& pairIndexes, QPSResultTable* innerTable, RowIndexTableMap& tableMap, const ResultTableColumnsVector& commonColumns);
	void populateSelectionTuple(SelectionTuples& selectionTuples, const QPSNode& selectionNode);
};
