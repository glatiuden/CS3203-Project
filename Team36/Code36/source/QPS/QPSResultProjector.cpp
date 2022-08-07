#include "QPSResultProjector.h"

constexpr auto EQUAL_SIGN = "=";
constexpr auto UNINITIALIZED_STMT = "-1";

QPSResultProjector::QPSResultProjector(list<string>& results) : results(results) {}

void QPSResultProjector::projectResult(const vector<string>& resultsFromPKB) {
	for (const string& result : resultsFromPKB) {
		if (result != EQUAL_SIGN && result != UNINITIALIZED_STMT) {
			results.push_back(result);
		}
	}
}
