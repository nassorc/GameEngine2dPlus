#include "Animation.h"

#include <iostream>
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "../math/Vec2.h"

const size_t TILE_SIZE = 64;

Animation::Animation() {
}

Animation::Animation(const std::string &name, sf::Texture &tex, const Vec2 &tileSize, const std::vector<Vec2>& frames)
        : m_name(name)
        , m_sprite(tex)
        , m_tileSize(tileSize)
        , m_frames(frames)
{
    if(m_frames.size() > 0) {
        m_sprite.setTextureRect(sf::IntRect(m_tileSize.x * m_frames[0].x, m_tileSize.y * m_frames[0].y, m_tileSize.x, m_tileSize.y));
        /*
         * TODO: Implement a more dynamic spritesheet.
         * current implementation forces the spritsheet to be of size 64x64 pixels
         */
        m_sprite.setScale((float) 64/24, (float) 64/24);
        m_sprite.setOrigin(tileSize.x / 2, tileSize.y / 2);
        m_frameCount = m_frames.size();
    }
}

void Animation::update() {
    if(m_speed > 0 && m_frameCount > 1) {
        // loop through each frame
        m_currentFrame = (m_totalFrames / m_speed) % m_frameCount;
        const Vec2& frame = m_frames[m_currentFrame];
        m_sprite.setTextureRect(sf::IntRect(
                frame.x * m_tileSize.x,
                frame.y * m_tileSize.y,
                m_tileSize.x,
                m_tileSize.y
        ));
        m_totalFrames++;
    }
}

Vec2 &Animation::getSize() {
    return m_tileSize;
}

sf::Sprite &Animation::getSprite() {
    return m_sprite;
}

void Animation::setSpeed(size_t speed) {
    m_speed = speed;
}


std::string &Animation::getName() {
    return m_name;
}

bool Animation::hasEnded() {
    if(m_frameCount == (m_currentFrame + 1)) {
        m_animationEnded = true;
    }
    return m_animationEnded;
}

