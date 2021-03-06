/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/LordOfPlagues.h>
#include <UnitFactory.h>

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    LordOfPlagues::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    NURGLE
};

bool LordOfPlagues::s_registered = false;

LordOfPlagues::LordOfPlagues() :
    Unit("Lord of Plagues", 4, WOUNDS, 9, 4, false),
    m_plagueriddenGreatBlade(Weapon::Type::Melee, "Plague-ridden Great Blade", 1, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, LORD_OF_PLAGUES};
}

bool LordOfPlagues::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_plagueriddenGreatBlade);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void LordOfPlagues::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_plagueriddenGreatBlade);
}

Unit *LordOfPlagues::Create(const ParameterList &parameters)
{
    auto unit = new LordOfPlagues();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void LordOfPlagues::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("LordOfPlagues", factoryMethod);
    }
}

int LordOfPlagues::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Plague-ridden Great Weapons
    if (unmodifiedHitRoll == 6)
    {
        Dice dice;
        // each 6 inflicts D6 hits
        return dice.rollD6();
    }

    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

} // namespace Nurgle

