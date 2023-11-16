//
// Created by Matthew Crossan on 10/10/23.
//

#include "SpellCasterSystem.h"
#include "../core/World.h"
#include "../components/Components.h"

extern World world;

SpellCasterSystem::SpellCasterSystem() : System() {

}
void SpellCasterSystem::update() {
    System::update();
    for(auto spellcaster : m_entities) {
        auto& state = world.get<CState>(spellcaster);
        auto& spell = world.get<CSpellCaster>(spellcaster);
        if(state.state == StateType::ATTACKING) {
            world.coreManager->prefab(PrefabType::FIREBALL);
            state.state = StateType::IDLE;
//            castSpell(spell.spellType, spell.caster);
        }
    }
}

void castSpell(SpellType spell, std::shared_ptr<Entity1> caster) {
//    world.createPrefab(spell);
}
