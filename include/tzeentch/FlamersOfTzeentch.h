/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FLAMERSOFTZEENTCH_H
#define FLAMERSOFTZEENTCH_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class FlamersOfTzeentch : public Unit
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 540;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    FlamersOfTzeentch();
    ~FlamersOfTzeentch() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_warpflame,
        m_warpflamePyrocaster,
        m_flamingMaw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Capricious Warpflame             No
// Locus of Transmogrification      No
//

} // Tzeentch

#endif //FLAMERSOFTZEENTCH_H