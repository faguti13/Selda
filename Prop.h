#ifndef PROP_H
#define PROP_H

#include "raylib.h"
#include <iostream> 
#include <string>

class Prop {
public:
    Prop(Vector2 pos, Texture2D tex, const std::string fileName);
    void Render(Vector2 knightPos, float deltaTime);
    Rectangle getCollisionRec(Vector2 knightPos);
    std::string getName() const;
    bool getState(float deltaTime);
private:
    Vector2 worldPos{};
    Texture2D texture{};
    std::string  name; 
    float scale{4.f};
 
   // Variables para  la animación
    float runningTime = 0.0f;
    int frame = 0;
    const int maxFrames = 4; // Número de cuadros en la animación
    float updateTime =  1.0f / 12.0f; // Tiempo entre cada cuadros de animación 

    //Variables para el choque (trampas)
    float collisionTime = 0.0f;
    float updateCollsionTime =  0.9f;
    bool state = true; 

    void animateAndDraw(Vector2 screenPos, float deltaTime);
};


#endif