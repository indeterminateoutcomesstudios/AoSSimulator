/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Zombies.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Death
{
static FactoryMethod factoryMethod = {
    Zombies::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Zombies::MIN_UNIT_SIZE}, Zombies::MIN_UNIT_SIZE, Zombies::MAX_UNIT_SIZE, Zombies::MIN_UNIT_SIZE},
    },
    DEATH,
    DEADWALKERS
};

bool Zombies::s_registered = false;

Zombies::Zombies() :
    Unit("Zombies", 4, WOUNDS, 10, NoSave, false),
    m_zombieBite(Weapon::Type::Melee, "Zombie Bite", 1, 1, 5, 5, 0, 1)
{
    m_keywords = {DEATH, ZOMBIE, DEADWALKERS, SUMMONABLE};
}

bool Zombies::configure(int numModels, bool standardBearer, bool noiseMaker)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // TODO: standard bearers debuff enemy Bravery within 6".
    m_standardBearer = standardBearer;
    // TODO: minimum charge distance is 6" with noise maker present
    m_noiseMaker = noiseMaker;

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_zombieBite);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Zombies::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_zombieBite);
}

Unit *Zombies::Create(const ParameterList &parameters)
{
    auto unit = new Zombies();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool standardBearers = GetBoolParam("standardBearers", parameters, false);
    bool noisemaker = GetBoolParam("noisemaker", parameters, false);

    bool ok = unit->configure(numModels, standardBearers, noisemaker);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Zombies::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Zombies", factoryMethod);
    }
}

int Zombies::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);

    // Vigour Mortis
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
    for (auto ip : units)
    {
        if (ip->hasKeyword(CORPSE_CARTS))
        {
            modifier += 1;
            break;
        }
    }

    // Dragged Down and Torn Apart
    if (remainingModels() >= 40)
    {
        modifier += 2;
    }
    else if (remainingModels() >= 20)
    {
        modifier += 1;
    }

    return modifier;
}

int Zombies::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toWoundModifier(weapon, target);

    // Dragged Down and Torn Apart
    if (remainingModels() >= 40)
    {
        modifier += 2;
    }
    else if (remainingModels() >= 20)
    {
        modifier += 1;
    }

    return modifier;
}

} //namespace Death
