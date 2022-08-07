#include "ArgNode.h"

ArgNode::ArgNode() {}

ArgNode::ArgNode(const ArgNodeType& type, const ArgNodeRef& ref) : type{ type }, ref{ ref }, attrName{ NodeType::EMPTY } {}

ArgNode::ArgNode(const ArgNodeType& type, const ArgNodeRef& ref, const ArgAttrName& attrName) : type{ type }, ref{ ref }, attrName{ attrName } {}

ArgNodeType ArgNode::getType() const {
	return type;
}

ArgNodeRef ArgNode::getRef() const {
	return ref;
}

ArgAttrName ArgNode::getAttrName() const {
	return attrName;
}

ArgNodeRef& ArgNode::getStrRef() {
	return ref;
}

bool ArgNode::operator==(const ArgNode& rhs) const {
	return type == rhs.type && ref == rhs.ref;
}

bool ArgNode::operator!=(const ArgNode& rhs) const {
	return type != rhs.type || ref != rhs.ref;
}
