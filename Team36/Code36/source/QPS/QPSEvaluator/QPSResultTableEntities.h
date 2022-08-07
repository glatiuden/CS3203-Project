#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

/*
* RowValue is a class made to be interchangeable used as a string, however in contrast to a string
* it caches the values and 2 RowValues with the same string would internally point to a single string object.
*/
struct RowValue {
	string* _val;
	static unordered_map<string, string> rowValueStringCache;
	RowValue();
	RowValue(string& str);
	RowValue(const RowValue& copy);
	RowValue(const string& str);
	string* assignVal(const string& str);
	operator string& () const;
	RowValue& operator= (const string& str);
	RowValue& operator= (const RowValue& v);
	string operator+ (const string& str);
	bool operator== (const RowValue& rhs);
};

string operator+(string lhs, RowValue rhs);
bool operator==(pair<RowValue, RowValue>lhs, pair<RowValue, RowValue>rhs);

typedef int VectIndex;
typedef string Column;
typedef int RowIndex;
typedef unordered_map<Column, VectIndex> ResultTableColumns;
typedef vector<Column> ResultTableColumnsVector;
typedef vector<RowValue> ResultTableRowData;
typedef vector<ResultTableRowData> ResultTableRows;
