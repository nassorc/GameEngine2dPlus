#include "Assets.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "SFML/Graphics.hpp"

Assets::Assets() = default;

void Assets::loadFromFile(const std::string &config) {
    // open file
    std::ifstream fin(config);
    if (!fin) {
        std::cerr << "Could not open asset file\n";
        exit(-1);
    }
    // loop through entry
    std::string type, name, path;
    while (fin >> type) {
        if (type == "#") getline(fin, type, '\n');
        std::transform(type.begin(), type.end(), type.begin(), ::toupper);
        if (type == "TEXTURE") {
            fin >> name >> path;
            addTexture(name, path);
        } else if (type == "ANIMATION") {
            std::string aniName, texName;
            float x, y;
            size_t frameCount, speed, frameSize;
            fin >> aniName >> texName >> frameSize >> x >> y >> frameCount >> speed;
            this->addAnimation(aniName, texName, frameSize, Vec2{x, y}, frameCount, speed);
        } else if (type == "FONT") {
            sf::Font font;
            std::string name, path;
            fin >> name >> path;
            this->loadFont(path);
            this->setFont(name, this->loadFont(path));
        } else if (type == "DEFAULTFONT") {
            fin >> m_defaultFont;
        }
    }
}

sf::Texture &Assets::getTexture(const std::string &name) {
    return m_textures[name];
}

void Assets::addTexture(const std::string &name, const std::string &path, bool repeated) {
    sf::Texture tex;
    if (!tex.loadFromFile(path)) {
        std::cerr << "Could not open texture " << name << " at " << path << std::endl;
        exit(-1);
    }
    tex.setRepeated(true);
    m_textures[name] = tex;
}

void Assets::addAnimation(const std::string &name, const std::string &texName, size_t frameSize, const Vec2 &loc,
                          size_t frameCount, size_t speed) {
    // init
    Animation animation(name, m_textures[texName], Vec2(frameSize, frameSize), loc);
//    animation.setFrameSize(frameSize);
    animation.setFrameCount(frameCount);
    animation.setSpeed(speed);
//    animation.setLocationInSprite(loc);
    // add to animation map
    m_animations[name] = animation;

}

void Assets::setFont(const std::string &name, const sf::Font &font) {
    m_fonts[name] = font;
}

sf::Font Assets::loadFont(const std::string &path) {
    sf::Font font;
    if (!font.loadFromFile(path)) {
        exit(-1);
    }
    return font;
}


Animation &Assets::getAnimation(const std::string &name) {
    return m_animations[name];
}

sf::Font &Assets::getFont(const std::string &name) {
    return m_fonts[name];
}

sf::Font &Assets::getDefaultFont() {
    return m_fonts[m_defaultFont];
}
