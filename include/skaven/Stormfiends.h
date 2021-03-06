/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STORMFIENDS_H
#define STORMFIENDS_H

#include <Unit.h>
#include <Weapon.h>

namespace Skaven
{

class Stormfiends : public Unit
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 9;
    static const int POINTS_PER_BLOCK = 290;
    static const int POINTS_MAX_UNIT_SIZE = 870;

    enum WeaponOption_1
    {
        WarpfireProjectors,
        Windlaunchers
    };
    enum WeaponOption_2
    {
        Grinderfists,
        RatlingCannons
    };
    enum WeaponOption_3
    {
        DoomflayerGauntlets,
        ShockGauntlets
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Stormfiends();
    ~Stormfiends() override = default;

    bool configure(int numModels, WeaponOption_1 weapon1, WeaponOption_2 weapon2, WeaponOption_3 weapon3);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    WeaponOption_1 m_weapons1 = WarpfireProjectors;
    WeaponOption_2 m_weapons2 = Grinderfists;
    WeaponOption_3 m_weapons3 = DoomflayerGauntlets;

    Weapon m_ratlingCannons,
        m_windlaunchers,
        m_warpfireProjectors,
        m_doomfireGauntlets,
        m_grinderfists,
        m_shockGauntlets,
        m_clubbingBlows;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Doomfire Gauntlets               No
// Grinderfist Tunnellers           No
// Shock Gauntlets                  No
// Warpfire Projectors              No
// Warpstone-laced Armour           No
// Windlaunchers                    No
//

} // namespace Skaven

#endif //STORMFIENDS_H