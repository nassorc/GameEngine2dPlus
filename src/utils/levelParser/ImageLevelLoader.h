//
// Created by Matthew Crossan on 9/24/23.
//

#pragma once
#include "LevelLoader.h"

class ImageLevelLoader : public LevelLoader {
public:
    ImageLevelLoader() {}
    void strategy(ScenePlay* level, json layer) override {
//        std::cout << data << std::endl;
        std::string textureName = layer["name"];

        bool repeatx = layer["repeatx"];
        float parallaxX = 0;
        float parallaxY = 0;
        if(layer.contains("parallaxx")) {
            parallaxX = layer["parallaxx"];
        }
        if(layer.contains("parallaxy")) {
            parallaxY = layer["parallaxy"];
        }
        Vec2 windowSize = level->m_game->getWindowSize();

        // set texture x repeating
        sf::Texture tex = level->m_assetManager.getTexture(textureName);
        Vec2 textureSize{(float) tex.getSize().x, (float) tex.getSize().y};
        Vec2 textureToWindowRatio{windowSize.x / textureSize.x, windowSize.y / textureSize.y};
        tex.setRepeated(repeatx);

        sf::Sprite sprite(level->m_assetManager.getTexture(textureName), sf::IntRect(0, 0, 1280 * 10, 768));
        sprite.scale(textureToWindowRatio.x, textureToWindowRatio.y);

        auto image =  level->m_entityManager.addEntity("image");
        image->addComponent<CSprite>(sprite);
        image->addComponent<CParallax>(parallaxX,parallaxY);
    }
};
