/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptInfernalCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    CryptInfernalCourtier::Create,
    nullptr,
    nullptr,
    {
        {
        },
    },
    DEATH,
    FLESH_EATERS_COURT
};

bool CryptInfernalCourtier::s_registered = false;

CryptInfernalCourtier::CryptInfernalCourtier() :
    Unit("Crypt Infernal Courtier", 12, WOUNDS, 10, 4, true),
    m_foetidBreath(Weapon::Type::Missile, "Foetid Breath", 9, 1, 4, 3, -1, RAND_D3),
    m_skeweringTalons(Weapon::Type::Melee, "Skewering Talons", 1, 5, 4, 3, -1, 2)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATERS_COURT, HERO, COURTIER, CRYPT_INFERNAL_COURTIER};
}

bool CryptInfernalCourtier::configure()
{
    Model infernal(BASESIZE, WOUNDS);
    infernal.addMissileWeapon(&m_foetidBreath);
    infernal.addMeleeWeapon(&m_skeweringTalons);
    addModel(infernal);

    m_points = POINTS_PER_UNIT;

    return true;
}

void CryptInfernalCourtier::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_foetidBreath);
    visitor(&m_skeweringTalons);
}

Unit *CryptInfernalCourtier::Create(const ParameterList &parameters)
{
    auto unit = new CryptInfernalCourtier();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void CryptInfernalCourtier::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Crypt Infernal Courtier", factoryMethod);
    }
}

Wounds CryptInfernalCourtier::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Skewering Strike
    if ((hitRoll == 6) && weapon->name() == m_skeweringTalons.name())
    {
        // Two mortal wounds.
        return {0, 2};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace FleshEaterCourt
