#include <algorithm>
#include <iostream>
#include <thread>

#include "QPSResultTable.h"
#include "../QPSFilter.h"
#include "../QPSCommunicator.h"

constexpr auto SPACE_STR = " ";
constexpr auto APPEND_SPACE = true;
constexpr auto SIZE_ZERO = 0;

namespace {
	void append(string& currStr, const string& str, const bool& shouldAppendSpace) {
		size_t len = currStr.size() + str.size();
		if (shouldAppendSpace) {
			currStr.reserve(len + sizeof(SPACE_STR));
			currStr.append(SPACE_STR).append(str);
		} else {
			currStr.reserve(len);
			currStr.append(str);
		}
	}
}

QPSResultTable::QPSResultTable() : columns(ResultTableColumns()), rows(ResultTableRows()), columnsVector(vector<Column>()) {}

QPSResultTable::QPSResultTable(ArgNode argNodes[2], const vector<pair<StmtData, StmtData>>& rowsData) {
	ArgNode& leftColumnNode = argNodes[0];
	ArgNode& rightColumnNode = argNodes[1];
	const ArgNodeType& leftType = leftColumnNode.getType();
	const ArgNodeType& rightType = rightColumnNode.getType();
	const ArgAttrName& leftAttrName = leftColumnNode.getAttrName();
	const ArgAttrName& rightAttrName = rightColumnNode.getAttrName();

	int rowSize = static_cast<int>(rowsData.size());
	pair<VectIndex, VectIndex> columnsVectorIndexes = populateColumns(argNodes, rowSize);
	const VectIndex& leftColVectIndex = columnsVectorIndexes.first;
	const VectIndex& rightColVectIndex = columnsVectorIndexes.second;
	if (leftColVectIndex == INVALID_VECTINDEX && rightColVectIndex == INVALID_VECTINDEX) {
		return;
	}

	unordered_set<pair<RowValue, RowValue>, QPSResultTableHash> existMap{};
	for (const pair<StmtData, StmtData>& pairData : rowsData) {
		const StmtData& leftData = pairData.first;
		const StmtData& rightData = pairData.second;
		const string& leftValue = getStringValue(leftData, leftType, leftAttrName);
		const string& rightValue = getStringValue(rightData, rightType, rightAttrName);
		populateRows(existMap, columnsVectorIndexes, make_pair(leftValue, rightValue));
	}
}

QPSResultTable::QPSResultTable(ArgNode argNodes[2], const vector<StmtData>& rowsData) {
	ArgNode& leftColumnNode = argNodes[0];
	ArgNode& rightColumnNode = argNodes[1];
	const ArgNodeType& leftType = leftColumnNode.getType();
	const ArgNodeType& rightType = rightColumnNode.getType();
	const ArgAttrName& leftAttrName = leftColumnNode.getAttrName();
	const ArgAttrName& rightAttrName = rightColumnNode.getAttrName();

	int rowSize = static_cast<int>(rowsData.size());
	const pair<VectIndex, VectIndex>& columnsVectorIndexes = populateColumns(argNodes, rowSize);
	const VectIndex& leftColVectIndex = columnsVectorIndexes.first;
	const VectIndex& rightColVectIndex = columnsVectorIndexes.second;
	if (leftColVectIndex == INVALID_VECTINDEX && rightColVectIndex == INVALID_VECTINDEX) {
		return;
	}

	unordered_set<pair<RowValue, RowValue>, QPSResultTableHash> existMap{};
	for (const StmtData& stmtData : rowsData) {
		const string& leftValue = getStringValue(stmtData, leftType, leftAttrName);
		const string& rightValue = getStringValue(stmtData, rightType, rightAttrName);
		populateRows(existMap, columnsVectorIndexes, make_pair(leftValue, rightValue));
	}
}

QPSResultTable::QPSResultTable(const ArgNode& argNode, const vector<StmtData>& rowsData) {
	const Column& colName = argNode.getRef();
	const ArgNodeType& colType = argNode.getType();
	const ArgAttrName& colAttrName = argNode.getAttrName();
	const VectIndex& vectIndex = insertColumn(colName, rowsData.size());
	unordered_set<string> existMap{};
	for (const StmtData& stmtData : rowsData) {
		const string& value = getStringValue(stmtData, colType, colAttrName);
		if (!existMap.count(value)) {
			rows[vectIndex][noOfRows] = value;
			existMap.insert(value);
			noOfRows++;
		}
	}
}

pair<VectIndex, VectIndex> QPSResultTable::populateColumns(ArgNode argNodes[2], const int& rowSize) {
	ArgNode& leftColumnNode = argNodes[0];
	ArgNode& rightColumnNode = argNodes[1];
	const ArgNodeRef& leftCol = leftColumnNode.getRef();
	const ArgNodeRef& rightCol = rightColumnNode.getRef();
	const ArgNodeType& leftType = leftColumnNode.getType();
	const ArgNodeType& rightType = rightColumnNode.getType();

	VectIndex leftColVectIndex = -1;
	VectIndex rightColVectIndex = -1;
	if (!isSynonym(leftType) && !isSynonym(rightType)) {
		return make_pair(leftColVectIndex, rightColVectIndex);
	}

	if (leftColumnNode == rightColumnNode) {
		leftColVectIndex = insertColumn(leftCol, rowSize);
	} else {
		if (isSynonym(leftType)) {
			leftColVectIndex = insertColumn(leftCol, rowSize);
		}
		if (isSynonym(rightType)) {
			rightColVectIndex = insertColumn(rightCol, rowSize);
		}
	}

	return make_pair(leftColVectIndex, rightColVectIndex);
}

void QPSResultTable::populateRows(unordered_set<pair<RowValue, RowValue>, QPSResultTableHash>& existMap, const pair<VectIndex, VectIndex>& vectorIndexes, const pair<RowValue, RowValue>& rowValues) {
	if (existMap.count(rowValues)) {
		return;
	}

	const VectIndex& leftColVectIndex = vectorIndexes.first;
	const VectIndex& rightColVectIndex = vectorIndexes.second;
	const bool populateLeftColumn = leftColVectIndex != INVALID_VECTINDEX;
	const bool populateRightColumn = rightColVectIndex != INVALID_VECTINDEX;

	if (populateLeftColumn) {
		rows[leftColVectIndex][noOfRows] = rowValues.first;
	}

	if (populateRightColumn) {
		rows[rightColVectIndex][noOfRows] = rowValues.second;
	}

	if (populateLeftColumn || populateRightColumn) {
		noOfRows++;
	}

	existMap.insert(rowValues);
}

string QPSResultTable::getStringValue(const StmtData& stmtData, const NodeType& nodeType, const NodeType& attrName) const {
	string value = to_string(stmtData.getStmtNo()); // Default to STMT
	NodeType projectionNodeType = QPSFilter::getProjectionNodeType(nodeType);
	if (projectionNodeType == NodeType::VARIABLE || projectionNodeType == NodeType::VARNAME || projectionNodeType == NodeType::VALUE) {
		value = stmtData.getValue();
	} else if (projectionNodeType == NodeType::PROCEDURE || projectionNodeType == NodeType::PROCNAME) {
		value = stmtData.getProcName();
	}

	return value;
}

shared_ptr<QPSResultTable> QPSResultTable::processIntermediateTable(const shared_ptr<QPSResultTable>& anotherTable) {
	unordered_set<Column> commonColumns{};
	sort(columnsVector.begin(), columnsVector.end());
	sort(anotherTable->columnsVector.begin(), anotherTable->columnsVector.end());
	set_intersection(columnsVector.begin(), columnsVector.end(), anotherTable->columnsVector.begin(), anotherTable->columnsVector.end(), inserter(commonColumns, commonColumns.begin()));
	bool isAllColumns = commonColumns.size() == columnSize() && commonColumns.size() == anotherTable->columnSize();
	bool hasCommonColumns = !isAllColumns && commonColumns.size() > SIZE_ZERO;
	bool hasNoCommonColumns = commonColumns.empty();

	if (isAllColumns) {
		return intersectAllColumns(anotherTable);
	} else if (hasCommonColumns) {
		return intersectCommonColumns(anotherTable, commonColumns);
	} else {
		return cartesianJoinTable(anotherTable);
	}
}

shared_ptr<QPSResultTable> QPSResultTable::intersectAllColumns(const shared_ptr<QPSResultTable>& anotherTable) {
	QPSResultTable* resultTable = new QPSResultTable();
	auto hashJoin = [&resultTable, &anotherTable, this]() {
		QPSResultTable* outerTable; // Used for Build: Smaller Table that fit in memory
		QPSResultTable* innerTable; // Used for Probe: Bigger Table that used for probe
		if (noOfRows >= anotherTable->noOfRows) {
			outerTable = anotherTable.get();
			innerTable = this;
		} else {
			outerTable = this;
			innerTable = anotherTable.get();
		}

		resultTable->rows.reserve(columnSize());
		RowsPointers rowsToInsert(columnSize());
		constructRowsPointers(rowsToInsert, resultTable, innerTable, SIZE_ZERO);

		unordered_set<string> tableMap{};
		buildTableMap(tableMap, outerTable);

		ResultTableRows& innerTableRows = innerTable->rows;
		ResultTableColumns& innerTableColumns = innerTable->columns;
		for (int rowIndex = 0; rowIndex < innerTable->noOfRows; rowIndex++) {
			string values{};
			for (const Column& column : outerTable->columnsVector) {
				append(values, innerTableRows[innerTableColumns[column]][rowIndex], APPEND_SPACE);
			}
			if (tableMap.count(values)) {
				for (const pair<ResultTableRowData*, ResultTableRowData*>& innerPair : rowsToInsert) {
					const auto& [row, innerRow] = innerPair;
					row->push_back(innerRow->operator[](rowIndex));
				}
				resultTable->noOfRows++;
			}
		}
	};
	thread th1(hashJoin);
	th1.join();
	return shared_ptr<QPSResultTable>(resultTable);
}

shared_ptr<QPSResultTable> QPSResultTable::intersectCommonColumns(const shared_ptr<QPSResultTable>& anotherTable, const unordered_set<Column>& commonColumns) {
	QPSResultTable* resultTable = new QPSResultTable();
	auto hashJoin = [&resultTable, &anotherTable, &commonColumns, this]() {
		ResultTableColumnsVector commonColumnsVector(commonColumns.begin(), commonColumns.end());
		QPSResultTable* outerTable; // Used for Build: Smaller Table that fit in memory
		QPSResultTable* innerTable; // Used for Probe: Bigger Table that used for probe
		if (noOfRows >= anotherTable->noOfRows) {
			outerTable = anotherTable.get();
			innerTable = this;
		} else {
			outerTable = this;
			innerTable = anotherTable.get();
		}
		const int& commonColumnsSize = commonColumns.size();
		int newColumnsSize = (outerTable->columnSize() - commonColumnsSize) + innerTable->columnSize();
		resultTable->rows.reserve(newColumnsSize);

		unordered_map<string, vector<RowIndex>> outerTableMap{};
		buildTableMap(outerTableMap, outerTable, commonColumnsVector);

		RowIndexQueue pairIndexes{};
		buildPairRowIndexes(pairIndexes, innerTable, outerTableMap, commonColumnsVector);

		int newRowSize = static_cast<int>(pairIndexes.size());
		RowsPointers outerRowsToInsert(outerTable->columnSize());
		constructRowsPointers(outerRowsToInsert, resultTable, outerTable, newRowSize);
		RowsPointers innerRowsToInsert(innerTable->columnSize() - commonColumns.size());
		constructRowsPointers(innerRowsToInsert, resultTable, innerTable, newRowSize, commonColumns);

		int newRowIndex = 0;
		while (!pairIndexes.empty()) {
			const auto& [outerTableRowIndex, innerTableRowIndex] = pairIndexes.front();
			for (const RowsPointer& outerPair : outerRowsToInsert) {
				const auto& [row, outerRow] = outerPair;
				row->operator[](newRowIndex) = outerRow->operator[](outerTableRowIndex);
			}
			for (const RowsPointer& innerPair : innerRowsToInsert) {
				const auto& [row, innerRow] = innerPair;
				row->operator[](newRowIndex) = innerRow->operator[](innerTableRowIndex);
			}
			pairIndexes.pop_front();
			newRowIndex++;
		}
		resultTable->noOfRows = newRowIndex;
	};
	thread th1(hashJoin);
	th1.join();
	return shared_ptr<QPSResultTable>(resultTable);
}

shared_ptr<QPSResultTable> QPSResultTable::cartesianJoinTable(const shared_ptr<QPSResultTable>& anotherTable) {
	QPSResultTable* resultTable = new QPSResultTable();
	auto cartesianJoin = [&resultTable, &anotherTable, this]() {
		QPSResultTable* outerTable; // Bigger Table
		QPSResultTable* innerTable; // Smaller Table (Nested)
		if (noOfRows <= anotherTable->noOfRows) {
			outerTable = this;
			innerTable = anotherTable.get();
		} else {
			outerTable = anotherTable.get();
			innerTable = this;
		}

		int newRowSize = outerTable->noOfRows * innerTable->noOfRows;
		int newColumnsSize = outerTable->noOfColumns + innerTable->noOfColumns;
		resultTable->rows.reserve(newColumnsSize);

		RowsPointers outerRowsToInsert(outerTable->columnSize());
		constructRowsPointers(outerRowsToInsert, resultTable, outerTable, newRowSize);
		RowsPointers innerRowsToInsert(innerTable->columnSize());
		constructRowsPointers(innerRowsToInsert, resultTable, innerTable, newRowSize);

		int newRowIndex = 0;
		for (int outerRowIndex = 0; outerRowIndex < outerTable->noOfRows; outerRowIndex++) {
			for (int innerRowIndex = 0; innerRowIndex < innerTable->noOfRows; innerRowIndex++) {
				for (const RowsPointer& outerPair : outerRowsToInsert) {
					const auto& [row, outerRow] = outerPair;
					row->operator[](newRowIndex) = outerRow->operator[](outerRowIndex);
				}
				for (const RowsPointer& innerPair : innerRowsToInsert) {
					const auto& [row, innerRow] = innerPair;
					row->operator[](newRowIndex) = innerRow->operator[](innerRowIndex);
				}
				newRowIndex++;
			}
		}
		resultTable->noOfRows = newRowIndex;
	};
	thread th1(cartesianJoin);
	th1.join();
	return shared_ptr<QPSResultTable>(resultTable);
}

void QPSResultTable::buildTableMap(unordered_set<string>& tableMap, QPSResultTable* outerTable) {
	ResultTableRows& outerTableRows = outerTable->rows;
	ResultTableColumns& outerTableColumns = outerTable->columns;
	ResultTableColumnsVector& outerTableColumnsVector = outerTable->columnsVector;
	for (int rowIndex = 0; rowIndex < outerTable->noOfRows; rowIndex++) {
		string values{};
		for (const Column& column : outerTableColumnsVector) {
			append(values, outerTableRows[outerTableColumns[column]][rowIndex], APPEND_SPACE);
		}
		tableMap.insert(values);
	}
}

void QPSResultTable::buildTableMap(RowIndexTableMap& tableMap, QPSResultTable* outerTable, const ResultTableColumnsVector& commonColumnsVector) {
	ResultTableRows& outerTableRows = outerTable->rows;
	ResultTableColumns& outerTableColumns = outerTable->columns;
	for (int rowIndex = 0; rowIndex < outerTable->noOfRows; rowIndex++) {
		string values{};
		for (const Column& column : commonColumnsVector) {
			append(values, outerTableRows[outerTableColumns[column]][rowIndex], APPEND_SPACE);
		}
		tableMap[values].push_back(rowIndex);
	}
}

void QPSResultTable::populateSelectionTuple(SelectionTuples& selectionTuples, const QPSNode& selectionNode) {
	for (int i = 0; i < selectionNode.getSize(); i++) {
		QPSNode& childNode = selectionNode.getChild(i);
		ArgNode& selectionArg = childNode.getLeftArgReference();
		const NodeType& selectionArgType = selectionArg.getType();
		const NodeType& selectionAttrName = selectionArg.getAttrName();
		const ArgNodeRef& selectionArgRef = selectionArg.getStrRef();
		const VectIndex& vectIndex = columns[selectionArgRef];
		selectionTuples[i] = move(make_tuple(selectionArgType, selectionAttrName, vectIndex));
	}
}

void QPSResultTable::buildPairRowIndexes(RowIndexQueue& pairIndexes, QPSResultTable* innerTable, RowIndexTableMap& tableMap, const vector<Column>& commonColumns) {
	ResultTableRows& innerTableRows = innerTable->rows;
	ResultTableColumns& innerTableColumns = innerTable->columns;
	for (int rowIndex = 0; rowIndex < innerTable->noOfRows; rowIndex++) {
		string values{};
		for (const Column& column : commonColumns) {
			append(values, innerTableRows[innerTableColumns[column]][rowIndex], APPEND_SPACE);
		}
		if (tableMap.count(values)) {
			const vector<RowIndex>& rowIndexes = tableMap[values];
			for (const int& index : rowIndexes) {
				pairIndexes.push_back(make_pair(index, rowIndex));
			}
		}
	}
}

void QPSResultTable::constructRowsPointers(RowsPointers& rowsPointer, QPSResultTable* resultTable, QPSResultTable* insertTable, const int& rowSize) {
	const vector<Column>& insertTableColumnsVector = insertTable->columnsVector;
	for (size_t i = 0; i < insertTableColumnsVector.size(); i++) {
		const string& column = insertTableColumnsVector[i];
		VectIndex newVectIndex = INVALID_VECTINDEX;
		if (rowSize > 0) {
			newVectIndex = resultTable->insertColumn(column, rowSize);
		} else {
			newVectIndex = resultTable->insertColumn(column);
		}
		rowsPointer[i] = make_pair(&resultTable->rows.operator[](newVectIndex), &insertTable->rows[insertTable->columns[column]]);
	}
}

void QPSResultTable::constructRowsPointers(RowsPointers& rowsPointer, QPSResultTable* resultTable, QPSResultTable* insertTable, const int& rowSize, const unordered_set<Column>& commonColumns) {
	const vector<Column>& insertTableColumnsVector = insertTable->columnsVector;
	int i = 0;
	for (const string& column : insertTableColumnsVector) {
		if (!commonColumns.count(column)) {
			VectIndex newVectIndex = INVALID_VECTINDEX;
			if (rowSize > SIZE_ZERO) {
				newVectIndex = resultTable->insertColumn(column, rowSize);
			} else {
				newVectIndex = resultTable->insertColumn(column);
			}
			rowsPointer[i] = make_pair(&resultTable->rows.operator[](newVectIndex), &insertTable->rows[insertTable->columns[column]]);
			i++;
		}
	}
}

void QPSResultTable::print() {
	int i = 0;
	for (auto const& [columnName, vectIndex] : columns) {
		cout << "Column " << i << ": " << columnName << endl;
		ResultTableRowData& data = rows[vectIndex];
		for (string& val : data) {
			cout << val << endl;
		}
	}
}

VectIndex QPSResultTable::insertColumn(const Column& column) {
	return insertColumn(column, SIZE_ZERO);
}

VectIndex QPSResultTable::insertColumn(const Column& column, const int& rowSize) {
	if (!columns.count(column)) {
		columns.emplace(column, columns.size());
		columnsVector.push_back(column);
		if (rowSize > SIZE_ZERO) {
			rows.push_back(ResultTableRowData(rowSize));
		} else {
			rows.push_back({});
		}
		noOfColumns++;
		return columns[column];
	}
	return INVALID_VECTINDEX;
}

void QPSResultTable::getRowString(string& rowString, const RowIndex& index, const SelectionTuples& selectionTuples) {
	for (const SelectionTuple& tuple : selectionTuples) {
		const auto& [selectionArgType, selectionAttrName, vectIndex] = tuple;
		const string& value = rows[vectIndex][index];
		bool shouldAppendSpace = !rowString.empty();
		if (isCallReadPrintSynonym(selectionArgType) && isVarNameOrProcName(selectionAttrName)) {
			append(rowString, callReadPrintMap[selectionArgType][value], shouldAppendSpace);
		} else {
			append(rowString, value, shouldAppendSpace);
		}
	}
}

void QPSResultTable::getResultsByColumns(const QPSNode& selectionRootNode, vector<string>& results) {
	callReadPrintMap = QPSCommunicator::getCallReadPrintMap();
	unordered_set<string> tempResultSet{};
	tempResultSet.reserve(noOfRows);

	SelectionTuples selectionTuples(selectionRootNode.getSize());
	populateSelectionTuple(selectionTuples, selectionRootNode);
	for (int i = 0; i < noOfRows; i++) {
		string rowString{};
		getRowString(rowString, i, selectionTuples);
		if (!tempResultSet.count(rowString)) {
			tempResultSet.insert(rowString);
		}
	}

	QPSFilter::setToVector(results, tempResultSet);
}

bool QPSResultTable::checkIfColumnExists(const Column& column) const {
	return columns.count(column);
}

bool QPSResultTable::isEmpty() const {
	return noOfRows == 0;
}

int QPSResultTable::size() const {
	return noOfRows;
}

int QPSResultTable::columnSize() const {
	return noOfColumns;
}

vector<Column> QPSResultTable::getColumns() const {
	return columnsVector;
}

void QPSResultTable::dropColumns(const ResultTableColumnsVector& columnNames) {
	for (const string& column : columnNames) {
		const int& vectIndex = columns[column];
		columnsVector.erase(remove(columnsVector.begin(), columnsVector.end(), column), columnsVector.end());
		rows[vectIndex] = {}; // Empty out the value
		columns.erase(column);
		noOfColumns--;
	}

	if (noOfColumns == 1) {
		removeDuplicateDataSingleColumn();
	} else {
		removeDuplicateDataMultipleColumns();
	}
}

void QPSResultTable::removeDuplicateDataMultipleColumns() {
	unordered_set<string> existMap{};
	deque<RowIndex> rowsToCopy{};
	for (int rowIndex = 0; rowIndex < noOfRows; rowIndex++) {
		string values{};
		for (const string& column : columnsVector) {
			append(values, rows[columns[column]][rowIndex], APPEND_SPACE);
		}
		if (!existMap.count(values)) {
			rowsToCopy.push_back(rowIndex);
			existMap.insert(values);
		}
	}

	noOfRows = rowsToCopy.size();
	unordered_map<Column, vector<RowValue>> populateMap{};
	for (const string& column : columnsVector) {
		populateMap[column] = vector<RowValue>(rowsToCopy.size());
	}

	int i = 0;
	while (!rowsToCopy.empty()) {
		const int& rowIndex = rowsToCopy.front();
		for (const string& column : columnsVector) {
			populateMap[column][i] = rows[columns[column]][rowIndex];
		}
		i++;
		rowsToCopy.pop_front();
	}

	for (const string& column : columnsVector) {
		rows[columns[column]] = populateMap[column];
	}
}

void QPSResultTable::removeDuplicateDataSingleColumn() {
	unordered_set<string> existMap{};
	const int& vectIndex = columns[columnsVector.front()];
	for (const string& value : rows[vectIndex]) {
		existMap.insert(value);
	}
	rows[vectIndex].assign(existMap.begin(), existMap.end());
	noOfRows = existMap.size();
}
