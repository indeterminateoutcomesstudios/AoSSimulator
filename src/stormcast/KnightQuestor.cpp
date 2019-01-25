/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightQuestor.h>
#include <iostream>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    KnightQuestor::Create,
    nullptr,
    nullptr,
    {
    }
};

bool KnightQuestor::s_registered = false;

Weapon KnightQuestor::s_warblade(Weapon::Type::Melee, "Questor Warblade", 1, 4, 3, 3, -1, 1);

KnightQuestor::KnightQuestor() :
    StormcastEternal("Knight-Questor", 5, WOUNDS, 8, 3, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, KNIGHT_QUESTOR };
}

bool KnightQuestor::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&s_warblade);
    addModel(model);

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_warblade.name() << ": " << s_warblade.strength() << std::endl;
    }

    return true;
}

Unit *KnightQuestor::Create(const ParameterList &parameters)
{
    auto unit = new KnightQuestor();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KnightQuestor::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Knight-Questor", factoryMethod);
    }
}

Rerolls KnightQuestor::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Heroic Challenge
    if (unit->hasKeyword(HERO))
    {
        return RerollFailed;
    }
    return StormcastEternal::toHitRerolls(weapon, unit);
}

Rerolls KnightQuestor::toSaveRerolls(const Weapon *weapon) const
{
    // Sigmarite Shield
    return RerollFailed;
}


} // namespace StormcastEternals