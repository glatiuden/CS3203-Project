#pragma once
#include <string>
#include <list>

#include "QPSTree/QPSTree.h"
#include "QPSEvaluator.h"

using namespace std;

class QPS {
public:
	static void processQuery(const string& query, list<string>& results);
private:
	static unique_ptr<QPSEvaluator> getEvaluator(QPSTree& qpsTree);
};
