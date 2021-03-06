/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/GlaivewraithStalkers.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    GlaivewraithStalkers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = GlaivewraithStalkers::MIN_UNIT_SIZE}, GlaivewraithStalkers::MIN_UNIT_SIZE,
            GlaivewraithStalkers::MAX_UNIT_SIZE, GlaivewraithStalkers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "drummer", {.m_boolValue = true}, false, false}
    },
    DEATH,
    NIGHTHAUNT
};

bool GlaivewraithStalkers::s_registered = false;

GlaivewraithStalkers::GlaivewraithStalkers() :
    Unit("Glaivewraith Stalkers", 6, WOUNDS, 10, 4, true),
    m_huntersGlaive(Weapon::Type::Melee, "Hunter's Glaive", 2, 2, 4, 3, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, GLAIVEWRAITH_STALKERS};
}

bool GlaivewraithStalkers::configure(int numModels, bool drummer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_drummer = drummer;
    m_runAndCharge = m_drummer;

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_huntersGlaive);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *GlaivewraithStalkers::Create(const ParameterList &parameters)
{
    auto unit = new GlaivewraithStalkers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool drummer = GetBoolParam("drummer", parameters, true);

    bool ok = unit->configure(numModels, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GlaivewraithStalkers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Glaivewraith Stalkers", factoryMethod);
    }
}

Rerolls GlaivewraithStalkers::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // The Point of Death
    if (m_charged || unit->charged())
    {
        return RerollFailed;
    }

    return Unit::toHitRerolls(weapon, unit);
}

int GlaivewraithStalkers::toSaveModifier(const Weapon *weapon) const
{
    // Ethereal - no save modifiers allowed.
    int modifier = 0;

    // Ethereal - ignore rend by cancelling it out.
    if (weapon->rend() < 0)
    {
        modifier = -weapon->rend();
    }

    return modifier;
}

void GlaivewraithStalkers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_huntersGlaive);
}

} // namespace Nighthaunt
