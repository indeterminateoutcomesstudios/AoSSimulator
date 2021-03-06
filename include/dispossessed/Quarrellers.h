/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef QUARRELLERS_H
#define QUARRELLERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Dispossessed
{

class Quarrellers : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    enum StandardOptions
    {
        None,
        RunicIcon,
        ClanBanner
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Quarrellers();
    ~Quarrellers() override = default;

    bool configure(int numModels, bool duardinBucklers, StandardOptions standard, bool drummer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int extraAttacks(const Weapon *weapon) const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;

private:

    bool m_duardinBucklers = false;
    StandardOptions m_standard = None;
    bool m_drummer = false;

    Weapon m_duardinCrossbow,
        m_duardinCrossbowVeteran,
        m_rangersAxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Volley Fire                      Yes
// Ruinic Icon                      No
// Clan Banner                      No
// Duardin Bucklers                 Yes
// Drummers                         No
//

} // namespace Dispossessed

#endif //QUARRELLERS_H