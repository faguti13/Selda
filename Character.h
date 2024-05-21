#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
Vector2 lastScreenPos;

public:
    Rectangle safeZone;
    Rectangle visionRectangle; // Nuevo miembro de datos para el rectángulo de visión
    Rectangle getVisionRectangle() { return visionRectangle; }
    Rectangle getWeaponCollisionRec() { return weaponCollisionRec; }
    Vector2 getLastScreenPos();    
    Character(int winWidth, int winHeight ,Texture2D idle_texture, Texture2D run_texture);
    ~Character();
    virtual void tick(float deltaTime) override;
    virtual Vector2 getScreenPos() override;        
    virtual Vector2 getLastPos() { return worldPosLastFrame; }
    void updateLastScreenPos() {
        lastScreenPos = getScreenPos();
    }
    void setSafeZone(Rectangle safeZone) {
        this->safeZone = safeZone;
    }
    float getHealth() const { return health; }
    void takeDamage(float damage);
    void receiveLife(float life);
    void updateLife(float life);
private:
    const bool debug = false;
    int windowWidth{};
    int windowHeight{};
    Texture2D weapon{LoadTexture("characters/weapon_sword.png")};
    Rectangle weaponCollisionRec{};
    Texture2D shield{LoadTexture("characters/shield.png")};
    Rectangle shieldCollisionRec{};
    float health{5.f};
};

#endif