//
// Created by Matthew Crossan on 9/14/23.
//

#include "ScenePlayground.h"
#include <algorithm>
#include <functional>

//class ConvexShapeCounter : public sf::ConvexShape {
//public:
//    ConvexShapeCounter(std::size_t pointCount = 0)
//        : sf::ConvexShape()
//    {}
//    &
//};

ScenePlayground::ScenePlayground(GameEngine *gameEngine, Assets &assetManager)
    : Scene(gameEngine, assetManager)
    {
    init();
}

void ScenePlayground::init() {
    auto e = m_entityManager.addEntity("shape");
    std::vector<Vec2> points = {Vec2(300, 300), Vec2(620, 300), Vec2(520, 650)};
    e->addComponent<CShape>(points, m_backgroundColor);

    auto e2 = m_entityManager.addEntity("shape");
    std::vector<Vec2> p2 = {Vec2(700, 80), Vec2(1000, 180), Vec2(1000, 300), Vec2(820, 300)};
    e2->addComponent<CShape>(p2, m_backgroundColor);

    auto e3 = m_entityManager.addEntity("shape");
    Vec2 wSize = m_game->getWindowSize();
    std::vector<Vec2> p3 = {Vec2(0, 0), Vec2(wSize.x, 0), wSize + Vec2(0, 0), Vec2(0, wSize.y)};
    e3->addComponent<CShape>(p3, sf::Color(255,255,255,0));

}

void ScenePlayground::update() {
    m_entityManager.update();
    sRender();
}

void ScenePlayground::sDoAction(const Action &action) {
    if(action.type() == "START") {
        if (action.name() == "MOUSE_MOVE") {
            m_mousePos = action.pos();
        }
    }
}

void ScenePlayground::sRender() {
    m_game->clearWindow(m_backgroundColor);
    if(m_drawGrid) {
        drawGrid();
    }

    for(auto& e : m_entityManager.getEntities()) {
        if(e->hasComponent<CShape>()) {
            auto& shape = e->getComponent<CShape>().shape;
            m_game->window().draw(shape);
        }
    }

    drawLight();
    drawCursor();
    drawCursorTextPos();
}

void ScenePlayground::onEnd() {

}

void ScenePlayground::drawLine(const Vec2 &p1, const Vec2 &p2) {
    sf::Vertex line[] = {sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y)};
    for(auto& l : line) {
        l.color = sf::Color::Black;
    }
    m_game->window().draw(line, 2, sf::Lines);
}

void ScenePlayground::drawLine(const Vec2 &p1, const Vec2 &p2, sf::Color color) {
    sf::Vertex line[] = {sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y)};
    for(auto& l : line) {
        l.color = color;
    }
    m_game->window().draw(line, 2, sf::Lines);
}

void ScenePlayground::drawGrid() {
    Vec2 windowSize = m_game->getWindowSize();
    float gridSize = 64;
    float maxCols = windowSize.x / gridSize;
    float maxRows = windowSize.y / gridSize;
    for(int row=0; row<maxRows; row++) {
        for(int col=0; col<maxCols; col++) {
            drawLine(Vec2(col*gridSize, row*gridSize), Vec2(col*gridSize + gridSize, row*gridSize));
            drawLine(Vec2(col*gridSize + gridSize, row*gridSize), Vec2(col*gridSize + gridSize, row*gridSize + gridSize));
        }
    }
}

void ScenePlayground::drawCursor() {
    sf::CircleShape cursor(4);
    cursor.setFillColor(sf::Color::Black);
    cursor.setOrigin(2, 2);
    cursor.setPosition(m_mousePos.x, m_mousePos.y);
    m_game->window().draw(cursor);
}

void ScenePlayground::drawCursorTextPos() {
    // mouse position
    std::string loc = std::to_string(m_mousePos.x) + ", " + std::to_string(m_mousePos.y);
    sf::Text text(loc, m_assetManager.getDefaultFont(), 18);
    text.setFillColor(sf::Color::Black);
    m_game->window().draw(text);
}

void ScenePlayground::drawLight() {
    std::vector<Vec2> all;
    for (auto& e : m_entityManager.getEntities("shape")) {
        auto& shape = e->getComponent<CShape>().shape;
        std::vector<Vec2> pointsFromShape = {};
        for(size_t idx=0; idx < shape.getPointCount(); idx++) {
            Vec2 point(shape.getPoint(idx).x, shape.getPoint(idx).y);
            Vec2 delta1 = (point + Vec2(0.1f, 0.1f)) - m_mousePos;
            Vec2 normalized1 = delta1.normalize() * 10000;
            Vec2 delta2 = (point + Vec2(-0.1f, -0.1f)) - m_mousePos;
            Vec2 normalized2 = delta2.normalize() * 10000;
            Vec2 new1 = point + normalized1;
            Vec2 new2 = point + normalized2;
            pointsFromShape.push_back(point);
            pointsFromShape.push_back(new1);
            pointsFromShape.push_back(new2);
        }

        for(auto& point : pointsFromShape) {
            Vec2 closestIntersectingPoint(point);
            for (auto& shape : m_entityManager.getEntities("shape")) {
                std::vector<Intersect> intersects = m_physics.ShapeIntersect(m_mousePos, point, shape);
                for (size_t idx=0; idx<intersects.size(); idx++) {
                    Vec2 delta1(intersects[idx].pos.x - m_mousePos.x, intersects[idx].pos.y - m_mousePos.y);
                    Vec2 delta2(closestIntersectingPoint.x - m_mousePos.x, closestIntersectingPoint.y - m_mousePos.y);
                    float distance1 = sqrt((delta1.x * delta1.x) + (delta1.y * delta1.y));
                    float distance2 = sqrt((delta2.x * delta2.x) + (delta2.y * delta2.y));
                    if (distance1 < distance2) {
                        closestIntersectingPoint = intersects[idx].pos;
                    }
                }
            }
            all.push_back(closestIntersectingPoint);
        }
    }

    Vec2 mousePos = m_mousePos;
    Vec2 windowPos = m_game->getWindowSize();
    std::sort(all.begin(), all.end(), [mousePos](Vec2& a, Vec2& b) {
        // SORT BY DEGREES RELATIVE TO MOUSE POINTER POSITION

        Vec2 deltaA = a - mousePos;
        Vec2 deltaB = b - mousePos;
        float angleA = atan2f(deltaA.y, deltaA.x);
        float angleB = atan2f(deltaB.y, deltaB.x);
        float degA = angleA * 57.2958f;
        float degB = angleB * 57.2958f;
        if(degA < 360.f) {
            degA = degA + 360.f;
        }
        if(degB < 360.f) {
            degB = degB + 360.f;
        }
        return degA < degB;
    });

    // DRAW RAYS ==========================================================
//    for(size_t idx=0; idx<all.size(); idx++) {
//        float ratio = (float) idx / (float) all.size();
//        unsigned char red = ratio * 255;
//        unsigned char blue = ratio * 255;
//        drawLine(m_mousePos, all[idx], sf::Color(red, 0,blue,255));
//    }
    if(all.size() > 0) {

        for (size_t idx=0; idx < all.size(); idx++) {
            sf::VertexArray triangle(sf::Triangles, 3);
            size_t nextIdx = ((int) idx + 1) % ((int) all.size());
            std::cout << idx << " " << nextIdx << std::endl;
            triangle[0].position = sf::Vector2f(m_mousePos.x, m_mousePos.y);
            triangle[1].position = sf::Vector2f(all[idx].x, all[idx].y);
            triangle[2].position = sf::Vector2f(all[nextIdx].x, all[nextIdx].y);
//            triangle[0].position = sf::Vector2f(50, 50);
//            triangle[1].position = sf::Vector2f(500, 50);
//            triangle[2].position = sf::Vector2f(500, 300);

            triangle[0].color = sf::Color(0,0,0,100);
            triangle[1].color = sf::Color(0,0,0,100);
            triangle[2].color = sf::Color(0,0,0,100);

            m_game->window().draw(triangle);
        }



//        sf::ConvexShape light;
//        light.setPointCount(all.size());
//        light.setOutlineThickness(3);
//        light.setOutlineColor(sf::Color(20, 239, 89));
//        light.setFillColor(sf::Color(0,0,0,100));
//        for (size_t idx=0; idx < all.size(); idx++) {
//            light.setPoint(idx, sf::Vector2f(all[idx].x, all[idx].y));
//        }
//        m_canvas->window().draw(light);

        // DRAW RAY NUMBER ================================================
//        for (size_t idx=0; idx < all.size(); idx++) {
//            sf::Text t(std::to_string(idx), m_canvas->getAssets().getDefaultFont(), 8);
//            t.setFillColor(sf::Color::Black);
//            Vec2 delta = all[idx] - m_mousePos;
//            Vec2 normalized = (delta.normalize(delta.distance())).scale(12 * idx);
//            Vec2 newV = all[idx] + normalized;
//            t.setPosition(newV.x, newV.y);
//            m_canvas->window().draw(t);
//        }
    }

}

