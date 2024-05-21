#include "Prop.h"
#include "raymath.h"
#include <string>

Prop::Prop(Vector2 pos, Texture2D tex, const std::string fileName) : worldPos(pos), texture(tex), name(fileName),  runningTime(0.0f), frame(0), collisionTime(0.0f), state(true) {}

void Prop::Render(Vector2 knightPos, float deltaTime)
{
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};
    if (name== "nature_tileset/fire.png" || name == "nature_tileset/spikes.png"
    || name == "nature_tileset/spikes2.png" || name == "nature_tileset/poison.png"){
         animateAndDraw(screenPos, deltaTime);
    }
    else {
        DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);
    }
}

/*Rectangle Prop::getCollisionRec(Vector2 knightPos)
{
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};
    return Rectangle{
        screenPos.x,screenPos.y,
        texture.width * scale,
        texture.height * scale,
    };
}*/


Rectangle Prop::getCollisionRec(Vector2 knightPos)
{
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};
    if (name == "nature_tileset/fire.png" || name == "nature_tileset/spikes.png"
    || name == "nature_tileset/spikes2.png" || name == "nature_tileset/poison.png") {
        float frameWidth = static_cast<float>(texture.width) / maxFrames;
        float frameHeight = static_cast<float>(texture.height);
        return Rectangle{
            screenPos.x, screenPos.y,
            frameWidth * scale, frameHeight * scale
        };
    } else {
        return Rectangle{
            screenPos.x, screenPos.y,
            texture.width * scale,
            texture.height * scale,
        };
    }
}

std::string Prop::getName() const {
    return name;
}

void Prop::animateAndDraw(Vector2 screenPos, float deltaTime) {
    runningTime += deltaTime;

    //std::cout << "Frame: " << frame << ", Running Time: " << runningTime << ", Update Time: " << updateTime << std::endl;

    if (runningTime >= updateTime) {
        frame += 1;
        runningTime = 0.f;
        //std::cout << "+1 " << std::endl;
        if (frame >= maxFrames) {
            frame = 0;
        }
    }

    //std::cout << "Frame2: " << frame << ", Running Time: " << runningTime << ", Update Time: " << updateTime << std::endl;

    float frameWidth = static_cast<float>(texture.width) / maxFrames;
    float frameHeight = static_cast<float>(texture.height);
    Rectangle source{ frameWidth * frame, 0.f, frameWidth, frameHeight };
    Rectangle dest{ screenPos.x, screenPos.y, scale * frameWidth, scale * frameHeight };
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);
}

bool Prop::getState(float deltaTime){
    if (state == true) {
        state = false;
        return true;
        
    }
    else {
        collisionTime += deltaTime;
        if (collisionTime >= 1.5f){
            collisionTime = 0.0f;
            return true;
        }
        else{
            return false; 
        }
    } 
}

