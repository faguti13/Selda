#include "Prop.h"
#include "raymath.h"
#include <string>

Prop::Prop(Vector2 pos, Texture2D tex, const std::string fileName) : worldPos(pos), texture(tex), name(fileName) {}

void Prop::Render(Vector2 knightPos)
{
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};
    DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);

        // Dibujar el rectángulo de colisión para depuración
    Rectangle collisionRec = getCollisionRec(knightPos);
    DrawRectangleLinesEx(collisionRec, 2, RED);

}

Rectangle Prop::getCollisionRec(Vector2 knightPos)
{
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};
    return Rectangle{
        screenPos.x,screenPos.y,
        texture.width * scale,
        texture.height * scale,
    };
}

std::string Prop::getName() const {
    return name;
}

