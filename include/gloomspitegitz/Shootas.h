/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SHOOTAS_H
#define SHOOTAS_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class Shootas : public Unit
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 20;
    static const int MAX_UNIT_SIZE = 60;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 390;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Shootas();
    ~Shootas() override = default;

    bool configure(int numModels, int numBarbedNets,
                   int numGongbashers, int numFlagbearers, int numIconbearers);

protected:

    int toWoundModifier(const Weapon* weapon, const Unit* unit) const override;
    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    int runModifier() const override;
    int battlshockModifier() const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    int m_numGongbashers = 0;
    int m_numFlagbearers = 0;
    int m_numIconbearers = 0;

    static Weapon s_slitta,
        s_slittaBoss,
        s_moonclanBow,
        s_moonclanBowBoss,
        s_barbedNet;

    static bool s_registered;
};

} // namespace GloomspiteGitz

#endif //SHOOTAS_H