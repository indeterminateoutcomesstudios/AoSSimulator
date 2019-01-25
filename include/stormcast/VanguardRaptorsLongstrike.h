/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VANGUARDRAPTORSLONGSTRIKE_H
#define VANGUARDRAPTORSLONGSTRIKE_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class VanguardRaptorsLongstrike : public StormcastEternal
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    VanguardRaptorsLongstrike();
    ~VanguardRaptorsLongstrike() override = default;

    bool configure(int numModels);

protected:

    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;

private:

    static Weapon s_longstikeCrossbow,
                  s_heavyStock,
                  s_beakAndClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Headshot                         Yes
// Hunting Call                     No
// Longshot                         No
//

} // namespace StormcastEternals

#endif //VANGUARDRAPTORSLONGSTRIKE_H