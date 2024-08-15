#include "graph.h"

Graph::Graph(int size) {
    for (int i = 0; i < size; i++) {
        nodes.push_back({{0, 0}, {}});
    }
}

void Graph::addEdge(int src, int dest) {
    nodes[src].neighbors.push_back(dest);
    nodes[dest].neighbors.push_back(src);
}

Node Graph::getNode(int index) {
    return nodes[index];
}
