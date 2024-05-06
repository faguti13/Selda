#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "BaseCharacter.h"
#include "Character.h"
#include <vector> // Asegúrate de incluir este header para usar std::vector

class Enemy : public BaseCharacter
{
public:
    Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float _visionRange); // Añade el argumento _visionRange
    ~Enemy();
    virtual void tick(float deltaTime) override;
    void SetTarget(Character* character);
    virtual Vector2 getScreenPos() override;
    void drawVisionRange(); // Declara la función drawVisionRange

    std::vector<Vector2> patrolPoints; // Añade esta línea
    int currentPatrolPoint = 0; // Añade esta línea

private:
    Character* target;
    float damagePerSec{10.f};
    float radius{25.f};
    float visionRange; // Declara la variable visionRange
};

#endif