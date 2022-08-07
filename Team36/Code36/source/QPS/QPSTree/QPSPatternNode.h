#pragma once
#include <optional>

#include "QPSNode.h"
#include "../../TNode.h"

using namespace std;

typedef bool PartialMatching;
typedef ArgNode PatternSynArg;
typedef ArgNode SemanticArg;

class QPSPatternNode : public QPSNode {
public:
	QPSPatternNode();
	QPSPatternNode(const LeftArg& leftArg, const PatternSynArg& patternSynArg, const SemanticArg& semanticArg);
	QPSPatternNode(const LeftArg& leftArg, const PatternSynArg& patternSynArg, const PartialMatching& isPartialMatching, const SemanticArg& semanticArg);
	QPSPatternNode(const LeftArg& leftArg, const PatternSynArg& patternSynArg, const PartialMatching& isPartialMatching, const optional<TNode*>& expression, const SemanticArg& semanticArg);

	virtual int getSize() const override;
	QPSPatternNode getChild() const;
	QPSPatternNode getChild(const Index& index) const;
	shared_ptr<QPSPatternNode> getChildPointer(const Index& index);
	PatternSynArg getPatternSynArg() const;
	PartialMatching getIsPartialMatching() const;
	NodeType getPatternSynArgNodeType();
	ArgNodeRef getPatternSynArgRef();
	NodeType getSemanticArgType();
	bool hasExpression();
	TNode* getExpression();

	void addChild(QPSPatternNode* _child);
	void setIsPartialMatching(const PartialMatching& isPartialMatching);
	void print() override;
	bool operator==(const QPSPatternNode& rhs) const;
private:
	vector<QPSPatternNode*> children{};
	PartialMatching isPartialMatching;
	optional<TNode*> expression;
	ArgNode semanticArg;
};
