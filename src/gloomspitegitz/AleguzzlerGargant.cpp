/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/AleguzzlerGargant.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    AleguzzlerGargant::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

struct TableEntry
{
    int m_move;
    int m_clubAttacks;
    int m_eadbuttDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, AleguzzlerGargant::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, RAND_3D6, RAND_D6},
        {6, RAND_2D6, RAND_D6},
        {5, RAND_2D6, RAND_D3},
        {4, RAND_D6, RAND_D3},
        {3, RAND_D6, 1}
    };

bool AleguzzlerGargant::s_registered = false;

AleguzzlerGargant::AleguzzlerGargant() :
    Unit("Aleguzzler Gargant", 8, WOUNDS, 6, 5, false),
    m_eadbutt(Weapon::Type::Melee, "'eadbutt", 1, 1, 4, 3, -3, RAND_D6),
    m_massiveClub(Weapon::Type::Melee, "Massive Club", 3, RAND_3D6, 3, 3, -1, 1),
    m_mightyKick(Weapon::Type::Melee, "Mighty Kick", 3, 1, 3, 3, -2, RAND_D3)
{
    m_keywords = {DESTRUCTION, GARGANT, GLOOMSPITE_GITZ, ALEGUZZLER, MONSTER};
}

bool AleguzzlerGargant::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_eadbutt);
    model.addMeleeWeapon(&m_massiveClub);
    model.addMeleeWeapon(&m_mightyKick);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int AleguzzlerGargant::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *AleguzzlerGargant::Create(const ParameterList &parameters)
{
    auto unit = new AleguzzlerGargant();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AleguzzlerGargant::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Aleguzzler Gargant", factoryMethod);
    }
}

void AleguzzlerGargant::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_eadbutt);
    visitor(&m_massiveClub);
    visitor(&m_mightyKick);
}

int AleguzzlerGargant::getDamageTableIndex() const
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

} // namespace GloomspiteGitz
