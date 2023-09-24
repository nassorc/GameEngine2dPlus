//
// Created by Matthew Crossan on 9/22/23.
//

#include "Camera.h"

class PlayerCamera : public Camera  {
private:
public:
    PlayerCamera();
    PlayerCamera(std::shared_ptr<Entity> target, sf::FloatRect rectangle);
    void update(sf::RenderWindow& window) override;
};

