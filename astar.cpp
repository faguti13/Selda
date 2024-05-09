#include <iostream>
#include <cstdlib>
#include "raylib.h"
#include "list.h"
#include <limits>

int min_score = std::numeric_limits<int>::max();  // Utilizando INT_MAX de forma segura

#define INT_MAX 2147483647  // Valor máximo para un int de 32 bits con signo
#define SCREEN_WIDTH (960)
#define SCREEN_HEIGHT (640)
#define TILE_SIZE (32)

#define WINDOW_TITLE "A* - raylib"

struct AStarNode {
    AStarNode* parent;

    float x;
    float y;

    int g;
    int h;
    int f;
};

int map[960*4][640*4] = {0};

AStarNode* CreateAStarNode(float x, float y, int g, int h) {
    AStarNode* node = new AStarNode();

    if (node == nullptr) {
        return nullptr;
    }

    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->f = 8 * g + 10 * h;

    return node;
}

int CheckIfInList(t_List* list, float x, float y) {
    for (int i = 0; i < list->size; i++) {
        AStarNode* tempNode = static_cast<AStarNode*>(list_get(list, i));
        if (tempNode == nullptr) {
            return 0;
        }
        if (tempNode->x == x && tempNode->y == y) {
            return i;
        }
    }
    return 0;
}

float DistanceBetween(float start_x, float start_y, float end_x, float end_y) {
    return (abs(start_x - end_x) + abs(start_y - end_y));
}

int CheckIfWall(float x, float y) {
    int xToInt = static_cast<int>(x);
    int yToInt = static_cast<int>(y);
    return map[xToInt][yToInt];
}

t_List* AStar(Vector2 start, Vector2 target) {
    t_List* openList = list_create();
    t_List* closedList = list_create();
    t_List* pathList = list_create();

    double runTime = GetTime();

    AStarNode* startNode = CreateAStarNode(start.x, start.y, 0, 0);
    AStarNode* targetNode = CreateAStarNode(target.x, target.y, 0, 0);

    list_push(openList, startNode);

    while (openList->size > 0) {
        int index = 0;
        int minScore = INT_MAX;

        for (int i = 0; i < openList->size; i++) {
            AStarNode* tempNode = static_cast<AStarNode*>(list_get(openList, i));
            if (tempNode->f <= minScore) {
                minScore = tempNode->f;
                index = i;
            }
        }

        AStarNode* currentNode = static_cast<AStarNode*>(list_get(openList, index));
        list_remove(openList, currentNode);
        list_push(closedList, currentNode);

        if (currentNode->x == target.x && currentNode->y == target.y) {
            targetNode->parent = currentNode;
            list_push(closedList, targetNode);

            AStarNode* node = targetNode;
            for (int i = 0; i < closedList->size; i++) {
                int nextIndex = list_find(closedList, node->parent);
                node = static_cast<AStarNode*>(list_get(closedList, nextIndex));

                AStarNode* pathNode = CreateAStarNode(node->x, node->y, node->g, node->h);
                list_push(pathList, pathNode);

                if (!DistanceBetween(node->x, node->y, startNode->x, startNode->y)) {
                    list_erase(openList);
                    list_erase(closedList);

                    runTime = GetTime() - runTime;
                    TraceLog(LOG_INFO, TextFormat("Elapsed time: %f seconds", runTime));

                    return pathList;
                }
            }
        }

        Vector2 nextPosition[4] = {{currentNode->x + 1, currentNode->y},
                                   {currentNode->x - 1, currentNode->y},
                                   {currentNode->x, currentNode->y + 1},
                                   {currentNode->x, currentNode->y - 1}};

        for (int i = 0; i < 4; i++) {
            if (!CheckIfInList(closedList, nextPosition[i].x, nextPosition[i].y) &&
                !CheckIfWall(nextPosition[i].x, nextPosition[i].y)) {
                AStarNode* child = CreateAStarNode(nextPosition[i].x, nextPosition[i].y, 0, 0);
                child->g = DistanceBetween(child->x, child->y, start.x, start.y);
                child->h = DistanceBetween(child->x, child->y, target.x, target.y);
                child->f = 8 * child->g + 10 * child->h;

                int index = CheckIfInList(openList, child->x, child->y);
                if (index > 0) {
                    AStarNode* oldNode = static_cast<AStarNode*>(list_get(openList, index));
                    if (oldNode != nullptr) {
                        if (child->g <= oldNode->g) {
                            oldNode->g = child->g;
                            oldNode->parent = currentNode;
                            delete child;
                        }
                    }
                } else {
                    child->parent = currentNode;
                    list_push(openList, child);
                }
            }
        }
    }

    list_erase(openList);
    list_erase(closedList);
    list_erase(pathList);
    return nullptr;
}

Entity player = {{3.0f, 3.0f}, BLUE, TILE_SIZE};
Entity enemy = {{15.0f, 7.0f}, RED, TILE_SIZE};
t_List* pathList = nullptr;

void UpdatePlayerPosition() {
    if (IsKeyDown(KEY_W)) player.position.y -= 1;
    if (IsKeyDown(KEY_S)) player.position.y += 1;
    if (IsKeyDown(KEY_A)) player.position.x -= 1;
    if (IsKeyDown(KEY_D)) player.position.x += 1;
}

void UpdateEnemyPosition() {
    // Si no hay un camino calculado, calcularlo
    if (pathList == nullptr || pathList->size == 0) {
        Vector2 start = { enemy.position.x, enemy.position.y };
        Vector2 target = { player.position.x, player.position.y };
        pathList = AStar(start, target);
    }

    // Si hay un camino válido
    if (pathList != nullptr && pathList->size > 0) {
        // Obtener el siguiente nodo en el camino
        AStarNode* nextNode = static_cast<AStarNode*>(list_get(pathList, 0));
        if (nextNode != nullptr) {
            // Mover al enemigo hacia el siguiente nodo en el camino
            enemy.position.x = nextNode->x;
            enemy.position.y = nextNode->y;

            // Eliminar el nodo actual del camino
            list_remove_first(pathList);
        }
        else {
            // Si no hay más nodos en el camino, eliminar el camino actual
            list_erase(pathList);
        }
    }
}
