#include "Enemy.h"
#include "raymath.h"
#include <iostream>
// #include <astar.h>

Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float _visionRange, int type)
    : visionRange(_visionRange) {
    worldPos = pos;
    texture = idle_texture;
    run = run_texture;
    idle = idle_texture;
    width = texture.width / maxFrames;
    height = texture.height;
    speed = 3.5f;
    patrol_speed= 100.f;
    tipo = type;
    
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

    // Gris
    if (tipo == 0) {
        // Crear una pila para almacenar los breadcrumbs

        Stack breadcrumbs;

        Vector2 screenPos = getScreenPos();

        if (rightLeft >= 0) {
            // Si el enemigo está mirando a la derecha, el rectángulo de visión está a la derecha del enemigo
            visionRectangle = { screenPos.x + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
        } else {
            // Si el enemigo está mirando a la izquierda, el rectángulo de visión está a la izquierda del enemigo
            visionRectangle = { screenPos.x - visionRange + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
        }

        //debug collision rectangle
        // Color visionColor = {255, 0, 0, 50}; // Color rojo completamente opaco
        // DrawRectangleRec(visionRectangle, visionColor);

        // Si el jugador está dentro del rango de visión del enemigo
        if (CheckCollisionRecs(visionRectangle, target->getCollisionRec())    && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone) && !colPath) {

            // callAllEnemies = true;

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

        else if (colPath && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone)){
            // Calcular la dirección hacia el próximo punto de la ruta de patrullaje
            Vector2 direction = Vector2Subtract(colPathPoints[currentColPathPoint], worldPos);

            rightLeft = direction.x >= 0 ? 1 : -1;

            // Si el enemigo está cerca del punto de la ruta, moverse al siguiente punto
            if (Vector2Length(direction) < (patrol_speed*10)  * deltaTime) {
                int nextPathPoint = currentColPathPoint + 1;
                if (nextPathPoint >= colPathPoints.size() / 5) {
                    colPath = false;
                }
                currentColPathPoint = nextPathPoint % colPathPoints.size();
            } 
            // Si el enemigo está lejos del punto de patrulla, moverse hacia el punto
            else {
                direction = Vector2Normalize(direction);
                worldPos = Vector2Add(worldPos, Vector2Scale(direction, (patrol_speed*2) * deltaTime));
            }
        }


        else if (callAllEnemies)
        {
            // Calcular la dirección hacia el próximo punto de la ruta de patrullaje
            Vector2 direction = Vector2Subtract(pathPoints[currentPathPoint], worldPos);

            rightLeft = direction.x >= 0 ? 1 : -1;

        // Si el enemigo está cerca del punto de la ruta, moverse al siguiente punto
            if (Vector2Length(direction) < patrol_speed * deltaTime) {
                currentPathPoint = (currentPathPoint + 1) % pathPoints.size();
            } 
            // Si el enemigo está lejos del punto de patrulla, moverse hacia el punto
            else {
                direction = Vector2Normalize(direction);
                worldPos = Vector2Add(worldPos, Vector2Scale(direction, patrol_speed * deltaTime));
            }
        }
        else if (isARat){
            undoMovement();
            velocity = {};
            
        }
        // Si el jugador está fuera del rango de visión del enemigo
        else {
            // Si hay breadcrumbs en la pila
            if (!breadcrumbs.empty()&& !pathPoints.empty()) {
                // Sacar el breadcrumb superior de la pila
                // Backtracking: el enemigo está volviendo sobre sus pasos
                Vector2 nextPos = breadcrumbs.pop();//xd

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
        Vector2 origin{};
        Vector2 offset{};
        float rotate{};

        if (rightLeft > 0.f)
        {
            origin = {0.f, weapon.height * scale};
            offset = {35.f, 55.f};
        }
        else
        {
            origin = {weapon.width * scale, weapon.height * scale};
            offset = {25.f, 55.f};
        }

        // draw sword
        Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
        Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};
        DrawTexturePro(weapon, source, dest, origin, rotate, WHITE);

        }
        
    //Rojo
    else if (tipo == 1){
    // Crear una pila para almacenar los breadcrumbs

    Stack breadcrumbs;

    Vector2 screenPos = getScreenPos();

    if (rightLeft >= 0) {
        // Si el enemigo está mirando a la derecha, el rectángulo de visión está a la derecha del enemigo
        visionRectangle = { screenPos.x + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
    } else {
        // Si el enemigo está mirando a la izquierda, el rectángulo de visión está a la izquierda del enemigo
        visionRectangle = { screenPos.x - visionRange + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
    }

    //debug collision rectangle
    // Color visionColor = {255, 0, 0, 50}; // Color rojo completamente opaco
    // DrawRectangleRec(visionRectangle, visionColor);

    // Define el centro y el radio del círculo
    Vector2 center = { screenPos.x + 32, screenPos.y + 32 };
    float radius = 300.0f;

    // Define el color del círculo con transparencia
    Color color = { 255, 255/2, 0, 100 }; // Blanco semi-transparente

    // Dibuja el círculo
    DrawCircleV(center, radius, color);

    // Si el jugador está dentro del rango de visión del enemigo
    if (CheckCollisionRecs(visionRectangle, target->getCollisionRec())    && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone)  ) {

        // callAllEnemies = true;

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


    else if (colPath && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone)){
        // Calcular la dirección hacia el próximo punto de la ruta de patrullaje
        Vector2 direction = Vector2Subtract(colPathPoints[currentColPathPoint], worldPos);

        rightLeft = direction.x >= 0 ? 1 : -1;

        // Si el enemigo está cerca del punto de la ruta, moverse al siguiente punto
        if (Vector2Length(direction) < (patrol_speed*10)  * deltaTime) {
            int nextPathPoint = currentColPathPoint + 1;
            if (nextPathPoint >= colPathPoints.size() / 5) {
                colPath = false;
            }
            currentColPathPoint = nextPathPoint % colPathPoints.size();
        } 
        // Si el enemigo está lejos del punto de patrulla, moverse hacia el punto
        else {
            direction = Vector2Normalize(direction);
            worldPos = Vector2Add(worldPos, Vector2Scale(direction, (patrol_speed*2) * deltaTime));
        }
    }



    else if (callAllEnemies)
    {
        // Calcular la dirección hacia el próximo punto de la ruta de patrullaje
        Vector2 direction = Vector2Subtract(pathPoints[currentPathPoint], worldPos);

        rightLeft = direction.x >= 0 ? 1 : -1;

    // Si el enemigo está cerca del punto de la ruta, moverse al siguiente punto
        if (Vector2Length(direction) < patrol_speed * deltaTime) {
            currentPathPoint = (currentPathPoint + 1) % pathPoints.size();
        } 
        // Si el enemigo está lejos del punto de patrulla, moverse hacia el punto
        else {
            direction = Vector2Normalize(direction);
            worldPos = Vector2Add(worldPos, Vector2Scale(direction, patrol_speed * deltaTime));
        }
    }
    // Si el jugador está fuera del rango de visión del enemigo
    else {
        // Si hay breadcrumbs en la pila
        if (!breadcrumbs.empty()&& !pathPoints.empty()) {
            // Sacar el breadcrumb superior de la pila
            // Backtracking: el enemigo está volviendo sobre sus pasos
            Vector2 nextPos = breadcrumbs.pop();//xd

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
    Vector2 origin{};
    Vector2 offset{};
    float rotate{};

    if (rightLeft > 0.f)
    {
        origin = {0.f, fireweapon.height * scale};
        offset = {35.f, 55.f};
    }
    else
    {
        origin = {fireweapon.width * scale, fireweapon.height * scale};
        offset = {25.f, 55.f};
    }

    // draw sword
    Rectangle source{0.f, 0.f, static_cast<float>(fireweapon.width) * rightLeft, static_cast<float>(fireweapon.height)};
    Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, fireweapon.width * scale, fireweapon.height * scale};
    DrawTexturePro(fireweapon, source, dest, origin, rotate, WHITE);        
    }
    
    //azul
    else if (tipo == 2){
                // Crear una pila para almacenar los breadcrumbs

    Stack breadcrumbs;

    Vector2 screenPos = getScreenPos();

    if (rightLeft >= 0) {
        // Si el enemigo está mirando a la derecha, el rectángulo de visión está a la derecha del enemigo
        visionRectangle = { screenPos.x + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
    } else {
        // Si el enemigo está mirando a la izquierda, el rectángulo de visión está a la izquierda del enemigo
        visionRectangle = { screenPos.x - visionRange + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
    }

    //debug collision rectangle
    // Color visionColor = {255, 0, 0, 50}; // Color rojo completamente opaco
    // DrawRectangleRec(visionRectangle, visionColor);

    // Si el jugador está dentro del rango de visión del enemigo
    if (CheckCollisionRecs(visionRectangle, target->getCollisionRec())    && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone)  ) {

        // callAllEnemies = true;

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


    else if (colPath && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone)){
        // Calcular la dirección hacia el próximo punto de la ruta de patrullaje
        Vector2 direction = Vector2Subtract(colPathPoints[currentColPathPoint], worldPos);

        rightLeft = direction.x >= 0 ? 1 : -1;

        // Si el enemigo está cerca del punto de la ruta, moverse al siguiente punto
        if (Vector2Length(direction) < (patrol_speed*10)  * deltaTime) {
            int nextPathPoint = currentColPathPoint + 1;
            if (nextPathPoint >= colPathPoints.size() / 5) {
                colPath = false;
            }
            currentColPathPoint = nextPathPoint % colPathPoints.size();
        } 
        // Si el enemigo está lejos del punto de patrulla, moverse hacia el punto
        else {
            direction = Vector2Normalize(direction);
            worldPos = Vector2Add(worldPos, Vector2Scale(direction, (patrol_speed*2) * deltaTime));
        }
    }


    else if (callAllEnemies)
    {
        // Teletransportarse a la posicion del ojo espectral
    }
    // Si el jugador está fuera del rango de visión del enemigo
    else {
        // Si hay breadcrumbs en la pila
        if (!breadcrumbs.empty()&& !pathPoints.empty()) {
            // Sacar el breadcrumb superior de la pila
            // Backtracking: el enemigo está volviendo sobre sus pasos
            Vector2 nextPos = breadcrumbs.pop();//xd

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
    Vector2 origin{};
    Vector2 offset{};
    float rotate{};

    if (rightLeft > 0.f)
    {
        origin = {0.f, weapon.height * scale};
        offset = {35.f, 55.f};
    }
    else
    {
        origin = {weapon.width * scale, weapon.height * scale};
        offset = {25.f, 55.f};
    }

    // draw sword
    Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
    Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};
    DrawTexturePro(weapon, source, dest, origin, rotate, WHITE);


    }
    
    //Ojo espectral
    else if (tipo == 3){
        
    Vector2 screenPos = getScreenPos();


    if (rightLeft >= 0) {
        // Si el enemigo está mirando a la derecha, el rectángulo de visión está a la derecha del enemigo
        visionRectangle = { screenPos.x + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
    } else {
        // Si el enemigo está mirando a la izquierda, el rectángulo de visión está a la izquierda del enemigo
        visionRectangle = { screenPos.x - visionRange + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
    }

    //debug collision rectangle
    // Color visionColor = {255, 0, 0, 50}; // Color rojo completamente opaco
    // DrawRectangleRec(visionRectangle, visionColor);

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

        // Si el jugador está dentro del rango de visión del enemigo
        if (CheckCollisionRecs(visionRectangle, target->getCollisionRec())    && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone)  ) {
            undoMovement();
        } 
        else 
        {

            
        }
    }
    
    //Raton
    else if (tipo == 4){
        static double lastDirectionChangeTime = 0;
        static Vector2 direction = {0, 0};
        double directionChangeInterval = 1.0; // Cambia la dirección cada 2 segundos

        double currentTime = GetTime();
        if (currentTime - lastDirectionChangeTime >= directionChangeInterval) {
            // Genera una nueva dirección aleatoria
            float directionX = GetRandomValue(0, 1);
            float directionY = GetRandomValue(0, 1);
            direction = {directionX, directionY};

            // Normaliza la dirección
            direction = Vector2Normalize(direction);

            lastDirectionChangeTime = currentTime;
        }

        // Mueve al enemigo en la dirección actual
        worldPos = Vector2Add(worldPos, Vector2Scale(direction, speed * deltaTime)); 

        velocity = direction;    

            
    }

    //Chocobo
    else if (tipo == 5){
        Vector2 screenPos = getScreenPos();
        if (rightLeft >= 0) {
            // Si el enemigo está mirando a la derecha, el rectángulo de visión está a la derecha del enemigo
            visionRectangle = { screenPos.x + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
        } else {
            // Si el enemigo está mirando a la izquierda, el rectángulo de visión está a la izquierda del enemigo
            visionRectangle = { screenPos.x - visionRange + (width*scale/2), screenPos.y - visionRange / 2, visionRange, visionRange };
        }
        
                // Si el jugador está dentro del rango de visión del enemigo
        if (!CheckCollisionRecs(target->getCollisionRec(),target->safeZone) && !colPath) {

            // callAllEnemies = true;

            // Actualizar la velocidad para seguir al jugador
            velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());

            // Si el enemigo está demasiado cerca del jugador, se detiene
            if (Vector2Length(velocity) < radius) {
                velocity = {};
            }
        } 

        else if (colPath && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone)){
            // Calcular la dirección hacia el próximo punto de la ruta de patrullaje
            Vector2 direction = Vector2Subtract(colPathPoints[currentColPathPoint], worldPos);

            rightLeft = direction.x >= 0 ? 1 : -1;

            // Si el enemigo está cerca del punto de la ruta, moverse al siguiente punto
            if (Vector2Length(direction) < (patrol_speed*10)  * deltaTime) {
                int nextPathPoint = currentColPathPoint + 1;
                if (nextPathPoint >= colPathPoints.size() / 5) {
                    colPath = false;
                }
                currentColPathPoint = nextPathPoint % colPathPoints.size();
            } 
            // Si el enemigo está lejos del punto de patrulla, moverse hacia el punto
            else {
                direction = Vector2Normalize(direction);
                worldPos = Vector2Add(worldPos, Vector2Scale(direction, (patrol_speed*2) * deltaTime));
            }
        }        
    }



    else if (tipo == 6){
        setScale(10.0f);
        scale = 10.0f;
        speed = 2.0f;
        patrol_speed = 50.0f;
        

        Vector2 screenPos = getScreenPos();
        if (rightLeft >= 0) {
            // Si el enemigo está mirando a la derecha, el rectángulo de visión está a la derecha del enemigo
            visionRectangle = { screenPos.x+64 + (width*scale/2), screenPos.y+64 - visionRange / 2, visionRange, visionRange };
        } else {
            // Si el enemigo está mirando a la izquierda, el rectángulo de visión está a la izquierda del enemigo
            visionRectangle = { screenPos.x-64 - visionRange + (width*scale/2), screenPos.y+64 - visionRange / 2, visionRange, visionRange };
        }
        
                // Si el jugador está dentro del rango de visión del enemigo
        if (!CheckCollisionRecs(target->getCollisionRec(),target->safeZone) && !colPath) {

            // callAllEnemies = true;

            // Actualizar la velocidad para seguir al jugador
            velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());

            Color visionColor = {255, 0, 0, 50}; // Color rojo completamente opaco
            // DrawRectangleRec(visionRectangle, visionColor);

            // Si el enemigo está demasiado cerca del jugador, se detiene
            if (CheckCollisionRecs(getVisionRectangle(), target->getCollisionRec())) {
                if (!isResting) {
                    isResting = true;
                    collisionTime = 0.0f;
                }
            }
            if (isResting) {
                collisionTime += deltaTime; // Aumenta el tiempo de colisión
                if (collisionTime >= 2.0f) { // Si han pasado 5 segundos
                    isResting = false; // El enemigo puede moverse de nuevo
                    isStriking = true; // El enemigo está en el estado de "golpeando"
                }
                velocity = {}; // El enemigo se queda quieto
            } else {
                isStriking = false; // El enemigo no está en el estado de "golpeando"
            }

            // Si el enemigo está en el estado de "golpeando"
            if (isStriking) {
                ;
                if (CheckCollisionRecs(getVisionRectangle(), target->getCollisionRec())) {
                target->takeDamage(damagePerSec);
                }
                // Inclina la espada para golpear al jugador
                // Aquí puedes agregar el código para inclinar la espada
            }
        } 

        else if (colPath && !CheckCollisionRecs(target->getCollisionRec(),target->safeZone)){
            // Calcular la dirección hacia el próximo punto de la ruta de patrullaje
            Vector2 direction = Vector2Subtract(colPathPoints[currentColPathPoint], worldPos);

            rightLeft = direction.x >= 0 ? 1 : -1;

            // Si el enemigo está cerca del punto de la ruta, moverse al siguiente punto
            if (Vector2Length(direction) < (patrol_speed*10)  * deltaTime) {
                int nextPathPoint = currentColPathPoint + 1;
                if (nextPathPoint >= colPathPoints.size() / 5) {
                    colPath = false;
                }
                currentColPathPoint = nextPathPoint % colPathPoints.size();
            } 
            // Si el enemigo está lejos del punto de patrulla, moverse hacia el punto
            else {
                direction = Vector2Normalize(direction);
                worldPos = Vector2Add(worldPos, Vector2Scale(direction, (patrol_speed*2) * deltaTime));
            }
        }
        Vector2 origin{};
        Vector2 offset{};
        float rotate{};

        if (rightLeft > 0.f)
        {
            origin = {0.f, weapon.height * scale};
            offset = {115.f, 150.f};
        }
        else
        {
            origin = {weapon.width * scale, weapon.height * scale};
            offset = {50.f, 150.f};
        }

        // draw sword
        Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
        Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};
        DrawTexturePro(weapon, source, dest, origin, rotate, WHITE);

        // DrawRectangleRec(getCollisionRec(), RED);

        if (health <= 0){
            setAlive(false);
        }
    }
    BaseCharacter::tick(deltaTime);
}

void Enemy::SetTarget(Character *character)
{
    target = character;
}

Vector2 Enemy::getScreenPos() {
    // Devuelve la posición del enemigo con respecto al jugador
    return Vector2Subtract(worldPos, target->getWorldPos());
}

int Enemy::getType()
{
    return tipo;
}

bool Enemy::checkLineOfSight(Vector2 start, Vector2 end)
{
    int x0 = static_cast<int>(start.x);
    int y0 = static_cast<int>(start.y);
    int x1 = static_cast<int>(end.x);
    int y1 = static_cast<int>(end.y);

    //Diferencia que hay entre el punto de partida al del caballero
    int dif_x = abs(x1 - x0);
    int dif_y = abs(y1 - y0);
    //Signo de la posición final
    int sign_x = x0 < x1 ? 1 : -1;
    int sign_y = y0 < y1 ? 1 : -1;
    //Ajustar la traectoria
    int err = dif_x - dif_y;

    while (true) {
        // Verificar si hay un obstáculo en las coordenadas (x0, y0)
        //if (isAnObstacle(x0,y0)) {
           // return false; // Hay un obstáculo en la línea de visión
        //}

        if (x0 == x1 && y0 == y1) break; //El chocobo llegó al caballero
        int err_2 = err * 2;
        if (err_2 > -dif_y) {
            err -= dif_y;
            x0 += sign_x;
        }
        if (err_2 < dif_x) {
            err += dif_x;
            y0 += sign_y;
        }
    }

    return true; // No obstaculos
}