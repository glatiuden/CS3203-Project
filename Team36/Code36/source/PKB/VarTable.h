#pragma once
#include <unordered_map>

using namespace std;

class Var;

class VarTable {
private:
	unordered_map<string, Var> varMap;

public:
	bool insertVar(Var var);
	Var getVar(string varName);
	int getSize();
};