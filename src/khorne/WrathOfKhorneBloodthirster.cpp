/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/WrathOfKhorneBloodthirster.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    WrathOfKhorneBloodthirster::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    KHORNE
};

struct TableEntry
{
    int m_move;
    int m_flailDamage;
    int m_axeToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WrathOfKhorneBloodthirster::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 6,       2},
        {9,  3,       3},
        {8,  3,       3},
        {7,  RAND_D3, 4},
        {6,  RAND_D3, 4}
    };

bool WrathOfKhorneBloodthirster::s_registered = false;

WrathOfKhorneBloodthirster::WrathOfKhorneBloodthirster() :
    Unit("Wrath Of Khorne Bloodthirster", 10, WOUNDS, 10, 4, true),
    m_bloodflail(Weapon::Type::Missile, "Bloodflail", 12, 1, 3, 3, -1, 6),
    m_mightyAxeOfKhorne(Weapon::Type::Melee, "Mighty Axe of Khorne", 2, 6, 3, 2, -2, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, WRATH_OF_KHORNE_BLOODTHIRSTER};
}

bool WrathOfKhorneBloodthirster::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_bloodflail);
    model.addMeleeWeapon(&m_mightyAxeOfKhorne);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void WrathOfKhorneBloodthirster::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bloodflail);
    visitor(&m_mightyAxeOfKhorne);
}

Unit *WrathOfKhorneBloodthirster::Create(const ParameterList &parameters)
{
    auto unit = new WrathOfKhorneBloodthirster();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WrathOfKhorneBloodthirster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Wrath Of Khorne Bloodthirster", factoryMethod);
    }
}

void WrathOfKhorneBloodthirster::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_mightyAxeOfKhorne.setToWound(g_damageTable[damageIndex].m_axeToWound);
    m_bloodflail.setDamage(g_damageTable[damageIndex].m_flailDamage);

    Unit::onWounded();
}

int WrathOfKhorneBloodthirster::getDamageTableIndex() const
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

Rerolls WrathOfKhorneBloodthirster::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Relentless Hunter
    if (target->hasKeyword(HERO) || target->hasKeyword(MONSTER))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

int WrathOfKhorneBloodthirster::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

} // namespace Khorne