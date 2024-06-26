#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H
#include "raylib.h"
#include <stdio.h>


class BaseCharacter
{
public:
    BaseCharacter();
    Vector2 getWorldPos() { return worldPos; }
    void undoMovement();
    Rectangle getCollisionRec();
    void drawCollisionRec();
    virtual void tick(float deltaTime);
    virtual Vector2 getScreenPos() = 0;
    bool getAlive(){return alive;}
    void setAlive(bool isAlive){ alive = isAlive;}
    float speed{4.f};
    float width{};
    float height{};
    float scale{4.0f};
    void setScale(float newScale) { scale = newScale; }
protected:
    Texture2D texture{};
    Texture2D idle{};
    Texture2D run{};
    Vector2 worldPos{};
    Vector2 worldPosLastFrame{};
    Vector2 screenPosLastFrame{};
    float rightLeft{1};
    float runningTime{};
    int frame{};
    int maxFrames{6};
    float updateTime{1.f / 12.f};
    float patrol_speed{2.f};
    Vector2 velocity{};
    int tipo{};
private:
    bool alive{true};
};

#endif