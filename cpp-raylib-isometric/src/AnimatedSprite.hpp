#pragma once

#include "raylib.h"
#include <string>

class AnimatedSprite {
public:
    bool Load(const std::string& path, int frameWidth, int frameHeight, int frameCount);
    void Unload();
    void Update(float deltaTime, bool moving);
    void Draw(Vector2 position, Color tint = WHITE) const;
    bool IsLoaded() const { return loaded_; }

private:
    Texture2D texture_{};
    int frameWidth_ = 0, frameHeight_ = 0, frameCount_ = 0, frame_ = 0;
    float timer_ = 0.0f;
    bool loaded_ = false, moving_ = false;
};