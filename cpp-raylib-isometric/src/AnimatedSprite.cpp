#include "AnimatedSprite.hpp"

#include <algorithm>

bool AnimatedSprite::Load(const std::string& path, int frameWidth, int frameHeight, int frameCount) {
    Unload();
    if (frameWidth <= 0 || frameHeight <= 0 || frameCount <= 0) return false;
    Image image = LoadImageSvg(path.c_str(), frameWidth * frameCount, frameHeight * 2);
    if (!image.data) return false;
    texture_ = LoadTextureFromImage(image);
    UnloadImage(image);
    frameWidth_ = frameWidth;
    frameHeight_ = frameHeight;
    frameCount_ = frameCount;
    loaded_ = IsTextureValid(texture_);
    return loaded_;
}

void AnimatedSprite::Unload() {
    if (loaded_) UnloadTexture(texture_);
    texture_ = {};
    loaded_ = false;
    frame_ = 0;
    timer_ = 0.0f;
}

void AnimatedSprite::Update(float deltaTime, bool moving) {
    moving_ = moving;
    if (!moving_) {
        frame_ = 0;
        timer_ = 0.0f;
        return;
    }
    timer_ += deltaTime;
    constexpr float frameDuration = 0.14f;
    while (timer_ >= frameDuration) {
        timer_ -= frameDuration;
        frame_ = (frame_ + 1) % frameCount_;
    }
}

void AnimatedSprite::Draw(Vector2 position, Color tint) const {
    if (!loaded_) return;
    const int row = moving_ ? 1 : 0;
    const Rectangle source{static_cast<float>(frame_ * frameWidth_), static_cast<float>(row * frameHeight_),
                           static_cast<float>(frameWidth_), static_cast<float>(frameHeight_)};
    const Rectangle dest{position.x, position.y, static_cast<float>(frameWidth_), static_cast<float>(frameHeight_)};
    DrawTexturePro(texture_, source, dest, {0, 0}, 0.0f, tint);
}