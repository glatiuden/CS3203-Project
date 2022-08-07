#include "QPSEvaluatorProcessor.h"

QPSEvaluatorProcessor::QPSEvaluatorProcessor() {};

void QPSEvaluatorProcessor::setIsGetAll(bool value) {
	isGetAll = value;
}

void QPSEvaluatorProcessor::setIsInvalid(bool value) {
	isInvalid = value;
}

bool QPSEvaluatorProcessor::getIsGetAll() const {
	return isGetAll;
}

bool QPSEvaluatorProcessor::getIsInvalid() const {
	return isInvalid;
}
