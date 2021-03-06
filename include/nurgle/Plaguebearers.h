/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUEBEARERS_H
#define PLAGUEBEARERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Nurgle
{

class Plaguebearers : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 270;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Plaguebearers();
    ~Plaguebearers() override = default;

    bool configure(int numModels, bool iconBearer, bool pipers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Wounds applyWoundSave(const Wounds& wounds) override;
    int targetHitModifier(const Weapon* weapon, const Unit* attacker) const override;

protected:

    bool m_iconBearer = false;
    bool m_pipers = false;

private:

    Weapon m_plaguesword,
        m_plagueswordPlagueRidden;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Disgustingly Resilient           Yes
// Cloud of Flies                   Yes
// Locus of Fecundity               No
//

} // Nurgle

#endif //PLAGUEBEARERS_H