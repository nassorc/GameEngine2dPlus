//
// Created by Matthew Crossan on 9/22/23.
//

#include "Camera.h"

class TargetCamera : public Camera  {
private:
    float m_diff = 32;
    float m_speed = 32;
public:
    TargetCamera();
    TargetCamera(std::shared_ptr<Entity> target, sf::FloatRect rectangle);
    void update(sf::RenderWindow& window) override;
};

