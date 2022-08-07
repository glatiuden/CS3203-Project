#pragma once
#include <string>

#include "../QPSHelper.h"
#include "../../NodeType.h"

using namespace std;

typedef NodeType ArgNodeType;
typedef string ArgNodeRef;
typedef NodeType ArgAttrName;

class ArgNode {
public:
	ArgNode();
	ArgNode(const ArgNodeType& type, const ArgNodeRef& ref);
	ArgNode(const ArgNodeType& type, const ArgNodeRef& ref, const ArgAttrName& attrName);
	ArgNodeType getType() const;
	ArgNodeRef getRef() const;
	ArgAttrName getAttrName() const;
	ArgNodeRef& getStrRef();
	bool operator==(const ArgNode& rhs) const;
	bool operator!=(const ArgNode& rhs) const;
protected:
	ArgNodeType type{};
	ArgNodeRef ref{};
	ArgAttrName attrName{};
};
