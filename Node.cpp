#include "Node.h"
#include <cmath>

Node::Node(int x, int y, Node* parent) : x(x), y(y), parent(parent), gCost(0), hCost(0) {}

float Node::fCost() const {
    return gCost + hCost;
}