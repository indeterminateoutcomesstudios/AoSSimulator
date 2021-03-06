/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/BladegheistRevenants.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    BladegheistRevenants::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = BladegheistRevenants::MIN_UNIT_SIZE}, BladegheistRevenants::MIN_UNIT_SIZE,
            BladegheistRevenants::MAX_UNIT_SIZE, BladegheistRevenants::MIN_UNIT_SIZE
        },
    },
    DEATH,
    NIGHTHAUNT
};

bool BladegheistRevenants::s_registered = false;

BladegheistRevenants::BladegheistRevenants() :
    Unit("Bladegheist Revenants", 8, WOUNDS, 10, 4, true),
    m_tombGreatblade(Weapon::Type::Melee, "Tomb Greatblade", 1, 2, 3, 3, -1, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, BLADEGHEIST_REVENANTS};
}

bool BladegheistRevenants::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_retreatAndCharge = true;

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_tombGreatblade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *BladegheistRevenants::Create(const ParameterList &parameters)
{
    auto unit = new BladegheistRevenants();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BladegheistRevenants::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bladegheist Revenants", factoryMethod);
    }
}

Rerolls BladegheistRevenants::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Fearful Frenzy
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 12.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(SPIRIT_TORMENTS) || ip->hasKeyword(CHAINGHASTS))
        {
            return RerollFailed;
        }
    }

    return Unit::toHitRerolls(weapon, unit);
}

int BladegheistRevenants::toSaveModifier(const Weapon *weapon) const
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

int BladegheistRevenants::extraAttacks(const Weapon *weapon) const
{
    // Whirling Death
    int attacks = Unit::extraAttacks(weapon);
    if (m_charged)
    {
        attacks += 1;
    }
    return attacks;
}

void BladegheistRevenants::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_tombGreatblade);
}

} // namespace Nighthaunt
