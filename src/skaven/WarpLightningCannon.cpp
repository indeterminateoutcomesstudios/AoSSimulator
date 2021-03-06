/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarpLightningCannon.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Skaven
{
static FactoryMethod factoryMethod = {
    WarpLightningCannon::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SKAVEN
};

bool WarpLightningCannon::s_registered = false;

WarpLightningCannon::WarpLightningCannon() :
    Unit("Warp Lightning Cannon", 3, WOUNDS, 4, 4, false),
    m_warpLightningBlast(Weapon::Type::Missile, "Warp Lightning Blast", 24, 0, 0, 0, 0, 0),
    m_teethAndKnives(Weapon::Type::Melee, "Teeth and Knives", 1, RAND_D6, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WAR_MACHINE, WARP_LIGHTNING_CANNON};
}

bool WarpLightningCannon::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_warpLightningBlast);
    model.addMeleeWeapon(&m_teethAndKnives);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void WarpLightningCannon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_warpLightningBlast);
    visitor(&m_teethAndKnives);
}

Unit *WarpLightningCannon::Create(const ParameterList &parameters)
{
    auto unit = new WarpLightningCannon();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WarpLightningCannon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Warp Lightning Cannon", factoryMethod);
    }
}

int WarpLightningCannon::generateMortalWounds(const Unit *unit)
{
    auto mortalWounds = Unit::generateMortalWounds(unit);

    if (m_shootingTarget)
    {
        Dice dice;

        bool moreMoreWarpLightning = ((dice.rollD6() >= 2) || remainingWounds() <= 2);

        // Warp Lightning Blast
        if (distanceTo(m_shootingTarget) <= m_warpLightningBlast.range())
        {
            int power = dice.rollD6();
            Dice::RollResult rollResult;
            dice.rollD6(6, rollResult);
            mortalWounds += rollResult.rollsGE(power);

            // More-more Warp Lightning!
            if (moreMoreWarpLightning)
            {
                bool foundEngineer = false;
                auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 3.0f);
                for (auto ip : units)
                {
                    if (ip->hasKeyword(WARLOCK) && ip->hasKeyword(ENGINEER))
                    {
                        foundEngineer = true;
                        break;
                    }
                }

                if (foundEngineer)
                {
                    Dice::RollResult rollResultMore;
                    dice.rollD6(6, rollResultMore);
                    mortalWounds += rollResultMore.rollsGE(power);

                    int mortalWoundsSelf = 0;
                    int numUnmodifiedOnes = rollResult.numUnmodified1s() + rollResultMore.numUnmodified1s();
                    for (auto i = 0; i < numUnmodifiedOnes; i++)
                    {
                        mortalWoundsSelf += dice.rollD3();
                    }
                    applyDamage({0, mortalWoundsSelf});
                }
            }
        }
    }

    return mortalWounds;
}

} // namespace Skaven
