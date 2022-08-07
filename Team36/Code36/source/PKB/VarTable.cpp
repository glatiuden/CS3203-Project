using namespace std;

#include "Var.h"
#include "VarTable.h"

bool VarTable::insertVar(Var var) {
	// Check for presence
	if (this->varMap.find(var.name) == this->varMap.end()) {
		return false;
	}

	this->varMap[var.name] = var;
	return true;
}

Var VarTable::getVar(string varName) {
	return this->varMap[varName];
}

int VarTable::getSize() {
	return this->varMap.size();
}