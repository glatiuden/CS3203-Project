#include "QPSResultTableEntities.h"

unordered_map<string, string> RowValue::rowValueStringCache;

RowValue::RowValue() : _val(nullptr) {}

RowValue::RowValue(string& str) : _val(assignVal(str)) {}

RowValue::RowValue(const RowValue& copy) : _val(assignVal(*copy._val)) {}

RowValue::RowValue(const string& str) : _val(assignVal(str)) {}

string* RowValue::assignVal(const string& str) {
	if (!rowValueStringCache.count(str)) {
		rowValueStringCache[str] = str;
	}
	return &rowValueStringCache[str];
}

RowValue::operator string& () const { return *_val; }

RowValue& RowValue::operator= (const string& str) {
	_val = assignVal(str);
	return *this;
}

RowValue& RowValue::operator= (const RowValue& v) {
	_val = v._val;
	return *this;
}

string RowValue::operator+ (const string& str) {
	return (*_val).append(str);
}

bool RowValue::operator== (const RowValue& rhs) {
	return this->_val == rhs._val;
}

string operator+(string lhs, RowValue rhs) {
	return lhs.append(*rhs._val);
}

bool operator==(pair<RowValue, RowValue> lhs, pair<RowValue, RowValue> rhs) {
	return lhs.first == rhs.first && lhs.second == rhs.second;
}
