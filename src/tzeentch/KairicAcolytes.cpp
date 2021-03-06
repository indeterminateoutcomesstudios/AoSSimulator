/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/KairicAcolytes.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Tzeentch
{
static FactoryMethod factoryMethod = {
    KairicAcolytes::Create,
    KairicAcolytes::ValueToString,
    KairicAcolytes::EnumStringToInt,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = KairicAcolytes::MIN_UNIT_SIZE}, KairicAcolytes::MIN_UNIT_SIZE,
            KairicAcolytes::MAX_UNIT_SIZE, KairicAcolytes::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "weapons", {.m_intValue = KairicAcolytes::CursedBlade}, KairicAcolytes::CursedBlade,
            KairicAcolytes::CursedBladeAndShield, 1
        },
        {ParamType::Integer, "numCursedGlaives", {.m_intValue = 0}, 0, KairicAcolytes::MAX_UNIT_SIZE/KairicAcolytes::MIN_UNIT_SIZE*3, 1},
        {ParamType::Integer, "numScrollsOfDarkArts", {.m_intValue = 0}, 0, KairicAcolytes::MAX_UNIT_SIZE/KairicAcolytes::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "numVulcharcs", {.m_intValue = 0}, 0, KairicAcolytes::MAX_UNIT_SIZE/KairicAcolytes::MIN_UNIT_SIZE, 1},
    },
    CHAOS,
    TZEENTCH
};

bool KairicAcolytes::s_registered = false;

KairicAcolytes::KairicAcolytes() :
    Unit("Kairic Acolytes", 6, WOUNDS, 5, 6, false),
    m_sorcerousBolt(Weapon::Type::Missile, "Sorcerous Bolt", 12, 1, 5, 4, 0, 1),
    m_sorcerousBoldAdept(Weapon::Type::Missile, "Sorcerous Bolt (Adept)", 12, 2, 5, 4, 0, 1),
    m_cursedBlade(Weapon::Type::Melee, "Cursed Blade", 1, 1, 4, 4, 0, 1),
    m_cursedGlaive(Weapon::Type::Melee, "Cursed Glaive", 1, 1, 4, 4, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, KAIRIC_ACOLYTES};
}

bool KairicAcolytes::configure(int numModels, WeaponOptions weapons, int numCursedGlaives, int numScrollsOfDarkArts, int numVulcharcs)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    const int maxGlaives = (numModels / 10) * 3;
    if (numCursedGlaives > maxGlaives)
    {
        // Invalid weapon configuration.
        return false;
    }
    const int maxScrolls = numModels / 10;
    const int maxVulcharcs = numModels / 10;
    if (numScrollsOfDarkArts > maxScrolls || numVulcharcs > maxVulcharcs)
    {
        return false;
    }

    m_weapons = weapons;
    m_numCursedGlaives = numCursedGlaives;
    m_numScrollsOfDarkArts = numScrollsOfDarkArts;
    m_numVulcharcs = numVulcharcs;

    // Scroll of Dark Arts
    if (m_numScrollsOfDarkArts)
    {
        m_sorcerousBoldAdept.setRange(18);
        m_sorcerousBolt.setRange(18);
    }

    Model adept(BASESIZE, WOUNDS);
    adept.addMissileWeapon(&m_sorcerousBoldAdept);
    adept.addMeleeWeapon(&m_cursedBlade);
    addModel(adept);

    for (auto i = 0; i < m_numCursedGlaives; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_sorcerousBolt);
        model.addMeleeWeapon(&m_cursedGlaive);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_sorcerousBolt);
        model.addMeleeWeapon(&m_cursedBlade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Wounds KairicAcolytes::applyWoundSave(const Wounds &wounds)
{
    // Arcanite Shield
    if (m_weapons == CursedBladeAndShield)
    {
        Dice dice;
        auto roll = dice.rollD6();
        if (roll == 6)
            return {0, 0};
    }
    return Unit::applyWoundSave(wounds);
}

void KairicAcolytes::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_sorcerousBolt);
    visitor(&m_sorcerousBoldAdept);
    visitor(&m_cursedBlade);
    visitor(&m_cursedGlaive);
}

void KairicAcolytes::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kairic Acolytes", factoryMethod);
    }
}

Unit *KairicAcolytes::Create(const ParameterList &parameters)
{
    auto unit = new KairicAcolytes();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOptions)GetEnumParam("weapons", parameters, CursedBlade);
    int numCursedGlaives = GetIntParam("numCursedGlaives", parameters, 0);
    int numScrollsOfDarkArts = GetIntParam("numScrollsOfDarkArts", parameters, 0);
    int numVulcharcs = GetIntParam("numVulcharcs", parameters, 0);

    bool ok = unit->configure(numModels, weapons, numCursedGlaives, numScrollsOfDarkArts, numVulcharcs);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string KairicAcolytes::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == CursedBlade)
        {
            return "CursedBlade";
        }
        else if (parameter.m_intValue == PairedCursedBlades)
        {
            return "PairedCursedBlades";
        }
        else if (parameter.m_intValue == CursedBladeAndShield)
        {
            return "CursedBladeAndShield";
        }
    }

    return ParameterValueToString(parameter);
}

int KairicAcolytes::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "CursedBlade")
    {
        return CursedBlade;
    }
    else if (enumString == "PairedCursedBlades")
    {
        return PairedCursedBlades;
    }
    else if (enumString == "CursedBladeAndShield")
    {
        return CursedBladeAndShield;
    }
    return 0;
}

int KairicAcolytes::castingModifier() const
{
    int modifier = Unit::castingModifier();

    // Gestalt Sorcery
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 9.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(TZEENTCH) && ip->hasKeyword(WIZARD))
        {
            modifier += 1;
            break;
        }
    }
    return modifier;
}

int KairicAcolytes::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);
    // Paired Cursed Blades
    if ((m_weapons == PairedCursedBlades) && (weapon->name() == m_cursedBlade.name()))
    {
        modifier += 1;
    }
    return modifier;
}

} //namespace Tzeentch