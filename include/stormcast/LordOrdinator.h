/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDORDINATOR_H
#define LORDORDINATOR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordOrdinator : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;

    enum WeaponOption
    {
        AstralHammers,
        AstralGrandhammer
    };

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    LordOrdinator();
    ~LordOrdinator() override = default;

    bool configure(WeaponOption weaponOption);

protected:

private:

    WeaponOption m_weaponOption = AstralHammers;

    static Weapon s_astralHammers,
        s_astralGrandhammer;

    static bool s_registered;
};

} // namespace StormcastEternals

#endif //LORDORDINATOR_H