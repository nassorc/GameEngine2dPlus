//
// Created by Matthew Crossan on 9/22/23.
//

#include "TargetCamera.h"

TargetCamera::TargetCamera() {

}

TargetCamera::TargetCamera(std::shared_ptr<Entity> target, sf::FloatRect rectangle)
    : Camera(target, rectangle)
{

}

void TargetCamera::update(sf::RenderWindow& window) {
    Vec2 &targetPos = m_target->getComponent<CTransform>().pos;
    Vec2 &targetVelocity = m_target->getComponent<CTransform>().velocity;
    Vec2 &targetBBox = m_target->getComponent<CBoundingBox>().size;
    auto windowSize = window.getSize();

    float cameraSpeed = 0.01;

//    std::cout << "PLAYER: " << targetPos.x << std::endl;
//    std::cout << "VIEW: " << m_cam << std::endl;
    int acc = 0;

    if(targetPos.x - targetBBox.x >= m_cameraView.getCenter().x)  {
        m_cameraView.move(sf::Vector2f(targetPos.x - targetBBox.x - ((float) windowSize.x / 2), 0));
    }
}
