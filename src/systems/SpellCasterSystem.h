//
// Created by Matthew Crossan on 10/10/23.
//

#ifndef LIGHTCASTING_SPELLCASTERSYSTEM_H
#define LIGHTCASTING_SPELLCASTERSYSTEM_H

#include "../include/System.h"
#include "../core/Types.h"
#include "../components/Components.h"

class SpellCasterSystem : public System {
public:
    SpellCasterSystem();
    void update() override;
    void castSpell(SpellType spell, std::shared_ptr<Entity1> caster);
};


#endif //LIGHTCASTING_SPELLCASTERSYSTEM_H
