#include "Character.h"
#include "raymath.h"
#include <iostream>
#include <string>

Character::Character(int winWidth, int winHeight, Texture2D idle_texture, Texture2D run_texture) : windowWidth(winWidth), windowHeight(winHeight)
{
    texture = idle_texture;
    run = run_texture;
    idle = idle_texture;
    width = texture.width / maxFrames;
    height = texture.height;
}

Character::~Character()
{
    UnloadTexture(texture);
    UnloadTexture(idle);
    UnloadTexture(run);
}

Vector2 Character::getScreenPos()
{
    return Vector2{
        static_cast<float>(windowWidth) / 2.0f - scale * (0.5f * width),
        static_cast<float>(windowHeight) / 2.0f - scale * (0.5f * height)};
}



Vector2 Character::getLastScreenPos() {
    return lastScreenPos;
}

void Character::tick(float deltaTime)
{

    if (!getAlive())
        return;

    if (IsKeyDown(KEY_A))
        velocity.x -= 1.0;
    if (IsKeyDown(KEY_D))
        velocity.x += 1.0;
    if (IsKeyDown(KEY_W))
        velocity.y -= 1.0;
    if (IsKeyDown(KEY_S))
        velocity.y += 1.0;

    // debug
    if (debug)
    {
        DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 20, GREEN);
        DrawText("worldPos: ", 0, 30, 20, YELLOW);
        DrawText(std::to_string(worldPos.x).c_str(), 0, 60, 20, YELLOW);
        DrawText(std::to_string(worldPos.y).c_str(), 0, 100, 20, YELLOW);
        DrawText("Direction: ", 0, 120, 20, YELLOW);
        DrawText(std::to_string(Vector2Normalize(velocity).x).c_str(), 0, 150, 20, YELLOW);
        DrawText(std::to_string(Vector2Normalize(velocity).y).c_str(), 0, 180, 20, YELLOW);
        DrawText("Vector2: ", 0, 200, 20, YELLOW);
        DrawText(std::to_string(Vector2Length(velocity)).c_str(), 0, 220, 20, YELLOW);
        DrawText("Health: ", 0, 240, 20, RED);
        DrawText(std::to_string(health).c_str(), 0, 260, 20, RED);
    }

    BaseCharacter::tick(deltaTime);

    Vector2 origin{};
    Vector2 offset{};
    Vector2 shieldOrigin{};
    Vector2 shieldOffset{};
    float rotate{};
    float visionRange = 200.f;

    if (rightLeft > 0.f)
    {
        origin = {0.f, weapon.height * scale};
        offset = {35.f, 55.f};
        weaponCollisionRec = {
            getScreenPos().x + offset.x,
            getScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale};
        IsKeyDown(KEY_SPACE) ? rotate = 35.f : rotate = 0.f;

        shieldOrigin = {0.f, shield.height * scale*3/4};
        shieldOffset = {-15.f, 70.f};
        shieldCollisionRec = {
            getScreenPos().x + shieldOffset.x,
            getScreenPos().y + shieldOffset.y - shield.height * scale*3/4,
            shield.width * scale*3/4,
            shield.height * scale*3/4};
        IsKeyDown(KEY_LEFT_ALT) ? shieldOffset.x = 35.f : shieldOffset.y = 70.f;
        if (IsKeyDown(KEY_LEFT_ALT))
        {
            speed = 2.f;
            Vector2 screenPos = getScreenPos();
            visionRectangle = { screenPos.x + (width*scale), screenPos.y , height*scale, width*scale };
            //debug collision rectangle
            Color visionColor = {255, 0, 0, 50}; // Color rojo completamente opaco
            DrawRectangleRec(visionRectangle, visionColor);
        }
        else
        {
            speed = 4.f;
            visionRectangle = {0.f, 0.f, 0.f, 0.f};
        }
    }
    else
    {
        origin = {weapon.width * scale, weapon.height * scale};
        offset = {25.f, 55.f};
        weaponCollisionRec = {
            getScreenPos().x + offset.x - weapon.width * scale,
            getScreenPos().y + offset.y - weapon.height * scale,
            weapon.width * scale,
            weapon.height * scale};
        IsKeyDown(KEY_SPACE) ? rotate = -35.f : rotate = 0.f;

        shieldOrigin = {shield.width * scale*3/4, shield.height * scale*3/4};
        shieldOffset = {75.f, 70.f};
        shieldCollisionRec = {
            getScreenPos().x + shieldOffset.x - shield.width * scale*3/4,
            getScreenPos().y + shieldOffset.y - shield.height * scale*3/4,
            shield.width * scale*3/4,
            shield.height * scale*3/4};
        IsKeyDown(KEY_LEFT_ALT) ? shieldOffset.x = 25.f : shieldOffset.y = 70.f;
        if (IsKeyDown(KEY_LEFT_ALT))
        {
            speed = 2.f;
            Vector2 screenPos = getScreenPos();
            visionRectangle = { screenPos.x - width*scale + (width*scale/4), screenPos.y, height*scale, width*scale };
            //debug collision rectangle
            Color visionColor = {255, 0, 0, 50}; // Color rojo completamente opaco
            DrawRectangleRec(visionRectangle, visionColor);
        }
        else
        {
            speed = 4.f;
            visionRectangle = {0.f, 0.f, 0.f, 0.f};
        }
    }

    // draw sword
    Rectangle source{0.f, 0.f, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
    Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale};
    DrawTexturePro(weapon, source, dest, origin, rotate, WHITE);

    // draw shield
    Rectangle shieldSource{0.f, 0.f, static_cast<float>(shield.width), static_cast<float>(shield.height)};
    Rectangle shieldDest{getScreenPos().x + shieldOffset.x, getScreenPos().y + shieldOffset.y, shield.width * scale*3/4, shield.height * scale*3/4};
    DrawTexturePro(shield, shieldSource, shieldDest, shieldOrigin, 0.f, WHITE);

}

void Character::takeDamage(float damage)
{
    health -= damage;
    if (health <= 0.f)
    {
        setAlive(false);
    }
}