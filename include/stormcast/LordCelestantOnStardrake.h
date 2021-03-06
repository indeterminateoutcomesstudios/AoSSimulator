/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDCELESTANTONSTARDRAKE_H
#define LORDCELESTANTONSTARDRAKE_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals
{

class LordCelestantOnStardrake : public StormcastEternal
{
public:

    static const int BASESIZE = 170; // x105 oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 560;

    enum WeaponOption
    {
        CelestineHammer,
        StormboundBlade,
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static void Init();

    LordCelestantOnStardrake();
    ~LordCelestantOnStardrake() override = default;

    bool configure(WeaponOption weapons);
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

    int extraAttacks(const Weapon *weapon) const override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    Rerolls toSaveRerolls(const Weapon* weapon) const override;
    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;
    void onStartShooting(PlayerId player) override;
    void onStartCombat(PlayerId player) override;
    void onEndCombat(PlayerId player) override;

protected:

    WeaponOption m_weapons = CelestineHammer;

private:

    Weapon m_celestineHammer,
        m_stormboundBlade,
        m_greatClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Arcane Lineage                   No
// Cavernous Jaws                   Yes
// Inescapable Vengeance            Yes
// Lord of the Heavens              Yes
// Sigmarite Thundershield          Yes
// Stormbound Blade                 Yes
// Sweeping Tail                    Yes
// Lord of the Celestial Host       No
//

} // namespace StormcastEternals

#endif //LORDCELESTANTONSTARDRAKE_H