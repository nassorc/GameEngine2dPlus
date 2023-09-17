#include "Animation.h"

#include <iostream>
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Vec2.h"

const size_t TILE_SIZE = 64;

Animation::Animation() {
}

Animation::Animation(const std::string &name, sf::Texture &tex, const Vec2 &tileSize, const Vec2& loc)
        : m_name(name), m_sprite(tex), m_size(tileSize), m_loc(loc) {
    float scalarX = m_size.x * (m_size.x / TILE_SIZE);
    float scalarY = m_size.y * (m_size.y / TILE_SIZE);
    m_sprite.setScale(scalarX, scalarY);
    m_sprite.setOrigin(m_size.x / 2, m_size.y / 2);
    m_sprite.setTextureRect(sf::IntRect(16 * loc.x, 16 * loc.y, 16, 16));
}

void Animation::update() {
    if(m_speed > 0 && m_frameCount > 1) {
        m_currentFrame = (m_totalFrames / 10) % m_frameCount;
        m_sprite.setTextureRect(sf::IntRect(
                m_loc.x * 16,
                (m_loc.y * 16) + m_currentFrame * 16,
                m_frameSize.x,
                m_frameSize.y
        ));
        // when player moves, it resets to zeror, then never increments. The functions
        // seems to enter the condition
        m_totalFrames++;
    }
}

Vec2 &Animation::getSize() {
    return m_size;
}

Vec2 &Animation::getFrameSize() {
    return m_frameSize;
}

Vec2 &Animation::getLocationInSpriteSheet() {
    return m_loc;
}

sf::Sprite &Animation::getSprite() {
    return m_sprite;
}

void Animation::setSpeed(size_t speed) {
    m_speed = speed;
}

void Animation::setFrameSize(size_t size) {
    m_frameSize = Vec2{(float) size, (float) size};
}

void Animation::setFrameCount(size_t frameCount) {
    m_frameCount = frameCount;
}

void Animation::setLocationInSprite(float x, float y) {
    m_loc = Vec2{x, y};
}

void Animation::setLocationInSprite(const Vec2 &loc) {
    m_loc = loc;
}

std::string &Animation::getName() {
    return m_name;
}

