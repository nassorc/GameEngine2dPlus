#include "Assets.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <array>
#include "SFML/Graphics.hpp"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

Assets::Assets() = default;

void Assets::loadFromFile(const std::string &config) {
    // open file
    std::ifstream fin(config);
    if (!fin) {
        std::cerr << "Could not open asset file\n";
        exit(-1);
    }
    m_fileReader.openFile(config);
    json assets = json::parse(m_fileReader.readFileContents());
    json textures = assets["textures"];
    json fonts = assets["fonts"];
    json animations = assets["animations"];
    json tilesheets = assets["tilesheets"];

    std::string name, path;
    for(auto& texture : textures) {
        name = texture["name"];
        path = texture["path"];
        addTexture(name, path);
    }
    for(auto& font : fonts) {
        name = font["name"];
        path = font["path"];
        addFont(name, path);
    }
    for(auto& animation : animations) {
        float width, height;
        std::string animationName, textureReference;
        std::vector<Vec2> frames;
        size_t frameCount, speed, frameSize;

        json frame       = animation["frame"];
        width            = frame["width"];
        height           = frame["height"];
        speed            = frame["speed"];
        animationName    = animation["name"];
        textureReference = animation["tex_ref"];
        for(auto frame : frame["frames"]) {
            frames.push_back(Vec2(frame[0], frame[1]));
        }
        addAnimation(animationName, textureReference, Vec2{width, height},  frameCount, speed, frames);
    }
    for(auto& tilesheet : tilesheets) {
        // create animation based for each tile
        size_t tileWidth  = tilesheet["tilewidth"];
        size_t tileHeight = tilesheet["tileheight"];
        size_t width      = tilesheet["width"];
        size_t height     = tilesheet["height"];

        std::string textureReference = tilesheet["texref"];
        sf::Texture& tex =  m_textures[textureReference];
        size_t idx = 0;

        for(size_t row=0; row<(tex.getSize().y/tileHeight); row++) {
            for(size_t col=0; col<(tex.getSize().x/tileWidth); col++) {
                std::vector<Vec2> frames;
                frames.push_back(Vec2{(float) col, (float) row});
                addAnimation(std::to_string(idx), textureReference, Vec2(24, 24),  1, 0, frames);
                idx += 1;
            }
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
    m_textureKeys.push_back(name);
}

void Assets::addAnimation(const std::string &name, const std::string &texName, Vec2 frameSize, size_t frameCount, size_t speed, std::vector<Vec2> frames) {
    Animation animation(name, m_textures[texName], Vec2(frameSize.x, frameSize.y), frames);
    animation.setSpeed(speed);
    m_animations[name] = animation;

}
void Assets::addFont(const std::string &name, const std::string &path) {
    setFont(name, loadFont(path));
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

animationMap &Assets::getAnimations() {
    return m_animations;
}

bool Assets::hasTexture(const std::string textureName) {
    auto itr = std::find(m_textureKeys.begin(), m_textureKeys.end(), textureName);
    return itr != m_textureKeys.end();
}
