#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "Vec2.h"


class Animation
{
protected:
  sf::Sprite    m_sprite;
  std::string   m_name         = "none";
  size_t        m_speed        = 0;
  size_t        m_frameCount   = 1;        //total number of frames in the animation 
  size_t        m_currentFrame = 0;        // Curent frame of animation being played
  size_t        m_totalFrames  = 0;        // frame life
  Vec2          m_loc          = {0, 0};   // x and y location in the spritesheet in terms of grid coodinates
  Vec2          m_size           {1, 1};
  Vec2          m_frameSize    = {16, 16};

public:
  Animation();
  Animation(const std::string& name, sf::Texture& tex, const Vec2& tileSize, const Vec2& loc);
  void update();
  bool hasEnded();

  std::string& getName();
  Vec2& getSize();
  Vec2& getFrameSize();
  Vec2& getLocationInSpriteSheet();
  sf::Sprite& getSprite();

  void setSpeed(size_t speed);
  void setFrameSize(size_t size);
  void setFrameCount(size_t frameCount);
  void setLocationInSprite(float x, float y);
  void setLocationInSprite(const Vec2& loc);
};