#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{

    const int windowWidth{800};
    const int windowHeight{800};

    InitWindow(windowWidth, windowHeight, "RPG! Top-Down");
    SetTargetFPS(60);

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}};

    Texture2D goblin_idle = LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblin_run = LoadTexture("characters/goblin_run_spritesheet.png");

    Texture2D slime_idle = LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slime_run = LoadTexture("characters/slime_run_spritesheet.png");

    Enemy* goblin1 = new Enemy(Vector2{400.f, 800.f}, goblin_idle, goblin_run, 400.f);
    goblin1->patrolPoints = {Vector2{400.f, 800.f}, Vector2{600.f, 800.f}, Vector2{600.f, 600.f}, Vector2{400.f, 600.f}};

    Enemy* goblin2 = new Enemy(Vector2{500.f, 800.f}, goblin_idle, goblin_run, 400.f);
    goblin2->patrolPoints = {Vector2{500.f, 800.f}, Vector2{700.f, 800.f}, Vector2{700.f, 600.f}, Vector2{500.f, 600.f}};

    Enemy* slime1 = new Enemy(Vector2{400.f, 700.f}, slime_idle, slime_run, 400.f);
    slime1->patrolPoints = {Vector2{400.f, 700.f}, Vector2{600.f, 700.f}, Vector2{600.f, 500.f}, Vector2{400.f, 500.f}};

    Enemy* slime2 = new Enemy(Vector2{500.f, 700.f}, slime_idle, slime_run, 400.f);
    slime2->patrolPoints = {Vector2{500.f, 700.f}, Vector2{700.f, 700.f}, Vector2{700.f, 500.f}, Vector2{500.f, 500.f}};


    Enemy* enemies[]{
        goblin1,
        goblin2,
        slime1,
        slime2,
    };

    for (auto enemy : enemies)
    {
        enemy->SetTarget(&knight);
    }

    for (Enemy* enemy : enemies) 
    {
        enemy->drawVisionRange();
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // draw map
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // draw props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }


        if (!knight.getAlive())
        {
            DrawText("Game Over!", windowWidth / 4, windowHeight / 3, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), windowWidth / 5, 50.f, 40, RED);
        }

        knight.tick(GetFrameTime());

        // check map bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
            enemy->drawVisionRange();

        }


        if (IsKeyDown(KEY_SPACE))
        {

            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }
    for (auto enemy : enemies)
    {
        delete enemy;
    }
    UnloadTexture(map);
    CloseWindow();
    return 0;
}