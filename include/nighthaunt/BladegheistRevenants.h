/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLADEGHEISTREVENANTS_H
#define BLADEGHEISTREVENANTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Nighthaunt
{

class BladegheistRevenants : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    BladegheistRevenants();
    ~BladegheistRevenants() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int toSaveModifier(const Weapon* weapon) const override;
    int extraAttacks(const Weapon* weapon) const override;

private:

    Weapon m_tombGreatblade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Ethereal                         Yes
// Fearful Frenzy                   Yes
// Whirling Death                   Yes


} // namespace Nighthaunt

#endif // BLADEGHEISTREVENANTS_H
