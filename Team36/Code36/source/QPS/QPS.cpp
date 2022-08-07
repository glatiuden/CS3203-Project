#include <iostream>
#include <string>
#include <list>

#include "QPS.h"
#include "QPSPreprocessor.h"
#include "QPSClauseParser.h"
#include "QPSEvaluator.h"
#include "QPSEvaluator/Evaluators/QPSSingleClauseEvaluator.h"
#include "QPSEvaluator/Evaluators/QPSMultiClauseEvaluator.h"
#include "QPSResultProjector.h"
#include "QPSTree/QPSTree.h"
#include "../CustomException.h"

using namespace std;

void QPS::processQuery(const string& query, list<string>& results) {
	try {
		QPSTree qpsTree{};
		QPSClauseParser qpsClauseParser{};

		// STEP 1: Validate Query Semantically
		QPSPreprocessor qpsPreprocessor(qpsClauseParser);
		QPSPreprocessorResult result = qpsPreprocessor.validateQuery(query);
		if (!result.isValid) {
			throw QPSValidatorException(result.message);
		}

		// STEP 2: Validate Query Semantically & Parse into QPSTree
		qpsClauseParser.validateSemanticsAndParseTree(qpsTree);

		// STEP 3: Evaluate Query
		unique_ptr<QPSEvaluator> qpsEvaluator = getEvaluator(qpsTree);
		vector<string>& displayResults = qpsEvaluator->evaluateQuery();

		// STEP 4: Project Results
		QPSResultProjector qpsResultProjector = QPSResultProjector(results);
		qpsResultProjector.projectResult(displayResults);
	} catch (QPSValidatorException& ve) {
		cerr << ve.what() << "\n";
	} catch (QPSClauseParserException& pe) {
		if (pe.isBoolean) {
			results.push_back(BOOLEAN_FALSE);
		}
	}
}

unique_ptr<QPSEvaluator> QPS::getEvaluator(QPSTree& qpsTree) {
	if (qpsTree.getIsSingleClause()) {
		return unique_ptr<QPSEvaluator>(new QPSSingleClauseEvaluator(qpsTree));
	} else {
		return unique_ptr<QPSEvaluator>(new QPSMultiClauseEvaluator(qpsTree));
	}
}
