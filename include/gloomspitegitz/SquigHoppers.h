/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SQUIGHOPPERS_H
#define SQUIGHOPPERS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class SquiqHoppers : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SquiqHoppers();
    ~SquiqHoppers() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Weapon m_fangFilledGob,
        m_slitta,
        m_slittaBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Boing! Boing! Boing!             No
//

} // namespace GloomspiteGitz

#endif //SQUIGHOPPERS_H