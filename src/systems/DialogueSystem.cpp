//
// Created by Matthew Crossan on 10/13/23.
//

#include "../components/Components.h"
#include "../core/CoreManager.h"
#include "../core/World.h"
#include "DialogueSystem.h"
#include "fstream"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

extern World world;

DialogueSystem::DialogueSystem() : System() {

}

void DialogueSystem::update() {
    System::update();
    for (auto entity : m_entities) {
        for(auto collision : world.get<CCollide1>(entity).collisions) {
            if(world.has<CDialogue1>(collision.collider)) {
                auto& dialogue = world.get<CDialogue1>(collision.collider);
                DialogueSystem::openDialogue(dialogue.dialoguePath, dialogue.name);
//                std::cout << entity->tag() << " talking " << collision.collider->tag() << std::endl;
            }
        }
    }
}

void DialogueSystem::openDialogue(const std::string& config, const std::string& name) {
    // assetManager->getDialogue('npc')
    // dialogue
    //    dialgoues: array
    //    current dialogue
    //    current sentence
    std::ifstream fin(config);
    assert(fin && "DialogueSystem::openDialogue Could not open config file");

    json dialogue = json::parse(fin);
    if(dialogue.find(name) != dialogue.end()) {

    }
}
