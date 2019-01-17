/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <WarhammerSim.h>


std::vector<Parameter>::const_iterator FindParam(const std::string& name, const ParameterList &parameters)
{
    auto matchName = [name](const Parameter& param)->bool { return (param.m_name == name); };
    auto pip = std::find_if(parameters.begin(), parameters.end(), matchName);
    return pip;
}

int GetIntParam(const std::string& name, const ParameterList& parameters, int defaultValue)
{
    int value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end())
    {
        if (pip->m_paramType == ParamType::Integer)
        {
            value = pip->m_intValue;
        }
    }
    return value;
}

bool GetBoolParam(const std::string& name, const ParameterList& parameters, bool defaultValue)
{
    bool value = defaultValue;
    auto pip = FindParam(name, parameters);
    if (pip != parameters.end())
    {
        if (pip->m_paramType == ParamType::Boolean)
        {
            value = pip->m_boolValue;
        }
    }
    return value;
}

std::string PhaseToString(Phase phase)
{
    switch (phase)
    {
        case Phase::Initiative:
            return "Initiative";
        case Phase::Hero:
            return "Hero";
        case Phase::Movement:
            return "Movement";
        case Phase::Shooting:
            return "Shooting";
        case Phase::Charge:
            return "Charge";
        case Phase::Combat:
            return "Combat";
        case Phase::Battleshock:
            return "Battleshock";
    }
}

std::string PlayerIdToString(PlayerId id)
{
    switch (id)
    {
        case PlayerId::None:
            return "None";
        case PlayerId::Red:
            return "Red";
        case PlayerId::Blue:
            return "Blue";
    }
}
