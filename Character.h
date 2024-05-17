#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Rectangle visionRectangle; // Nuevo miembro de datos para el rectángulo de visión
    Character(int winWidth, int winHeight ,Texture2D idle_texture, Texture2D run_texture);
    ~Character();
    virtual void tick(float deltaTime) override;
    virtual Vector2 getScreenPos() override;
    Rectangle getWeaponCollisionRec() { return weaponCollisionRec; }
    float getHealth() const { return health; }
    void takeDamage(float damage);
    Rectangle getVisionRectangle() { return visionRectangle; }
    virtual Vector2 getLastPos() { return worldPosLastFrame; }

    
private:
    const bool debug = false;
    int windowWidth{};
    int windowHeight{};
    Texture2D weapon{LoadTexture("characters/weapon_sword.png")};
    Rectangle weaponCollisionRec{};
    Texture2D shield{LoadTexture("characters/shield.png")};
    Rectangle shieldCollisionRec{};
    float health{100.f};
};

#endif