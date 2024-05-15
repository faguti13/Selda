#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
#include <iostream> 
#include <fstream>
#include <vector>
#include <sstream>
#include "astar.h"

void drawScene1(const int& windowWidth, const int& windowHeight);
void drawScene2(const int& windowWidth, const int& windowHeight);
std::string actualScene = "scene1";
int pts = 0;

int main()
{
    const int windowWidth{900};
    const int windowHeight{550};

    InitWindow(windowWidth, windowHeight, "Selda");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (actualScene == "scene1"){
            drawScene1(windowWidth, windowHeight);
        }
        if (actualScene == "scene2"){
            drawScene2( windowWidth, windowHeight);
        }
    }
    //UnloadTexture(map);
    CloseWindow();
    return 0;
}

// Carga las texturas
Texture2D heartTexture;
Texture2D emptyHeartTexture;

void DrawHeart(int x, int y) {
    // Define el rectángulo de origen como toda la textura
    Rectangle sourceRec = {0, 0, heartTexture.width, heartTexture.height};

    // Define el rectángulo de destino como el área en la que quieres dibujar la textura
    Rectangle destRec = {x, y, heartTexture.width / 30, heartTexture.height / 30}; // reducido a la vigésima parte

    // Define el punto de origen como el centro de la textura
    Vector2 origin = {heartTexture.width / 50, heartTexture.height / 50}; // ajustado para el nuevo tamaño

    // Dibuja la textura
    DrawTexturePro(heartTexture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void DrawEmptyHeart(int x, int y) {
    // Define el rectángulo de origen como toda la textura
    Rectangle sourceRec = {0, 0, emptyHeartTexture.width, emptyHeartTexture.height};

    // Define el rectángulo de destino como el área en la que quieres dibujar la textura
    Rectangle destRec = {x, y, emptyHeartTexture.width / 30, emptyHeartTexture.height / 30}; // reducido a la vigésima parte

    // Define el punto de origen como el centro de la textura
    Vector2 origin = {emptyHeartTexture.width / 50, emptyHeartTexture.height / 50}; // ajustado para el nuevo tamaño

    // Dibuja la textura
    DrawTexturePro(emptyHeartTexture, sourceRec, destRec, origin, 0.0f, WHITE);
}

// Cargar la matriz del mapa desde un archivo
std::vector<std::vector<int>> LoadMatFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<std::vector<int>> map;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::vector<int> row;
        int value;

        while (ss >> value) {
            row.push_back(value);
        }

        map.push_back(row);
    }

    file.close();
    return map;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawScene1(const int& windowWidth, const int& windowHeight){

    LoadMapFromFile("mapa1.txt");

    // Carga las texturas
    heartTexture = LoadTexture("nature_tileset/heart.png");
    emptyHeartTexture = LoadTexture("nature_tileset/eheart.png");

    Texture2D map = LoadTexture("nature_tileset/piso1.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f}; // escala del mapa

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[15]{
        Prop{Vector2{2816.f, 15.f}, LoadTexture("nature_tileset/ladder.png"), "nature_tileset/ladder.png"},

        // jarrones 
        Prop{Vector2{128.f, 256.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{2176.f, 256.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{3328.f, 256.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{1024.f, 2176.f}, LoadTexture("nature_tileset/vase.png")," nature_tileset/vase.png"},

        // cofres 
        Prop{Vector2{1536.f, 1024.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{384.f, 1280.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{2560.f, 1664.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},

        // bloques arriba der
        Prop{Vector2{896.f, 768.f}, LoadTexture("nature_tileset/6bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1664.f, 768.f}, LoadTexture("nature_tileset/5upCorR.png"), "nature_tileset/5upCorR.png"},

        // bloques abajo izq
        Prop{Vector2{384.f, 1664.f}, LoadTexture("nature_tileset/6bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1152.f, 1664.f}, LoadTexture("nature_tileset/5upCorR.png"), "nature_tileset/5upCorR.png"},

        // bloques arriba izq
        Prop{Vector2{1920.f, 1792.f}, LoadTexture("nature_tileset/7bricks.png"), "nature_tileset/7bricks.png"},
        Prop{Vector2{2688.f, 768.f}, LoadTexture("nature_tileset/8up.png"), "nature_tileset/8up.png"},
        Prop{Vector2{2688.f, 1792.f}, LoadTexture("nature_tileset/brickFrontDownR.png"), "nature_tileset/brickFrontDownR.png"}};

    Texture2D goblin_idle = LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblin_run = LoadTexture("characters/goblin_run_spritesheet.png");

    Texture2D slime_idle = LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slime_run = LoadTexture("characters/slime_run_spritesheet.png");

    Enemy* goblin1 = new Enemy(Vector2{400.f, 800.f}, goblin_idle, goblin_run, 500.f);
    goblin1->patrolPoints = {Vector2{400.f, 800.f}, Vector2{600.f, 800.f}, Vector2{600.f, 600.f}, Vector2{400.f, 600.f}};

    Enemy* goblin2 = new Enemy(Vector2{500.f, 800.f}, goblin_idle, goblin_run, 500.f);
    goblin2->patrolPoints = {Vector2{500.f, 800.f}, Vector2{700.f, 800.f}, Vector2{700.f, 600.f}, Vector2{500.f, 600.f}};

    Enemy* slime1 = new Enemy(Vector2{400.f, 700.f}, slime_idle, slime_run, 500.f);
    slime1->patrolPoints = {Vector2{400.f, 700.f}, Vector2{600.f, 700.f}, Vector2{600.f, 500.f}, Vector2{400.f, 500.f}};

    Enemy* slime2 = new Enemy(Vector2{500.f, 700.f}, slime_idle, slime_run, 500.f);
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


    bool collisionLLadder= false; // Para controlar la detección de choques con la bandera

    while (!WindowShouldClose())
    {   

        BeginDrawing();
        ClearBackground({51, 1, 6, 255});

        // dibuja el mapa
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

    ////debug matriz de colision

        // std::vector<std::vector<int>> mapmat = LoadMatFromFile("mapa1.txt");
        // for (int x = 0; x < mapmat.size(); ++x) {
        //     for (int y = 0; y < mapmat[x].size(); ++y) {
        //         // Dibujar la celda en la posición (x, y) ajustada por la posición del jugador
        //         Vector2 rectPos = Vector2Add(Vector2Scale({(float)y, (float)x}, 8.f), mapPos);
        //         DrawRectangle(rectPos.y, rectPos.x, 8, 8, mapmat[x][y] == 1 ? BLACK : WHITE);
        //     }
        // }
        
    ////debug matriz de colision

        // dibuja los props
        for (auto& prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        knight.tick(GetFrameTime());

        // barreras del mapa
        if (knight.getWorldPos().x < -256.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale + 256||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        for (auto& prop : props) // Iterar sobre props para detectar la colisión con el caballero
        {   
            //CheckCollusionRecs de Raylibs compara los rectangulos 
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                std::string name = prop.getName();
                if (name == "nature_tileset/ladder.png" && not collisionLLadder){
                    collisionLLadder= true;
                    actualScene = "scene2";
                    break;
                }
                knight.undoMovement();   
            }
        }    

        // Colision enemigo con prop
        for (auto& prop : props)
        {
            Vector2 startPoint = {2.0f, 3.0f};
            Vector2 targetPoint = {3.0f, 4.0f};
            t_List* pathList = AStar(startPoint, targetPoint);
            
            LoadMapFromFile("mapa1.txt");

            for (auto enemy : enemies) {
                if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), enemy->getCollisionRec())) {
                    float currentTime = GetTime(); // Obtén el tiempo actual
                    if (currentTime - enemy->lastPathCalculationTime > 1.0f) { // Si ha pasado más de 1 segundo
                        float ex = round((enemy->getWorldPos().x/128));
                        float ey = round((enemy->getWorldPos().y/128));
                        float kx = round((knight.getWorldPos().x/128));
                        float ky = round((knight.getWorldPos().y/128));

                        Vector2 startPoint = {ex, ey};
                        Vector2 targetPoint = {kx, ky};
                        pathList = AStar(startPoint, targetPoint);

                        std::vector<Vector2> pathPoints;
                        if (pathList != nullptr) {
                            for (int i = 0; i < pathList->size; i++) {
                                AStarNode* node = static_cast<AStarNode*>(list_get(pathList, i));
                                if (node != nullptr) {
                                    Vector2 point = {node->x, node->y};
                                    pathPoints.push_back(point);
                                }
                            }
                        }

                        enemy->pathPoints = pathPoints; // Almacena la ruta en el enemigo
                        enemy->currentPathPoint = 0; // Reinicia el punto de ruta actual
                        enemy->lastPathCalculationTime = currentTime; // Actualiza la hora de la última calculación
                    }

                    // Mueve al enemigo a lo largo de la ruta
                    if (!enemy->pathPoints.empty()) {
                        Vector2 direction = Vector2Subtract(enemy->pathPoints[enemy->currentPathPoint], enemy->getWorldPos());
                        if (Vector2Length(direction) < enemy->speed * GetFrameTime()) {
                            enemy->currentPathPoint = (enemy->currentPathPoint + 1) % enemy->pathPoints.size();
                        } else {
                            direction = Vector2Normalize(direction);
                            enemy->setWorldPos(Vector2Add(enemy->getWorldPos(), Vector2Scale(direction, enemy->speed * GetFrameTime())));
                        }
                    }
                }
            }
        }    
        
        //debug collisionrec jugador
        // Rectangle rec = knight.getCollisionRec();
        // DrawRectangleLinesEx(rec, 2, RED); // Dibuja el rectángulo con un grosor de 2 y en color rojo


        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        

            ////debug collisionrec enemigos
            // Rectangle enemyRec = enemy->getCollisionRec();
            // DrawRectangleLinesEx(enemyRec, 2, RED); // Dibuja el rectángulo con un grosor de 2 y en color rojos
        }

        //puntaje
        DrawText("Puntaje: ", windowWidth-250-100, 50, 40, RED);
        DrawText(std::to_string(pts).c_str(), windowWidth -80-100, 50, 40, RED);
        

        if (IsKeyDown(KEY_SPACE))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()) && !CheckCollisionRecs(enemy->getVisionRectangle(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                    pts += 10;
                }
            }
        }

        if (actualScene == "scene2") {
            return;
        }

        std::vector<std::vector<int>> mapmat = LoadMatFromFile("mapa1.txt");

        // Define la posición del minimapa en la pantalla
        Vector2 minimapPos = {windowHeight-160, windowWidth-240};

        for (int x = 0; x < mapmat.size(); ++x) {
            for (int y = 0; y < mapmat[x].size(); ++y) {
                // Dibujar la celda en la posición (x, y) ajustada por la posición del minimapa
                Vector2 rectPos = Vector2Add(Vector2Scale({(float)y, (float)x}, 8.f), minimapPos);
                DrawRectangle(rectPos.y, rectPos.x, 8, 8, mapmat[x][y] == 1 ? Color{ 0, 0, 0, 128 } : Color{ 255, 255, 255, 128 });
            }
        }

        if (!knight.getAlive())
        {
            DrawText("Game Over!", windowWidth / 4, windowHeight / 3, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            int health = knight.getHealth();
            int numHearts = (health + 19) / 20; // Redondea hacia arriba

            for (int i = 0; i < numHearts; i++) {
                // Dibuja un corazón en la posición (windowWidth / 5 + i * 50, 50)
                DrawHeart(windowWidth / 5 + i * 50, 50);
            }

            for (int i = numHearts; i < 5; i++) {
                // Dibuja un corazón vacío en la posición (windowWidth / 5 + i * 50, 50)
                DrawEmptyHeart(windowWidth / 5 + i * 50, 50);
            }
        }

        EndDrawing();
    }

    for (auto enemy : enemies)
    {
        delete enemy;
    }

    UnloadTexture(map);
    return ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawScene2 (const int& windowWidth, const int& windowHeight){

    // Carga las texturas
    heartTexture = LoadTexture("nature_tileset/heart.png");
    emptyHeartTexture = LoadTexture("nature_tileset/eheart.png");

    Texture2D map = LoadTexture("nature_tileset/piso2.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[14]{
        Prop{Vector2{2816.f, 15.f}, LoadTexture("nature_tileset/fire.png"), "nature_tileset/fire.png"},

        // jarrones 
        Prop{Vector2{512.f, 256.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{1408.f, 1280.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{2816.f, 1536.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{2688.f, 1536.f}, LoadTexture("nature_tileset/vase.png")," nature_tileset/vase.png"},

        // cofres 
        Prop{Vector2{128.f, 2176.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{640.f, 1024.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{2560.f, 256.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},

        // bloques del medio
        Prop{Vector2{1152.f, 640.f}, LoadTexture("nature_tileset/11up.png"), "nature_tileset/11up.png"},

        // bloque derecha
        Prop{Vector2{2944.f, 640.f}, LoadTexture("nature_tileset/10up.png"), "nature_tileset/10up.png"},
        
        // bloques izq
        Prop{Vector2{1536.f, 1152.f}, LoadTexture("nature_tileset/2up.png"), "nature_tileset/2up.png"},
        Prop{Vector2{1536.f, 1408.f}, LoadTexture("nature_tileset/brickFrontDownR.png"), "nature_tileset/brickFrontDownR.png"},
        Prop{Vector2{1024.f, 1408.f}, LoadTexture("nature_tileset/4bricks.png"), "nature_tileset/4bricks.png"},
        Prop{Vector2{896.f, 1408.f}, LoadTexture("nature_tileset/4upL.png"), "nature_tileset/4UPL.png"}};

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


    bool collisionLLadder= false; // Para controlar la detección de choques con la bandera

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground({51, 1, 6, 255});

        // dibuja el mapa
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // dibuja los props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        knight.tick(GetFrameTime());

        // barreras del mapa
        if (knight.getWorldPos().x < -256.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale + 256||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        for (auto& prop : props) // Iterar sobre props para detectar la colisión con el caballero
        {   
            //CheckCollusionRecs de Raylibs compara los rectangulos 
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                std::string name = prop.getName();
                if (name == "nature_tileset/ladder.png" && not collisionLLadder){
                    collisionLLadder= true;
                    actualScene = "scene2";
                    break;
                }
                knight.undoMovement();   
            }

            // Colision enemigo con prop
            for (auto enemy : enemies) {
                if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), enemy->getCollisionRec())) {
                    enemy->undoMovement();
                }
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
            //debug vision range
            //enemy->drawVisionRange();
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

        if (actualScene == "scene3") {
            return;
        }

        ////debug collsiionrec enemigos
        // for (auto& enemy : enemies)
        // {
        //     enemy->drawCollisionRec();
        // }

        std::vector<std::vector<int>> mapmat = LoadMatFromFile("mapa2.txt");

        // Define la posición del minimapa en la pantalla
        Vector2 minimapPos = {windowHeight-160, windowWidth-240};

        for (int x = 0; x < mapmat.size(); ++x) {
            for (int y = 0; y < mapmat[x].size(); ++y) {
                // Dibujar la celda en la posición (x, y) ajustada por la posición del minimapa
                Vector2 rectPos = Vector2Add(Vector2Scale({(float)y, (float)x}, 8.f), minimapPos);
                DrawRectangle(rectPos.y, rectPos.x, 8, 8, mapmat[x][y] == 1 ? Color{ 0, 0, 0, 128 } : Color{ 255, 255, 255, 128 });
            }
        }

        if (!knight.getAlive())
        {
            DrawText("Game Over!", windowWidth / 4, windowHeight / 3, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            int health = knight.getHealth();
            int numHearts = (health + 19) / 20; // Redondea hacia arriba

            for (int i = 0; i < numHearts; i++) {
                // Dibuja un corazón en la posición (windowWidth / 5 + i * 50, 50)
                DrawHeart(windowWidth / 5 + i * 50, 50);
            }

            for (int i = numHearts; i < 5; i++) {
                // Dibuja un corazón vacío en la posición (windowWidth / 5 + i * 50, 50)
                DrawEmptyHeart(windowWidth / 5 + i * 50, 50);
            }
        }

        EndDrawing();
    }

    for (auto enemy : enemies)
    {
        delete enemy;
    }
    UnloadTexture(map);
    
    return ;
}