#pragma once
#include "stdafx.h"
#include "QPS/QPSTree/ArgNode.h"

class ArgNodeStub : public ArgNode {
public:
	ArgNodeStub::ArgNodeStub() {}
	ArgNodeStub::ArgNodeStub(const ArgNodeType& type, const ArgNodeRef& ref) : ArgNode(type, ref) {}
	ArgNodeStub::ArgNodeStub(const ArgNodeType& type, const ArgNodeRef& ref, const ArgAttrName& attrName) : ArgNode(type, ref, attrName) {}

	bool ArgNodeStub::operator==(const ArgNodeStub& rhs) const {
		return type == rhs.type && ref == rhs.ref;
	}

	bool ArgNodeStub::operator!=(const ArgNodeStub& rhs) const {
		return type != rhs.type || ref != rhs.ref;
	}
};
