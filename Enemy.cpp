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
    speed = 2.f;
    
}


Enemy::~Enemy() 
{
    UnloadTexture(texture);
    UnloadTexture(idle);
    UnloadTexture(run);
}


void Enemy::tick(float deltaTime)
{
    if(!getAlive()) return;
    //Breadcrumbing viejo / original
    // get toTarget
    float distanceToPlayer = Vector2Length(Vector2Subtract(target->getScreenPos(), getScreenPos()));
    if (distanceToPlayer < visionRange) {
        // Si el jugador está dentro del rango de visión, actualizar la velocidad para seguir al jugador
        velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
        if (Vector2Length(velocity) < radius) {
            velocity = {};
        }
    } else {
        // Si el jugador está fuera del rango de visión, detener al enemigo
        velocity = {};
    }

    BaseCharacter::tick(deltaTime);

    if(CheckCollisionRecs(getCollisionRec(), target->getCollisionRec()) ){
        target->takeDamage(damagePerSec * deltaTime);
    }

    if (CheckCollisionCircleRec(worldPos, visionRange, target->getCollisionRec())) {}
    
}

void Enemy::SetTarget(Character *character)
{
    target = character;
}

Vector2 Enemy::getScreenPos() {
    // Devuelve la posición del enemigo con respecto al jugador
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::drawVisionRange() 
{
    Color visionColor = {255, 0, 0, 50}; // Color transparente
    Vector2 screenPos = getScreenPos();
    DrawCircleV(screenPos, visionRange, visionColor);
}