/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/SquigGobba.h>
#include <UnitFactory.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    SquigGobba::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool SquigGobba::s_registered = false;

SquigGobba::SquigGobba() :
    Unit("Squig Gobba", 4, WOUNDS, 4, 5, false),
    m_spitSquigs(Weapon::Type::Missile, "Spit-squigs", 30, 6, 4, 3, 0, RAND_D3),
    m_bashinSticks(Weapon::Type::Melee, "Bashin' Sticks", 1, 3, 5, 5, 0, 1),
    m_cavernousMaw(Weapon::Type::Melee, "Cavernous Maw", 2, 3, 3, 3, -2, RAND_D3)
{
    m_keywords = {DESTRUCTION, GLOOMSPITE_GITZ, SQUIG, MOONCLAN, MONSTER, SQUIG_GOBBA};
}

bool SquigGobba::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMissileWeapon(&m_spitSquigs);
    model.addMeleeWeapon(&m_bashinSticks);
    model.addMeleeWeapon(&m_cavernousMaw);

    m_points = POINTS_PER_UNIT;

    addModel(model);

    return true;
}

void SquigGobba::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_spitSquigs);
    visitor(&m_bashinSticks);
    visitor(&m_cavernousMaw);
}

Unit *SquigGobba::Create(const ParameterList &parameters)
{
    auto unit = new SquigGobba();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SquigGobba::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Squig Gobba", factoryMethod);
    }
}

int SquigGobba::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);

    // Arcing Spit
    if (weapon->name() == m_spitSquigs.name() && target->remainingModels() >= 10)
        modifier += 1;

    return modifier;
}

} // namespace GloomspiteGitz
