// load textures from file to memory
// allow retrieval of assets

// storage: map<string name, tex texture>
#pragma once

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include "SFML/Graphics.hpp"
#include "Animation.h"

typedef std::map<std::string, sf::Font> fontMap;
typedef std::map<std::string, sf::Texture> textureMap;
typedef std::map<std::string, Animation> animationMap;

class Assets {
private:
    fontMap m_fonts;
    textureMap m_textures;
    animationMap m_animations;
    std::string m_defaultFont;

    void addTexture(const std::string &name, const std::string &path, bool repeated = false);

    void addAnimation(
            const std::string &name, const std::string &texName, size_t frameSize, const Vec2 &loc,
            size_t frameCount, size_t speed
    );

    void setFont(const std::string &name, const sf::Font &font);

    sf::Font loadFont(const std::string &path);

public:
    Assets();

    void loadFromFile(const std::string &config);

    sf::Texture &getTexture(const std::string &name);

    Animation &getAnimation(const std::string &name);

    sf::Font &getFont(const std::string &name);

    sf::Font &getDefaultFont();
};