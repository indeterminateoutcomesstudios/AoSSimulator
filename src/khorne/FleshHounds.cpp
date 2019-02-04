/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/FleshHounds.h>
#include <UnitFactory.h>
#include <iostream>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    FleshHounds::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = FleshHounds::MIN_UNIT_SIZE}, FleshHounds::MIN_UNIT_SIZE,
         FleshHounds::MAX_UNIT_SIZE, FleshHounds::MIN_UNIT_SIZE},
    }
};

bool FleshHounds::s_registered = false;

Weapon FleshHounds::s_burningRoar(Weapon::Type::Missile, "Burning Roar", 8, 1, 2, 4, 0, 1);
Weapon FleshHounds::s_blooddarkClaws(Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1);

FleshHounds::FleshHounds() :
    Unit("FleshHounds", 8, WOUNDS, 10, 5, false)
{
    m_keywords = {CHAOS, DAEMON, KHORNE, FLESH_HOUNDS};
}

bool FleshHounds::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    int numGoreHounds = numModels / 5;
    // Add the Gore Hounds
    for (auto i = 0; i < numGoreHounds; i++)
    {
        Model goreHound(BASESIZE, WOUNDS);
        goreHound.addMissileWeapon(&s_burningRoar);
        goreHound.addMeleeWeapon(&s_blooddarkClaws);
        addModel(goreHound);
    }

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_blooddarkClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

Unit *FleshHounds::Create(const ParameterList &parameters)
{
    auto unit = new FleshHounds();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void FleshHounds::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("FleshHounds", factoryMethod);
    }
}

void FleshHounds::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_burningRoar);
    visitor(&s_blooddarkClaws);
}

} // namespace Khorne