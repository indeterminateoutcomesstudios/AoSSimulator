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

    void charge(PlayerId player) override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    void onSlain() override;
    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits, const WoundingHits& wounds) override;

    int getDamageTableIndex() const;

private:

    Weapon m_puffSpores,
        m_enormousJaws,
        m_tramplingFeet;

    static bool s_registered;
};

} // namespace GloomspiteGitz

#endif //COLOSSALSQUIG_H