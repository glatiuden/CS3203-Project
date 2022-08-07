#pragma once
#include <vector>
#include <string>

#include "../../QPSCommunicator.h"
#include "../../QPSTree/QPSNode.h"
#include "../../QPSHelper.h"
#include "../../QPSFilter.h"

#define EMPTY_RESULT {}

using namespace std;

typedef vector<pair<StmtData, StmtData>> QPSResults;
typedef vector<StmtData> QPSSingleResults;

class QPSEvaluatorProcessor {
public:
	QPSEvaluatorProcessor();
	bool getIsGetAll() const;
	bool getIsInvalid() const;
protected:
	void setIsGetAll(bool);
	void setIsInvalid(bool);
	bool isGetAll = false;
	bool isInvalid = false;
};
