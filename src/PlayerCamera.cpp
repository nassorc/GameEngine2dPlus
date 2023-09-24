//
// Created by Matthew Crossan on 9/22/23.
//

#include "PlayerCamera.h"

PlayerCamera::PlayerCamera() {

}

PlayerCamera::PlayerCamera(std::shared_ptr<Entity> target, sf::FloatRect rectangle)
    : Camera(target, rectangle)
{

}

void PlayerCamera::update(sf::RenderWindow& window) {
    Vec2 &targetPos = m_target->getComponent<CTransform>().pos;
    auto windowSize = window.getSize();
    if(targetPos.x >= m_cameraView.getCenter().x)  {
        m_cameraView.move(sf::Vector2f(targetPos.x - ((float) windowSize.x / 2), 0));
    }
}
