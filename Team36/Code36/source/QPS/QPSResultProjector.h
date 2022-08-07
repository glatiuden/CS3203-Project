#pragma once
#include <string>
#include <list>
#include <vector>

using namespace std;

class QPSResultProjector {
public:
	QPSResultProjector(list<string>& results);
	void projectResult(const vector<string>& resultsFromPKB);
private:
	list<string>& results;
};
