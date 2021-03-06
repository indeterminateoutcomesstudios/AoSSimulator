/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef UNITFACTORY_H
#define UNITFACTORY_H

#include <map>
#include <vector>
#include "WarhammerSim.h"

class Unit;

typedef Unit* (*CreateMethod)(const std::vector<Parameter>& parameters);
typedef std::string (*ParamValueToString)(const Parameter& parameter);
typedef int (*EnumStringToInt)(const std::string& enumString);

struct FactoryMethod
{
public:
    CreateMethod m_create;
    ParamValueToString m_paramToString;
    EnumStringToInt m_enumStringToInt;
    std::vector<Parameter> m_parameters;
    Keyword m_grandAlliance;
    Keyword m_faction;
};

class UnitFactory
{
public:

    static bool Register(const std::string& name, FactoryMethod factoryMethod);

    static Unit* Create(const std::string& name, const std::vector<Parameter>& parameters);

    static std::map<std::string, FactoryMethod>::const_iterator RegisteredUnitsBegin() { return s_registeredUnits.begin(); }
    static std::map<std::string, FactoryMethod>::const_iterator RegisteredUnitsEnd() { return s_registeredUnits.end(); }

    static const FactoryMethod* LookupUnit(const std::string& name);

protected:

    static std::map<std::string, FactoryMethod> s_registeredUnits;
};

#endif // UNITFACTORY_H
