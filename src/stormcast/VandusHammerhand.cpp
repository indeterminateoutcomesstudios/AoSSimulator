/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/VandusHammerhand.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    VandusHammerhand::Create,
    nullptr,
    nullptr,
    {
    }
};

bool VandusHammerhand::s_registered = false;

Weapon VandusHammerhand::s_heldensen(Weapon::Type::Melee, "Heldensen", 2, 3, 3, 2, -1, 3);
Weapon VandusHammerhand::s_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 4, 3, 3, -1, 1);

VandusHammerhand::VandusHammerhand() :
    StormcastEternal("Vandus Hammerhand", 10, WOUNDS, 9, 3, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, HAMMERS_OF_SIGMAR, LORD_CELESTANT, VANDUS_HAMMERHAND };
}

bool VandusHammerhand::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&s_heldensen);
    model.addMeleeWeapon(&s_clawsAndFangs);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *VandusHammerhand::Create(const ParameterList &parameters)
{
    auto unit = new VandusHammerhand();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VandusHammerhand::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Vandus Hammerhand", factoryMethod);
    }
}

int VandusHammerhand::extraAttacks(const Weapon *weapon) const
{
    int attacks = Unit::extraAttacks(weapon);

    // Heldensen
    if (m_charged && weapon->name() == s_heldensen.name())
    {
        Dice dice;
        attacks += dice.rollD3();
    }
    return attacks;
}

void VandusHammerhand::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_heldensen);
    visitor(&s_clawsAndFangs);
}

int VandusHammerhand::damageModifier(const Weapon *weapon, const Unit *target, const Dice::RollResult &woundRolls) const
{
    // Intolerable Damage
    if (weapon->name() == s_clawsAndFangs.name())
    {
        Dice dice;
        int modifier = 0;
        for (auto i = 0; i < woundRolls.numUnmodified6s(); i++)
        {
            modifier += dice.rollD3() - 1; // D3 instead of 1
        }
        return modifier;
    }
    return StormcastEternal::damageModifier(weapon, target, woundRolls);
}

} // namespace StormcastEternals