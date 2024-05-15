#include "Enemy.h"
#include "raymath.h"


Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float _visionRange)
    : visionRange(_visionRange) {
    worldPos = pos;
    texture = idle_texture;
    run = run_texture;
    idle = idle_texture;
    width = texture.width / maxFrames;
    height = texture.height;
    speed = 3.5f;
    patrol_speed= 100.f;
    
}

Enemy::~Enemy() 
{
    UnloadTexture(texture);
    UnloadTexture(idle);
    UnloadTexture(run);
}

class Node {
public:
    Vector2 pos;
    Node* next;

    Node(Vector2 pos) : pos(pos), next(nullptr) {}
};

class Stack {
private:
    Node* topNode;

public:
    Stack() : topNode(nullptr) {}

    void push(Vector2 pos) {
        Node* newNode = new Node(pos);
        newNode->next = topNode;
        topNode = newNode;
    }

    Vector2 pop() {
        if (topNode == nullptr) {
            return Vector2{}; // Devuelve un vector vacío si la pila está vacía
        }

        Vector2 topPos = topNode->pos;
        Node* oldTop = topNode;
        topNode = topNode->next;
        delete oldTop;
        return topPos;
    }

    bool empty() {
        return topNode == nullptr;
    }
};

void Enemy::tick(float deltaTime)
{
    if(!getAlive()) return;

    // Crear una pila para almacenar los breadcrumbs
    Stack breadcrumbs;

    // Calcular la distancia al jugador
    //float distanceToPlayer = Vector2Length(Vector2Subtract(target->getScreenPos(), getScreenPos()));
    
    Vector2 screenPos = getScreenPos();

    if (rightLeft >= 0) {
        // Si el enemigo está mirando a la derecha, el rectángulo de visión está a la derecha del enemigo
        visionRectangle = { screenPos.x + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
    } else {
        // Si el enemigo está mirando a la izquierda, el rectángulo de visión está a la izquierda del enemigo
        visionRectangle = { screenPos.x - visionRange + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
    }

    //debug collision rectangle
    Color visionColor = {255, 0, 0, 50}; // Color rojo completamente opaco
    DrawRectangleRec(visionRectangle, visionColor);

    // Si el jugador está dentro del rango de visión del enemigo
    if (CheckCollisionRecs(visionRectangle, target->getCollisionRec())) {
        // Actualizar la velocidad para seguir al jugador
        velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());

        // Si el enemigo está demasiado cerca del jugador, se detiene
        if (Vector2Length(velocity) < radius) {
            velocity = {};
        }

        // Añade la posición actual del enemigo a la pila de breadcrumbs
        // Esto hace que el enemigo pueda volver a la ruta de patrullaje cuando pierda al jugador de vista
        breadcrumbs.push(getScreenPos());
    } 
    // Si el jugador está fuera del rango de visión del enemigo
    else {
        // Si hay breadcrumbs en la pila
        if (!breadcrumbs.empty()&& !pathPoints.empty()) {
            // Sacar el breadcrumb superior de la pila
            // Backtracking: el enemigo está volviendo sobre sus pasos
            Vector2 nextPos = breadcrumbs.pop();

            // Actualizar la velocidad para moverse hacia el breadcrumb
            velocity = Vector2Subtract(nextPos, getScreenPos());
        } 
        // Si no hay breadcrumbs en la pila
        else {
            // Si no hay puntos de patrulla, no hacer nada
            if (patrolPoints.empty()) {
                return;
            }

            // Calcular la dirección hacia el próximo punto de la ruta de patrullaje
            Vector2 direction = Vector2Subtract(patrolPoints[currentPatrolPoint], worldPos);

            rightLeft = direction.x >= 0 ? 1 : -1;

            // Si el enemigo está cerca del punto de la ruta, moverse al siguiente punto
            if (Vector2Length(direction) < patrol_speed * deltaTime) {
                currentPatrolPoint = (currentPatrolPoint + 1) % patrolPoints.size();
            } 
            // Si el enemigo está lejos del punto de patrulla, moverse hacia el punto
            else {
                direction = Vector2Normalize(direction);
                worldPos = Vector2Add(worldPos, Vector2Scale(direction, patrol_speed * deltaTime));
            }
        }
    }

    BaseCharacter::tick(deltaTime);

    if(CheckCollisionRecs(getCollisionRec(), target->getCollisionRec()) && CheckCollisionRecs(visionRectangle, target->getCollisionRec())  && !CheckCollisionRecs(getCollisionRec(),target->getVisionRectangle()) ){
        target->takeDamage(damagePerSec);
    }
    
    if (patrolPoints.empty()) {
        // Si patrolPoints está vacío, no accede a sus elementos
        return;
    }

    Vector2 direction = Vector2Subtract(patrolPoints[currentPatrolPoint], worldPos);
    if (Vector2Length(direction) < speed * deltaTime) {
        // Si el enemigo está lo suficientemente cerca del punto de patrulla, pasa al siguiente
        currentPatrolPoint = (currentPatrolPoint + 1) % patrolPoints.size();
    } else {
        // Si no, mueve al enemigo hacia el punto de patrulla
        direction = Vector2Normalize(direction);
        worldPos = Vector2Add(worldPos, Vector2Scale(direction, speed * deltaTime));
    }

    

}

void Enemy::SetTarget(Character *character)
{
    target = character;
}

Vector2 Enemy::getScreenPos() {
    // Devuelve la posición del enemigo con respecto al jugador
    return Vector2Subtract(worldPos, target->getWorldPos());
}