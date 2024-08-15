#ifndef NODE_H
#define NODE_H

class Node {
public:
    int x, y;
    float gCost, hCost;
    Node* parent;

    Node(int x, int y, Node* parent = nullptr);

    float fCost() const;
};

#endif // NODE_H