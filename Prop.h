#ifndef PROP_H
#define PROP_H

#include "raylib.h"
#include <iostream> 
#include <string>

class Prop {
public:
    // Constructor con un parámetro opcional
    Prop(Vector2 pos, Texture2D tex, const std::string fileName, bool isTorch = false);
    void Render(Vector2 knightPos, float deltaTime);
    Rectangle getCollisionRec(Vector2 knightPos);
    std::string getName() const;
    bool getState(float deltaTime);
    bool isBreakable() const;
    void breakVase(); 
    void breakChest();
    bool isTorch() const; // Nuevo método para verificar si es una antorcha

private:
    Vector2 worldPos{};
    Texture2D texture{};
    std::string  name; 
    float scale{4.f};
    bool isTorch_; // Nuevo miembro de datos
 
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
    int frameWidth ; // Ancho de cada fotograma de la animación
    int frameHeight; // Alto de cada fotograma de la animación
    int totalFrames; // Cantidad total de fotogramas en la animación
    float frameTime; // Tiempo entre cada fotograma (ajusta esto según la velocidad deseada)
    int currentFrame; // Fotograma actual
    float frameCounter;
    bool broken{false}; // Estado del jarrón
};


#endif