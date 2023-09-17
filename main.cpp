// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <string>
#include "GameEngine.h"
//#include "imgui.h"
//#include "imgui-SFML.h"

int main()
{
  GameEngine game("bin/assets2.txt");
  game.run();
}

//#include <SFML/Graphics/CircleShape.hpp>
//#include <SFML/Graphics/RenderWindow.hpp>
//#include <SFML/System/Clock.hpp>
//#include <SFML/Window/Event.hpp>

//int main() {
//    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
//    window.setFramerateLimit(60);
//    ImGui::SFML::Init(window);
//
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    sf::Texture t;
//    if(!t.loadFromFile("bin/assets/zelda.png")) {
//        std::cout << "could not load bardo" << std::endl;
//    }
//
//    int TILE_SIZE = 16;
//    sf::Sprite sprite(t, sf::IntRect(0, 0, t.getSize().x, t.getSize().y));
//
//    sf::Clock deltaClock;
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            ImGui::SFML::ProcessEvent(window, event);
//
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//        }
//
//
//        ImGui::SFML::Update(window, deltaClock.restart());
//
//        ImGui::Begin("Hello, world!");
////        ImGui::ImageButton(&shape, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
//        ImGui::Button("Look at this pretty button");
//        if (ImGui::Button("Press for some")) {
//            std::cout << "working?" << std::endl;
//        }
//        ImGui::BeginTable("assets", (t.getSize().x / TILE_SIZE));
//        for(int row=0; row < (t.getSize().y / TILE_SIZE); row++) {
//            ImGui::TableNextRow();
//            for(int col=0; col < (t.getSize().x / TILE_SIZE); col++) {
//                ImGui::TableNextColumn();
//                sprite.setTextureRect(sf::IntRect(col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE));
//                if(ImGui::ImageButton(sprite, 10)) {
//                    std::cout << "picked asset" << std::endl;
//                }
//            }
//
//        }
//        ImGui::EndTable();
//        ImGuiIO& io = ImGui::GetIO();
//        ImGui::TextColored(ImVec4(0,1,1,1), "Field 1");
//        ImGui::BeginChild("items");
//        for (int i=0; i<50; i++) {
//            ImGui::Text("item 1", i);
//        }
//        ImGui::EndChild();
//        ImGui::End();
//
//        window.clear();
//        window.draw(shape);
//        ImGui::SFML::Render(window);
//        window.display();
//    }
//
//    ImGui::SFML::Shutdown();
//}
