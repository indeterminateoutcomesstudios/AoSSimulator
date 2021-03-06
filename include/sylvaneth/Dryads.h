/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRYADS_H
#define DRYADS_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Dryads : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 270;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Dryads();
    ~Dryads() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

    int toSaveModifier(const Weapon *weapon) const override;

private:

    Weapon m_wrackingTalons,
        m_wrackingTalonsNymph;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Blessing of the Forest           Yes
// Enrapturing Song                 No
// Impenetrable Thicket             Yes
//

} // namespace Sylvaneth

#endif //DRYADS_H