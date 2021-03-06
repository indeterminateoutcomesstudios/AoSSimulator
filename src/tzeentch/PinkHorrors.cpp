/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/PinkHorrors.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{
static FactoryMethod factoryMethod = {
    PinkHorrors::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = PinkHorrors::MIN_UNIT_SIZE}, PinkHorrors::MIN_UNIT_SIZE,
            PinkHorrors::MAX_UNIT_SIZE, PinkHorrors::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "iconBearer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "hornblower", {.m_boolValue = true}, false, false},
    },
    CHAOS,
    TZEENTCH
};

bool PinkHorrors::s_registered = false;

PinkHorrors::PinkHorrors() :
    Unit("Pink Horrors", 5, WOUNDS, 10, 5, false),
    m_magicalFlames(Weapon::Type::Missile, "Magical Flames", 18, 1, 4, 4, 0, 1),
    m_graspingHands(Weapon::Type::Melee, "Grasping Hands",  1, 1, 5, 4, 0, 1),
    m_graspingHandsHorror(Weapon::Type::Melee, "Grasping Hands (Iridescent Horror)", 1, 2, 5, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, TZEENTCH, WIZARD, PINK_HORRORS};
}

bool PinkHorrors::configure(int numModels, bool iconBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_hornblower = hornblower;

    Model horror(BASESIZE, WOUNDS);
    horror.addMissileWeapon(&m_magicalFlames);
    horror.addMeleeWeapon(&m_graspingHandsHorror);
    addModel(horror);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_magicalFlames);
        model.addMeleeWeapon(&m_graspingHands);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void PinkHorrors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_magicalFlames);
    visitor(&m_graspingHands);
    visitor(&m_graspingHandsHorror);
}

Unit *PinkHorrors::Create(const ParameterList &parameters)
{
    auto unit = new PinkHorrors();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("iconBearer", parameters, false);
    bool hornblower = GetBoolParam("hornblower", parameters, false);

    bool ok = unit->configure(numModels, iconBearer, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PinkHorrors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Pink Horrors", factoryMethod);
    }
}

void  PinkHorrors::computeBattleshockEffect(int roll, int& numFled, int& numAdded) const
{
    Unit::computeBattleshockEffect(roll, numFled, numAdded);
    if (m_iconBearer)
    {
        // Icon Bearer
        if (roll == 1)
        {
            Dice dice;
            numAdded = dice.rollD6();
        }
    }
}

void PinkHorrors::restoreModels(int numModels)
{
    // Icon Bearer
    Model model(BASESIZE, WOUNDS);
    model.addMissileWeapon(&m_magicalFlames);
    model.addMeleeWeapon(&m_graspingHands);
    for (auto i = 0; i < numModels; i++)
    {
        addModel(model);
    }
}

int PinkHorrors::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);
    // Flickering Flames
    if (remainingModels() >= 20)
        modifier += 1;
    return modifier;
}

int PinkHorrors::castingModifier() const
{
    int modifier = Unit::castingModifier();

    // Locus of Conjuration
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 9.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(TZEENTCH) && ip->hasKeyword(DAEMON) && ip->hasKeyword(HERO))
        {
            modifier+=1;
            break;
        }
    }
    return modifier;
}

} //namespace Tzeentch