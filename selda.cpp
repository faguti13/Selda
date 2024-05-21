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
void drawScene3(const int& windowWidth, const int& windowHeight);
void drawScene4(const int& windowWidth, const int& windowHeight);
void drawScene5(const int& windowWidth, const int& windowHeight);

std::string actualScene = "scene1";
int pts = 0;
int remainignLife = 5;
int foundChests = 0;
Texture2D heartTexture;
Texture2D emptyHeartTexture;
Texture2D chestTexture;

// Métodos para mostrar las vidas y tesoros encontrados en pantalla
void DrawHeart(float x, float y) {
    Rectangle sourceRec = {0, 0, static_cast<float>(heartTexture.width), static_cast<float>(heartTexture.height)};
    Rectangle destRec = {x, y, static_cast<float>(heartTexture.width) / 30.0f, static_cast<float>(heartTexture.height) / 30.0f}; // reducido a la trigésima parte
    Vector2 origin = {static_cast<float>(heartTexture.width) / 50.0f, static_cast<float>(heartTexture.height) / 50.0f}; // ajustado para el nuevo tamaño
    DrawTexturePro(heartTexture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void DrawEmptyHeart(float x, float y) {
    Rectangle sourceRec = {0, 0, static_cast<float>(emptyHeartTexture.width), static_cast<float>(emptyHeartTexture.height)};
    Rectangle destRec = {x, y, static_cast<float>(emptyHeartTexture.width) / 30.0f, static_cast<float>(emptyHeartTexture.height) / 30.0f}; // reducido a la trigésima parte
    Vector2 origin = {static_cast<float>(emptyHeartTexture.width) / 50.0f, static_cast<float>(emptyHeartTexture.height) / 50.0f}; // ajustado para el nuevo tamaño
    DrawTexturePro(emptyHeartTexture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void DrawChest(float x, float y) {
    Rectangle sourceRec = {0, 0, static_cast<float>(chestTexture.width), static_cast<float>(chestTexture.height)};
    Rectangle destRec = {x, y, static_cast<float>(chestTexture.width) / 1.0f, static_cast<float>(chestTexture.height) / 1.0f}; // reducido a la trigésima parte
    Vector2 origin = {static_cast<float>(chestTexture.width) / 10.0f, static_cast<float>(chestTexture.height) / 10.0f}; // ajustado para el nuevo tamaño
    DrawTexturePro(chestTexture, sourceRec, destRec, origin, 0.0f, WHITE);
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

int main()
{
    const int windowWidth{900};
    const int windowHeight{550};

    InitWindow(windowWidth, windowHeight, "RPG! Top-Down");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (actualScene == "scene1"){
            drawScene1(windowWidth, windowHeight);
        }
        if (actualScene == "scene2"){
            drawScene2( windowWidth, windowHeight);
        }
        if (actualScene == "scene3"){
            drawScene3(windowWidth, windowHeight);
        }
        if (actualScene == "scene4"){
            drawScene4(windowWidth, windowHeight);
        }
        if (actualScene == "scene5"){
            drawScene5(windowWidth, windowHeight);
        }
    }
    //UnloadTexture(map);
    CloseWindow();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawScene1(const int& windowWidth, const int& windowHeight){

    
    Texture2D map = LoadTexture("nature_tileset/piso1.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f}; // escala del mapa
    LoadMapFromFile("mapa1.txt");

    // Carga las texturas
    heartTexture = LoadTexture("nature_tileset/heart.png");
    emptyHeartTexture = LoadTexture("nature_tileset/eheart.png");
    chestTexture = LoadTexture("nature_tileset/chest2.png");

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[17]{
        Prop{Vector2{2816.f, 15.f}, LoadTexture("nature_tileset/ladder.png"), "nature_tileset/ladder.png"},

        // trampas
        Prop{Vector2{1280.f, 2176.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"},
        Prop{Vector2{1024.f, 256.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"},

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


    // Carga las imágenes
    Image goblin_idle_image = LoadImage("characters/goblin_idle_spritesheet.png");
    Image goblin_run_image = LoadImage("characters/goblin_run_spritesheet.png");
    Image red_goblin_idle_image = LoadImage("characters/goblin_idle_spritesheet.png");
    Image red_goblin_run_image = LoadImage("characters/goblin_run_spritesheet.png");
    Image spectral_eye = LoadImage("characters/spectral_eye.png");


    // Aplica el tinte a las imágenes
    ImageColorTint(&red_goblin_idle_image, RED);
    ImageColorTint(&red_goblin_run_image, RED);

    // Convierte las imágenes a texturas
    Texture2D goblin_idle = LoadTextureFromImage(goblin_idle_image);
    Texture2D goblin_run = LoadTextureFromImage(goblin_run_image);
    Texture2D red_goblin_idle = LoadTextureFromImage(red_goblin_idle_image);
    Texture2D red_goblin_run = LoadTextureFromImage(red_goblin_run_image);
    Texture2D eye = LoadTextureFromImage(spectral_eye);

    Enemy* goblin1 = new Enemy(Vector2{600.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin1->patrolPoints = {Vector2{600.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin2 = new Enemy(Vector2{1200.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin2->patrolPoints = {Vector2{1200.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin3 = new Enemy(Vector2{1800.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin3->patrolPoints = {Vector2{1800.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* enemies[]{
        goblin1,
        goblin2,
        goblin3,
    };

    for (auto enemy : enemies)
    {
        enemy->SetTarget(&knight);
    }

    bool collisionLLadder= false; // Para controlar la detección de choques con la bandera
    Vector2 startPoint = {2.0f, 3.0f};
    Vector2 targetPoint = {3.0f, 4.0f};
    t_List* pathList = AStar(startPoint, targetPoint);
    
    while (!WindowShouldClose())
    {   
        BeginDrawing();
        ClearBackground({51, 1, 6, 255});

        // dibuja el mapa
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // dibuja los props
        for (auto& prop : props)
        {
            prop.Render(knight.getWorldPos(), GetFrameTime());
        }

        if (!knight.getAlive())
        {
            DrawText("Game Over!", windowWidth / 4, windowHeight / 3, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {

            int numHearts = knight.getHealth();

            for (int i = 0; i < numHearts; i++) {
                // Dibuja un corazón en la posición (windowWidth / 5 + i * 50, 50)
                DrawHeart(windowWidth / 5 + i * 50 - 115, 50);
            }

            for (int i = numHearts; i < 5; i++) {
                // Dibuja un corazón vacío en la posición (windowWidth / 5 + i * 50, 50)
                DrawEmptyHeart(windowWidth / 5 + i * 50 - 115, 50);
            }
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

        //datos pantalla
        DrawText("Level 1", windowWidth / 5 + 6 * 50 - 160, 30, 32, BLACK);
        DrawText("Score: ", windowWidth-250-100, 30, 32, BLACK);
        DrawText(std::to_string(pts).c_str(), windowWidth -80-150, 30, 32, RED);
        DrawChest(windowWidth-150, 30);
        DrawText(" : ", windowWidth-130, 30, 32, BLACK);
        DrawText(std::to_string(foundChests).c_str(), windowWidth -105, 30, 32, RED);

        if (actualScene == "scene2") {
            return;
        }

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

        //Zona segura
        Vector2 szworldpos = {128*4,128*15};
        Vector2 szScreenPos{Vector2Subtract(szworldpos, knight.getWorldPos())};
        Rectangle safeZone = {szScreenPos.x, szScreenPos.y, 128*5, 128*3};
        DrawRectangleRec(safeZone, Color{0, 255, 0, 55});
        knight.setSafeZone(safeZone);

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

    Texture2D map = LoadTexture("nature_tileset/piso2.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    heartTexture = LoadTexture("nature_tileset/heart.png");
    emptyHeartTexture = LoadTexture("nature_tileset/eheart.png");
    chestTexture = LoadTexture("nature_tileset/chest2.png");

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[23]{

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
        Prop{Vector2{896.f, 1408.f}, LoadTexture("nature_tileset/4upL.png"), "nature_tileset/4UPL.png"},
        
        Prop{Vector2{1152.f, 1423.f}, LoadTexture("nature_tileset/ladder.png"), "nature_tileset/ladder.png"},
        
        //Antorchas
        Prop{Vector2{2816.f, 143.f}, LoadTexture("nature_tileset/fire.png"), "nature_tileset/fire.png"},
        Prop{Vector2{768.f, 143.f}, LoadTexture("nature_tileset/fire.png"), "nature_tileset/fire.png"},
        Prop{Vector2{1408.f, 1536.f}, LoadTexture("nature_tileset/fire.png"), "nature_tileset/fire.png"},
        Prop{Vector2{1792.f, 640.f}, LoadTexture("nature_tileset/fire.png"), "nature_tileset/fire.png"},
        Prop{Vector2{2944.f, 1152.f}, LoadTexture("nature_tileset/fire.png"), "nature_tileset/fire.png"},
        Prop{Vector2{2048.f, 1920.f}, LoadTexture("nature_tileset/fire.png"), "nature_tileset/fire.png"},
        Prop{Vector2{0.f, 1296.f}, LoadTexture("nature_tileset/fire.png"), "nature_tileset/fire.png"},
        
        // trampas 
        Prop{Vector2{1792.f, 768.f}, LoadTexture("nature_tileset/spikes2.png"), "nature_tileset/spikes2.png"},
        Prop{Vector2{2048.f, 1536.f}, LoadTexture("nature_tileset/spikes.png"),"nature_tileset/spikes.png"}};

    Texture2D goblin_idle = LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblin_run = LoadTexture("characters/goblin_run_spritesheet.png");

    Texture2D slime_idle = LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slime_run = LoadTexture("characters/slime_run_spritesheet.png");

    Enemy* goblin1 = new Enemy(Vector2{600.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin1->patrolPoints = {Vector2{600.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin2 = new Enemy(Vector2{1200.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin2->patrolPoints = {Vector2{1200.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin3 = new Enemy(Vector2{1800.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin3->patrolPoints = {Vector2{1800.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* enemies[]{
        goblin1,
        goblin2,
        goblin3,
    };

    for (auto enemy : enemies)
    {
        enemy->SetTarget(&knight);
    }

    bool collisionLLadder= false; // Para controlar la detección de choques con la bandera

    knight.updateLife(remainignLife);

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground({42, 43, 48, 255});

        // dibuja el mapa
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // dibuja los props
        for (auto& prop : props)
        {
            prop.Render(knight.getWorldPos(), GetFrameTime());
        }


        if (!knight.getAlive())
        {
            DrawText("Game Over!", windowWidth / 4, windowHeight / 3, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            int numHearts = knight.getHealth();

            for (int i = 0; i < numHearts; i++) {
                DrawHeart(windowWidth / 5 + i * 50 - 115, 50);
            }

            for (int i = numHearts; i < 5; i++) {
                DrawEmptyHeart(windowWidth / 5 + i * 50 - 115, 50);
            }
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
        {   //CheckCollusionRecs de Raylibs compara los rectangulos 
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                std::string name = prop.getName();
                if (name == "nature_tileset/ladder.png" && not collisionLLadder){
                    collisionLLadder= true;
                    int numHearts = knight.getHealth();
                    remainignLife = numHearts;
                    actualScene = "scene3";
                    break;
                }
                if ((name == "nature_tileset/spikes.png" || name == "nature_tileset/spikes2.png" || name == "nature_tileset/poison.png" )  
                && prop.getState(GetFrameTime())){
                    int numHearts = knight.getHealth();
                    remainignLife = numHearts - 1;
                    knight.updateLife(remainignLife);
                    break;
                }
                else{
                    knight.undoMovement();
                }
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }


        if (IsKeyDown(KEY_SPACE))
        {

            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    if(enemy->getAlive()){
                        pts += 10;
                    }
                    enemy->setAlive(false);
                }
            }
        }

        //datos pantalla
        DrawText("Level 2", windowWidth / 5 + 6 * 50 - 160, 30, 32, BLACK);
        DrawText("Score: ", windowWidth-250-100, 30, 32, BLACK);
        DrawText(std::to_string(pts).c_str(), windowWidth -80-150, 30, 32, RED);
        DrawChest(windowWidth-150, 30);
        DrawText(" : ", windowWidth-130, 30, 32, BLACK);
        DrawText(std::to_string(foundChests).c_str(), windowWidth -105, 30, 32, RED);

        if (actualScene == "scene3") {
            return;
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

void drawScene3(const int& windowWidth, const int& windowHeight){

    Texture2D map = LoadTexture("nature_tileset/piso3.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f}; // escala del mapa

    heartTexture = LoadTexture("nature_tileset/heart.png");
    emptyHeartTexture = LoadTexture("nature_tileset/eheart.png");
    chestTexture = LoadTexture("nature_tileset/chest2.png");

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[22]{
        
        // jarrones 
        Prop{Vector2{1152.f, 256.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{128.f, 2176.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{1152.f, 1792.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{2944.f, 1024.f}, LoadTexture("nature_tileset/vase.png")," nature_tileset/vase.png"},

        // cofres 
        Prop{Vector2{640.f, 512.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{1792.f, 1536.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{3584.f, 256.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},

        // bloques izq
        Prop{Vector2{2816.f, 768.f}, LoadTexture("nature_tileset/4bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2816.f, 1664.f}, LoadTexture("nature_tileset/4bricks.png"), "nature_tileset/5upCorR.png"},

        // bloque der
        Prop{Vector2{512.f, 640.f}, LoadTexture("nature_tileset/3bricks.png"), "nature_tileset/6bricks.png"},
        
        // paredes arriba
        Prop{Vector2{1280.f, 0.f}, LoadTexture("nature_tileset/5upCorR.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2432.f, 0.f}, LoadTexture("nature_tileset/5upCorL.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1408.f, 640.f}, LoadTexture("nature_tileset/8bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1280.f, 640.f}, LoadTexture("nature_tileset/BrickFrontDown.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2432.f, 640.f}, LoadTexture("nature_tileset/brickFrontDownR.png"), "nature_tileset/6bricks.png"},
        
        //paredes abajo 
        Prop{Vector2{1280.f, 1664.f}, LoadTexture("nature_tileset/5upCorL.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2432.f, 1664.f}, LoadTexture("nature_tileset/5upCorR.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1408.f, 1664.f}, LoadTexture("nature_tileset/8bricks.png"), "nature_tileset/6bricks.png"},

        // escalera
        Prop{Vector2{3072.f, 1674.f}, LoadTexture("nature_tileset/ladder.png"), "nature_tileset/ladder.png"},
        
        // trampas
        Prop{Vector2{512.f, 1664.f}, LoadTexture("nature_tileset/spikes2.png"), "nature_tileset/spikes2.png"},
        Prop{Vector2{2560.f, 2176.f}, LoadTexture("nature_tileset/poison.png"), "nature_tileset/poison.png"},
        Prop{Vector2{3456.f, 1280.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"}
        };

    Texture2D goblin_idle = LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblin_run = LoadTexture("characters/goblin_run_spritesheet.png");

    Texture2D slime_idle = LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slime_run = LoadTexture("characters/slime_run_spritesheet.png");

    Enemy* goblin1 = new Enemy(Vector2{600.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin1->patrolPoints = {Vector2{600.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin2 = new Enemy(Vector2{1200.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin2->patrolPoints = {Vector2{1200.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin3 = new Enemy(Vector2{1800.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin3->patrolPoints = {Vector2{1800.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* enemies[]{
        goblin1,
        goblin2,
        goblin3,
    };

    for (auto enemy : enemies)
    {
        enemy->SetTarget(&knight);
    }

    bool collisionLLadder= false; // Para controlar la detección de choques con la bandera

    knight.updateLife(remainignLife);

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground({42, 43, 48, 255});

        // dibuja el mapa
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // dibuja los props
        for (auto& prop : props)
        {
            prop.Render(knight.getWorldPos(), GetFrameTime());
        }


        if (!knight.getAlive())
        {
            DrawText("Game Over!", windowWidth / 4, windowHeight / 3, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            int numHearts = knight.getHealth();

            for (int i = 0; i < numHearts; i++) {
                DrawHeart(windowWidth / 5 + i * 50 - 115, 50);
            }

            for (int i = numHearts; i < 5; i++) {
                DrawEmptyHeart(windowWidth / 5 + i * 50 - 115, 50);
            }
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
        {   //CheckCollusionRecs de Raylibs compara los rectangulos 
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                std::string name = prop.getName();
                if (name == "nature_tileset/ladder.png" && not collisionLLadder){
                    collisionLLadder= true;
                    int numHearts = knight.getHealth();
                    remainignLife = numHearts;
                    actualScene = "scene4";
                    break;
                }

                if ((name == "nature_tileset/spikes.png" || name == "nature_tileset/spikes2.png" || name == "nature_tileset/poison.png" )  
                && prop.getState(GetFrameTime())){
                    int numHearts = knight.getHealth();
                    remainignLife = numHearts - 1;
                    knight.updateLife(remainignLife);
                    break;
                }
                else{
                    knight.undoMovement();
                }
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }


        if (IsKeyDown(KEY_SPACE))
        {

            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    if(enemy->getAlive()){
                        pts += 10;
                    }
                    enemy->setAlive(false);
                }
            }
        }

        //datos pantalla
        DrawText("Level 3", windowWidth / 5 + 6 * 50 - 160, 30, 32, BLACK);
        DrawText("Score: ", windowWidth-250-100, 30, 32, BLACK);
        DrawText(std::to_string(pts).c_str(), windowWidth -80-150, 30, 32, RED);
        DrawChest(windowWidth-150, 30);
        DrawText(" : ", windowWidth-130, 30, 32, BLACK);
        DrawText(std::to_string(foundChests).c_str(), windowWidth -105, 30, 32, RED);

        if (actualScene == "scene4") {
            return;
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

void drawScene4(const int& windowWidth, const int& windowHeight){

    Texture2D map = LoadTexture("nature_tileset/piso4.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f}; // escala del mapa

    heartTexture = LoadTexture("nature_tileset/heart.png");
    emptyHeartTexture = LoadTexture("nature_tileset/eheart.png");
    chestTexture = LoadTexture("nature_tileset/chest2.png");

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[24]{
        
        // jarrones 
        Prop{Vector2{2819.f, 2048.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{128.f, 768.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
       
        // cofres 
        Prop{Vector2{3328.f, 640.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{3584.f, 1536.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{128.f, 2176.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},

        // paredes abajo 
        Prop{Vector2{1664.f, 2048.f}, LoadTexture("nature_tileset/8bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1536.f, 2048.f}, LoadTexture("nature_tileset/2upL.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2688.f, 2048.f}, LoadTexture("nature_tileset/2upR.png"), "nature_tileset/5upCorR.png"},
        
        // paredes arriba
        Prop{Vector2{128.f, 896.f}, LoadTexture("nature_tileset/8bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1152.f, 896.f}, LoadTexture("nature_tileset/8bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2176.f, 896.f}, LoadTexture("nature_tileset/4bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2688.f, 896.f}, LoadTexture("nature_tileset/4upR.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2688.f, 1408.f}, LoadTexture("nature_tileset/brickFrontDownR.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1664.f, 1408.f}, LoadTexture("nature_tileset/8bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1536.f, 1408.f}, LoadTexture("nature_tileset/BrickFrontDown.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{128.f, 1280.f}, LoadTexture("nature_tileset/8bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1152.f, 1280.f}, LoadTexture("nature_tileset/3bricks.png"), "nature_tileset/6bricks.png"},

        // escalera
        Prop{Vector2{512.f, 1295.f}, LoadTexture("nature_tileset/ladder.png"), "nature_tileset/ladder.png"},
        
        //trampas
        Prop{Vector2{512.f, 512.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"},
        Prop{Vector2{1792.f, 384.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"},
        Prop{Vector2{3584.f, 256.f}, LoadTexture("nature_tileset/poison.png"), "nature_tileset/poison.png"},
        Prop{Vector2{2304.f, 640.f}, LoadTexture("nature_tileset/spikes2.png"), "nature_tileset/spikes2.png"},
        Prop{Vector2{3200.f, 1664.f}, LoadTexture("nature_tileset/spikes2.png"), "nature_tileset/spikes2.png"},
        Prop{Vector2{896.f, 1792.f}, LoadTexture("nature_tileset/poison.png"), "nature_tileset/poison.png"}
        };

    Texture2D goblin_idle = LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblin_run = LoadTexture("characters/goblin_run_spritesheet.png");

    Texture2D slime_idle = LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slime_run = LoadTexture("characters/slime_run_spritesheet.png");

    Enemy* goblin1 = new Enemy(Vector2{600.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin1->patrolPoints = {Vector2{600.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin2 = new Enemy(Vector2{1200.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin2->patrolPoints = {Vector2{1200.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin3 = new Enemy(Vector2{1800.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin3->patrolPoints = {Vector2{1800.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* enemies[]{
        goblin1,
        goblin2,
        goblin3,
    };

    for (auto enemy : enemies)
    {
        enemy->SetTarget(&knight);
    }


    bool collisionLLadder= false; // Para controlar la detección de choques con la bandera

    knight.updateLife(remainignLife);

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground({42, 43, 48, 255});

        // dibuja el mapa
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // dibuja los props
        for (auto& prop : props)
        {
            prop.Render(knight.getWorldPos(), GetFrameTime());
        }


        if (!knight.getAlive())
        {
            DrawText("Game Over!", windowWidth / 4, windowHeight / 3, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            int numHearts = knight.getHealth();

            for (int i = 0; i < numHearts; i++) {
                DrawHeart(windowWidth / 5 + i * 50 - 115, 50);
            }

            for (int i = numHearts; i < 5; i++) {
                DrawEmptyHeart(windowWidth / 5 + i * 50 - 115, 50);
            }
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
        {   //CheckCollusionRecs de Raylibs compara los rectangulos 
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                std::string name = prop.getName();
                if (name == "nature_tileset/ladder.png" && not collisionLLadder){
                    collisionLLadder= true;
                    int numHearts = knight.getHealth();
                    remainignLife = numHearts;
                    actualScene = "scene5";
                    break;
                }

                if ((name == "nature_tileset/spikes.png" || name == "nature_tileset/spikes2.png" || name == "nature_tileset/poison.png" )  
                && prop.getState(GetFrameTime())){
                    int numHearts = knight.getHealth();
                    remainignLife = numHearts - 1;
                    knight.updateLife(remainignLife);
                    break;
                }
                else{
                    knight.undoMovement();
                }
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }


        if (IsKeyDown(KEY_SPACE))
        {

            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {

                    if(enemy->getAlive()){
                        pts += 10;
                    }
                    enemy->setAlive(false);
                }
            }
        }

        //datos pantalla
        DrawText("Level 4", windowWidth / 5 + 6 * 50 - 160, 30, 32, BLACK);
        DrawText("Score: ", windowWidth-250-100, 30, 32, BLACK);
        DrawText(std::to_string(pts).c_str(), windowWidth -80-150, 30, 32, RED);
        DrawChest(windowWidth-150, 30);
        DrawText(" : ", windowWidth-130, 30, 32, BLACK);
        DrawText(std::to_string(foundChests).c_str(), windowWidth -105, 30, 32, RED);


        if (actualScene == "scene5") {
            return;
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

void drawScene5(const int& windowWidth, const int& windowHeight){

    Texture2D map = LoadTexture("nature_tileset/piso5.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f}; // escala del mapa

    heartTexture = LoadTexture("nature_tileset/heart.png");
    emptyHeartTexture = LoadTexture("nature_tileset/eheart.png");
    chestTexture = LoadTexture("nature_tileset/chest2.png");

    Character knight{windowWidth, windowHeight, LoadTexture("characters/knight_idle_spritesheet.png"), LoadTexture("characters/knight_run_spritesheet.png")};

    Prop props[26]{
        
        // jarrones 
        Prop{Vector2{3584.f, 768.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
        Prop{Vector2{128.f, 1280.f}, LoadTexture("nature_tileset/vase.png"), "nature_tileset/vase.png"},
       
        // cofres 
        Prop{Vector2{1024.f, 1664.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{2944.f, 256.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},
        Prop{Vector2{3584.f, 2176.f}, LoadTexture("nature_tileset/chest1.png"), "nature_tileset/chest1.png"},

        // paredes abajo 
        Prop{Vector2{3328.f, 896.f}, LoadTexture("nature_tileset/3bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{3328.f, 1280.f}, LoadTexture("nature_tileset/3bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{3200.f, 896.f}, LoadTexture("nature_tileset/3upL.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{3200.f, 1280.f}, LoadTexture("nature_tileset/BrickFrontDown.png"), "nature_tileset/5upCorR.png"},
        
        // paredes arriba
        Prop{Vector2{1152.f, 512.f}, LoadTexture("nature_tileset/2bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1024.f, 512.f}, LoadTexture("nature_tileset/BrickFrontDown.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1408.f, 512.f}, LoadTexture("nature_tileset/brickFrontDownR.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1024.f, 128.f}, LoadTexture("nature_tileset/3up.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1408.f, 128.f}, LoadTexture("nature_tileset/3up.png"), "nature_tileset/6bricks.png"},

        //bloques de abajo
        Prop{Vector2{1280.f, 1152.f}, LoadTexture("nature_tileset/8bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{2304.f, 1152.f}, LoadTexture("nature_tileset/2bricks.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1152.f, 1152.f}, LoadTexture("nature_tileset/5upCorL.png"), "nature_tileset/6bricks.png"},
        Prop{Vector2{1152.f, 1792.f}, LoadTexture("nature_tileset/4up.png"), "nature_tileset/6bricks.png"},

        // escalera
        Prop{Vector2{1536.f, 1167.f}, LoadTexture("nature_tileset/ladder.png"), "nature_tileset/ladder.png"},

        // trampas
        Prop{Vector2{384.f, 640.f}, LoadTexture("nature_tileset/spikes2.png"), "nature_tileset/spikes2.png"},
        Prop{Vector2{640.f, 1152.f}, LoadTexture("nature_tileset/spikes2.png"), "nature_tileset/spikes2.png"},
        Prop{Vector2{2688.f, 640.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"},
        Prop{Vector2{2176.f, 1408.f}, LoadTexture("nature_tileset/poison.png"), "nature_tileset/poison.png"},
        Prop{Vector2{2176.f, 1792.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"},
        Prop{Vector2{2304.f, 1792.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"},
        Prop{Vector2{2432.f, 1792.f}, LoadTexture("nature_tileset/spikes.png"), "nature_tileset/spikes.png"}
        
        };

    Texture2D goblin_idle = LoadTexture("characters/goblin_idle_spritesheet.png");
    Texture2D goblin_run = LoadTexture("characters/goblin_run_spritesheet.png");

    Texture2D slime_idle = LoadTexture("characters/slime_idle_spritesheet.png");
    Texture2D slime_run = LoadTexture("characters/slime_run_spritesheet.png");

    Enemy* goblin1 = new Enemy(Vector2{600.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin1->patrolPoints = {Vector2{600.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin2 = new Enemy(Vector2{1200.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin2->patrolPoints = {Vector2{1200.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* goblin3 = new Enemy(Vector2{1800.f, 400.f}, goblin_idle, goblin_run, 1000.f,0);
    goblin3->patrolPoints = {Vector2{1800.f, 400.f}, Vector2{3000.f, 400.f}, Vector2{3000.f, 2200.f}, Vector2{1700.f, 2200.f},Vector2{400.f, 400.f}};

    Enemy* enemies[]{
        goblin1,
        goblin2,
        goblin3,
    };

    for (auto enemy : enemies)
    {
        enemy->SetTarget(&knight);
    }


    bool collisionLLadder= false; // Para controlar la detección de choques con la bandera

    knight.updateLife(remainignLife);

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground({42, 43, 48, 255});

        // dibuja el mapa
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // dibuja los props
        for (auto& prop : props)
        {
            prop.Render(knight.getWorldPos(), GetFrameTime());
        }


        if (!knight.getAlive())
        {
            DrawText("Game Over!", windowWidth / 4, windowHeight / 3, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            int numHearts = knight.getHealth();

            for (int i = 0; i < numHearts; i++) {
                DrawHeart(windowWidth / 5 + i * 50 - 115, 50);
            }

            for (int i = numHearts; i < 5; i++) {
                DrawEmptyHeart(windowWidth / 5 + i * 50 - 115, 50);
            }
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
        {   //CheckCollusionRecs de Raylibs compara los rectangulos 
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                std::string name = prop.getName();
                if (name == "nature_tileset/ladder.png" && not collisionLLadder){
                    collisionLLadder= true;
                    int numHearts = knight.getHealth();
                    remainignLife = numHearts;
                    //actualScene = "win";
                    break;
                }

                if ((name == "nature_tileset/spikes.png" || name == "nature_tileset/spikes2.png" || name == "nature_tileset/poison.png" )  
                && prop.getState(GetFrameTime())){
                    int numHearts = knight.getHealth();
                    remainignLife = numHearts - 1;
                    knight.updateLife(remainignLife);
                    break;
                }
                else{
                    knight.undoMovement();
                }
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }


        if (IsKeyDown(KEY_SPACE))
        {

            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {

                    if(enemy->getAlive()){
                        pts += 10;
                    }
                    enemy->setAlive(false);
                }
            }
        }

        //datos pantalla
        DrawText("Level 5", windowWidth / 5 + 6 * 50 - 160, 30, 32, BLACK);
        DrawText("Score: ", windowWidth-250-100, 30, 32, BLACK);
        DrawText(std::to_string(pts).c_str(), windowWidth -80-150, 30, 32, RED);
        DrawChest(windowWidth-150, 30);
        DrawText(" : ", windowWidth-130, 30, 32, BLACK);
        DrawText(std::to_string(foundChests).c_str(), windowWidth -105, 30, 32, RED);


        if (actualScene == "win") {
            return;
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