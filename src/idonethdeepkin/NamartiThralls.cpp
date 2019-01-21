/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/NamartiThralls.h>
#include <UnitFactory.h>
#include <iostream>

namespace IdonethDeepkin {

static FactoryMethod factoryMethod = {
    NamartiThralls::Create,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = NamartiThralls::MIN_UNIT_SIZE}, NamartiThralls::MIN_UNIT_SIZE,
         NamartiThralls::MAX_UNIT_SIZE, NamartiThralls::MIN_UNIT_SIZE},
        {ParamType::Integer, "numIconBearers", {.m_intValue = 0}, 0, NamartiThralls::MAX_UNIT_SIZE/10}
    }
};

Weapon NamartiThralls::s_lanmariBlade(Weapon::Type::Melee, "Lanmari Blade", 1, 2, 3, 3, -1, 1);

bool NamartiThralls::s_registered = false;

NamartiThralls::NamartiThralls() :
    Unit("Namarti Thralls", 6, WOUNDS, 6, 5, false)
{
    m_keywords = { ORDER, AELF, IDONETH_DEEPKIN, NAMARTI, THRALLS };
}

bool NamartiThralls::configure(int numModels, int numIconBearers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
        return false;

    if (numIconBearers > MAX_UNIT_SIZE / 10)
        return false;

    m_numIconBearers = numIconBearers;

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_lanmariBlade);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_lanmariBlade.name() << ": " << s_lanmariBlade.strength() << std::endl;
    }

    return true;
}

Unit *NamartiThralls::Create(const ParameterList &parameters)
{
    auto unit = new NamartiThralls();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    int numIconBearers = GetIntParam("numIconBearers", parameters, 0);

    bool ok = unit->configure(numModels, numIconBearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void NamartiThralls::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Namarti Thralls", factoryMethod);
    }
}

} // namespace IdonethDeepkin