/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DIREWOLVES_H
#define DIREWOLVES_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{
class DireWolves : public Unit
{
public:

    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    DireWolves();
    ~DireWolves() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int toWoundModifier(const Weapon* weapon, const Unit* target) const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    Weapon m_fangsAndClaws,
        m_fangsAndClawsDoom;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Slavering Charge                 Yes
// Vigour Necris                    Yes
//

} //namespace Death

#endif //DIREWOLVES_H