#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "BaseCharacter.h"
#include "Character.h"
#include <vector> 
#include "list.h"
#include "raymath.h"

class Enemy : public BaseCharacter
{
public:
    Rectangle visionRectangle; // Nuevo miembro de datos para el rectángulo de visión
    t_List* pathList = nullptr;
    float lastPathCalculationTime;
    int type;
    Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float _visionRange, int type);
    ~Enemy();
    virtual void tick(float deltaTime) override;
    void SetTarget(Character* character);
    virtual Vector2 getScreenPos() override;
    std::vector<Vector2> pathPoints; // Almacena los puntos de la ruta
    int currentPathPoint = 0; // Almacena el índice del punto de la ruta actual
    std::vector<Vector2> patrolPoints; 
    int currentPatrolPoint = 0; 

    void moveTo(Vector2 newWorldPos) {
        worldPos = newWorldPos;

    }

    void setWorldPos(const Vector2& newPos) {
        worldPos = newPos;
    }

    Rectangle getVisionRectangle() {
        return visionRectangle;
    }

    int getType();

    void callEnemies() {
        callAllEnemies = true;
    }
private:
    Character* target;
    float damagePerSec{100.f};
    float radius{25.f};
    float visionRange;
    float scale{4.0f};
    bool callAllEnemies{false};
    Texture2D weapon{LoadTexture("characters/weapon_sword.png")};
    Texture2D fireweapon{LoadTexture("characters/firesword.png")};
};

#endif