/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Hammerers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Dispossessed
{

static FactoryMethod factoryMethod = {
    Hammerers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Hammerers::MIN_UNIT_SIZE}, Hammerers::MIN_UNIT_SIZE,
            Hammerers::MAX_UNIT_SIZE, Hammerers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "standardBearer", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "musician", {.m_boolValue = false}, false, false, false}
    },
    ORDER,
    DISPOSSESSED
};

bool Hammerers::s_registered = false;

Hammerers::Hammerers() :
    Unit("Hammerers", 4, WOUNDS, 7, 4, false),
    m_greatHammer(Weapon::Type::Melee, "Gromril Great Hammer", 1, 2, 3, 3, -1, 1),
    m_greatHammerKeeper(Weapon::Type::Melee, "Gromril Great Hammer (Keeper)", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HAMMERERS};
}

bool Hammerers::configure(int numModels, bool standardBearer, bool musician)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_musician = musician;

    Model keeper(BASESIZE, WOUNDS);
    keeper.addMeleeWeapon(&m_greatHammerKeeper);
    addModel(keeper);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_greatHammer);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Hammerers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_greatHammer);
    visitor(&m_greatHammerKeeper);
}

Unit *Hammerers::Create(const ParameterList &parameters)
{
    auto unit = new Hammerers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool standardBearer = GetBoolParam("standardBearer", parameters, false);
    bool musician = GetBoolParam("musician", parameters, false);

    bool ok = unit->configure(numModels, standardBearer, musician);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Hammerers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Hammerers", factoryMethod);
    }
}

bool Hammerers::battleshockRequired() const
{
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 16.0f);
    for (auto ip : units)
    {
        // Kingsguard
        if (ip->hasKeyword(DISPOSSESSED) && ip->hasKeyword(HERO))
            return false;
    }
    return true;
}

} // namespace Dispossessed
