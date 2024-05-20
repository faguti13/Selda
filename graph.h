#pragma once
#include <vector>
#include "raylib.h"

struct Node {
    Vector2 position;
    std::vector<int> neighbors;
};

class Graph {
public:
    Graph(int size);
    void addEdge(int src, int dest);
    Node getNode(int index);
private:
    std::vector<Node> nodes;
};
