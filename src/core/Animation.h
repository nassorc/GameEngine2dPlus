#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "../math/Vec2.h"


class Animation {
protected:
    sf::Sprite m_sprite;
    std::string m_name = "none";
    size_t m_speed = 0;
    size_t m_frameCount = 1;        //total number of frames in the animation
    size_t m_currentFrame = 0;        // Curent frame of animation being played
    size_t m_totalFrames = 0;        // frame life
    Vec2 m_tileSize{1, 1};
    Vec2 m_scale{1, 1};
//    Vec2 m_size;
    std::vector<Vec2> m_frames;
    bool m_animationEnded = false;

public:
    Animation();
    Animation(const std::string &name, sf::Texture &tex, const Vec2 &tileSize, const std::vector<Vec2>& frames);

    void update();
    bool hasEnded();
    std::string &getName();
    Vec2 &getSize();
    Vec2 &getFrameSize();
    sf::Sprite &getSprite();
    void setSpeed(size_t speed);
    void setFrameSize(size_t size);
};