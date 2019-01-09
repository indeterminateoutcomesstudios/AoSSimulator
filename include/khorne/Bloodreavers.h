/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODREAVERS_H
#define BLOODREAVERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodreavers : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;

    enum WeaponOption
    {
        ReaverBlades,
        MeatripperAxe
    };

    Bloodreavers();
    ~Bloodreavers() override = default;

    bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool hornblowers);

protected:

    Rerolls toHitRerolls(const Unit* unit) const override;
    int battlshockModifier() const override;

    WeaponOption m_weaponOption = ReaverBlades;
    bool m_iconBearer = false;
    bool m_hornblower = false;

    static Weapon s_reaverBlades,
                  s_reaverBladesChieftain,
                  s_meatripperAxe,
                  s_meatripperAxeChieftain;
};

} // namespace Khorne

#endif //BLOODREAVERS_H