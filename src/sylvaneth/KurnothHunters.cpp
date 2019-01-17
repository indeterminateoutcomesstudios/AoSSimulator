/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/KurnothHunters.h>
#include <UnitFactory.h>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
        KurnothHunters::Create,
        {
                {ParamType::Integer, "numModels", KurnothHunters::MIN_UNIT_SIZE, KurnothHunters::MIN_UNIT_SIZE, KurnothHunters::MAX_UNIT_SIZE},
                {ParamType::Integer, "weapons", KurnothHunters::Greatswords, KurnothHunters::Greatswords, KurnothHunters::Greatbows}
        }
};

bool KurnothHunters::s_registered = false;

Weapon KurnothHunters::s_greatbow(Weapon::Type::Missile, "Kurnoth Greatbow", 30, 2, 4, 3, -1, RAND_D3);
Weapon KurnothHunters::s_greatbowHuntmaster(Weapon::Type::Missile, "Kurnoth Greatbow", 30, 2, 3, 3, -1, RAND_D3);
Weapon KurnothHunters::s_greatsword(Weapon::Type::Melee, "Kurnoth Greatsword", 1, 4, 3, 3, -1, 2);
Weapon KurnothHunters::s_greatswordHuntmaster(Weapon::Type::Melee, "Kurnoth Greatsword", 1, 4, 2, 3, -1, 2);
Weapon KurnothHunters::s_scythe(Weapon::Type::Melee, "Kurnoth Scythe", 2, 3, 3, 3, -2, RAND_D3);
Weapon KurnothHunters::s_scytheHuntmaster(Weapon::Type::Melee, "Kurnoth Scythe", 2, 3, 2, 3, -2, RAND_D3);
Weapon KurnothHunters::s_viciousClaws(Weapon::Type::Melee, "Quiverling's Vicious Claws", 1, 3, 4, 4, 0, 1);

KurnothHunters::KurnothHunters() :
    Unit("Kurnoth Hunters", 5, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, SYLVANETH, KURNOTH_HUNTERS };
}

bool KurnothHunters::configure(int numModels, WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_weaponOption = weapons;

    Model huntmaster(BASESIZE, WOUNDS);
    if (m_weaponOption == Greatbows)
    {
        huntmaster.addMissileWeapon(&s_greatbowHuntmaster);
        huntmaster.addMeleeWeapon(&s_viciousClaws);
    }
    else if (m_weaponOption == Greatswords)
    {
        huntmaster.addMeleeWeapon(&s_greatswordHuntmaster);
    }
    else if (m_weaponOption == Scythes)
    {
        huntmaster.addMeleeWeapon(&s_scytheHuntmaster);
    }
    addModel(huntmaster);

    for (int i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == Greatbows)
        {
            model.addMissileWeapon(&s_greatbow);
            model.addMeleeWeapon(&s_viciousClaws);
        }
        else if (m_weaponOption == Greatswords)
        {
            model.addMeleeWeapon(&s_greatsword);
        }
        else if (m_weaponOption == Scythes)
        {
            model.addMeleeWeapon(&s_scythe);
        }
        addModel(model);
    }
    return true;
}

Unit *KurnothHunters::Create(const ParameterList &parameters)
{
    auto unit = new KurnothHunters();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption)GetIntParam("weapons", parameters, KurnothHunters::Greatswords);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KurnothHunters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kurnoth Hunters", factoryMethod);
    }
}

} // namespace Sylvaneth
