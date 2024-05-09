#ifndef PROP_H
#define PROP_H

#include "raylib.h"
#include <iostream> 
#include <string>

class Prop {
public:
    Prop(Vector2 pos, Texture2D tex, const std::string fileName);
    void Render(Vector2 knightPos);
    Rectangle getCollisionRec(Vector2 knightPos);
    std::string getName() const;
private:
    Vector2 worldPos{};
    Texture2D texture{};
    std::string  name; 
    float scale{4.f};
    bool isAnimated;
    int frameWidth ; // Ancho de cada fotograma de la animación
    int frameHeight; // Alto de cada fotograma de la animación
    int totalFrames; // Cantidad total de fotogramas en la animación
    float frameTime; // Tiempo entre cada fotograma (ajusta esto según la velocidad deseada)
    int currentFrame; // Fotograma actual
    float frameCounter;
};


#endif