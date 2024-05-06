#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{

    const int windowWidth{900};
    const int windowHeight{550};

    InitWindow(windowWidth, windowHeight, "RPG! Top-Down");
    SetTargetFPS(60);

    Texture2D map = LoadTexture("nature_tileset/piso11.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f}; //map scale 

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[17]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{2816.f, 15.f}, LoadTexture("nature_tileset/ladder.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")},

        // jarrones 
        Prop{Vector2{128.f, 256.f}, LoadTexture("nature_tileset/vase.png")},
        Prop{Vector2{2176.f, 256.f}, LoadTexture("nature_tileset/vase.png")},
        Prop{Vector2{3328.f, 256.f}, LoadTexture("nature_tileset/vase.png")},
        Prop{Vector2{1024.f, 2176.f}, LoadTexture("nature_tileset/vase.png")},

        // cofres 
        Prop{Vector2{1536.f, 1024.f}, LoadTexture("nature_tileset/chest1.png")},
        Prop{Vector2{384.f, 1280.f}, LoadTexture("nature_tileset/chest1.png")},
        Prop{Vector2{2560.f, 1664.f}, LoadTexture("nature_tileset/chest1.png")},

        // bloques arriba der
        Prop{Vector2{896.f, 768.f}, LoadTexture("nature_tileset/6bricks.png")},
        Prop{Vector2{1664.f, 768.f}, LoadTexture("nature_tileset/5upCorR.png")},

        // bloques abajo izq
        Prop{Vector2{384.f, 1664.f}, LoadTexture("nature_tileset/6bricks.png")},
        Prop{Vector2{1152.f, 1664.f}, LoadTexture("nature_tileset/5upCorR.png")},

        // bloques arriba izq
        Prop{Vector2{1920.f, 1792.f}, LoadTexture("nature_tileset/7bricks.png")},
        Prop{Vector2{2688.f, 768.f}, LoadTexture("nature_tileset/8up.png")},
        Prop{Vector2{2688.f, 1792.f}, LoadTexture("nature_tileset/brickFrontDownR.png")}};

    Texture2D goblin_idle = LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblin_run = LoadTexture("characters/goblin_run_spritesheet.png");

    Texture2D slime_idle = LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slime_run = LoadTexture("characters/slime_run_spritesheet.png");

    Enemy goblin1(Vector2{400.f, 800.f}, goblin_idle, goblin_run, 300.f);
    Enemy goblin2(Vector2{500.f, 800.f}, goblin_idle, goblin_run, 300.f);

    Enemy slime1(Vector2{400.f, 700.f}, slime_idle, slime_run, 200.f);
    Enemy slime2(Vector2{500.f, 700.f}, slime_idle, slime_run, 200.f);
    Enemy *enemies[]{
        &goblin1,
        &goblin2,
        &slime1,
        &slime2,
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
        ClearBackground({51, 1, 6, 255});

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
        if (knight.getWorldPos().x < -256.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale + 256||
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
    UnloadTexture(map);
    CloseWindow();
    return 0;
}