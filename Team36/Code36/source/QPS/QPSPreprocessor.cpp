#include "QPSPreprocessor.h"
#include "../SP/SourceValidator.h"
#include "../CustomException.h"

QPSPreprocessor::QPSPreprocessor() : qpsClauseParser(*(new QPSClauseParser())) {}

QPSPreprocessor::QPSPreprocessor(QPSClauseParser& qpsClauseParser) : qpsClauseParser(qpsClauseParser) {}

QPSPreprocessorResult QPSPreprocessor::validateQuery(const string& queryStr) {
	QPSPreprocessorResult result = { true, "Valid" };
	const char* queryArr = queryStr.c_str();

	Lexer lex(queryArr);
	Token token;

	while (lex.hasNext()) {
		token = lex.next();
		try {
			if (token.isName()) {
				if (isInVector(token, DESIGN_ENITITIES)) {
					validateDeclaration(token, lex);
				} else {
					if (token.getValue() == "Select") {
						validateSelect(lex);
						goto v1;
					} else {
						throw QPSValidatorException("Invalid declaration or Select keyword!");
					}
				}
			} else {
				throw QPSValidatorException("Invalid PQL!");
			}
		} catch (const exception& ex) {
			result.isValid = false;
			result.message = ex.what();
			goto v1;
		}
	}
v1:	return result;
}

bool QPSPreprocessor::isInVector(const Token token, const vector<string> v) {
	return find(v.begin(), v.end(), token.getValue()) != v.end();
}

bool QPSPreprocessor::isInVector(const NodeType nodeType, const vector<NodeType> v) {
	return find(v.begin(), v.end(), nodeType) != v.end();
}

bool QPSPreprocessor::isStmtRef(Token token, Direction direction) {
	bool isStmtRefResult = false;
	string tokenValue = token.getValue();
	NodeType tokenType = NodeType::EMPTY;

	if (token.isName()) {
		tokenType = getArgNodeTypeFromMap(token.getValue());
		if (isInVector(tokenType, ALL_SYNONYMS)) {
			isStmtRefResult = true;
		}
	} else if (token.isUnderscore()) {
		tokenType = NodeType::WILDCARD;
		isStmtRefResult = true;
	} else if (token.isDigit()) {
		tokenType = NodeType::INTEGER;
		isStmtRefResult = true;
	}

	if (isStmtRefResult && !isEmpty(tokenType)) {
		qpsClauseParser.addSuchThatNodeArgument(iterationCount, direction, make_pair(tokenType, tokenValue));
	}

	return isStmtRefResult;
}

bool QPSPreprocessor::isEntRef(Token token, Lexer& lex, const Direction direction, bool isPattern) {
	bool isEntRefResult = false;
	string tokenValue = token.getValue();
	NodeType tokenType = NodeType::EMPTY;

	if (token.isName()) {
		tokenType = getArgNodeTypeFromMap(token.getValue());
		if (isInVector(tokenType, ALL_SYNONYMS)) {
			isEntRefResult = true;
		}
	} else if (token.isUnderscore()) {
		tokenType = NodeType::WILDCARD;
		isEntRefResult = true;
	} else if (token.isDoubleQuote()) {
		Token currToken = lex.getNext();
		if (currToken.isName()) {
			if (lex.getNext().isDoubleQuote()) {
				tokenType = NodeType::IDENT;
				tokenValue = currToken.getValue();
				isEntRefResult = true;
			}
		}
	}

	if (isEntRefResult && !isEmpty(tokenType)) {
		pair<NodeType, ArgNodeRef> valuePair = make_pair(tokenType, tokenValue);
		if (isPattern) {
			qpsClauseParser.addPatternNodeArgument(iterationCount, direction, valuePair);
		} else {
			qpsClauseParser.addSuchThatNodeArgument(iterationCount, direction, valuePair);
		}
	}

	return isEntRefResult;
}

bool QPSPreprocessor::isEntRef(Lexer& lex, const Direction direction, bool isPattern) {
	Token token = lex.getNext();
	return isEntRef(token, lex, direction, isPattern);
}

/**
* Validate the following PQL grammar rules:
*	ref : '"' IDENT '"' | INTEGER | attrRef
*	attrRef : synonym '.' attrName
*/
bool QPSPreprocessor::isRef(Lexer& lex, const Direction direction) {
	bool isRefResult = false;
	Token token = lex.getNext();
	string tokenValue = token.getValue();
	NodeType tokenType = NodeType::EMPTY;
	NodeType attrType = NodeType::EMPTY;

	if (token.isDoubleQuote()) {
		Token currToken = lex.getNext();
		if (currToken.isName()) {
			if (lex.getNext().isDoubleQuote()) {
				tokenType = NodeType::IDENT;
				tokenValue = currToken.getValue();
				isRefResult = true;
			}
		}
	} else if (token.isDigit()) {
		tokenType = NodeType::INTEGER;
		isRefResult = true;
	} else if (token.isName() && lex.getNext().isPeriod()) {
		tokenType = getArgNodeTypeFromMap(token.getValue());
		isRefResult = isAttrName(lex, attrType);
	} else {
		isRefResult = false;
	}

	if (isRefResult && !isEmpty(tokenType)) {
		qpsClauseParser.addWithNodeArgument(iterationCount, direction, make_tuple(tokenType, tokenValue, attrType));
	}

	return isRefResult;
}

bool QPSPreprocessor::isAttrName(Lexer& lex, NodeType& attrType) {
	// Lexer view "#" as a separate token, hence combining 2 tokens together before validating them.
	bool isAttrNameResult = false;
	Token attrNameToken = lex.getNext();
	Token newToken = attrNameToken;
	if (attrNameToken.getValue() == "stmt") {
		string newValue = "stmt" + lex.getNext().getValue();
		newToken = Token(attrNameToken.getType(), newValue.c_str(), 5);
	}
	isAttrNameResult = isInVector(newToken, ATTR_NAME);

	if (isAttrNameResult) {
		attrType = convertStringToNodeType(newToken.getValue());
	}

	return isAttrNameResult;
}

void QPSPreprocessor::validateDeclaration(const Token declarationToken, Lexer& lex) {
	Token declarationValueToken = lex.getNext();
	validateSynonym(declarationValueToken); // must have one synonym
	NodeType declarationNodeType = convertStringToNodeType(declarationToken.getValue());
	qpsClauseParser.addSelectionDeclaration(declarationValueToken.getValue(), declarationNodeType);
	// either COMMA + NAME or SEMICOLON next; while loop so that it can have multiple declarations.
	while (lex.hasNext()) {
		Token token = lex.next();
		if (token.isComma()) {
			declarationValueToken = lex.getNext();
			validateSynonym(declarationValueToken);
			qpsClauseParser.addSelectionDeclaration(declarationValueToken.getValue(), declarationNodeType);
		} else if (token.isSemicolon()) {
			break;
		} else {
			throw QPSValidatorException("Missing semicolon!");
		}
	}
}

void QPSPreprocessor::validateSelect(Lexer& lex) {
	Token selectToken = lex.getNext();
	validateResultClause(selectToken, lex); // must have one synonym
	// such that and pattern are optional; while loop so that it can have multiple clauses
	while (lex.hasNext() && !qpsClauseParser.getIsSemanticError()) {
		iterationCount++;
		Token token = lex.next();
		if (token.getValue() == "such") {
			if (lex.getNext().getValue() == "that") {
				prevRelRefType = ClauseType::SUCHTHAT;
				validateSuchThat(lex);
			}
		} else if (token.getValue() == "pattern") {
			prevRelRefType = ClauseType::PATTERN;
			validatePattern(lex);
		} else if (token.getValue() == "with") {
			prevRelRefType = ClauseType::WITH;
			validateWith(lex);
		} else if (token.getValue() == "and") {
			switch (prevRelRefType) {
				case ClauseType::SUCHTHAT:
					validateSuchThat(lex);
					break;
				case ClauseType::PATTERN:
					validatePattern(lex);
					break;
				case ClauseType::WITH:
					validateWith(lex);
					break;
				default:
					throw QPSValidatorException("Invalid usage of AND before any clause!");
			}
		} else if (token.getType() == Token::Type::END_OF_FILE) {
			break;
		} else {
			throw QPSValidatorException("Invalid suchthat-cl or pattern-cl or with-cl!");
		}
	}
}

/**
* There is 2 different spot for createResultNode(token) as shown below:
* - synonym or synonym.attrRef
* - <elem, elem, ...>
*/
void QPSPreprocessor::validateResultClause(Token token, Lexer& lex) {
	if (token.isName()) { // single elem
		QPSPreprocessorElemResult elemResult = checkElem(token, lex);
		if (elemResult.isValid) {
			createResultNode(elemResult); // atrRef
		} else {
			throw QPSValidatorException("Invalid attrRef!");
		}
	} else if (token.isLAngle()) { // tuple
		Token synonymToken = lex.getNext();
		validateTupleSynonym(synonymToken);
		QPSPreprocessorElemResult elemResult = checkElem(synonymToken, lex);
		vector<QPSPreprocessorElemResult> elemResultList;
		if (elemResult.isValid) {
			elemResultList.push_back(elemResult);
			Token nextToken = lex.getNext();
			while (nextToken.isComma()) {
				synonymToken = lex.getNext();
				validateTupleSynonym(synonymToken);
				elemResult = checkElem(synonymToken, lex);
				if (elemResult.isValid) {
					elemResultList.push_back(elemResult);
				} else {
					throw QPSValidatorException("Invalid elem!");
				}
				nextToken = lex.getNext();
			}

			if (!nextToken.isRAngle()) {
				throw QPSValidatorException("Missing Right Angle Bracket!");
			}

			for (const auto& elemRes : elemResultList) {
				createResultNode(elemRes);
			}

		} else {
			throw QPSValidatorException("Invalid elem!");
		}
	} else {
		throw QPSValidatorException("Invalid result-cl!");
	}
}

void QPSPreprocessor::validateSuchThat(Lexer& lex) {
	Token relRefToken = lex.getNext();
	if (!isInVector(relRefToken, REL_REF)) {
		throw QPSValidatorException("Invalid relREF!");
	}

	bool hasStar = false;
	Token starToken = lex.getNext();
	if (isInVector(relRefToken, REL_REF_STAR) && starToken.isMultiply()) {
		hasStar = true;
		validateLeftParenthesis(lex.getNext());
	} else {
		validateLeftParenthesis(starToken);
	}

	if (isInVector(relRefToken, REL_REF_SS)) {
		validateStmtRef(lex.getNext(), LEFT_ARG);
		validateComma(lex.getNext());
		validateStmtRef(lex.getNext(), RIGHT_ARG);
	} else if (isInVector(relRefToken, REL_REF_SE_OR_EE)) {
		Token refToken = lex.getNext();
		if (!isStmtRef(refToken, LEFT_ARG) && !isEntRef(refToken, lex, LEFT_ARG, false)) {
			throw QPSValidatorException("Invalid stmtRef or entRef!");
		}
		validateComma(lex.getNext());
		validateEntRef(lex, RIGHT_ARG, false);
	} else if (isInVector(relRefToken, REL_REF_EE)) {
		validateEntRef(lex, LEFT_ARG, false);
		validateComma(lex.getNext());
		validateEntRef(lex, RIGHT_ARG, false);
	} else {
		throw QPSValidatorException("Invalid relREF or not yet implemented yet!");
	}

	qpsClauseParser.addSuchThatNodeType(iterationCount, convertStringtoQPSNodeType(relRefToken.getValue() + (hasStar ? "*" : "")));
	validateRightParenthesis(lex.getNext());
}

void QPSPreprocessor::validatePattern(Lexer& lex) {
	Token patternSynToken = lex.getNext();
	string patternSynValue = patternSynToken.getValue();
	NodeType patternNodeType = getArgNodeTypeFromMap(patternSynValue);
	qpsClauseParser.addPatternNodeArgument(iterationCount, PATTERN_SYN, make_pair(patternNodeType, patternSynValue));
	validateLeftParenthesis(lex.getNext());
	validateEntRef(lex, LEFT_ARG, true);
	validateComma(lex.getNext());

	if (patternNodeType == NodeType::ASSIGN) {
		validateExpressionSpec(lex);
	} else if (patternNodeType == NodeType::WHILE || patternNodeType == NodeType::IF) {
		validateUnderscore(lex.getNext());

		Token commaOrRPToken = lex.getNext();
		NodeType rightArgType = NodeType::EMPTY;
		if (commaOrRPToken.isComma()) { // pattern w (entRef,_,_)
			validateUnderscore(lex.getNext());
			validateRightParenthesis(lex.getNext());
			if (patternNodeType == NodeType::IF) {
				rightArgType = NodeType::WILDCARD;
			}
		} else { // pattern w (entRef,_)
			validateRightParenthesis(commaOrRPToken);
			if (patternNodeType == NodeType::WHILE) {
				rightArgType = NodeType::WILDCARD;
			}
		}
		qpsClauseParser.addPatternNodeArgument(iterationCount, RIGHT_ARG, make_pair(rightArgType, "_"));
	} else {
		qpsClauseParser.setIsSemanticErrorTrue(); // Semantic Error
	}
}

void QPSPreprocessor::validateWith(Lexer& lex) {
	if (!isRef(lex, LEFT_ARG)) {
		throw QPSValidatorException("Invalid with-ref!");
	}
	if (!lex.getNext().isEqual()) {
		throw QPSValidatorException("Missing equal");
	}
	if (!isRef(lex, RIGHT_ARG)) {
		throw QPSValidatorException("Invalid with-ref!");
	}
	qpsClauseParser.addSuchThatNodeType(iterationCount, QPSNodeType::WITH);
}

void QPSPreprocessor::validateSynonym(Token token) {
	if (!token.isName()) {
		throw QPSValidatorException("Missing synonym;");
	}
}

void QPSPreprocessor::validateTupleSynonym(Token token) {
	if (getArgNodeTypeFromMap(token.getValue()) == NodeType::BOOLEAN) {
		throw QPSValidatorException("Invalid element! BOOLEAN is not allowed to be selected unless it is declared as a synonym!");
	}
}

void QPSPreprocessor::validateStmtRef(const Token token, const Direction direction) {
	if (!isStmtRef(token, direction)) {
		throw QPSValidatorException("Invalid stmtRef!");
	}
}

void QPSPreprocessor::validateEntRef(Lexer& lex, const Direction direction, bool isPattern) {
	if (!isEntRef(lex, direction, isPattern)) {
		throw QPSValidatorException("Invalid entRef!");
	}
}

void QPSPreprocessor::validateExpressionSpec(Lexer& lex) {
	bool hasUnderscore = false;
	bool hasDoubleQuote = false;
	Token token = lex.getNext();
	if (token.isUnderscore()) {
		hasUnderscore = true;
		token = lex.getNext();
		if (token.isDoubleQuote()) {
			hasDoubleQuote = true;
		} else {
			NodeType nodeType = NodeType::EMPTY;
			if (token.isComma()) {
				// pattern(enRef,_,_)
				validateUnderscore(lex.getNext());
				validateRightParenthesis(lex.getNext());
			} else {
				// pattern(enRef,_)
				validateRightParenthesis(token);
				nodeType = NodeType::WILDCARD;
			}
			qpsClauseParser.addPatternNodeArgument(iterationCount, RIGHT_ARG, make_pair(nodeType, "_"));
		}
	} else if (token.isDoubleQuote()) {
		hasDoubleQuote = true;
	} else {
		throw QPSValidatorException("Missing either underscore or double quotes!");
	}

	if (hasDoubleQuote) {
		vector<Token> exprTokens{};
		string exprString{};
		Token token = lex.getNext();
		while (!token.isDoubleQuote()) {
			exprTokens.push_back(token);
			exprString += token.getValue();
			token = lex.getNext();
		}

		validateExpression(exprTokens);
		validateDoubleQuote(token);

		if (hasUnderscore) {
			validateUnderscore(lex.getNext());
		}

		Token commaOrRPToken = lex.getNext();
		NodeType patternNodeType = NodeType::EMPTY;
		if (commaOrRPToken.isComma()) {
			// pattern(enRef,_"expr"_,_)
			validateUnderscore(lex.getNext());
			validateRightParenthesis(lex.getNext());
		} else {
			// pattern(enRef,_"expr"_)
			validateRightParenthesis(commaOrRPToken);
			patternNodeType = NodeType::IDENT;
		}
		qpsClauseParser.addPatternNodeArgument(iterationCount, RIGHT_ARG, make_pair(patternNodeType, exprString));
		qpsClauseParser.addPatternExpression(iterationCount, make_pair(hasUnderscore, exprTokens));
	}
}

void QPSPreprocessor::validateExpression(vector<Token> exprTokens) {
	if (!ValidatorHelper::isExpr(exprTokens)) {
		throw QPSValidatorException("Invalid expression!");
	}
}

void QPSPreprocessor::validateLeftParenthesis(Token token) {
	if (!token.isLParam()) {
		throw QPSValidatorException("Missing LEFT_PARENTHESIS!");
	}
}

void QPSPreprocessor::validateRightParenthesis(Token token) {
	if (!token.isRParam()) {
		throw QPSValidatorException("Missing RIGHT_PARENTHESIS!");
	}
}

void QPSPreprocessor::validateComma(Token token) {
	if (!token.isComma()) {
		throw QPSValidatorException("Missing COMMA!");
	}
}

void QPSPreprocessor::validateUnderscore(Token token) {
	if (!token.isUnderscore()) {
		throw QPSValidatorException("Missing WILDCARD!");
	}
}

void QPSPreprocessor::validateDoubleQuote(Token token) {
	if (!token.isDoubleQuote()) {
		throw QPSValidatorException("Missing WILDCARD!");
	}
}

void QPSPreprocessor::createResultNode(const QPSPreprocessorElemResult elemResult) {
	string selectTokenValue = elemResult.synonymToken.getValue();
	NodeType attrName = NodeType::EMPTY;
	if (elemResult.hasAttrName) {
		attrName = convertStringToNodeType(elemResult.attrNameToken.getValue());
	}
	getArgNodeTypeFromMap(selectTokenValue);
	qpsClauseParser.addSelectionAttrName(selectTokenValue, attrName);
}

NodeType QPSPreprocessor::getArgNodeTypeFromMap(const string value) {
	const NodeType& result = qpsClauseParser.getArgNodeTypeFromDeclarationMap(value);
	if (isEmpty(result) && value == "such") { // Between Select & such that, the syn is missing
		throw QPSValidatorException("Missing SELECTION SYNONYM!");
	}
	return result;
}

QPSPreprocessorElemResult QPSPreprocessor::checkElem(Token token, Lexer& lex) {
	QPSPreprocessorElemResult result;
	result.isValid = false;
	result.hasAttrName = false;
	result.synonymToken = token;

	if (token.isName() && lex.peep().isPeriod()) {
		lex.next();
		Token attrNameToken = lex.getNext();
		if (attrNameToken.getValue() == "stmt") {
			string newValue = "stmt" + lex.getNext().getValue();
			Token newToken = Token(attrNameToken.getType(), newValue.c_str(), 5);
			if (isInVector(newToken, ATTR_NAME)) {
				result.isValid = true;
				result.hasAttrName = true;
				result.attrNameToken = newToken;
			}
		} else {
			if (isInVector(attrNameToken, ATTR_NAME)) {
				result.isValid = true;
				result.hasAttrName = true;
				result.attrNameToken = attrNameToken;
			}
		}
	} else {
		result.isValid = true;
	}
	return result;
}
