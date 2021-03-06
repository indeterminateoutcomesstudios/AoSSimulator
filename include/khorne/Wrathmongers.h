/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WRATHMONGERS_H
#define WRATHMONGERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Wrathmongers : public Unit
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 720;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Wrathmongers();
    ~Wrathmongers() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* target) const override;

private:

    Weapon m_wrathflails,
        m_wrathflailsMaster;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Wraith-flails                    Yes
// Crimson Haze                     No
// Bloodfury                        No
//

} // namespace Khorne

#endif //WRATHMONGERS_H