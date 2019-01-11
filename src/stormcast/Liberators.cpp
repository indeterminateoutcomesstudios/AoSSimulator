/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Liberators.h>

namespace StormcastEternals
{

Weapon Liberators::s_warhammer(Weapon::Type::Melee, "Warhammer", 1, 2, 4, 3, 0, 1);
Weapon Liberators::s_warhammerPrime(Weapon::Type::Melee, "Warhammer", 1, 3, 4, 3, 0, 1);
Weapon Liberators::s_warblade(Weapon::Type::Melee, "Warblade", 1, 2, 3, 4, 0, 1);
Weapon Liberators::s_warbladePrime(Weapon::Type::Melee, "Warblade", 1, 3, 3, 4, 0, 1);
Weapon Liberators::s_grandhammer(Weapon::Type::Melee, "Grandhammer", 1, 2, 4, 3, -1, 2);
Weapon Liberators::s_grandblade(Weapon::Type::Melee, "Grandblade", 1, 2, 3, 4, -1, 2);

Liberators::Liberators() :
    StormcastEternal("Liberators", 5, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, REDEEMER, LIBERATORS };
}

bool Liberators::configure(int numModels, WeaponOption weapons, bool pairedWeapons, int numGrandhammers, int numGrandblades)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxGrandweapons = numModels / 5;
    if (numGrandblades + numGrandhammers > maxGrandweapons)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_weaponOption = weapons;
    m_pairedWeapons = pairedWeapons;

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (m_weaponOption == Warhammer)
    {
        primeModel.addMeleeWeapon(&s_warhammerPrime);
    }
    else if (m_weaponOption == Warblade)
    {
        primeModel.addMeleeWeapon(&s_warbladePrime);
    }
    addModel(primeModel);

    for (auto i = 0; i < numGrandblades; i++)
    {
        Model grandbladeModel(BASESIZE, WOUNDS);
        grandbladeModel.addMeleeWeapon(&s_grandblade);
        addModel(grandbladeModel);
    }
    for (auto i = 0; i < numGrandhammers; i++)
    {
        Model grandhammerModel(BASESIZE, WOUNDS);
        grandhammerModel.addMeleeWeapon(&s_grandhammer);
        addModel(grandhammerModel);
    }
    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == Warhammer)
            model.addMeleeWeapon(&s_warhammer);
        else if (m_weaponOption == Warblade)
            model.addMeleeWeapon(&s_warblade);
        addModel(model);
    }

    return true;
}

int Liberators::toHitModifier(const Weapon* weapon, const Unit* unit) const
{
    int modifier = StormcastEternal::toHitModifier(weapon, unit);

    // Lay Low the Tyrants
    if (unit->wounds() >= 5)
        modifier += 1;

    return modifier;
}

Rerolls Liberators::toSaveRerolls(const Weapon* weapon) const
{
    // Sigmarite Shields
    if (m_weaponOption == Warhammer || m_weaponOption == Warblade)
        return RerollOnes;

    return StormcastEternal::toSaveRerolls(weapon);
}

Hits Liberators::applyHitModifiers(const Weapon *weapon, const Unit *unit, const Hits &hits) const
{
    Hits modifiedHits = hits;
    if ((hits.rolls.numUnmodified6s() > 0) && m_pairedWeapons &&
        (weapon->name() == s_warblade.name() || weapon->name() == s_warhammer.name()))
    {
        // each 6 inflicts an additional hit
        modifiedHits.numHits += hits.rolls.numUnmodified6s();
    }

    // modifiers accumulate
    return Unit::applyHitModifiers(weapon, unit, modifiedHits);
}

} // namespace StormcastEternals
