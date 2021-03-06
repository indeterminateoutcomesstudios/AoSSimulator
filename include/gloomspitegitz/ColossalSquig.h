/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COLOSSALSQUIG_H
#define COLOSSALSQUIG_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class ColossalSquig : public Unit
{
public:
    static const int BASESIZE = 120; // oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ColossalSquig();
    ~ColossalSquig() override = default;

    int move() const override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    void onSlain() override;
    void onCharged() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int targetHitModifier(const Weapon* weapon, const Unit* attacker) const override;

    int getDamageTableIndex() const;

private:

    Weapon m_puffSpores,
        m_enormousJaws,
        m_tramplingFeet;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Crazed Charge                    Yes
// Fungoid Squig Explosion          Yes
// Puff Spores                      Yes
// Swallowed Whole                  Yes
//

} // namespace GloomspiteGitz

#endif //COLOSSALSQUIG_H