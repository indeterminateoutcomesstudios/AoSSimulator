/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Treelord.h>
#include <UnitFactory.h>
#include <iostream>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    Treelord::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    SYLVANETH
};

bool Treelord::s_registered = false;

struct TableEntry
{
    int m_stranglerootsToHit;
    int m_sweepingBlowsAttacks;
    int m_talonsToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, Treelord::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {2, 4, 2},
        {3, 3, 2},
        {4, 2, 3},
        {5, 2, 3},
        {6, 1, 4}
    };

Treelord::Treelord() :
    Unit("Treelord", 6, WOUNDS, 6, 3, false),
    m_strangleroots(Weapon::Type::Missile, "Strangleroots", 12, 5, 2, 3, -1, 1),
    m_sweepingBlows(Weapon::Type::Melee, "Sweeping Blows", 3, 4, 3, 3, -1, RAND_D6),
    m_massiveImpalingTalons(Weapon::Type::Melee, "Massive Impaling Talons", 1, 1, 3, 2, -2, 1)
{
    m_keywords = {ORDER, SYLVANETH, MONSTER, TREELORD};
}

bool Treelord::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_strangleroots);
    model.addMeleeWeapon(&m_sweepingBlows);
    model.addMeleeWeapon(&m_massiveImpalingTalons);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Treelord::hero(PlayerId id)
{
}

void Treelord::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_strangleroots.setToHit(g_damageTable[damageIndex].m_stranglerootsToHit);
    m_sweepingBlows.setAttacks(g_damageTable[damageIndex].m_sweepingBlowsAttacks);
    m_massiveImpalingTalons.setToWound(g_damageTable[damageIndex].m_talonsToWound);
}

int Treelord::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();
    for (auto i = 0; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}

Unit *Treelord::Create(const ParameterList &parameters)
{
    auto unit = new Treelord();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Treelord::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Treelord", factoryMethod);
    }
}

void Treelord::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_strangleroots);
    visitor(&m_sweepingBlows);
    visitor(&m_massiveImpalingTalons);
}

} // namespace Sylvaneth
