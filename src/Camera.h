//
// Created by Matthew Crossan on 9/22/23.
//

#pragma once
#include "Entity.h"
#include "Vec2.h"

class Camera {
protected:
    std::shared_ptr<Entity>    m_target = nullptr;
    sf::View   m_cameraView;
    Vec2       m_trap{1,1};
    float      m_smoothingSpeed{1};
public:
    Camera();
    Camera(std::shared_ptr<Entity> target, sf::FloatRect rectangle);
    /*
     * Function sets the window view to the camera's view
     */
    void render(sf::RenderWindow& window);
    /*
     * Function updates the camera position
     */
    virtual void update(sf::RenderWindow &window) = 0;
};


