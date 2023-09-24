//
// Created by Matthew Crossan on 9/22/23.
//

#include "Camera.h"

Camera::Camera() {

}

Camera::Camera(std::shared_ptr<Entity> target, sf::FloatRect rectangle)
    : m_target(target)
    , m_cameraView(rectangle)
{
}

void Camera::render(sf::RenderWindow &window)
{
    window.setView(m_cameraView);
}

//void Camera::update(sf::RenderWindow &window)
//{
//    Vec2 &targetPos = m_target->getComponent<CTransform>().pos;
//    auto windowSize = window.getSize();
//    if(targetPos.x >= m_cameraView.getCenter().x)  {
//        m_cameraView.move(sf::Vector2f(targetPos.x - ((float) windowSize.x / 2), 0));
//    }
//}

